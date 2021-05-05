#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "op.h"
#include "redblacktree.h"

uint32_t NUM_LEVEL;
uint32_t BLOCKSIZE, REPL_POLICY, BLOCK_OFFSET_WIDTH;
char *TRACE_FILE;
uint64_t trace_count;

cache *CACHE;

uint64_t *OPTIMAL_TRACE;

int main(int argc, char *argv[])
{

	// Step1: Test the correctness of the number of the arguments. It should be 9.

	if (argc != 9)
	{
		printf("Usage: %s <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <REPL_POLICY> <INCLUSION> <TRACE_FILE>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Step2: Extract the cache levels

	NUM_LEVEL = ((atoi(argv[4])) == 0) ? 1 : 2;

	/* Step3: initial to parse arugments size, assoc, inclusion*/

	uint32_t *size, *assoc, *inclusion;
	size = (uint32_t *)malloc(sizeof(uint32_t) * NUM_LEVEL);
	if (size == NULL)
		_error_exit("malloc")

			// Complete the rest

			parse_arguments(argc, argv, size, assoc, inclusion);

	//Step4:
	/* if it is optimal replace policy */

	/*
  Bélády's algorithm or optimal policy: see https://en.wikipedia.org/wiki/Cache_replacement_policies#:~:text=Caching%20improves%20performance%20by%20keeping,room%20for%20the%20new%20ones.
  */

	/*
 https://www.geeksforgeeks.org/optimal-page-replacement-algorithm/
 */

	/* we need to pre read the trace file to inital rank array */
	if (REPL_POLICY == OPTIMAL)
		OPTIMAL_TRACE_Initial();

	/* Step 5: inital the cache */
	Cache_Initial(size, assoc, inclusion);

	/* step 6: Free memory */

	free(size);
	free(assoc);
	free(inclusion);

	/* Step7: open trace file to read */
	FILE *trace_file_fp = fopen(TRACE_FILE, "r");
	if (trace_file_fp == NULL)
		_error_exit("fopen")

			/* Step 8 : */

			while (1)
		{

			int result;
			uint8_t OP, line;
			uint64_t ADDR;
			result = fscanf(trace_file_fp, "%c %llx%c", &OP, &ADDR, &line);

			trace_count++;
			uint64_t rank_value = (REPL_POLICY == OPTIMAL) ? OPTIMAL_TRACE[trace_count - 1] : trace_count;
			if (result == EOF)
				/* if we reach the end of trace file*/
				break;

			switch (OP)
			{
			case 'r':
			{
				block *blk = (block *)malloc(sizeof(block));
				Read(L1, ADDR, blk, rank_value);
				free(blk);
				break;
			}
			case 'w':
				Write(L1, ADDR, DIRTY, rank_value);
				break;
			default:
				_input_error_exit("error: wrong operation type. Legal operations are read 'r' and write 'w'.\n") break;
			}
		}

	fclose(trace_file_fp);
	/* output the statistic result */
	output(stdout);

	if (REPL_POLICY == OPTIMAL)
		free(OPTIMAL_TRACE);
	Cache_free();
	return 0;
}

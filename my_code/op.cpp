#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "op.h"
#include "redblacktree.h"

char *NAME_REPL_POLICY[] = {"LRU", "FIFO", "Pseudo", "Optimal"};
char *NAME_INCLUSION[] = {"non-inclusive", "inclusive", "exclusive"};

/*
 *	improved log2 function
 *	log2(0) = 0
 */
uint32_t log_2(uint32_t num)
{
	uint32_t result = 0, tmp = num;
	while (tmp >>= 1)
		result++;
	return result;
}

/*
 *	Step1: parse arguments and check inputs
 */
void parse_arguments(int argc, char *argv[], uint32_t *size, uint32_t *assoc, uint32_t *inclusion)
{
	uint32_t i;

	BLOCKSIZE = atoi(argv[1]);
	size[L1] = atoi(argv[2]);
	assoc[L1] = atoi(argv[3]);
	inclusion[L1] = atoi(argv[7]);
	if (NUM_LEVEL == 2)
	{
		size[L2] = atoi(argv[4]);
		assoc[L2] = atoi(argv[5]);
		inclusion[L2] = NON_INCLUSIVE;
	}
	REPL_POLICY = atoi(argv[6]);
	TRACE_FILE = argv[8];

	/* input check for any possible error*/
	if (REPL_POLICY < LRU || REPL_POLICY > OPTIMAL)
		_input_error_exit("error: wrong replacement policy\n")

	/* complete this part for other possible errors



	_input_error_exit("error: wrong cache size\n")
  _input_error_exit("error: wrong associatity\n")
  _input_error_exit("error: wrong inclusion\n")
  _input_error_exit("error: wrong inclusion\n")



  */
}

/*
 *	Step2: output cache statistic results to file or stdout
 */
void output(FILE *fp)
{

	fprintf(fp, "===== Simulator configuration =====\n");
	fprintf(fp, "BLOCKSIZE:             %u\n", BLOCKSIZE);

	/* Complete this part





  */
}

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "cache.h"
#include "op.h"
#include "redblacktree.h"

/*
 *	Step1: Inital the cache
 */
void Cache_Initial(uint32_t *size, uint32_t *assoc, uint32_t *inclusion)
{
	uint32_t j;
	CACHE = (cache *)malloc(sizeof(cache) * NUM_LEVEL);
	if (CACHE == NULL)
		_error_exit("malloc")
			BLOCK_OFFSET_WIDTH = log_2(BLOCKSIZE);
	for (j = 0; j < NUM_LEVEL; j++)
	{
		if (size[j] == 0)
			break;
		/* first, initial the attributes of cache */
		CACHE[j].CACHE_ATTRIBUTES.SIZE = size[j];
		CACHE[j].CACHE_ATTRIBUTES.ASSOC = assoc[j];
		CACHE[j].CACHE_ATTRIBUTES.INCLUSION = inclusion[j];
		CACHE[j].CACHE_ATTRIBUTES.SET_NUM = CACHE[j].CACHE_ATTRIBUTES.SIZE / (CACHE[j].CACHE_ATTRIBUTES.ASSOC * BLOCKSIZE);

		CACHE[j].CACHE_ATTRIBUTES.INDEX_WIDTH = log_2(CACHE[j].CACHE_ATTRIBUTES.SET_NUM);
		CACHE[j].CACHE_ATTRIBUTES.TAG_WIDTH = 64 - CACHE[j].CACHE_ATTRIBUTES.INDEX_WIDTH - BLOCK_OFFSET_WIDTH;

		/* then, allocate space for sets (including blocks and tag array) */
		CACHE[j].SET = (set *)malloc(sizeof(set) * CACHE[j].CACHE_ATTRIBUTES.SET_NUM);
		if (CACHE[j].SET == NULL)
			_error_exit("malloc")
				uint32_t i;
		for (i = 0; i < CACHE[j].CACHE_ATTRIBUTES.SET_NUM; i++)
		{
			CACHE[j].SET[i].BLOCK = (block *)malloc(sizeof(block) * CACHE[j].CACHE_ATTRIBUTES.ASSOC);
			if (CACHE[j].SET[i].BLOCK == NULL)
				_error_exit("malloc")
					memset(CACHE[j].SET[i].BLOCK, 0, sizeof(block) * CACHE[j].CACHE_ATTRIBUTES.ASSOC);
			CACHE[j].SET[i].RANK = (uint64_t *)malloc(sizeof(uint64_t) * CACHE[j].CACHE_ATTRIBUTES.ASSOC);
			if (CACHE[j].SET[i].RANK == NULL)
				_error_exit("malloc")
					memset(CACHE[j].SET[i].RANK, 0, sizeof(uint64_t) * CACHE[j].CACHE_ATTRIBUTES.ASSOC);
		}
		/* initial the statistics information of cache */
		memset(&(CACHE[j].CACHE_STAT), 0, sizeof(cache_stat));
	}
}

/*
 * STep2: Inital the rank value array for optimal replacement policy
 */
void OPTIMAL_TRACE_Initial()
{
	FILE *trace_file_fp = fopen(TRACE_FILE, "r");
	if (trace_file_fp == NULL)
		_error_exit("fopen()")
			uint64_t trace_len = 1000,
					 i = 0;
	uint64_t *trace = (uint64_t *)malloc(sizeof(uint64_t) * trace_len);
	if (trace == NULL)
		_error_exit("malloc")
			/* read tracefile, record the address in array "trace" in order */
			while (1)
		{
			int result;
			uint8_t OP, line;
			uint64_t ADDR;
			result = fscanf(trace_file_fp, "%c %llx%c", &OP, &ADDR, &line);
			if (result == EOF)
				break;
			if (i > trace_len)
			{
				trace_len *= 2;
				trace = (uint64_t *)realloc(OPTIMAL_TRACE, sizeof(uint64_t) * trace_len);
				if (trace == NULL)
					_error_exit("realloc")
			}
			trace[i] = ADDR;
			i++;
		}
	fclose(trace_file_fp);

	OPTIMAL_TRACE = (uint64_t *)malloc(sizeof(uint64_t) * i);
	if (OPTIMAL_TRACE == NULL)
		_error_exit("malloc")

			/* we use red black tree to help organize the trace */
			TreePtr T = (TreePtr)malloc(sizeof(struct rbTree));
	if (T == NULL)
		_error_exit("malloc")
			T->root = NULL;
	uint64_t j;
	/* we need to read operation address sequence in reverse order */
	for (j = 0; j < i; j++)
	{
		TreeNode _result = rb_tree_search(T, trace[i - j - 1]);
		if (_result == NULL)
		{
			/* if this address never show up before */
			/* this access time is the last access time of this address */
			/* keep the rank value as infinity */
			/* here rank value use total trace entries number + 1 */
			_rb_tree_insert(T, trace[i - j - 1], i - j - 1);
			OPTIMAL_TRACE[i - j - 1] = i + 1;
		}
		else
		{
			/* if this address has shown up before */
			/* the difference between two access time is its rank value */
			OPTIMAL_TRACE[i - j - 1] = _result->value - (i - j - 1);
			_result->value = i - j - 1;
		}
#ifdef DBG
		fprintf(debug_fp, "%llu ", OPTIMAL_TRACE[i - j - 1]);
#endif
	}
#ifdef DBG
	fprintf(debug_fp, "\n\n");
#endif
	free(trace);
	_rb_tree_clear(T);
}

/*
 *	Step3: Interpreter the address into tag, index
 */
void Interpret_Address(uint32_t level, uint64_t ADDR, uint64_t *tag, uint64_t *index)
{
	uint32_t tag_width = CACHE[level].CACHE_ATTRIBUTES.TAG_WIDTH;
	*tag = ADDR >> (64 - tag_width);
	*index = (ADDR << tag_width) >> (tag_width + BLOCK_OFFSET_WIDTH);
}

/*
 *	Step4: Rebuild address from tag and index
 */
uint64_t Rebuild_Address(uint32_t level, uint64_t tag, uint64_t index)
{
	uint64_t ADDR = 0;

	/* complete This part


*/

	return ADDR;
}

/*
 *	Step5: Search the cache of level "level"
 *	return the result: HIT or MISS
 *	if HIT, the hit way number will return by input parameter pointer "*way_num"
 */
uint8_t Cache_Search(uint32_t level, uint64_t tag, uint64_t index, uint32_t *way_num)
{
	uint32_t i, k = CACHE[level].CACHE_ATTRIBUTES.ASSOC;

	/* complete This part


*/
}

/*
 *	Step6: Maintain the rank array
 */
void Rank_Maintain(uint32_t level, uint64_t index, uint32_t way_num, uint8_t result, uint64_t rank_value)
{
	uint64_t *rank = CACHE[level].SET[index].RANK;
	switch (REPL_POLICY)
	{
	case FIFO:
		/* FIFO: only update when the data is allocated */
		if (result == MISS)
			rank[way_num] = rank_value;
		break;
	case PLRU:
	{

		/* Complete this part






  */

		break;
	}
	default:
		rank[way_num] = rank_value;
		break;
	}
}

/*
 *	Step7: Return the way number to be placed or replaced
 */
uint32_t Rank_Top(uint32_t level, uint64_t index)
{
	uint32_t i, assoc = CACHE[level].CACHE_ATTRIBUTES.ASSOC;
	/* we first use invalid block location */
	for (i = 0; i < assoc; i++)
		if (CACHE[level].SET[index].BLOCK[i].VALID_BIT == INVALID)
			return i;
	uint64_t *rank = CACHE[level].SET[index].RANK;
	switch (REPL_POLICY)
	{
	case PLRU:
	{

		/* Complete this part






  */
	}
	case OPTIMAL:
	{

		/* Complete this part






  */
	}
	default:
	{
		uint32_t k = 0;
		for (i = 0; i < assoc; i++)
			if (rank[i] < rank[k])
				k = i;
		return k;
	}
	}
}

/*
 *	Step 8: Evict one block from cache "level" set "index"
 *	to make space for the block to be allocated in
 */
uint32_t Cache_Evict(uint32_t level, uint64_t index)
{
	uint32_t way_num = Rank_Top(level, index);
	block *tmp = &(CACHE[level].SET[index].BLOCK[way_num]);
	/* if location is valid */
	if (tmp->VALID_BIT == VALID)
	{

		/* Complete this part






  */

		/* if location is dirty, it need to be written back */
		if (tmp->DIRTY_BIT == DIRTY)
		{

			/* Complete this part






  */
		}
		else
		{
			/* if this level and next level are exlusive relationship */
			/* any block evicted need to be written back */
			if (CACHE[level].CACHE_ATTRIBUTES.INCLUSION == EXCLUSIVE)
			{

				/* Complete this part






  */
			}
		}
		/* if this level and higher level cache are inclusive relationship */
		/* (the judgement is made inside Invalidation()) */
		/* we need to invalid the block in higher level cache */
		if (level > L1)
			Invalidation(level - 1, ADDR);
	}
	/* if location is invalid, we don't need to evict the block */
	return way_num;
}

/*
 *	Step9: Allocate (Place or Replace) block "blk" on cache "level", set "index", way "way_num"
 */
void Cache_Replacement(uint32_t level, uint64_t index, uint32_t way_num, block blk)
{
	CACHE[level].SET[index].BLOCK[way_num].VALID_BIT = VALID;
	CACHE[level].SET[index].BLOCK[way_num].TAG = blk.TAG;
	CACHE[level].SET[index].BLOCK[way_num].DIRTY_BIT = blk.DIRTY_BIT;
}

/*
 *	Step10: Send invalidation signal to cache "level" to invalid block with address "ADDR"
 */
void Invalidation(uint32_t level, uint64_t ADDR)
{
	/* store the cache level where invalidation signal comes from */
	/* we need to write the dirty data to that level cache */
	uint32_t level_invalidation_signal_from = level + 1;
	while (level >= L1)
	{
		switch (CACHE[level].CACHE_ATTRIBUTES.INCLUSION)
		{
		/* only when it is inclusive cache, we need to invalid the data */
		case INCLUSIVE:
		{

			/* Complete this part






  */
		}
		default:
			return;
		}
	}
}

/*
 *	Step11: read operation
 *	block content is sent back by input parameter "*blk"
 */
uint32_t Read(uint32_t level, uint64_t ADDR, block *blk, uint64_t rank_value)
{
	if (level >= NUM_LEVEL)
	{

		CACHE[NUM_LEVEL - 1].CACHE_STAT.num_blocks_transferred++;
		blk->DIRTY_BIT = CLEAN;
		blk->VALID_BIT = VALID;
		return 0;
	}
	/* update cache[level] statistics */
	CACHE[level].CACHE_STAT.num_access++;
	CACHE[level].CACHE_STAT.num_reads++;

	/* if we use Optimal Replacement Policy, the rank value should be passed down to every level */
	/* otherwise, rank_value should be the access time of this specif level cache */
	rank_value = (REPL_POLICY == OPTIMAL) ? rank_value : CACHE[level].CACHE_STAT.num_access;

	/* search this level cache */
	uint64_t tag, index;
	uint32_t way_num = 0;
	Interpret_Address(level, ADDR, &tag, &index);
	uint8_t result = Cache_Search(level, tag, index, &way_num);

	/* if read hit */
	if (result == HIT)
	{

		/* get the block */
		*blk = CACHE[level].SET[index].BLOCK[way_num];

		/* if read() was called by read()/write(), it means this level is not the top operation level */
		/* if this level and higher level are exclusive relationship */
		/* then we upload the block, and mark the block of this level as INVALID */
		if (level > L1 && CACHE[level - 1].CACHE_ATTRIBUTES.INCLUSION == EXCLUSIVE)
		{
			CACHE[level].SET[index].BLOCK[way_num].VALID_BIT = INVALID;

			return way_num;
		}
		/* otherwise, we upload the block of this level, and mark the block of higher level as CLEAN */
		blk->DIRTY_BIT = CLEAN;

		/* maintain the rank array */
		Rank_Maintain(level, index, way_num, HIT, rank_value);
	}
	else
	{

		/* if read miss */
		CACHE[level].CACHE_STAT.num_read_misses++;

		/* if this is the L1 level, or, the higher level and this level are not exclusive relationship */
		/* we need to load the block into this level cache */

		/* if we need to allocate block to this level, first make space for the block */
		if (level == L1 || CACHE[level - 1].CACHE_ATTRIBUTES.INCLUSION != EXCLUSIVE)
			way_num = Cache_Evict(level, index);

		/* then, we need read next lower level */
		Read(level + 1, ADDR, blk, rank_value);
		/* update the block tag */
		blk->TAG = tag;

		/* if this is the L1 level, or, the higher level and this level are not exclusive relationship */
		/* we need to load the block into this level cache */
		if (level == L1 || CACHE[level - 1].CACHE_ATTRIBUTES.INCLUSION != EXCLUSIVE)
		{
			Cache_Replacement(level, index, way_num, *blk);
			/* maintain the rank array */
			Rank_Maintain(level, index, way_num, MISS, rank_value);
		}
	}
	return way_num;
}

/*
 *	Step12: write operation
 */
void Write(uint32_t level, uint64_t ADDR, uint8_t dirty_bit, uint64_t rank_value)
{
	if (level >= NUM_LEVEL)
	{

		CACHE[NUM_LEVEL - 1].CACHE_STAT.num_blocks_transferred++;
		return;
	}
	/* update cache[level] statistics */

	/* Complete this part






  */

	/* if we use Optimal Replacement Policy, the rank value should be passed down to every level */
	/* otherwise, rank_value should be the access time of this specif level cache */
	rank_value = (REPL_POLICY == OPTIMAL) ? rank_value : CACHE[level].CACHE_STAT.num_access;
	/* search this level cache */
	uint64_t tag, index;
	uint32_t way_num;
	Interpret_Address(level, ADDR, &tag, &index);
	uint8_t result = Cache_Search(level, tag, index, &way_num);

	/* if write hit */
	if (result == HIT)
	{

		/* Complete this part






  */
	}
	else
	{

		/* if write miss */
		/* Complete this part






  */
	}
}

/*
 * Step13: free space allocated for cache
 */
void Cache_free()
{
	uint32_t i;
	for (i = 0; i < NUM_LEVEL; i++)
	{
		uint32_t j;
		for (j = 0; j < CACHE[i].CACHE_ATTRIBUTES.SET_NUM; j++)
		{
			free(CACHE[i].SET[j].BLOCK);
			free(CACHE[i].SET[j].RANK);
		}
		free(CACHE[i].SET);
	}
	free(CACHE);
}

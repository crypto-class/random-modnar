#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

typedef struct block
{
	size_t ulen;
	struct block *next;
	uint8_t u[1];
} block;
typedef struct pool
{
	block *blocks;
	void * (*alloc)(struct pool *p, size_t nbytes);
	void(*finish)(struct pool *p);
} pool;


void * pool_alloc(pool *p, size_t nbytes)
{
	size_t sz = nbytes + sizeof(block);
	assert(sz > nbytes);
	block *b = (block*)malloc(sz);
	if (!b)
	{
		perror("allocation failed");
		abort();
	}
	memset(b, 0, sz);

	b->ulen = nbytes;
	b->next = p->blocks;
	p->blocks = b;
	return b->u;
}

void pool_report(const pool *p)
{
	size_t nblocks = 0;
	size_t nbytes = 0;

	for (block *b = p->blocks;
	b != NULL;
		b = b->next)
	{
		nblocks++;
		nbytes += b->ulen;
	}

	printf("pool has %zu bytes in %zu blocks\n", nbytes, nblocks);
}

void pool_finish(pool *p)
{
	for (block *b = p->blocks;
	b != NULL;
		)
	{
		block *next = b->next;
		free(b);
		b = next;
	}

	memset(p, 0, sizeof(*p));
}

pool pool_create(void)
{
	pool r = { NULL, pool_alloc, pool_finish };
	return r;
}
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>

#include "common.h"

char *s_buf, *r_buf;
char s_buf_original[MYBUFSIZE];
char r_buf_original[MYBUFSIZE];

int skip = 1000;
int loop = 10000;

int64_t getMicrosecondTimeStamp() 
{
    int64_t retval;
    struct timeval tv; 
    if (gettimeofday(&tv, NULL)) {
        perror("gettimeofday");
        abort();
    }   
    retval = ((int64_t)tv.tv_sec) * 1000000 + tv.tv_usec;
    return retval;
}

void *align_buffer (void * ptr, unsigned long align_size)
{
    return (void *)(((unsigned long)ptr + (align_size - 1)) / align_size *
            align_size);
}

int allocate_memory (char ** sbuf, char ** rbuf)
{
    unsigned long align_size = getpagesize();
    assert(align_size <= MAX_ALIGNMENT);

    *sbuf = align_buffer(s_buf_original, align_size);
    *rbuf = align_buffer(r_buf_original, align_size);
    return 0;
}

void print_header ()
{
	fprintf(stderr, "%-*s%*s\n", 10, "# Size", FIELD_WIDTH, "Latency (us)");
}

void touch_data (void * sbuf, void * rbuf, size_t size)
{
        memset(sbuf, 'a', size);
        memset(rbuf, 'b', size);
}

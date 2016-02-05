#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

/* Usage:

   ./memcpy
*/

int main (int argc, char *argv[])
{
	int i;
	int size;
	double t_start = 0.0, t_end = 0.0;
	double latency;

	if (allocate_memory(&s_buf, &r_buf)) {
		/* Error allocating memory */
		exit(-1);
	}

	print_header();

	/* Latency test */
	for(size = 0; size <= MAX_MSG_SIZE; size = (size ? size * 2 : 1)) {
		touch_data(s_buf, r_buf, size);

		if(size > LARGE_MESSAGE_SIZE) {
			loop = LOOP_LARGE;
			skip = SKIP_LARGE;
		}

		for(i = 0; i < loop + skip; i++) {
			if(i == skip)
				t_start = TIME();
			memcpy(s_buf, r_buf, size);
		}
		t_end = TIME();
		latency = (1.0 * (t_end-t_start)) / loop;
		fprintf(stderr, "%-*d%*.*f\n", 10, size, FIELD_WIDTH,
			FLOAT_PRECISION, latency);
	}
	return 0;
}

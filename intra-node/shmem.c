#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <fcntl.h>
#include <errno.h>

#include "common.h"

/* Usage:

   - Server:
   ./shmem

   - Client:
   ./shmem <some string>
*/

char *shm_name = "bench";
void *shm_ptr;
int shm_fd;

void *allocate_shmem(int is_server)
{
	uint64_t flags = O_RDWR;

	if (is_server)
		flags |= O_CREAT;

	shm_fd = shm_open(shm_name, flags, S_IRUSR | S_IWUSR);
	if (shm_fd < 0) {
		fprintf(stderr, "shm_open failed\n");
		return NULL;
	}

	if (ftruncate(shm_fd, MAX_MSG_SIZE * 2) == -1) {
		fprintf(stderr, "ftruncate failed\n");
		shm_unlink(shm_name);
		close(shm_fd);
		return NULL;
	}

	shm_ptr = mmap(NULL, MAX_MSG_SIZE * 2, PROT_READ | PROT_WRITE,
		       MAP_SHARED, shm_fd, 0);
	if (shm_ptr == MAP_FAILED) {
		fprintf(stderr, "mmap failed\n");
		shm_unlink(shm_name);
		close(shm_fd);
		return NULL;
	}
	return shm_ptr;
}

void free_shmem()
{
	shm_unlink(shm_name);
	if (munmap(shm_ptr, MAX_MSG_SIZE * 2)) {
		fprintf(stderr, "munmap failed: %s\n", strerror(errno));
	}
	close(shm_fd);
}

int main (int argc, char *argv[])
{
	int i;
	int size, is_server;
	double t_start = 0.0, t_end = 0.0;
	double latency;

	is_server = (argc == 1);
	if ((shm_ptr = allocate_shmem(is_server)) == NULL) {
		fprintf(stderr, "error in allocating shmem\n");
		exit(-1);
	}

	if (is_server) {
		fprintf(stderr, "server ready\n");
		while (1);
	}

	s_buf = shm_ptr;
	r_buf = (char *)shm_ptr + MAX_MSG_SIZE;

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
	free_shmem();
	return 0;
}

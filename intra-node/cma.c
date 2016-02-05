#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/uio.h>

#include "common.h"

/* Usage:

   - Server:
   ./cma
   (will print <pid> and <remote_memory>)

   - Client:
   ./cma <pid> <remote_memory>
*/

ssize_t process_vm_writev(pid_t pid,
				  const struct iovec *lvec,
				  unsigned long liovcnt,
				  const struct iovec *rvec,
				  unsigned long riovcnt,
				  unsigned long flags);

int main (int argc, char *argv[])
{
	int i, ret;
	int size, is_server;
	double t_start = 0.0, t_end = 0.0;
	double latency;
	struct iovec s_iov, r_iov;
	uintptr_t peer_addr;
	int peer_pid;
	
	if (allocate_memory(&s_buf, &r_buf)) {
		/* Error allocating memory */
		exit(-1);
	}
	
	is_server = (argc == 1);
	if (is_server) {
		fprintf(stderr, "server ready: %d, %p\n", getpid(), r_buf);
		while (1);
	}
	
	peer_pid = atoi(argv[1]);
	peer_addr = (uintptr_t) strtol(argv[2], NULL, 16);
	
	s_iov.iov_base = s_buf;
	r_iov.iov_base = (void *) peer_addr;
	
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

			s_iov.iov_len = r_iov.iov_len = size;
			ret = process_vm_writev(peer_pid,
						&s_iov, 1, 
						&r_iov, 1, 0);
			if (ret != size) {
				fprintf(stderr, "process_vm_writev failed: %d\n", ret);
				exit(-1);
			}
		}
		t_end = TIME();
		latency = (1.0 * (t_end-t_start)) / loop;
		fprintf(stderr, "%-*d%*.*f\n", 10, size, FIELD_WIDTH,
			FLOAT_PRECISION, latency);
	}
	return 0;
}

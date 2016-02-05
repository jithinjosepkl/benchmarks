all: memcpy shmem cma

COMMON=common.c common.h
CFLAGS=-Wall -O2

memcpy: memcpy.c $(COMMON)
	gcc $(CFLAGS) common.c memcpy.c -o $@

shmem: shmem.c $(COMMON)
	gcc $(CFLAGS) common.c shmem.c -lrt -o $@

cma: cma.c $(COMMON)
	gcc $(CFLAGS) common.c cma.c -o $@

clean:
	rm memcpy shmem cma

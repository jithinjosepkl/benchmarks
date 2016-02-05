#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef FIELD_WIDTH
#   define FIELD_WIDTH 20
#endif

#ifndef FLOAT_PRECISION
#   define FLOAT_PRECISION 2
#endif

#define MESSAGE_ALIGNMENT 64
#define MAX_ALIGNMENT 65536
#define MAX_MSG_SIZE (1<<22)
#define MYBUFSIZE (MAX_MSG_SIZE + MAX_ALIGNMENT)

#define LOOP_LARGE  100
#define SKIP_LARGE  10
#define LARGE_MESSAGE_SIZE  8192

#define TIME() getMicrosecondTimeStamp()

extern char *s_buf, *r_buf;
extern char s_buf_original[MYBUFSIZE];
extern char r_buf_original[MYBUFSIZE];
extern int skip, loop;

int64_t getMicrosecondTimeStamp();

int allocate_memory (char **sbuf, char **rbuf);
void print_header ();
void touch_data (void *sbuf, void *rbuf, size_t size);

#endif /* _COMMON_H */

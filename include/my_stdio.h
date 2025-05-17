#ifndef _MY_STDIO_H_
#define _MY_STDIO_H_

#include <stddef.h>  // for size_t

// Custom EOF
#define MY_EOF (-1)

#define MY_BUFSIZ 8192  // Buffer size

// Buffering modes
#define MY_IONBF 0
#define MY_IOLBF 1
#define MY_IOFBF 2

// File seeking
#define MY_SEEK_SET 0
#define MY_SEEK_CUR 1
#define MY_SEEK_END 2

// Define your own FILE struct
typedef struct _MY_FILE {
    int fd;
    char *buffer;
    size_t bufsize;
    size_t pos;
    size_t data_size;
    int mode;         // read = 0, write = 1, etc.
    int eof;
    int error;
    int bufown;
    int read_mode;
    int write_mode;
} MY_FILE;

// Function declarations
MY_FILE *my_fopen(const char *filename, const char *mode);
int my_fflush(MY_FILE *stream);
void my_setbuf(MY_FILE *stream, char *buf);
int my_setvbuf(MY_FILE *stream, char *buf, int mode, size_t size);
int my_fpurge(MY_FILE *stream);
size_t my_fread(void *ptr, size_t size, size_t count, MY_FILE *stream);
size_t my_fwrite(const void *ptr, size_t size, size_t count, MY_FILE *stream);
int my_fgetc(MY_FILE *stream);
int my_fputc(int c, MY_FILE *stream);
char *my_fgets(char *str, int n, MY_FILE *stream);
int my_fputs(const char *str, MY_FILE *stream);
int my_fseek(MY_FILE *stream, long offset, int whence);
int my_feof(MY_FILE *stream);
int my_fclose(MY_FILE *stream);

// Optional: basic printf imitation
int my_printf(const char *format, ...);

#endif // _MY_STDIO_H_

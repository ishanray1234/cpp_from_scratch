#include "../include/my_stdio.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Minimal itoa: converts an int to a string (base 10)
int itoa(int val, char *buf) {
    char temp[12]; // Enough for 32-bit int
    int i = 0, j, is_negative = 0;

    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return 1;
    }

    if (val < 0) {
        is_negative = 1;
        val = -val;
    }

    while (val != 0) {
        temp[i++] = (val % 10) + '0';
        val /= 10;
    }

    if (is_negative) {
        temp[i++] = '-';
    }

    // Reverse the string into buf
    for (j = 0; j < i; ++j) {
        buf[j] = temp[i - j - 1];
    }
    buf[j] = '\0';

    return i;
}

int ftoa(float num, char *buf, int precision) {
    int len = 0;

    // Handle negative numbers
    if (num < 0) {
        buf[len++] = '-';
        num = -num;
    }

    // Extract integer part
    int int_part = (int)num;
    float fraction = num - (float)int_part;

    // Convert integer part
    char int_buf[20];
    int int_len = itoa(int_part, int_buf);
    for (int i = 0; i < int_len; ++i)
        buf[len++] = int_buf[i];

    buf[len++] = '.';

    // Convert fractional part
    for (int i = 0; i < precision; ++i) {
        fraction *= 10;
        int digit = (int)fraction;
        buf[len++] = '0' + digit;
        fraction -= digit;
    }

    buf[len] = '\0';
    return len;
}

// Open a file
MY_FILE *my_fopen(const char *filename, const char *mode) {
    int flags;
    if (strcmp(mode, "r") == 0) {
        flags = O_RDONLY;
    } else if (strcmp(mode, "w") == 0) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "a") == 0) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else {
        return NULL;
    }

    int fd = open(filename, flags, 0644);
    if (fd < 0) return NULL;

    MY_FILE *stream = (MY_FILE *)malloc(sizeof(MY_FILE));
    if (!stream) return NULL;

    stream->fd = fd;
    stream->buffer = (char *)malloc(MY_BUFSIZ);
    stream->bufsize = MY_BUFSIZ;
    stream->pos = 0;
    stream->data_size = 0;
    stream->eof = 0;
    stream->error = 0;
    stream->bufown = 1;
    stream->read_mode = (flags == O_RDONLY);
    stream->write_mode = !stream->read_mode;

    return stream;
}

// Flush the buffer
int my_fflush(MY_FILE *stream) {
    if (stream->write_mode && stream->pos > 0) {
        ssize_t written = write(stream->fd, stream->buffer, stream->pos);
        if (written < 0) {
            stream->error = 1;
            return MY_EOF;
        }
        stream->pos = 0;
    }
    return 0;
}

// Set buffering (no-op for simplicity)
void my_setbuf(MY_FILE *stream, char *buf) {
    my_setvbuf(stream, buf, buf ? MY_IOFBF : MY_IONBF, MY_BUFSIZ);
}

// Set buffering
int my_setvbuf(MY_FILE *stream, char *buf, int mode, size_t size) {
    (void)mode; // Ignore mode for simplicity
    if (stream->bufown) free(stream->buffer);
    stream->buffer = buf ? buf : (char *)malloc(size);
    stream->bufown = buf ? 0 : 1;
    stream->bufsize = size;
    stream->pos = 0;
    return 0;
}

// Clear buffer (simple implementation)
int my_fpurge(MY_FILE *stream) {
    stream->pos = 0;
    stream->data_size = 0;
    return 0;
}

// Read from file
size_t my_fread(void *ptr, size_t size, size_t count, MY_FILE *stream) {
    size_t total = size * count;
    ssize_t n = read(stream->fd, ptr, total);
    if (n == 0) stream->eof = 1;
    if (n < 0) {
        stream->error = 1;
        return 0;
    }
    return n / size;
}

// Write to file
size_t my_fwrite(const void *ptr, size_t size, size_t count, MY_FILE *stream) {
    size_t total = size * count;
    ssize_t n = write(stream->fd, ptr, total);
    if (n < 0) {
        stream->error = 1;
        return 0;
    }
    return n / size;
}

// Get a character
int my_fgetc(MY_FILE *stream) {
    char c;
    ssize_t n = read(stream->fd, &c, 1);
    if (n == 0) {
        stream->eof = 1;
        return MY_EOF;
    } else if (n < 0) {
        stream->error = 1;
        return MY_EOF;
    }
    return (unsigned char)c;
}

// Put a character
int my_fputc(int c, MY_FILE *stream) {
    char ch = (char)c;
    if (write(stream->fd, &ch, 1) != 1) {
        stream->error = 1;
        return MY_EOF;
    }
    return c;
}

// Read a line
char *my_fgets(char *str, int n, MY_FILE *stream) {
    int i = 0;
    while (i < n - 1) {
        int c = my_fgetc(stream);
        if (c == MY_EOF) break;
        str[i++] = c;
        if (c == '\n') break;
    }
    str[i] = '\0';
    return (i == 0 && stream->eof) ? NULL : str;
}

// Write a string
int my_fputs(const char *str, MY_FILE *stream) {
    size_t len = strlen(str);
    return (my_fwrite(str, 1, len, stream) == len) ? 0 : MY_EOF;
}

// Seek
int my_fseek(MY_FILE *stream, long offset, int whence) {
    off_t pos = lseek(stream->fd, offset, whence);
    if (pos == (off_t)-1) {
        stream->error = 1;
        return -1;
    }
    stream->pos = 0;
    stream->eof = 0;
    return 0;
}

// End of file?
int my_feof(MY_FILE *stream) {
    return stream->eof;
}

// Close file
int my_fclose(MY_FILE *stream) {
    my_fflush(stream);
    if (stream->bufown) free(stream->buffer);
    int result = close(stream->fd);
    free(stream);
    return result;
}

// Basic printf to stdout (only supports %s and %d)
int my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int total = 0;
    for (const char *p = format; *p; ++p) {
        if (*p == '%' && *(p + 1)) {
            ++p;
            if (*p == 's') {
                char *str = va_arg(args, char *);
                write(1, str, strlen(str));
                total += strlen(str);
            } else if (*p == 'd') {
                int val = va_arg(args, int);
                char buf[32];
                int len = itoa(val, buf);
                write(1, buf, len);
                total += len;
            } else if (*p == 'c') {
                char ch = (char)va_arg(args, int);
                write(1, &ch, 1);
                total++;
            } else if (*p == 'f') {
                float fval = (float)va_arg(args, double);
                char buf[32];
                int len = ftoa(fval, buf, 2); // 2 decimal places
                write(1, buf, len);
                total += len;
            } else {
                write(1, p - 1, 2);
                total += 2;
            } 
        } else {
            write(1, p, 1);
            total++;
        }
    }
    va_end(args);
    return total;
}

/* utils.c */

/*
 * read stream until EOF
 */

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

char *read_until_eof(FILE *stream) {
	// alloc
	off_t len = 0;
	char buf[1024];
	char *str = (char *)malloc(1);
	assert(str);

	// read
	while (!feof(stream)) {
		size_t n = fread(buf, 1, 1024, stream);
		len += strlen(buf);
		str = (char *)realloc(str, len);
		strcat(str, buf);
	}

	return str;
}
/* lexer.h */

#ifndef LEXER_H
#define LEXER_H

#include<sys/types.h>

#include"token.h"


#ifndef JSON_BUF_SIZE
#define JSON_BUF_SIZE 1024
#endif

typedef struct {
	char *error;
	int stash;
	const char *filename;
	char *source;
	int lineno;
	off_t offset;
	json_token_t tok;
	char buf[JSON_BUF_SIZE];
}json_lexer_t;

/*
 * protos
 */

int json_scan(json_lexer_t *self);

void json_lexer_init(json_lexer_t *self, char *source, const char *filename);

#endif // LEXER_H
/* lexer.c */

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

#include"lexer.h"

/*
 * next character in the array
 */

#ifdef EBUG_LEXER
#define next \
	(self->stash = self->source[self->offset++]\
	, fprintf(stderr, "'%c'\n", self->stash)\
	, self->stash)
#else
#define next (self->stash = self->source[self->offset++])
#endif

/*
 * undo the previous character
 */

#define undo (self->source[--self->offset] = self->stash)

/*
 * assign token 't'
 */

#define token(t) (self->tok.type = JSON_TOKEN_##t)

/*
 * set error 'msg' and assign ILLEGAL token
 */

#define error(msg) (self->error = msg, token(ILLEGAL))



/*
 * initialize lexer with the given 'source' and 'filename'
 */

void json_lexer_init(json_lexer_t *self, char *source, const char *filename) {
	self->error = NULL;
	self->filename = filename;
	self->source = source;
	self->lineno = 1;
	self->offset = 0;
}

/*
 * scan identifier
 */

static int scan_ident(json_lexer_t *self, int c) {
	int len = 0;
	char buf[128];
	token(ID);

	do {
		buf[len++] = c;
	} while (isalpha(c = next) || isdigit(c) || '_' == c);
	undo;

	buf[len++] = 0;
	switch(len - 1) {
	case 4:
		if (0 == strcmp("true", buf)) return token(TRUE);
		if (0 == strcmp("null", buf)) return token(NULL_LITERAL);
		break;
	default:
		if (0 == strcmp("false", buf)) return token(FALSE);
	}

	self->tok.value.as_string = strdup(buf);
	return 1;
}

/*
 * scan number
 */

static int scan_number(json_lexer_t *self, int c) {

}

/*
 * convert hex digit 'c' to a base 10 int, returning -1 on failure.
 */

static int hex(const char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;

	return -1;
}


/*
 * scan string hex literal, returning -1 on invalid digits
 */

static int hex_literal(json_lexer_t *self) {
	int a = hex(next);
	int b = hex(next);

	// 4 hexadecimal digits

	if (a > -1 || b > -1)
		return a << 4 | b;
	error("string hex literal \\u contains invalid digits");

	return -1;
}

/*
 * scan string
 */

static int scan_string(json_lexer_t *self, int quote) {
	int c, len = 0;
	char buf[128];
	token(STRING);

	while (quote != (c = next)) {
		switch (c) {
		case '\n': ++self->lineno; break;
		case '\\': 
			switch (c = next) {
			case 'b': c = '\b'; break;
			case 'n': c = '\n'; break;
			case '\\': c = '\\'; break;
			case '\/': c = '\/'; break;
			case 'f': c = '\f'; break;
			case 'r': c = '\r'; break;
			case 't': c = '\t'; break;
			case '"': c = '\"'; break;
			case 'u':
				if (-1 == (c = hex_literal(self)))
					return 0;
			}
			break;
		}
		buf[len++] = c;
	}

	buf[len++] = 0;
	
	return 1;
}

/*
 * scan the next token in the stream, return 0 on EOS, 
 * ILLEGAL token, or a syntax error
 */

int json_scan(json_lexer_t *self) {
	int c;
	token(ILLEGAL);

	// scan
scan:
	switch(c = next) {
	case ' ':
	case '\t': goto scan;
	case '{': return token(LBRACE);
	case '}': return token(RBRACE);
	case '[': return token(LBRACK);
	case ']': return token(RBRACK);
	case ',': return token(COMMA);
	case ':': return token(COLON);
	case '\n':
	case '\r':
		++self->lineno;
		goto scan;
	case '"':
		return scan_string(self, c);
	case 0:
		token(EOS);
		return 0;
	default:
		error("illegal character");
		return 0;
	}
}
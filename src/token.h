/* token.h */

#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include<assert.h>

/*
 * tokens
 */

#define JSON_TOKEN_LIST \
	t(ILLEGAL, "illegal") \
	t(EOS, "end-of-stream") \
	t(ID, "id") \
	t(STRING, "string") \
	t(TRUE, "true") \
	t(FALSE, "false") \
	t(NULL_LITERAL, "null") \
	t(LBRACK, "[") \
	t(RBRACK, "]") \
	t(LBRACE, "{") \
	t(RBRACE, "}") \
	t(COMMA, ",") \
	t(COLON, ":")

/*
 * tokens enum
 */

typedef enum {
#define t(tok, str) JSON_TOKEN_##tok,
	JSON_TOKEN_LIST
	NUM_TOKENS
#undef t
}json_token;

/*
 * token strings
 */

static char *json_token_strings[] = {
#define t(tok, str) str,
	JSON_TOKEN_LIST
#undef t
};

/*
 * token struct
 */

typedef struct {
	int len;
	json_token type;
	struct {
		char *as_string;

	}value;
}json_token_t;

/*
 * return the string associated with the 
 * given token 'type'
 */

// TODO: use inline
static const char *json_token_type_string(json_token type) {
	assert(type < NUM_TOKENS);  // token unsigned
	return json_token_strings[type];
}

/*
 * protos
 */

void json_token_inspect(json_token_t *tok);

#endif // JSON_TOKEN_H
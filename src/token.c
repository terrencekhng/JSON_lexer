/* token.c */

#include<stdio.h>

#include"token.h"

/*
 * inspect the given 'tok', outputting
 * debugging information to stdout
 */

void json_token_inspect(json_token_t *tok) {
	printf(" inspect===> %s", json_token_type_string(tok->type));

	printf("\n");
}

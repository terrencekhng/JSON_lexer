/* test_token.c */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#include"../src/token.h"
#include"../src/lexer.h"
#include"../src/utils.h"

static int tokens = 0;

const char **parse_args(int *argc, const char **argv) {
  const char *arg, **args = argv;

  int i, len;
  for (i = 0, len = *argc; i < len; ++i) {
    arg = args[i];

    if (!strcmp("-T", arg) || !strcmp("--tokens", arg)) {
      tokens = 1;
      --*argc;
      ++argv;
    } else if ('-' == arg[0]) {
      fprintf(stderr, "unknown flag %s\n", arg);
      exit(1);
    }
  }

  return argv;
}

int eval(char *source, const char *path) {
  json_lexer_t lex;
  json_lexer_init(&lex, source, path);

  if (tokens) {
    while (json_scan(&lex)) {
      printf(" %d ", lex.lineno);
      json_token_inspect(&lex.tok);
    }
    return 0;
  }
}

int main(int argc, const char **argv)
{
  char *source;

  // parse arguments
  argv = parse_args(&argc, argv);

  // eval stdin
  if (1 == argc) {
    source = read_until_eof(stdin);
    return eval(source, "stdin");
  }
}

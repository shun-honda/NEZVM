#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>

#include "nezvm.h"

static void nez_ShowUsage(const char *file) {
  fprintf(stderr, "\npegvm <command> optional files\n");
  fprintf(stderr, "  -p <filename> Specify an PEGs grammar bytecode file\n");
  fprintf(stderr, "  -i <filename> Specify an input file\n");
  fprintf(stderr, "  -o <filename> Specify an output file\n");
  fprintf(stderr, "  -t <type>     Specify an output type\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  const char *syntax_file = NULL;
  const char *input_file = NULL;
  const char *output_type = NULL;
  const char *output_file = NULL;
  const char *orig_argv0 = argv[0];
  int opt;
  while ((opt = getopt(argc, argv, "p:i:t:o:h:")) != -1) {
    switch (opt) {
    case 'p':
      syntax_file = optarg;
      break;
    case 'i':
      input_file = optarg;
      break;
    case 't':
      output_type = optarg;
      break;
    case 'o':
      output_file = optarg;
      break;
    case 'h':
      nez_ShowUsage(orig_argv0);
    default: /* '?' */
      nez_ShowUsage(orig_argv0);
    }
  }
  nez_context *context = nez_create_context(input_file);
  nez_expression *expr = nez_load_grammar(syntax_file);
  nezvm_instruction *inst = nezvm_compile(expr, "File");

  nez_node *ast = nez_parse(context, inst);

  nez_dispose_node(ast);
  nezvm_dispose_instruction(inst);
  nez_dispose_expression(expr);
  nez_dispose_context(context);
  return 0;
}
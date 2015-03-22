#include "nezvm.h"

nez_context* nez_create_context(char *input_file) {
  nez_context* ctx = (nez_context*)malloc(sizeof(nez_context));
  ctx->pos = ctx->input_size = 0;
  ctx->mpool = (memory_pool*)malloc(sizeof(struct memory_pool));
  ctx->inputs = load_file(filename, &ctx->input_size);
  ctx->stacked_symbol_table =
      (nez_symbol_table_entry*)malloc(sizeof(nez_symbol_table_entry) * 256);
  ctx->stack_pointer_base =
      (long *)malloc(sizeof(long) * NEZVM_MAX_STACK_LENGTH);
  ctx->node_stack_pointer_base = (nez_ast **)malloc(
      sizeof(nez_ast*) * NEZVM_MAX_STACK_LENGTH);
  ctx->call_stack_pointer_base = (nezvm_instruction **)malloc(
      sizeof(nezvm_instruction *) * NEZVM_MAX_STACK_LENGTH);
  ctx->stack_pointer = &ctx->stack_pointer_base[0];
  ctx->node_stack_pointer = &ctx->node_stack_pointer_base[0];
  ctx->call_stack_pointer = &ctx->call_stack_pointer_base[0];
  return ctx;
}

void nez_dispose_context(nez_context* ctx) {
  free(ctx->inputs);
  free(ctx->mpool);
  free(ctx->stacked_symbol_table);
  free(ctx->call_stack_pointer_base);
  free(ctx->stack_pointer_base);
  free(ctx->node_stack_pointer_base);
  free(ctx);
}
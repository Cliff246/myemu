#ifndef __ME_ASSEMBLER__
#define __ME_ASSEMBLER__

#include <stdlib.h>

static size_t errorcount = 0;
void splitline(char **splitline, char tosplitby,  char *baseline);

void assemble(char *dir, size_t size);
#endif
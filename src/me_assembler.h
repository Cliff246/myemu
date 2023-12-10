#ifndef __ME_ASSEMBLER__
#define __ME_ASSEMBLER__

#include <stdlib.h>

void splitline(char **splitline, char tosplitby,  char *baseline);

void assemble(char *dir, size_t size);
#endif
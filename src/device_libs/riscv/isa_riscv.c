#include "commons.h"
#include "isa_riscv.h"

#ifdef _WIN32
  #define ISA_API __declspec(dllexport)
#else
  #define ISA_API __attribute__((visibility("default")))
#endif



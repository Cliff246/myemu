#include "commons.h"
#include "device_descriptor.h"
#include "isa_riscv.h"

#ifdef _WIN32
  #define ISA_API __declspec(dllexport)
#else
  #define ISA_API __attribute__((visibility("default")))
#endif


static devdesc_t riscv_descriptor = 
{
    .id = 0x01,
    .device_name = "riscv",
    .device_typeclass = "cpu",
    .lib_name = "riscv_isa",
    .flags = { false },  // or fill in your flags
    .carries_emulation_core = true,
    .carries_assembly_core = true,

    .create_device = NULL,  // replace with your real creation function later
    .free_device = NULL,    // same here

    .device_interface = NULL,        // optional for now
    .device_assembler_core = NULL, // 
    .device_emulator_core = NULL,  // your emu vtable

    .extras = NULL
};


const devdesc_t *get_device_descriptor(void)
{
    return &riscv_descriptor;
}

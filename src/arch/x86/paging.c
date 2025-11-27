#include <arch/x86/paging.h>

#include <stdint.h>

static uint32_t paging_directory[1024] __attribute__((aligned(4096)));
static uint32_t paging_table[1024] __attribute__((aligned(4096)));

void paging_init()
{
    int i;

    for (i = 0; i < 1024; i++)
    {
        paging_directory[i] = 2; // read-write, not present
    }

    for (i = 0; i < 1024; i++)
    {
        paging_table[i] = (i * 0x1000) | 3;
    }

    paging_directory[0] = (uint32_t)paging_table | 3;

    __asm__ volatile(
        "mov %0, %%eax\n"
        "mov %%eax, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "or 0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n"
        :
        : "m"(paging_directory)
        : "eax"
    );
}
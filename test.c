#ifndef TEST_PSP
    #define TEST_PSP 1
#endif

#ifndef TEST_FPU
    #define TEST_FPU 0
#endif

#include <stdint.h>

extern void _stack(void);

#define CPACR   (*(volatile uint32_t *)0xE000ED88)

asm(
"    .global _stack\n"
"    _stack = 0x20001000"
   );

static void die(void)
{
    for (;;)
        asm("bkpt #1");
}

static void enable_fpu(void)
{
    CPACR = 0xF << 20;
}

static void use_psp(void)
{
    asm(
    "    mrs   r0,  MSP \n"
    "    msr   PSP, r0 \n"
    "    mrs   r0,  CONTROL \n"
    "    orrs  r0,  #2 \n"
    "    msr   CONTROL, r0 \n"
       );
}

static void some_function(void)
{
    asm("svc #0");
}

void _reset(void)
{
#if TEST_FPU
    static volatile float f;
    enable_fpu();
    f = 1.f;
    f += 2.f;
#endif
#if TEST_PSP
    use_psp();
#endif
    some_function();
    die();
}

void svcall(void)
{
    asm("bkpt #0");
}

__attribute__((section("isr_vector")))
void (*const exception_table[])(void) =
{
    _stack,
    _reset,
    die,        // NMI
    die,        // Hardfault
    die,        // MemManage
    die,        // BusFault
    die,        // UsageFault
    0,0, 0, 0,  // Reserved [7-10]
    svcall,     // 
};

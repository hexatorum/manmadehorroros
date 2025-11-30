#include <arch/x86/interrupts.h>

#include <stdbool.h>

#include <arch/x86/io.h>
#include <arch/x86/pic.h>
#include <stdint.h>

#define IDT_FLAGS_TRAP 0b10001111
#define IDT_FLAGS_INTERRUPT 0b10001110

__attribute__((noreturn))
static void interrupts_exception(uint8_t interrupt);
static void interrupts_handle(uint8_t interrupt);

#define ISR_ERROR(index, name) \
    __attribute__((naked)) \
    static void isr_##name() \
    { \
        __asm__( \
            "pushl %0\n" \
            "call %P1\n" \
            "add $4, %%esp\n" \
            "iret\n" \
            : \
            : "i"(index), "i"(interrupts_exception) \
        ); \
    }
#define ISR_NO_ERROR(index, name) \
    __attribute__((naked)) \
    static void isr_##name() \
    { \
        __asm__( \
            "pushl %0\n" \
            "call %P1\n" \
            "add $4, %%esp\n" \
            "iret\n" \
            : \
            : "i"(index), "i"(interrupts_handle) \
        ); \
    }

static idt_entry_t idt[256] = {};
static idtr_t idtr = {};

static interrupt_handler_t handlers[256] = {};

ISR_NO_ERROR(0, divide_error)
ISR_NO_ERROR(1, debug)
ISR_NO_ERROR(2, nmi)
ISR_NO_ERROR(3, breakpoint)
ISR_NO_ERROR(4, overflow)
ISR_NO_ERROR(5, bound_range_exceeded)
ISR_NO_ERROR(6, invalid_opcode)
ISR_NO_ERROR(7, device_unavailable)
ISR_ERROR(8, double_fault)
ISR_NO_ERROR(9, coprocessor_overrun)
ISR_ERROR(10, invalid_tss)
ISR_ERROR(11, segment_not_present)
ISR_ERROR(12, stack_segment_fault)
ISR_ERROR(13, protection_fault)
ISR_NO_ERROR(14, page_fault)
ISR_NO_ERROR(16, floating_point_error)
ISR_ERROR(17, alignment_check)
ISR_NO_ERROR(18, machine_check)
ISR_NO_ERROR(19, simd_exception)
ISR_NO_ERROR(20, virtualization_exception)
ISR_ERROR(21, control_protection_exception)

ISR_NO_ERROR(32, irq_pit)
ISR_NO_ERROR(33, irq_keyboard)
ISR_NO_ERROR(34, irq_cascade)
ISR_NO_ERROR(35, irq_com2)
ISR_NO_ERROR(36, irq_com1)
ISR_NO_ERROR(37, irq_lpt2)
ISR_NO_ERROR(38, irq_floppy)
ISR_NO_ERROR(39, irq_lpt1)
ISR_NO_ERROR(40, irq_cmos)
ISR_NO_ERROR(41, irq_peripheral1)
ISR_NO_ERROR(42, irq_peripheral2)
ISR_NO_ERROR(43, irq_peripheral3)
ISR_NO_ERROR(44, irq_mouse)
ISR_NO_ERROR(45, irq_fpu)
ISR_NO_ERROR(46, irq_primary_hdd)
ISR_NO_ERROR(47, irq_secondary_hdd)

__attribute__((noreturn))
static void interrupts_exception(uint8_t interrupt)
{
    if (handlers[interrupt]) handlers[interrupt]();
}

static void interrupts_handle(uint8_t interrupt)
{   
    if (handlers[interrupt]) handlers[interrupt]();

    if (interrupt >= 32 && interrupt <= 47)
        irq_eoi(interrupt - 32);
}

void interrupts_set(uint8_t interrupt, interrupt_handler_t handler)
{
    handlers[interrupt] = handler;
}

void interrupts_enable()
{
    __asm__("sti");
}

void interrupts_disable()
{
    __asm__("cli");
}

static void idt_set(uint8_t vector, void *handler, uint8_t flags)
{
    idt_entry_t *entry = &idt[vector];

    entry->base_low = (uint32_t)handler & 0xFFFF;
    entry->base_high = ((uint32_t)handler >> 16) & 0xFFFF;
    entry->segment = 0x08;
    entry->attributes = flags;
    entry->_unused = 0;
}

void idt_init()
{
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)&idt;

    idt_set(0, isr_divide_error, IDT_FLAGS_TRAP);
    idt_set(1, isr_debug, IDT_FLAGS_TRAP);
    idt_set(2, isr_nmi, IDT_FLAGS_TRAP);
    idt_set(3, isr_breakpoint, IDT_FLAGS_TRAP);
    idt_set(4, isr_overflow, IDT_FLAGS_TRAP);
    idt_set(5, isr_bound_range_exceeded, IDT_FLAGS_TRAP);
    idt_set(6, isr_invalid_opcode, IDT_FLAGS_TRAP);
    idt_set(7, isr_device_unavailable, IDT_FLAGS_TRAP);
    idt_set(8, isr_double_fault, IDT_FLAGS_TRAP);
    idt_set(9, isr_coprocessor_overrun, IDT_FLAGS_TRAP);
    idt_set(10, isr_invalid_tss, IDT_FLAGS_TRAP);
    idt_set(11, isr_segment_not_present, IDT_FLAGS_TRAP);
    idt_set(12, isr_stack_segment_fault, IDT_FLAGS_TRAP);
    idt_set(13, isr_protection_fault, IDT_FLAGS_TRAP);
    idt_set(14, isr_page_fault, IDT_FLAGS_TRAP);
    idt_set(16, isr_floating_point_error, IDT_FLAGS_TRAP);
    idt_set(17, isr_alignment_check, IDT_FLAGS_TRAP);
    idt_set(18, isr_machine_check, IDT_FLAGS_TRAP);
    idt_set(19, isr_simd_exception, IDT_FLAGS_TRAP);
    idt_set(20, isr_virtualization_exception, IDT_FLAGS_TRAP);
    idt_set(21, isr_control_protection_exception, IDT_FLAGS_TRAP);

    idt_set(32, isr_irq_pit, IDT_FLAGS_INTERRUPT);
    idt_set(33, isr_irq_keyboard, IDT_FLAGS_INTERRUPT);
    idt_set(34, isr_irq_cascade, IDT_FLAGS_INTERRUPT);
    idt_set(35, isr_irq_com2, IDT_FLAGS_INTERRUPT);
    idt_set(36, isr_irq_com1, IDT_FLAGS_INTERRUPT);
    idt_set(37, isr_irq_lpt1, IDT_FLAGS_INTERRUPT);
    idt_set(38, isr_irq_floppy, IDT_FLAGS_INTERRUPT);
    idt_set(39, isr_irq_lpt1, IDT_FLAGS_INTERRUPT);
    idt_set(40, isr_irq_cmos, IDT_FLAGS_INTERRUPT);
    idt_set(41, isr_irq_peripheral1, IDT_FLAGS_INTERRUPT);
    idt_set(42, isr_irq_peripheral2, IDT_FLAGS_INTERRUPT);
    idt_set(43, isr_irq_peripheral3, IDT_FLAGS_INTERRUPT);
    idt_set(44, isr_irq_mouse, IDT_FLAGS_INTERRUPT);
    idt_set(45, isr_irq_fpu, IDT_FLAGS_INTERRUPT);
    idt_set(46, isr_irq_primary_hdd, IDT_FLAGS_INTERRUPT);
    idt_set(47, isr_irq_secondary_hdd, IDT_FLAGS_INTERRUPT);
    
    __asm__ volatile(
        "lidt %0"
        :
        : "m"(idtr)
    );
}
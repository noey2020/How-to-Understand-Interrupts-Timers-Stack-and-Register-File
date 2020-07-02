# How-to-Understand-Interrupts-Timers-Stack-and-Register-File

How to Understand Interrupts, Timers, Stack, and Register File	July 2, 2020

I appreciate comments. Shoot me an email at noel_s_cruz@yahoo.com!

We'll explore the internal workings of our newly written Systick Timer code. It will 
interweave the concepts of interrupts, timers, stack, and how these revolve around the
famous load/store register file in a Harvard Architecture ARM core with peripherals.

I am still using the STM32L1 Discovery board for this tutorial. First, we need to set
our "Options for Target" as shown. The key ones are: Dialog DLL: DARMSTM.DLL and 
Parameter: -pSTM32L152RB. Click OK to close.

Click "Start/Stop a Debug Session" and note the initial data without running the code.
Shown is a pristine snapshot recording the values of interest.

All the register values are zero from R0 to R12, R13(SP) stack pointer points to top
of code memory at 0x20000670. It bigger towards 0x0 meaning as we push into stack, we
decrement the stack pointer. The stack contents are all zeroes shown in memory 1 window
illustrating from 0x20000650 to 0x20000770. Memory 2 shows 0x00000000 and memory 3 shows
0x0800024C which is the address of the SysTick Handler from startup_stml152xb.s. The 
diassembly shows the opcode for LDR r0,[pc,#8] 4802 and loaded reverse as in little
endian looking at memory 3 window. The xPSR register bit ISR is 0 so there is still no
interrupt as verified from Sys Tick Timer and NVIC dialogs. SysTick Timer interrupt is 
enabled, not pending, not active, and no priority(see NVIC dialog). R14(LR) is:
0xFFFFFFFF and R15(PC) equal to 0x08000190.

We have "error 65: access violation at 0x40023800 : no 'read' permission" so we have to 
grant that using Debug\Memory map 0x40000000, 0x420FFFFF, Read, Write. That should
resolve it. 

We insert 2 breakpoints so we can investigate what happens when our code enters an
interrupt and when it exits. Breakpoints at opening and closing braces. We know that
when normal code and an interrupt occurs, the microcode(not application code) in cpu
automatically pushes 8 registers to stack, namely xPSR, PC, LR, SP, r3 to r0 in that
order. So we expect SP will be decremented by 32 bytes. Each register is one memory
address but the PC is incremented by 4 bytes. So to transfer 8 registers in 8 memory
addresses requires 8x4=32 bytes. 32 is 0x20 so SP will be from 0x20000670 to 0x20000650.

Now we are ready to run. We click run and it will stop at the first breakpoint when it
reaches the opening brace. We re-examine our controlled inputs and reach conclusions
about our observations. It will reveal what matches our expected results.

Yes, points to elaborate on. PC is 0x0800024C and it will execute the SysTick handler 
code contained in memory address 0x0800024C(dereferenced 4802...) up to 0x08000254. 
0x08000254 is the BX lr instruction and returns from interrupt. The LR has been loaded
with special code 0xFFFFFFF9. The SP has been decremented by 0x20 to 0x20000650. The
ISR is 15 in both the xPSR register file and NVIC dialogs. In NVIC dialog, IDx is 15,
SysTick Timer is enabled, active(from pending), not pending, and priority 15. Memory 3
shows memory address 0x0800024C populated with SysTick handler code from disassembly.
Memory 1 shows how the stack was populated with values from r0 to r3 and for the rest
of the 8 registers pushed into.

We single step and when we exit the interrupt service meaning the PC is 0x08000254, we
return to normal execution. PC is set to execution address saved before interrupt 
occurred 0x080002CA. Stack contents popped out so SP points again to 0x20000670. xPSR
is 0x21000000 where ISR is 0 meaning no active ISR. By the way, there is only one active
ISR at a time, several pending, enabled, and priorities. Interrupt pre-emption, multiple
interrupt requests, tail-chaining is another topic in itself.

Just acclimatize yourself debugging, single stepping, stepping in/out/over and run
full-speed. Be intimate and tinker around.

They are all highlighted in red lines and circles. 

Here I repost https://github.com/noey2020/How-to-Write-Systick-Timer-CMSIS-Style for reference:

#include "stm32l1xx.h"

volatile uint32_t msTicks = 0;        /* Variable to store millisecond ticks */

void SysTick_Handler(void)

{   /* SysTick interrupt Handler. */

    msTicks++;                        /* See startup file startup_LPC17xx.s for SysTick vector */
    
}

int main(void){

    uint32_t returnCode;

    returnCode = SysTick_Config(SystemCoreClock / 1000);    /* Configure SysTick to generate an interrupt every millisecond */
    

    if(returnCode != 0){                                    /* Check return code for errors */
    
    // Error Handling
    
    }
    
    while(1);
    
}


I appreciate comments. Shoot me an email at noel_s_cruz@yahoo.com!

Happy coding!

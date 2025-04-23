/*
    Copyright (C)2006 onward WITTENSTEIN high integrity systems Limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorise use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems Limited, Registered Office: Brown's Court,
    Long Ashton Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 1275 395 600
    Email: info@highintegritysystems.com
    www.highintegritysystems.com
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20030000;       /* end of RAM */

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0;         /* required amount of heap  */
_Min_Stack_Size = 0x800;    /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 192K
    CCMRAM (rw)     : ORIGIN = 0x10000000, LENGTH = 64K
}

lnkKernelRegionSize = 0x8000;       /* size of kernel code */
lnkKernelDataSize   = 0x400;        /* size of kernel data */

/* Define output sections */
SECTIONS
{
    /* The program code and other data goes into FLAS */
    .text :
    {
        lnkFlashStartAddr = .;
        lnkKernelFuncStartAddr = .;

        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Startup code */

        . = ALIGN(4);
        *( kernel_functions )

        /* User code must be after the kernel region and the kernel region has to be a power of 2 in size */
        . = ALIGN( lnkKernelRegionSize );

        lnkKernelFuncEndAddr = .;


        *(.text)            /* .text sections (code) */
        *(.text*)           /* .text* sections (code) */
        *(.rodata)          /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)         /* .rodata* sections (constants, strings, etc.) */
        *(.glue_7)          /* glue arm to thumb code */
        *(.glue_7t)         /* glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;         /* define a global symbols at end of code */
    } >FLASH


    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    .preinit_array      :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH
    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    lnkFlashEndAddr = .;

    /* used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* Initialized data sections goes into RAM, load LMA copy after code */
    .data :
    {
        . = ALIGN( lnkKernelDataSize );
        _sdata = .;         /* create a global symbol at data start */

        lnkKernelDataStartAddr = .;
        *( kernel_data )
        . = ALIGN( lnkKernelDataSize );
        lnkKernelDataEndAddr = .;

        . = ALIGN( 0x800 );
        lnkTimerTestDataStart = .;
        *(__timer_demo_task_data__)
        *(__timer_demo_task_zero_data__)
        . = ALIGN( 0x800 );
        lnkTimerTestDataEnd = .;
        
        . = ALIGN( 0x400 );
        lnkRecMutexDataStart = .;
        *(__rec_mutex_data__)
        . = ALIGN( 0x400 );
        lnkRecMutexDataEnd = .;
        
        . = ALIGN( 0x40 );
        lnkMathsTestDataStart = .;
        *(__maths_test_data__)
        . = ALIGN( 0x40 );
        lnkMathsTestDataEnd = .;

        . = ALIGN( 0x80 );
        lnkBlockQDataStart = .;
        *( __block_q_data__ )
        . = ALIGN( 0x80 );
        lnkBlockQDataEnd = .;
    
        . = ALIGN( 0x20 );
        lnkBlockTimeDataStart = .;
        *(__block_tim_data__)
        . = ALIGN( 0x20 );
        lnkBlockTimeDataEnd = .;

        . = ALIGN( 0x80 );
        lnkComTestDataStart = .;
        *(__comtest_task_data__)
        *(__comtest_task_zero_data__)
        . = ALIGN( 0x80 );
        lnkComTestDataEnd = .;

        . = ALIGN( 0x40 );
        lnkCountSemDataStart = .;
        *(__counting_semaphore_task_data__)
        *(__counting_semaphore_task_zero_data__)
        . = ALIGN( 0x40 );
        lnkCountSemDataEnd = .;

        . = ALIGN( 0x20 );
        lnkCreateDeleteDataStart = .;
        *(__create_delete_data__)
        . = ALIGN( 0x20 );
        lnkCreateDeleteDataEnd = .;

        . = ALIGN( 0x40 );
        lnkDynamicTaskDataStart = .;
        *(__dynamic_task_data__)
        . = ALIGN( 0x40 );
        lnkDynamicTaskDataEnd = .;

        . = ALIGN( 0x20 );
        lnkLedTaskDataStart = .;
        *(__led_task_data__)
        . = ALIGN( 0x20 );
        lnkLedTaskDataEnd = .;

        . = ALIGN( 0x20 );
        lnkPollQDataStart = .;
        *(__poll_q_data__)
        . = ALIGN( 0x20 );
        lnkPollQDataEnd = .;
    
        . = ALIGN( 0x40 );
        lnkBinarySemaphoreDataStart = .;
        *(__binary_semaphore_task_data__)
        . = ALIGN( 0x40 );
        lnkBinarySemaphoreDataEnd = .;

        . = ALIGN( 0x20 );
        lnkTaskNotifyDataStart = .;
        *(__notified_task_data__)
        *(__notified_task_zero_data__)
        . = ALIGN( 0x20 );
        lnkTaskNotifyDataEnd = .;

        . = ALIGN( 0x40 );
        lnkEventPollDemoDataStart = .;
        *(__event_poll_demo_data__)
        . = ALIGN( 0x40 );
        lnkEventPollDemoDataEnd = .;

        . = ALIGN( 0x20 );
        lnkIdleHookDataStart = .;
        *(__idle_hook_init_data__)
        *(__idle_hook_zero_data__)
        . = ALIGN( 0x20 );
        lnkIdleHookDataEnd = .;
        
        /* --------------------- */
        . = ALIGN( 0x40 );
        lnkEvtMplxDataStart = .;
        *(__evt_mplx_data__)
        . = ALIGN( 0x40 );
        lnkEvtMplxDataEnd = .;

        . = ALIGN( 0x200 );
        lnkStreamBufferTestDataStart = .;
        *(__stream_buffer_task_data__)
        . = ALIGN( 0x200 );
        lnkStreamBufferTestDataEnd = .;
        
        /* --------------------- */

        *(.data)                /* .data sections */
        *(.data*)               /* .data* sections */

        . = ALIGN(4);
        _edata = .;             /* define a global symbol at data end */
    } >RAM AT> FLASH
    
    _siccmram = LOADADDR(.ccmram);

    /* CCM-RAM section 
     * 
     * IMPORTANT NOTE! 
     * If initialized variables will be placed in this section, 
     * the startup code needs to be modified to copy the init-values.   
     */
    .ccmram :
    {
        . = ALIGN(4);
        _sccmram = .;           /* create a global symbol at ccmram start */
        *(.ccmram)
        *(.ccmram*)
        
        . = ALIGN(4);
        _eccmram = .;           /* create a global symbol at ccmram end */
    } >CCMRAM AT> FLASH

    /* Uninitialized data section */
    . = ALIGN(4);
    .bss :
    {
        /* This is used by the startup in order to initialize the .bss secion */
        _sbss = .;              /* define a global symbol at bss start */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;              /* define a global symbol at bss end */
        __bss_end__ = _ebss;
    } >RAM

    /* User_heap_stack section, used to check that there is enough RAM left */
    ._user_heap_stack :
    {
        . = ALIGN(4);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(4);
    } >RAM


    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}

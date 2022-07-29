ENTRY(_start)

/*
    FUTURE: Use disk reader
    A bit of messy here.
    We will pull our higher half kernel out of this shit
      and load it separately once we have our disk reader.
*/


SECTIONS {
    . = 0x100000;

    /* 这里是我们的高半核初始化代码段和数据段 */

    .hhk_init_text BLOCK(4K) : {
          __kernel_start = .;
        * (.multiboot)
        build/obj/arch/x86/*.o (.hhk_init)
        build/obj/arch/x86/*.o (.text)
    }

    .hhk_init_bss BLOCK(4K) : {
        build/obj/arch/x86/*.o (.bss)
    }

    .hhk_init_data BLOCK(4K) : {
        build/obj/arch/x86/*.o (.data)
    }

    .hhk_init_rodata BLOCK(4K) : {
        build/obj/arch/x86/*.o (.rodata)
    }
    __init_hhk_end = ALIGN(4K);

    /* Relocation of our higher half kernel */

    /* 好了，我们的内核…… */
    .text BLOCK(4K) :  {
        __kernel_start = .;
        build/obj/kernel/*.o (.text)
        build/obj/hal/*.o (.text)
    }

    __usrtext_start = ALIGN(4K);
    .usrtext BLOCK(4K) :  {
        build/obj/kernel/*.o (.usrtext)
    }
    __usrtext_end = ALIGN(4K);

    .bss BLOCK(4K) :  {
        build/obj/kernel/*.o (.bss)
        build/obj/hal/*.o (.bss)
    }

    .data BLOCK(4k) : {
        build/obj/kernel/*.o (.data)
        build/obj/hal/*.o (.data)
    }

    .rodata BLOCK(4K) : {
        build/obj/kernel/*.o (.rodata)
        build/obj/hal/*.o (.rodata)
    }

    .kpg BLOCK(4K) :{
        build/obj/arch/x86/*.o (.kpg)
    }

    __kernel_end = ALIGN(4K);
}
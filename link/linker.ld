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




}
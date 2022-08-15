ENTRY(_start)



SECTIONS {

    . = 0x100000;
    /* setup 引导开始…… */
    .text BLOCK(4K) :  {
     * (.multiboot)
      *.o (.text)
      . = ALIGN(4K);
    }
    .usrtext BLOCK(4K) :  {
       *.o  (.usrtext)
          . = ALIGN(4K);
    }
    .setup_heap BLOCK(4K) :{
        *.o (.setup_heap)
        . = ALIGN(4K);
    }

    .bss BLOCK(4K) :  {
        *.o  (.bss)
        . = ALIGN(4K);
    }

    .data BLOCK(4k) : {
       *.o  (.data)
          . = ALIGN(4K);
    }

    .rodata BLOCK(4K) : {
        *.o  (.rodata)
           . = ALIGN(4K);
    }

}
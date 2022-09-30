ENTRY(_start)



SECTIONS {

    . = 0xffffffff80000000;

    .text BLOCK(4K) :  {
      *.o (.text)
      . = ALIGN(4K);
    }
    .usrtext BLOCK(4K) :  {
       *.o  (.usrtext)
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

    __upset_end = ALIGN(4K);
}
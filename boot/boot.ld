ENTRY(_start)



SECTIONS {

    . = 0xc8000000;

    .text  :  {
      *.o (.text)
    }

    .init.data  :  {
       *.o  (.init.data)
    }


    .usrtext  :  {
       *.o  (.usrtext)
    }

    .bss  :  {
        *.o  (.bss)
    }

    .data : {
       *.o  (.data)
    }

    .rodata : {
        *.o  (.rodata)
    }

    __upset_end = ALIGN(4K);
}
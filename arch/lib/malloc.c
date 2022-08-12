//
// Created by Administrator on 2022/8/12.
//
#include "malloc.h"
#include "stdio.h"
uint32_t malloc_addr;
uint32_t malloc_heap_size;
uint32_t malloc_heap_use_size ;

void *malloc(uint32_t size){

    if(malloc_heap_size < malloc_heap_use_size+size){
        kprintf("The heap is overflowing,max size [%d],use size [%d],need size [%d]",malloc_heap_size,malloc_heap_use_size,size);
        return 0;
     }
    malloc_heap_use_size += size;
    return (void *) (malloc_addr + malloc_heap_use_size);

}

void heap_init(uint32_t addr,uint32_t heap_size){
    malloc_addr = addr;
    malloc_heap_size = heap_size;
    malloc_heap_use_size = 0;
}
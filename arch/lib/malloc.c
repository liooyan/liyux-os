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
    void *arr =  (void *) (malloc_addr + malloc_heap_use_size);
    malloc_heap_use_size += size;
    return arr;

}

uint64_t *malloc_4k(uint32_t size){
    if(malloc_heap_use_size%4096 != 0){
        uint32_t residue = 4096- malloc_heap_use_size%4096;
        malloc_heap_use_size+= residue;
    }
    return malloc(size);

}

void heap_init(uint32_t addr,uint32_t heap_size){
    malloc_addr = addr;
    malloc_heap_size = heap_size;
    for (int i = 0; i < heap_size; ++i) {
       char *data = (char *)(addr +i);
       *data = 0;
    }
    malloc_heap_use_size = 0;
}
//
// Created by Administrator on 2022/8/12.
//
#include "mm/malloc.h"
#include "lib/stdio.h"
u32 malloc_addr;
u32 malloc_heap_size;
u32 malloc_heap_use_size ;

void *malloc(u32 size){

    if(malloc_heap_size < malloc_heap_use_size+size){
        kprintf("The heap is overflowing,max size [%d],use size [%d],need size [%d]",malloc_heap_size,malloc_heap_use_size,size);
        return 0;
     }
    malloc_heap_use_size += size;
    void *arr =  (void *) (malloc_addr + malloc_heap_use_size);
    malloc_heap_use_size += size;
    return arr;

}

u32 *malloc_4k(u32 size){
    if(malloc_heap_use_size%4096 != 0){
        u32 residue = 4096- malloc_heap_use_size%4096;
        malloc_heap_use_size+= residue;
    }
    return malloc(size);

}

void heap_init(u32 addr,u32 heap_size){
    malloc_addr = addr;
    malloc_heap_size = heap_size;
    for (u32 i = 0; i < heap_size; ++i) {
       char *data = (char *)(addr +i);
       *data = 0;
    }
    malloc_heap_use_size = 0;
}
//
// Created by server1 on 21-4-2.
//
#include <stdio.h>
#include <stdlib.h>
#include "ResialbeArray.h"

const int BLOCK_SIZE = 20;

Array array_create(int init_size){
    Array array;
    array.size = init_size;
    array.array =(int*) malloc(sizeof(int)*array.size);
    return array;
}

void array_free(Array *a){
    free(a->array);
    a->array = NULL;//free 0,free NULL是无害的
    a->size = 0;
}

//封装
int array_size(const Array *a){
    return a->size;
}

int* array_at(Array *a,int index){//为什么这个函数要返回int*呢，利于array的直接赋值　*array_at(&a,index) = 10　　，或者该函数可以改写成get,set函数
    if(index >= a->size){
        array_inflate(a,(index/BLOCK_SIZE+1) * BLOCK_SIZE - a->size);//每次涨大ＢＬＯＣＫＳＩＺＥ的空间
    }
    return &(a->array[index]);
}
/**
 * void array_set(Array *a,int index,int value){
 *          a->array[index] = value;
 *     }
 * int array_get(const Array *a,int index){
 *          return a->array[index];
 *      }
 */

void array_inflate(Array *a,int more_size){//可变数组的曾长
    int *p = (int*)malloc(sizeof(int)*(a->size+more_size));//申请一块新的更大的空间
    int i;
    for(i = 0; i < a->size; i++){ //复制扩大前的元素　　　库函数memcpy（）
        p[i] = a->array[i];
    }
    free(a->array);//free创建时申请的空间
    a->array = p;
    a->size += more_size;
}

int main(int argc,const char *argv[10]){
    Array a = array_create(5);
    printf("size = %d\n",array_size(&a));
    *array_at(&a,10) = 10;
    printf("array[10]=%d\n",*array_at(&a,10));
    int number;
    int cnt = 0;
    while (number != -1){
        scanf("%d",&number);
//        scanf("%d",array_at(&a,cnt++));
        if(number != -1)
            *array_at(&a,cnt++) = number;
    }
    for(int j = 0; j < a.size; j++){
        if(a.array[j] != 0)
        printf("%d\t",a.array[j]);
    }
    array_free(&a);
    return 0;
}
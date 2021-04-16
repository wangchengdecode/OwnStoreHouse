//
// Created by server1 on 21-4-2.
//

#ifndef STUDYFORC_RESIALBEARRAY_H
#define STUDYFORC_RESIALBEARRAY_H

/**
 * Growable
 * Get the current size
 * Access to th element
 * array_create,array_free,array_at,array_inflate
 */
 typedef struct {
     int *array;
     int size;
 }Array;  //如果这里是*Array ,那么 Array a ,a就是一个指针，但是函数中需要本地变量array时做不出来了

 Array array_create(int init_size);
 void array_free(Array *a);
 int array_size(const Array *a);
 int* array_at(Array *a,int index);
 void array_inflate(Array *a,int more_size);



#endif //STUDYFORC_RESIALBEARRAY_H

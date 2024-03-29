#include <stddef.h>
#include <stdbool.h>

#define DEF_RSZ_FCTR 2
#define DEF_CAP 1024

typedef struct arrayList{
    size_t len;
    char *_elem;
    
    size_t _cap;
    size_t _elem_size;

    float resize_factor;
}arrayList;


arrayList arrayList_make(size_t elem_size, bool alloc);

void arrayList_free(arrayList *arr);

void arrayList_deep_cpy(arrayList *src, arrayList *dest);

void *arrayList_get_ptr(arrayList *arr, size_t i);

void arrayList_get_cpy(arrayList *arr, size_t i, void *buf);

void arrayList_set(arrayList *arr, size_t i, void *elem);

size_t arrayList_append(arrayList *arr, void *elem);

size_t arrayList_extend(arrayList *dest, arrayList *src);

size_t arrayList_pop(arrayList *arr, void *buf);


//  == Iterators

void *aL_first(arrayList *arr);

void *aL_idx(arrayList *arr, size_t idx);

void *aL_next(arrayList *arr, void *iter);

void *aL_done(arrayList *arr);

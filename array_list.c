#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "util.h"
#include "array_list.h"


//  == Utility Functions ==

static inline void *inc_by_bytes(void *ptr, size_t nbytes){
    return (void *) (((char *) ptr) + nbytes);
}

static void arrayList_resize(arrayList *arr){

    size_t new_cap = 0;
    if(arr->_elem == NULL){
        new_cap = DEF_CAP;
        arr->_elem = calloc(new_cap, arr->_elem_size);
    }else{
        new_cap = arr->_cap * arr->resize_factor;
        arr->_elem = realloc(arr->_elem, new_cap * arr->_elem_size);
        memset(&arr->_elem[arr->_cap * arr->_elem_size], 0, new_cap - arr->_cap);
    }

    arr->_cap = new_cap;

    assert(arr->_cap != 0);
}

//  ==

//  == Iterator Functions ==

/** Returns an iterator to the first element of an arrayList.
 *
 *  @param arr pointer to an arrayList
 *
 *  @return iterator to first element of arr
 */
void *aL_first(arrayList *arr){
    return (void *) arr->_elem;
}

/** Returns an iterator to an element at particular index.
 *
 *  @param arr pointer to an arrayList
 *  @param idx of arr
 *
 *  @return iterator to element at idx
 */
void *aL_idx(arrayList *arr, size_t idx){
    return arrayList_get_ptr(arr, idx);
}

/** Increments a given iterator to next element.
 *
 *  @param arr pointer to an arrayList
 *  @param iter iterator of arr
 *
 *  @return iterator to next element
 */
void *aL_next(arrayList *arr, void *iter){
    return inc_by_bytes(iter, arr->_elem_size);
}

/** Returns an iterator to last element of an arrayList.
 *  
 *  @param arr pointer to an arrayList
 *
 *  @return iterator to last element of arr
 */

void *aL_done(arrayList *arr){
    if(arr->_elem == NULL)
        return NULL;

    return inc_by_bytes(arr->_elem, arr->len * arr->_elem_size);
}

//  ==
 
/** Constructs an arrayList with a given element size.
 *
 *  @param elem_size size of arraylist elements
 *  @param alloc if true, sufficient heap memory
 *  will be allocated for DEF_CAP elements
 *        
 *  @return constructed arrayList 
 */
arrayList arrayList_make(size_t elem_size, bool alloc){
    arrayList arr;

    if(alloc){
        arr._elem = calloc(DEF_CAP * elem_size, 1);
        arr._cap = DEF_CAP;
    }else{
        arr._elem = NULL;
        arr._cap = 0;
    }
   
    arr.resize_factor = DEF_RSZ_FCTR; 
    arr._elem_size = elem_size;
    arr.len = 0;

    return arr;
}

/** Creates an arrayList in the location pointed to
 *  by dest which contains copies of each element of src.
 *
 *
 *  @param dest pointer to arrayList
 *  @param src  pointer to arrayList to be copied
 */
void arrayList_deep_cpy(arrayList *dest, arrayList *src){
    free(dest->_elem);

    dest->_elem = check_malloc(
            src->_cap * src->_elem_size,
            "malloc in arrayList_deep_cpy");
    dest->_cap = src->_cap;
    dest->_elem_size = src->_elem_size;
    dest->len = 0;
    
    void *elem;
    for(elem = aL_first(src); elem != aL_done(src); elem = aL_next(src, elem))
        arrayList_append(dest, elem);

    assert(dest->len == src->len);
    
}

/** Frees the memory allocated for arr.
 *
 *  @param arr pointer to arrayList
 */
void arrayList_free(arrayList *arr){
    free(arr->_elem);
}

/** Returns a pointer to the element stored at index i
 *  in arr.
 *  
 *  \warning returned pointer unsafe after subsequent
 *  calls to arrayList_append. 
 *
 *  @param pointer to an arrayList
 *  @param i index of element
 *
 *  @return pointer to element of arr at index i
 */

void *arrayList_get_ptr(arrayList *arr, size_t i){
    return inc_by_bytes(arr->_elem, i * arr->_elem_size);
}

/** Copies the element stored in arr at index i, to
 *  to the memory location pointed to by void.
 *
 *  @param arr pointer to an arrayList
 *  @param i index of element
 *  @param buf pointer to a memory buffer of 
 *  size at least elem_size
 */
void arrayList_get_cpy(arrayList *arr, size_t i, void *buf){
    memcpy(buf, &arr->_elem[i * arr->_elem_size], arr->_elem_size);
}


/** Removes and the last element of arr and copies
 *  it to buf
 *
 *  @param arr pointer to an arrayList
 *  @param buf pointer to memory buffer of size
 *  at least elem_size
 *
 *  @return new len of dest
 */
size_t arrayList_pop(arrayList *arr, void *buf){
    if(arr->len == 0){
        fprintf(stderr, "indexError: pop from empty list\n");
        exit(1);
    }
    arr->len--;
    arrayList_get_cpy(arr, arr->len, buf);

    return arr->len;
}   


/** Copies the element stored at elem to index i of arr.
 *
 *  @param arr pointer to an arrayList
 *  @param i index of arr
 *  @param elem pointer to memory buffer of
 *  size at least elem_size
 *
 */
void arrayList_set(arrayList *arr, size_t i, void *elem){
    if(i >= arr->len){
        fprintf(stderr, "indexError:  %ld\n", i);
        exit(1);
    }

    memcpy(&arr->_elem[i * arr->_elem_size], elem, arr->_elem_size);
}

/** Copies the element pointed to by elem to the
 *  end of arr.
 *
 *  @param pointer to an arrayList
 *  @param elem pointer to a memory buffer of
 *  size at least elem_size
 *
 *  @return new length of arr
 *
 */
size_t arrayList_append(arrayList *arr, void *elem){

    if(arr->len == arr->_cap)
        arrayList_resize(arr);

    arr->len = arr->len + 1;
    arrayList_set(arr, arr->len - 1, elem);

    return arr->len;
}

/** Appends each element on src to dest
 *
 *  @param dest pointer to destination arrayList
 *  @param src pointer to source arrayList
 *
 *  @return new len of dest
 */
size_t arrayList_extend(arrayList *dest, arrayList *src){

    void *elem;
    for(elem = aL_first(src); elem != aL_done(src); elem = aL_next(src, elem)){
        arrayList_append(dest, elem);
    }

    return dest->len;
}



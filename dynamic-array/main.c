#include <stdio.h>
#include <stdlib.h>

const int RESIZE_CONSTANT = 2;

/*
 * "DynamicArray": Array with Dynamic features such as auto resizing and length tracking.
 *
 * It uses a regular C array with custom metadata attached to it.
 * Based on metadata, it automatically rallocates the size of the base array.
 */
typedef struct {
    int *data;       // regular/base array
    size_t capacity; // number of elements the array can hold
    size_t size;     // number of elements present in the array
} DynamicArray;

int init_dynamic_array(DynamicArray *arr, size_t init_cap);
int resize_dynamic_array(DynamicArray *arr);

int main() {
    DynamicArray arr;
    int res = init_dynamic_array(&arr, 2);
    if (res == EXIT_FAILURE) {
        fprintf(stderr, "failed to initialize the dynamic array\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/*
 * Allocates space for an array with initial capacity.
 * Takes a DynamicArray and an inital capacity.
 */
int init_dynamic_array(DynamicArray *arr, size_t init_cap) {
    if (arr == NULL || init_cap == 0) {
        return EXIT_FAILURE;
    }

    int *temp = malloc(sizeof(int) * init_cap);
    if (temp == NULL) {
        fprintf(stderr, "failed to allocate memory to Dynamic Array\n");

        arr->capacity = 0;
        arr->size = 0;
        arr->data = NULL;
        return EXIT_FAILURE;
    }

    arr->data = temp;
    arr->capacity = init_cap;
    arr->size = 0;

    return EXIT_SUCCESS;
}

/*
 * Safely resizes the array
 */
int resize_dynamic_array(DynamicArray *arr) {
    // there is no point in resizing the array or base array if either of them is null
    if (arr == NULL || arr->data == NULL) {
        fprintf(stderr, "Failed to resize, either dynamic array or base array is NULL\n");
        return EXIT_FAILURE;
    }

    // Restrict resizing if capacity is not reached
    if (arr->capacity > arr->size) {
        fprintf(stderr, "cannot resize a dynamic array when capacity is not reached\n");
        return EXIT_FAILURE;
    }

    /*
     * Reallocate the array safely.
     * realloc the base array with existing data and new capacity.
     */
    int *temp = realloc(arr->data, (arr->capacity * RESIZE_CONSTANT) * sizeof(*temp));
    if (temp == NULL) {
        fprintf(stderr, "failed to resize the array\n");
        return EXIT_FAILURE;
    }

    arr->data = temp;
    arr->capacity *= RESIZE_CONSTANT;

    return EXIT_SUCCESS;
}

int push(DynamicArray *arr, int payload) {
    if (arr == NULL || arr->data == NULL) {
        fprintf(stderr, "failed to push payload as either dynamic array or base array is NULL\n");
        return EXIT_FAILURE;
    }

    // If capacity is reached then we resize the array
    if (arr->capacity == arr->size) {
        int resizeResult = resize_dynamic_array(arr);
        if (resizeResult == EXIT_FAILURE) {
            fprintf(stderr, "failed to push as capacity is reached and resize failed\n");
            return EXIT_FAILURE;
        }
    }

    // add the element to base array and update size
    arr->data[arr->size] = payload;
    arr->size++;

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

int global_var = 42; // global/static memory

int main() {
    int stack_var = 10; // stack variable
    int *heap_var = malloc(sizeof(int)); // heap allocation
    *heap_var = 20;

    printf("Address of stack_var: %p\n", (void*)&stack_var);
    printf("Address of heap_var: %p\n", (void*)heap_var);
    printf("Address of global_var: %p\n", (void*)&global_var);

    free(heap_var); // always free heap memory
    return 0;
}

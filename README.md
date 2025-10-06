# CS50 Labwork
---

## Memory Management

### Memory Layout

#### **1. Understanding Stack vs Heap**

Before writing code, you must understand **memory layout**.

1. **Stack:**

   * Stores **local variables** (inside functions), function parameters, and return addresses.
   * Fast allocation/deallocation (via adjusting the stack pointer).
   * Memory is **automatically managed**—when a function returns, its local variables are gone.
   * **Size is limited** (e.g., a few MBs on most systems).

2. **Heap:**

   * Stores **dynamically allocated memory** (via `malloc` in C or `Box::new` in Rust).
   * Manual allocation/deallocation (`free` in C, `drop` in Rust or RAII for automatic).
   * Slower than stack because it needs bookkeeping and possibly more complex algorithms.
   * Can grow much larger than stack (limited by system memory).

**Kernel engineer insight:**

* Stack is **contiguous**, easy to index, and extremely fast. Overflowing it is catastrophic (stack smashing).
* Heap is fragmented, managed by a memory allocator, and often comes with **metadata overhead** (think of `malloc` headers).
* When debugging, seeing addresses can reveal whether memory is on stack or heap. Stack addresses are usually **high in memory** (on x86_64 Linux) and heap addresses are **lower**, but this depends on OS.

---

#### **2. Simple C Program**

Here’s a **minimal C program** that prints stack vs heap addresses:

```c
#include <stdio.h>
#include <stdlib.h>

int global_var = 42; // global/static memory

int main() {
    int stack_var = 10;            // stack variable
    int *heap_var = malloc(sizeof(int)); // heap allocation
    *heap_var = 20;

    printf("Address of stack_var: %p\n", (void*)&stack_var);
    printf("Address of heap_var: %p\n", (void*)heap_var);
    printf("Address of global_var: %p\n", (void*)&global_var);

    free(heap_var); // always free heap memory
    return 0;
}
```

**Explanation for a kernel-level perspective:**

| Variable type | Memory region | Lifetime                | Allocation/deallocation      |
| ------------- | ------------- | ----------------------- | ---------------------------- |
| `stack_var`   | Stack         | Scope of function       | Automatic (on function exit) |
| `heap_var`    | Heap          | Until freed             | Manual (`malloc` + `free`)   |
| `global_var`  | Data segment  | Entire program lifetime | Static allocation            |

**Tips kernel engineers would highlight:**

* Always cast addresses to `(void*)` when printing pointers.
* Remember **stack overflows** are dangerous.
* Memory alignment may affect addresses.
* Comparing addresses can give insights about memory layout of the process.

---

#### **3. Rust Version**

Rust has **similar memory concepts**, but with safety guarantees and ownership rules.

```rust
fn main() {
    let stack_var = 10;              // stack variable
    let heap_var = Box::new(20);     // heap allocation using Box
    static GLOBAL_VAR: i32 = 42;     // global/static memory

    println!("Address of stack_var: {:p}", &stack_var);
    println!("Address of heap_var: {:p}", &*heap_var); // deref Box to get heap address
    println!("Address of GLOBAL_VAR: {:p}", &GLOBAL_VAR);
}
```

**Rust fundamentals:**

* `Box::new` allocates memory on the heap.
* `stack_var` is automatically deallocated when `main` ends.
* Rust **automatically frees** heap memory when `Box` goes out of scope (RAII), unlike manual `free` in C.
* Ownership system ensures **no double-free or dangling pointers**, which are common in C kernel development mistakes.

**Kernel-level wisdom in Rust:**

* Rust prevents use-after-free bugs at compile time.
* Stack vs heap debugging in Rust is almost identical to C (you still look at addresses), but memory safety is guaranteed.
* You can still observe memory layout, but you cannot arbitrarily manipulate memory like in C (unless using `unsafe`).

---

#### **4. Things a seasoned kernel engineer would emphasize**

1. **Memory safety vs speed trade-offs**

   * Stack: fast but limited.
   * Heap: flexible but slower and fragmented.
2. **Memory alignment**

   * Stack and heap addresses are aligned; misaligned access can crash CPUs.
3. **Lifetime management**

   * C: Manual (`malloc`/`free`).
   * Rust: Automatic via ownership, lifetimes, and RAII.
4. **Debugging and security**

   * Stack addresses can reveal stack layout for buffer overflow attacks.
   * Heap fragmentation can lead to subtle bugs.
5. **Print addresses for learning, not production**

   * Kernel engineers often examine addresses during debugging or kernel crash dumps.
6. **Observing memory segments**

   * Kernel memory layout: text/code, data (globals), heap, stack, mmap regions.

---


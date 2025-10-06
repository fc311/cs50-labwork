fn main() {
    let stack_var = 10;              // stack variable
    let heap_var = Box::new(20);     // heap allocation using Box
    static GLOBAL_VAR: i32 = 42;     // global/static memory

    println!("Address of stack_var: {:p}", &stack_var);
    println!("Address of heap_var: {:p}", &*heap_var); // deref Box
    println!("Address of GLOBAL_VAR: {:p}", &GLOBAL_VAR);
}
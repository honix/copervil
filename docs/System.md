### new node registration

nodes are defined as .so objects in C calling format. they can be loaded in runtime.

.so file will define ```void register_library()``` function to export nodes definitions by calling ```void register_function(struct function_note)``` for each node:
```c
void register_library()
{
	register_function((struct function_note){
		"add", add_op_init, add_op});
	register_function((struct function_note){
		"sub", sub_op_init, sub_op});
	register_function((struct function_note){
		"mul", mul_op_init, mul_op});
	register_function((struct function_note){
		"div", div_op_init, div_op});
}
```
see ```struct function_note``` *(src/core/dl_loader.h)* for more callback options

see *src/nodes/honix/patch_editor/number_io.c* for custom node representation/controls example

see *src/nodes/honix/cpp_test/cpp_test.cpp* for basic cpp interop example

to load library use ```void load_library(char*)```:
```c
load_library("./src/nodes/honix/utils/utils.so");
```

### loop.c

loop is time tracking node queue. it execute node with time == current_time or sleeps until it happends. loop works inside main thread after main initialization. as we started using time independed threads, loop is really needless (see sleep node for time controll), after that main thread can be utilized as default working thread for nodes. right now loop only helps for delayed start for on_open node.

### system

nodes defined as .so objects in C calling format.

node will define ```void register_library()``` function to export nodes definitions by calling ```void register_function(struct function_note)``` for each node:
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
(see ```struct function_note``` (src/core/dl_loader.h) for more callback options)

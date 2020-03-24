# c (temporary name)
Node-based programming in C language

### status
work in progress, early prototype

### build
```
make
```
but before it each node will be compiled by its makefile:
```
cd src/nodes/honix/patch_editor
make
```

### todo (topmost is priority)
- patch_editor custom user nodes (inputboxes, buttons, graphs and etc)
- better pins interface (NULL link handling)
- rethink node creation API from C
- better, systematic error logging
- thirdparty libraries auto-wrap? (use header files to generate node functions)
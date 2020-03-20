# c (temporary name)
Node-based programming with C language

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
- split nanovg, glfw context creation into nodes, so user can make another window
- better, systematic error logging
- thirdparty libraries auto-wrap? (use header files to generate node functions)
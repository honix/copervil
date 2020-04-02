# c (temporary name)
Node-based programming in C language

### status
work in progress, early prototype

developed and tested to work on linux, but will be easy to port on windows and other platforms

### dependencies to run from sources
- core deps: ```make, gcc```

thirdparty libs:
- nanovg deps: ```premake4, make, gcc```

### build and run
run this commands in project root folder:
```
make
```

### todo (topmost is priority)
- node reqursive call problem (for example changing loop step in real-time will cause more loops calls)
- patch_editor custom user nodes (inputboxes, buttons, graphs and etc)
- better pins interface (NULL link handling)
- rethink node creation API from C
- better, systematic error logging
- patch_editor *.so objects is too big, they staticly linked with nanovg
- patch_editor eats cpu - redraw on event, not every 1/60 seconds
- thirdparty libraries auto-wrap? (use header files to generate node functions)

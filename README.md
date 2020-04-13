# c (temporary name)
Node-based programming in C language

### status
work in progress, early prototype

developed and tested to work on linux, but will be easy to port on windows and other platforms

### dependencies to run from sources
- common deps: ```make, gcc```
- patch editor deps: ```libglfw3-dev```

thirdparty libs:
- nanovg deps: ```premake4, make, gcc```

### build and run
run this commands in project root folder:
```
make
```

### todo (topmost is priority)
- question - will _all_ nodes have trigger in trigger out pins? (see do_times add sequence, when we want add to not send trigger to draw_triangle)
- patch_editor custom user nodes (inputboxes, buttons, graphs and etc)
- patch_editor eats cpu - redraw on event, not every 1/60 seconds
- rethink node creation API from C
- better, systematic error logging
- try to plugin big drawing/sound engines like ogre, try run both in same time
- patch_editor *.so objects is too big, they staticly linked with nanovg
- thirdparty libraries auto-wrap? (use header files to generate node functions)

### strange ideas
- lua (or janet) interpreter as node, can be used as command line; such interpreter can be used as patch file definition using interpreter syntax
- pin nodes to the screen space, and use them as part of ui

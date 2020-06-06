# c (temporary name)
Node-based programming in C language

![](docs/Screenshot%20from%202020-06-06%2014-01-28.png)

### status
work in progress, early prototype

developed and tested to work on linux, but will be easy to port on windows and other platforms

### dependencies to run from sources

- common deps: ```make, gcc```
- patch editor deps: ```libglfw3-dev``` (> 3.3.0)

thirdparty libs:
- nanovg deps: ```premake4```

### build and run
clone with submodiles:
```
git clone --recurse-submodules git@github.com:honix/c.git
```
run this commands in project root folder:
```
make
make run
```

### todo (topmost is priority)
- messaging over threads using signals
- maybe loop.h (scheduler) will be dropped after (already possible but main thread will turn into default working thread)
- patch_editor custom user nodes (inputboxes, buttons, graphs and etc)
- rethink node creation API from C
- better, systematic error logging
- try to plugin big drawing/sound engines like ogre, try run both in same time
- patch_editor *.so objects is too big, they staticly linked with nanovg
- thirdparty libraries auto-wrap? (use header files to generate node functions)

### strange ideas
- lua (or janet) interpreter as node, can be used as command line; such interpreter can be used as patch file definition using interpreter syntax
- pin nodes to the screen space, and use them as part of ui

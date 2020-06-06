### summary
patch editor (src/nodes/honix/patch_editor/patch_editor.so) is a representation of nodes and links. patch editor itself is a node, so core can work without it.

### controlls
- select node - left mouse click
- create node - type node name (text will appear in left-bottom corner) and press enter (hold shift to save node name)
- move node - left mouse drag
- execute node - tab key
- remove node - delete key
- link nodes - left mouse click on pin, then left mouse click on another pin

### number_io
number_io is a example of custom drawn node, it shows current number, draws bar, and allows to change value using keyboard:
- increment number - arrow up key
- decrement number - arrow down key

(increment with 0.1 step with shift holded, and 0.01 with alt)

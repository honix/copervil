NANOVG_OBJ = thirdparty/nanovg/build/obj/Debug/nanovg/nanovg.o

patch_editor.so:

%.so: %.c
	gcc -Wall -shared -fPIC -g -I../../.. -Ithirdparty $< $(NANOVG_OBJ) -o $@ -lm -lGL -lglfw

NANOVG_MAKE = thirdparty/nanovg/build/nanovg.make

$(NANOVG_OBJ): $(NANOVG_MAKE)
	cd $(dir $(NANOVG_MAKE)) && make -f $(notdir $(NANOVG_MAKE))

NANOVG_PREMAKE = thirdparty/nanovg/premake4.lua

$(NANOVG_MAKE): $(NANOVG_PREMAKE)
	cd $(dir $(NANOVG_PREMAKE)) && premake4 gmake


clean:
	rm *.so

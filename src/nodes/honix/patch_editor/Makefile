HEADERS_CORE = \
	../../../../src/core/node_api.h

THIRDPARTY = ../../../../thirdparty
NANOVG_OBJ = $(THIRDPARTY)/nanovg/build/obj/Debug/nanovg/nanovg.o

all: patch_editor.so number_io.so

%.so: %.c $(HEADERS_CORE) $(NANOVG_OBJ)
	gcc -Wall -shared -fPIC -g -I../../.. -I$(THIRDPARTY) $< $(NANOVG_OBJ) -o $@ -lm -lGL -lglfw

NANOVG_MAKE = $(THIRDPARTY)/nanovg/build/nanovg.make

$(NANOVG_OBJ): $(NANOVG_MAKE)
	cd $(dir $(NANOVG_MAKE)) && make -f $(notdir $(NANOVG_MAKE))

NANOVG_PREMAKE = $(THIRDPARTY)/nanovg/premake4.lua

$(NANOVG_MAKE): $(NANOVG_PREMAKE)
	cd $(dir $(NANOVG_PREMAKE)) && premake4 gmake


clean:
	rm *.so


SRC = ../../..
HEADERS_CORE = $(SRC)/core/node_api.h

THIRDPARTY = ../../../../thirdparty
NANOVG_OBJ = $(THIRDPARTY)/nanovg/build/obj/Debug/nanovg/nanovg.o

LIBSX_INCLUDE = $(THIRDPARTY)/sx/include
LIBSX_OBJ = $(THIRDPARTY)/sx/libsx.a

all: patch_editor.so number_io.so

%.so: %.c $(HEADERS_CORE) $(NANOVG_OBJ) $(LIBSX_OBJ)
	gcc -Wall -shared -fPIC -g -I$(SRC) -I$(THIRDPARTY) -I$(LIBSX_INCLUDE) $< $(NANOVG_OBJ) $(LIBSX_OBJ) -o $@ -lm -lGL -lglfw

NANOVG_MAKE = $(THIRDPARTY)/nanovg/build/nanovg.make

$(NANOVG_OBJ): $(NANOVG_MAKE)
	cd $(dir $(NANOVG_MAKE)) && make -f $(notdir $(NANOVG_MAKE))

NANOVG_PREMAKE = $(THIRDPARTY)/nanovg/premake4.lua

$(NANOVG_MAKE): $(NANOVG_PREMAKE)
	cd $(dir $(NANOVG_PREMAKE)) && premake4 gmake


clean:
	rm *.so

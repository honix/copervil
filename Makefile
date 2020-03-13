HEADERS_CORE = src/core/link.h \
	src/core/list.h \
	src/core/loop.h \
	src/core/node.h \
	src/core/sized_array.h \
	src/core/utils.h

HEADERS_ALL = $(HEADERS_CORE)

headers_all_file = $(notdir $(HEADERS_ALL))
objects_all_file = $(headers_all_file:.h=.o)
OBJECTS = $(addprefix obj/,$(objects_all_file))

NANOVG_OBJ = thirdparty/nanovg/build/obj/Debug/nanovg/nanovg.o

run: bin/editor
	./$<


bin:
	mkdir bin

bin/editor: src/editor/main.c $(OBJECTS) $(NANOVG_OBJ) bin
	gcc -g $< $(OBJECTS) $(NANOVG_OBJ) -o $@ -lm -lGL -lglfw


obj:
	mkdir obj

obj/%.o: src/core/%.c $(HEADERS_CORE) obj 
	gcc -g -c $< -o $@

obj/%.o: src/editor/%.c $(HEADERS_CORE) $(HEADERS_EDITOR) obj
	gcc -g -c $< -o $@


NANOVG_MAKE = thirdparty/nanovg/build/nanovg.make

$(NANOVG_OBJ): $(NANOVG_MAKE)
	cd $(dir $(NANOVG_MAKE)) && make -f $(notdir $(NANOVG_MAKE))

NANOVG_PREMAKE = thirdparty/nanovg/premake4.lua

$(NANOVG_MAKE): $(NANOVG_PREMAKE)
	cd $(dir $(NANOVG_PREMAKE)) && premake4 gmake
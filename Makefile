HEADERS_CORE = src/core/link.h \
	src/core/list.h \
	src/core/loop.h \
	src/core/node.h \
	src/core/sized_array.h \
	src/core/dl_loader.h 
	#src/core/utils.h

HEADERS_ALL = $(HEADERS_CORE)

headers_all_file = $(notdir $(HEADERS_ALL))
objects_all_file = $(headers_all_file:.h=.o)
OBJECTS = $(addprefix obj/,$(objects_all_file))


run: bin/core
	./$<


bin:
	mkdir bin

bin/core: src/core/main.c $(OBJECTS) bin
	gcc -Wall -g -rdynamic $< $(OBJECTS) -o $@ -lm -ldl

# # TODO (done): editor will plugin as shared library
# bin/editor: src/editor/main.c $(OBJECTS) $(NANOVG_OBJ) bin
# 	gcc -Wall -g $< $(OBJECTS) $(NANOVG_OBJ) -o $@ -lm -lGL -lglfw


obj:
	mkdir obj

obj/%.o: src/core/%.c $(HEADERS_CORE) obj 
	gcc -Wall -g -c $< -o $@

obj/%.o: src/editor/%.c $(HEADERS_CORE) $(HEADERS_EDITOR) obj
	gcc -Wall -g -c $< -o $@

clean:
	rm -rf bin
	rm -rf obj

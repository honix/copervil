HEADERS_CORE = \
	src/core/link.h \
	src/core/list.h \
	src/core/loop.h \
	src/core/node.h \
	src/core/dl_loader.h \
	src/core/geometry.h \
	src/core/type_bank.h

NODE_API = src/core/node_api.h

HEADERS_ALL = $(HEADERS_CORE)

headers_all_file = $(notdir $(HEADERS_ALL))
objects_all_file = $(headers_all_file:.h=.o)
OBJECTS = $(addprefix obj/,$(objects_all_file))


build: bin/core nodes


install:


run: bin/core nodes
	./$<


bin:
	mkdir bin

bin/core: src/core/main.c $(OBJECTS) thirdparty/sx/libsx.a | bin
	gcc -Wall -g -rdynamic $< $(OBJECTS) thirdparty/sx/libsx.a -o $@ -lm -ldl -pthread


nodes: $(NODE_API) thirdparty/sx/libsx.a
	cd src/nodes && for m in `find | grep Makefile`; do make -C `dirname $$m`; done


$(NODE_API): $(HEADERS_CORE)
	touch $@


obj:
	mkdir obj

obj/%.o: src/core/%.c $(HEADERS_CORE) | obj
	gcc -Wall -g -c $< -o $@ -I thirdparty/sx/include


thirdparty/sx/libsx.a: thirdparty/sx/Makefile
	cd thirdparty/sx && make

thirdparty/sx/Makefile: thirdparty/sx/CMakeLists.txt
	cd thirdparty/sx && cmake CMakeLists.txt && make


clean:
	rm -rf bin
	rm -rf obj
	cd src/nodes && for m in `find | grep Makefile`; do make -C `dirname $$m` clean; done


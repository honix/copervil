HEADERS_CORE = \
	../../../../src/core/link.h \
	../../../../src/core/list.h \
	../../../../src/core/loop.h \
	../../../../src/core/node.h \
	../../../../src/core/sized_array.h \
	../../../../src/core/dl_loader.h \
	../../../../src/core/geometry.h
	
utils.so:

%.so: %.c $(HEADERS_CORE)
	gcc -Wall -shared -fPIC -g -I../../.. $< -o $@

clean:
	rm *.so

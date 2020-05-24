HEADERS_CORE = \
	../../../../src/core/node_api.h

THIRDPARTY = ../../../../thirdparty

LIBSX_INCLUDE = $(THIRDPARTY)/sx/include
LIBSX_OBJ = $(THIRDPARTY)/sx/libsx.a
	
all: utils.so

%.so: %.c $(HEADERS_CORE)
	gcc -Wall -shared -fPIC -g -I../../.. -I$(LIBSX_INCLUDE) $< $(LIBSX_OBJ) -o $@


clean:
	rm -f *.so

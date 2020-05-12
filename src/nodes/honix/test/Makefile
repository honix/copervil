HEADERS_CORE = \
	../../../../src/core/node_api.h
	
all: utils.so

%.so: %.c $(HEADERS_CORE)
	gcc -Wall -shared -fPIC -g -I../../.. $< -o $@


clean:
	rm *.so

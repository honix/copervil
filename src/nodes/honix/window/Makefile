HEADERS_CORE = \
	../../../../src/core/node_api.h

all: window.so

%.so: %.c $(HEADERS_CORE)
	gcc -Wall -shared -fPIC -g -I../../.. $< -o $@ -lGL -lglfw


clean:
	rm *.so

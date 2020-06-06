SRC = ../../..
HEADERS_CORE = $(SRC)/core/node_api.h

all: cpp_test.so

%.so: %.cpp $(HEADERS_CORE)
	gcc -Wall -shared -fPIC -g -I$(SRC) $< -lstdc++ -o $@


clean:
	rm -f *.so

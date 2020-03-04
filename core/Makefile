CORE_BIN=core-bin

all: $(CORE_BIN)

$(CORE_BIN):
	mkdir -p bin
	gcc -Wall -g src/*.c -o bin/$(CORE_BIN)

main.o: src/main.c
	gcc -c src/main.c

node.o: src/node.c
	gcc -c src/node.c


run: all
	./bin/$(CORE_BIN)

clean:
	rm -rf bin
CORE_BIN=core-bin

all: $(CORE_BIN)

$(CORE_BIN):
	mkdir -p bin
	gcc -Wall -g src/*.c -o bin/$(CORE_BIN)

run: all
	./bin/$(CORE_BIN)

clean:
	rm -rf bin
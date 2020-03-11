EDITOR_BIN=editor-bin

all: $(EDITOR_BIN)

$(EDITOR_BIN):
	mkdir -p bin
	gcc -Wall -g \
		src/*.c \
		../core/obj/*.o \
		nanovg/build/obj/Debug/nanovg/nanovg.o \
		-o bin/$(EDITOR_BIN) \
		-lglfw -lGL -lm

main.o: src/main.c
	gcc -c src/main.c

run: all
	cd bin && ./$(EDITOR_BIN)

clean:
	rm -rf bin
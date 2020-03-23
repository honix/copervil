window.so:

%.so: %.c
	gcc -Wall -shared -fPIC -g -I../../.. $< -o $@ -lGL -lglfw -lglut

clean:
	rm *.so

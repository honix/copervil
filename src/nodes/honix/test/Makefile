utils.so:

%.so: %.c
	gcc -Wall -shared -fPIC -g -I../../.. $< -o $@

clean:
	rm *.so

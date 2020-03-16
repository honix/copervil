utils.so:

%.so: %.c
	gcc -Wall -shared -fPIC -g $< -o $@

clean:
	rm *.so

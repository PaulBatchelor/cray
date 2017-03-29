.PHONY: render

CFLAGS=-ansi -Wall -Wpedantic -g
LDFLAGS=-lm
OBJ=main.o ray.o sphere.o hitablelist.o sqrtf.o rand.o camera.o

default: cray

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

cray: $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

render: cray
	./cray 
	convert out.ppm out.png
	feh out.png

clean: 
	rm -rf $(OBJ) cray out.ppm out.png

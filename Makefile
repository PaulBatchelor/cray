.PHONY: render

CFLAGS=-ansi -Wall -Wpedantic -g -O3
LDFLAGS=-lm -lrunt -ldl
OBJ=main.o ray.o sphere.o hitablelist.o sqrtf.o rand.o camera.o material.o\
	lambertian.o metal.o dielectric.o scene.o runt.o

default: cray

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

cray: $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

render: cray
	./cray test.rnt
	feh out.ppm

clean: 
	rm -rf $(OBJ) cray out.ppm out.png

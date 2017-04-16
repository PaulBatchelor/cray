.PHONY: render

CFLAGS=-ansi -Wall -Wpedantic -g -O3
LDFLAGS=-lm -lrunt -ldl
OBJ=ray.o sphere.o hitablelist.o sqrtf.o rand.o camera.o material.o\
	lambertian.o metal.o dielectric.o scene.o runt.o demo.o

default: cray

all: cray librunt_cray.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

cray: $(OBJ) main.c
	$(CC) main.c $(OBJ) -o $@ $(LDFLAGS)

librunt_cray.a: $(OBJ)
	ar rcs $@ $(OBJ)

render: cray
	./cray test.rnt
	feh out.ppm

install: librunt_cray.a cray
	mkdir -p ~/.runt/lib
	mkdir -p ~/.runt/bin
	mkdir -p ~/.runt/include
	cp cray ~/.runt/bin
	cp librunt_cray.a ~/.runt/lib
	cp scene.h ~/.runt/include/cray.h

clean: 
	rm -rf $(OBJ) cray out.ppm out.png
	rm -rf librunt_cray.a

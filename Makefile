.PHONY: render

CFLAGS=-ansi -Wall -Wpedantic
LDFLAGS=-lm
OBJ=main.o ray.o

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

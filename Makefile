.PHONY: render

CFLAGS=-ansi -Wall -Wpedantic
OBJ=main.o

default: cray

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

cray: $(OBJ)
	$(CC) $(OBJ) -o $@

render: cray
	./cray 
	convert out.ppm out.png
	feh out.png

clean: 
	rm -rf $(OBJ) cray out.ppm out.png

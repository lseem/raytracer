SRC = raytracer.c
OBJ = $(SRC:.c=.o)

all: raytracer

.c.o:
	$(CC) -c $<

raytracer: $(OBJ)
	$(CC) -o $@ $(OBJ) -lpng

clean:
	rm $(OBJ)

run: raytracer
	./raytracer

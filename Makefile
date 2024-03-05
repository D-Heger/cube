#build file cube.c into file cube, link math
build:
	gcc -o cube cube.c -lm

#run built cube file
run:
	./cube

#build and run
all: build run
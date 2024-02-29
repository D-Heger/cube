build:
	gcc -o cube cube.c -lm

run:
	./cube

all: build run
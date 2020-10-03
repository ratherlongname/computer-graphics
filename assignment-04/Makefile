.PHONY: run clean
.DEFAULT: run

run: a.out
	./$^

a.out: main.cpp
	gcc -o $@ $^ -lGL -lGLU -lglut

clean:
	-rm a.out
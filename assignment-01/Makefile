.PHONY: run clean
.DEFAULT: run

run: a.out
	./$^

a.out: test.cpp
	gcc -o $@ $^ -lGL -lGLU -lglut

clean:
	-rm a.out
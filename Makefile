all: GLmandel

GLmandel: main.c
	gcc -o GLmandel main.c -lGL -lglfw2 -lGLEW

clean:
	rm GLmandel

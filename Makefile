all: GLmandel

GLmandel: main.c
	gcc -o GLmandel main.c -lglfw -lGLEW

clean:
	rm GLmandel

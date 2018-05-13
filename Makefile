all:
	gcc main.c camera.c utils.c model.c draw.c -lglut -lGL -lGLU -lm -o grafika
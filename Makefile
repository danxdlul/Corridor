all:
	gcc main.c camera.c utils.c model.c draw.c -lSOIL -lglut -lGL -lGLU -lm -o grafika
CC = gcc
CFLAGS = -Ofast -Wall
LDLIBS = -lm

TARGET = life
SRC = main.c life.c
OBJ = main.o life.o

FRAMES = output_%03d.ppm
VIDEO = animation.mp4

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDLIBS) -o $(TARGET)

main.o: main.c life.h
	$(CC) $(CFLAGS) -c main.c -o main.o

life.o: life.c life.h
	$(CC) $(CFLAGS) -c life.c -o life.o

run: $(TARGET)
	./$(TARGET)

video: run
	ffmpeg -y -framerate 30 -i $(FRAMES) \
		-c:v libx264 -pix_fmt yuv420p $(VIDEO)

clean:
	rm -f $(TARGET) $(OBJ) output_*.ppm $(VIDEO)

rebuild-run-video: clean video


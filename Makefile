CC = gcc
CFLAGS = -Ofast -Wall -pthread
LDLIBS = -lm -pthread

TARGET = life
SRC = main.c life.c
OBJ = main.o life.o

FRAMES = output_%03d.ppm
VIDEO = animation.mp4

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDLIBS)

main.o: main.c life.h barrier.h
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

clean-not-video:
	rm -f $(TARGET) $(OBJ) output_*.ppm

rebuild: clean video
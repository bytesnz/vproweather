CC = gcc
CFLAGS = -O2 -Wall -fpack-struct -I/usr/include

TARGET = vproweather
all:	$(TARGET)

$(TARGET): main.o dhandler.o
	$(CC) main.o dhandler.o -o $(TARGET)

main.o: main.c main.h dhandler.h
	$(CC) $(CFLAGS) -c main.c

dhandler.o: dhandler.c dhandler.h names.h
	$(CC) $(CFLAGS) -c dhandler.c

clean:
	rm -f $(TARGET) *.o


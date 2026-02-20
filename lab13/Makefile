.PHONY: clean

CC = gcc
CFLAGS = -Wall -Iinclude
TARGET = main.exe

all: $(TARGET)

warehouse.o: src/warehouse.c
	$(CC) $(CFLAGS) -c src/warehouse.c -o warehouse.o

warehouse.dll: warehouse.o
	$(CC) -shared -o warehouse.dll warehouse.o

$(TARGET): src/main.c warehouse.dll
	$(CC) $(CFLAGS) src/main.c -L. -lwarehouse -o $(TARGET)

clean:
	del -rf *.o *.dll $(TARGET)
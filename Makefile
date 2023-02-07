CFLAGS += -Wall -O2

all: signal-strength

signal-strength: main.c
	$(CC) $(CFLAGS) main.c -o signal-strength -lpcap

clean:
	rm -f signal-strength *.o


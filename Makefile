CC=gcc
CFLAGS=-Wall -O2
TARGET=collatz

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) collatz_results.csv
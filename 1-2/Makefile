CC = gcc
CFLAGS = -O1 -g -Wall 

CFILES := main.c timsort.c

timsort: $(CFILES)
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm timsort
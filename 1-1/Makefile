CC = gcc
CFLAGS = -O1 -g -Wall 

CFILES := qsort.c check.c
CLIST_FILES := qsort_list.c check_list.c

check: $(CFILES)
	$(CC) $^ -o $@ $(CFLAGS)

check_list: $(CLIST_FILES)
	$(CC) $^ -o $@ $(CFLAGS)
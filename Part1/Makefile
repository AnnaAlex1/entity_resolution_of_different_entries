CC=gcc
CFLAGS=-Wall -g

project1: readX.o readW.o hashTable.o ids_list.o result.o main.o
	$(CC) $(CFLAGS) -o project1 readX.o readW.o hashTable.o ids_list.o result.o main.o

test_project1: readX.o readW.o hashTable.o ids_list.o result.o test.o
	$(CC) $(CFLAGS) -o test_project1 readX.o readW.o hashTable.o ids_list.o result.o test.o

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

readX.o: readX.c
	$(CC) $(CFLAGS) -c readX.c

readW.o: readW.c
	$(CC) $(CFLAGS) -c readW.c

hashTable.o: hashTable.c
	$(CC) $(CFLAGS) -c hashTable.c

ids_list.o: ids_list.c
	$(CC) $(CFLAGS) -c ids_list.c

result.o: result.c
	$(CC) $(CFLAGS) -c result.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

.PHONY: clean

clean:
	rm -f *.o project1 output.csv test_project1

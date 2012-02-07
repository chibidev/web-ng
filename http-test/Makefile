CC:=g++

OPT:=2
CPU:=generic

ifndef release
	EXTRA_FLAGS:=-ggdb
else
	EXTRA_FLAGS:=-O$(OPT) -march=$(CPU) -fno-rtti -fno-exceptions -DNDEBUG
endif

CFLAGS=-c -ansi -pedantic -W -Wall -Wextra -Wcast-qual -Wcast-align -Wconversion -Wformat=2 -Wformat-security -fshort-enums -fno-common -Wno-format-y2k -Wpointer-arith -Wredundant-decls -Wshadow -Wwrite-strings $^ -I../common -o $@
LDFLAGS=$^ -o $@

OBJ:=src/main.o

all: bin/main.bin

clean:
	if [ -d bin ]; then rm -rf bin; fi
	rm $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) $(EXTRA_FLAGS)

bin/main.bin: $(OBJ)
	if [ ! -d bin ]; then mkdir bin; fi
	$(CC) $(LDFLAGS)

valgrind: bin/main.bin
	valgrind --leak-check=full bin/main.bin


CC:=g++

OPT:=2
CPU:=i686

ifndef release
	EXTRA_FLAGS:=-ggdb -O0
else
	EXTRA_FLAGS:=-O$(OPT) -march=$(CPU) -fno-rtti -fno-exceptions -DNDEBUG
endif

CFLAGS=-c -ansi -pedantic -W -Wall -Wextra -Wcast-qual -Wcast-align -Wconversion -Wformat=2 -Wformat-security -fshort-enums -fno-common -Wno-format-y2k -Wpointer-arith -Wredundant-decls -Wshadow -Wwrite-strings $^ -I../common -o $@ -std=c++0x
LDFLAGS=$^ -o $@

OBJ:=src/main.o

all: bin/main.bin

clean:
	@if [ -d bin ]; then echo "Cleaning bin"; rm -rf bin; fi
	@echo "Cleaning $(OBJ)"; rm $(OBJ)

%.o: %.cpp
	@echo "	CC $@"; $(CC) $(CFLAGS) $(EXTRA_FLAGS)

bin/main.bin: $(OBJ)
	@echo "Creating directory bin"; if [ ! -d bin ]; then mkdir bin; fi
	@echo "	LD $@"; $(CC) $(LDFLAGS)

valgrind: bin/main.bin
	valgrind --leak-check=full bin/main.bin


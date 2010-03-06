# CC=mips-linux-gnu-g++ 
# MIPS_FLAG=-EL
MIPS_FLAG=
CC=g++
CFLAGS=$(MIPS_FLAG)
LDFLAGS=$(MIPS_FLAG) -lpthread
LDLIBS=

PROJNAME = mediaproxy
BIN      = bin

CPP_HEADERS := $(wildcard *.h)
CPP_SOURCES := $(wildcard *.cpp)
OBJECTS  = $(addprefix $(BIN)/, $(CPP_SOURCES:.cpp=.o))
BINARY   = $(BIN)/$(PROJNAME)

all: $(BIN) $(BINARY)

$(BIN)/%.o: %.cpp $(CPP_HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BINARY): $(BIN) $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS) 

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -f $(BINARIES) $(OBJECTS) $(BIN)/*.elf $(BIN)/*.gdb $(BIN)/*.o
	rm -rf $(BIN)

.PHONY: clean


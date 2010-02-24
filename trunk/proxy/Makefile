# CC=mips-linux-gnu-g++ 
# MIPS_FLAG=-EL
MIPS_FLAG=
CC=g++
CFLAGS=$(MIPS_FLAG)
CXXFLAGS=$(MIPS_FLAG)
CPPFLAGS=$(MIPS_FLAG)
LDFLAGS=$(MIPS_FLAG) -lpthread
ASFLAGS=$(MIPS_FLAG)

PROJNAME = mediaproxy
BIN      = bin

CPP_SOURCES := $(wildcard *.cpp)
OBJECTS  = $(addprefix $(BIN)/, $(CPP_SOURCES:.cpp=.o))
BINARIES = $(OBJECTS:.o=)


all: $(BIN) $(BINARIES)

$(BIN)/%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN)/%: $(BIN)/%.o
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -f $(BINARIES) $(OBJECTS) $(BIN)/*.elf $(BIN)/*.gdb $(BIN)/*.o
	rm -rf $(BIN)

.PHONY: clean


CC=clang++
CFLAGS=-Wall -Wextra -std=c++11 -pedantic -c
LDFLAGS=-Wall -Wextra -std=c++11 -pedantic
SOURCES=bit_stream.cpp construct_tree.cpp decode.cpp encode.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=arj

all: clean $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
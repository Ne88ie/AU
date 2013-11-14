CC=clang++
CFLAGS=-Wall -std=c++11 -c
LDFLAGS=-Wall -std=c++11
SOURCES=main.cpp coder.cpp decoder.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=arj

all: clean $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
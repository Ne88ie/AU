CC=clang++
CFLAGS=-Wall -std=c++11 -c
LDFLAGS=-Wall -std=c++11
SOURCES=main1.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=arj

all: clean $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
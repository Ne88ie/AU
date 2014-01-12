CC = clang++
CFLAGS =-Wall -std=c++11 -Wextra -pedantic -stdlib=libc++

BINDIR = bin
SOURCEDIR = src
OBJDIR = $(BINDIR)/obj

SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%, $(OBJDIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))

TARGET = $(BINDIR)/PP

$(TARGET): clean init $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean init

init:
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(BINDIR)



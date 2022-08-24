PROGRAM=cfbdiff
SORUCES=$(wildcard src/*.cpp)
OBJECTS=$(SORUCES:%.cpp=%.o)

CXX=g++

all: $(PROGRAM)

clean:
	rm $(PROGRAM) $(OBJECTS)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $^

.cpp.o:
	$(CXX) -c $< -o $@

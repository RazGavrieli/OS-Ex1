#!make -f

CXX=clang-9
CXXFLAGS=

OBJECTS=main.c

all: main server
	./$^

server: server.c
	$(CXX) $(CXXFLAGS) $^ -o server

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o main

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@


clean:
	rm -f *.o main server
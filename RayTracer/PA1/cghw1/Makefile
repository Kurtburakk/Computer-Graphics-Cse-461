CXX = g++
CXXFLAGS = -std=c++11 -Itinyxml2
LDFLAGS = 

SRC = parser.cpp main.cpp raytracer.cpp tinyxml2/tinyxml2.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = program

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) *.png

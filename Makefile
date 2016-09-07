TARGET=bin/huffman-converter
INCLUDE=.
CXX=g++
CXXFLAGS= -std=c++14 -O2
OBJ=huffman-converter.o huffman-decoder.o huffman-encoder.o
SRC=$(OBJ:.o=.cpp)
HEADERS=$(OBJ:.o=.h)

#APP=main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -I. $(CXXFLAGS) -o $(TARGET) $(OBJ)
	chmod +x $(TARGET)

huffman-converter.o: $(SRC) huffman-converter.h
	$(CXX) -c $*.cpp $(CXXFLAGS)

huffman-decoder.o: huffman-decoder.cpp huffman-converter.h 
	$(CXX) -c $*.cpp $(CXXFLAGS)

huffman-encoder.o: huffman-encoder.cpp huffman-converter.h
	$(CXX) -c $*.cpp $(CXXFLAGS)

run:
	./$(TARGET)

clean:
	rm *.o
	rm $(TARGET)

SOURCES = graph.cpp util.cpp oracle.cpp lockedCircuit.cpp BDE.cpp
LDFLAGS = -fopenmp       
OBJECTS = $(SOURCES:.cpp=.o)  
TARGET = BDE     

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install
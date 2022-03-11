EXECUTABLE=main
CXX=g++
CXXFLAGS=-Wall -std=c++11 -pthread
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJS)

$(OBJS): %.o: %.cpp

.PHONY: clean
clean:
	$(RM) $(OBJS) $(EXECUTABLE)
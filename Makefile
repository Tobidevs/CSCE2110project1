CXX      = g++
CXXFLAGS = -std=c++17 -Wall
TARGET   = qparking
SRCS     = main.cpp config.cpp status.cpp
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) output.txt

run: $(TARGET)
	./$(TARGET)

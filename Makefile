CXX = gcc
CXXFLAGS = -Wall -Werror 

OBJS = main.o board.o game.o player.o alg.o
TARGET = main

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^


%.o: %.c %.h
	$(CXX) $(CXXFLAGS) -c $<


clean:
	rm -f $(OBJS) $(TARGET)
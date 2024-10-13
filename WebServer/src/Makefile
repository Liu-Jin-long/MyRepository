CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -g 
LDFLAGS:= -pthread -lmysqlclient
TARGET = main
SRCS = $(wildcard log/*.cpp) $(wildcard pool/*.cpp) $(wildcard timer/*.cpp) \
       $(wildcard http/*.cpp) $(wildcard server/*.cpp) $(wildcard buffer/*.cpp) \
       main.cpp
OBJS = $(SRCS:.cpp=.o)
all: $(TARGET)
$(TARGET): $(OBJS)  
	$(CXX) -o $@ $^ $(LDFLAGS)
%.o: %.cpp  
	$(CXX) $(CXXFLAGS) -c $< -o $@  

.PHONY:clean rebuild
rebuild:clean all
clean:
	rm -rf $(OBJS) $(TARGET)

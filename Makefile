CXX = g++
TARGET = Server
SOURCE = $(wildcard ./source/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))
INCLUDE = -I./header
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) -lpthread
%.o:%.cpp
	$(CXX) $(INCLUDE) -c $^ -o $@

.PHONY:clean
clean:
	rm $(OBJS) $(TARGET)
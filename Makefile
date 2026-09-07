# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -O2

# Target executable
TARGET = common_wrapper

# Source files
SRCS = \
	common_wrapper/common_wrapper.cpp \
	Assignment_1/src/bfs.cpp \
	Assignment_1/src/csr.cpp \
	Assignment_1/src/dfs.cpp \
	Assignment_1/src/parser.cpp \
	Assignment_1/src/sssp.cpp \
	Assignment_1/driver/graph_driver.cpp \
	Assignment_2/src/betweenness_centrality.cpp \
	Assignment_2/src/connected_components.cpp \
	Assignment_2/src/triangle_counting.cpp \
	Assignment_2/driver/graph_driver.cpp \
	Assignment_3/src/gradient_descent.cpp \
	Assignment_3/src/maxflow.cpp \
	Assignment_3/driver/graph_driver.cpp \
	Assignment_4/src/kmeans.cpp \
	Assignment_4/driver/graph_driver.cpp 

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove generated files
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all clean
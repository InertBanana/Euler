# Euler makefile

# VARS
CXX = g++
CXXFLAGS = -std=c++11 

#DEPS
DEPS = problem_11.h utils.h
OBJ = euler.o problem_11.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# TARGETS
euler: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

# euler.o: euler.cpp problem_11.h
# 	$(CXX) $(CXXFLAGS) -o euler.cpp 

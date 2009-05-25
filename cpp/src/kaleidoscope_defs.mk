
CPP := g++

# the find upwards is not needed, it searches ., .., ../.., etc
INCLUDE_DIR := $(find_upwards include)

INCLUDES := -I$(INCLUDE_DIR) -I/usr/include/SDL

%.o : %.cpp
	$(CPP) $(INCLUDES) -c $(input) -o $(output) 


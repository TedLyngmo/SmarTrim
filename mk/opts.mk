BUILD_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

STD := c++23
CXXOPTS := -std=$(STD) -O3 -DNDEBUG -Wall -Wextra -pedantic-errors -g -fsanitize=address,undefined -I $(BUILD_DIR)/include
LDFLAGS := -fsanitize=address,undefined

COMPILE = $(CXX) -c -o $@ $< $(CXXOPTS)
LINK = $(CXX) -o $@ $(filter-out %.h %.hpp,$^) $(LDFLAGS)

%.o : %.cpp $(wildcard $*.hpp)
	$(COMPILE)

#% : %.o
#	$(LINK)

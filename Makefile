CXX      ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -Iinclude -g
LDFLAGS  ?=
LDLIBS   ?=

PROGRAM   := bin/main
BUILD_DIR := build

# Keep the source list in one place so adding a .cpp file under src/ includes
# it automatically in the build.
SOURCES := $(shell find src -type f -name '*.cpp' -print)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -rf $(BUILD_DIR) $(PROGRAM)

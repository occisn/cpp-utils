CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O3 -fopenmp -pthread -march=native
SRCDIR = src
TESTDIR = tests
BUILDDIR = build
INCLUDEDIR = include

# Find all source files
MAIN_SOURCES = $(wildcard $(SRCDIR)/*.cpp)
MAIN_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(MAIN_SOURCES))
TARGET = $(BUILDDIR)/main

# Test files
TEST_SOURCES = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS = $(patsubst $(TESTDIR)/%.cpp,$(BUILDDIR)/test_%.o,$(TEST_SOURCES))

# Source files for tests (exclude main.cpp)
SRC_FOR_TESTS = $(filter-out $(SRCDIR)/main.cpp,$(wildcard $(SRCDIR)/*.cpp))
SRC_TEST_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRC_FOR_TESTS))

# Catch2 amalgamated
CATCH_OBJ = $(BUILDDIR)/catch_amalgamated.o

# All test objects
ALL_TEST_OBJS = $(TEST_OBJS) $(SRC_TEST_OBJS) $(CATCH_OBJ)
TEST_TARGET = $(BUILDDIR)/tests

all: $(BUILDDIR) $(TARGET)

test: $(BUILDDIR) $(TEST_TARGET)
	$(TEST_TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Build main executable
$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_OBJS)

# Build test executable
$(TEST_TARGET): $(ALL_TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(ALL_TEST_OBJS)

# Compile source files for main
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(INCLUDEDIR) -c $< -o $@

# Compile test files
$(BUILDDIR)/test_%.o: $(TESTDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(INCLUDEDIR) -c $< -o $@

# Compile Catch2 amalgamated implementation
$(BUILDDIR)/catch_amalgamated.o: $(INCLUDEDIR)/catch_amalgamated.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

run: $(TARGET)
	$(TARGET)

.PHONY: all test clean run


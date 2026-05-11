CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src

SRC = src/models/Student.cpp \
      src/models/UndergraduateStudent.cpp \
      src/models/GraduateStudent.cpp \
      src/models/PhDStudent.cpp \
      src/main.cpp

TARGET = partie1

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean

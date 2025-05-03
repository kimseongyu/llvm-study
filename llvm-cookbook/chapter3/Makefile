CC = clang++
SOURCE = toy.cpp
TARGET = toy

$(TARGET) : $(SOURCE)
	clang-format -style=google -i $(SOURCE)
	$(CC) $(SOURCE) -o  $(TARGET) -g -O3 `llvm-config --cxxflags --ldflags --system-libs --libs core`

clean :
	rm $(TARGET)
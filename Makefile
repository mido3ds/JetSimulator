.PHONY: all cmake build run

all: build run

build:
	cmake --build build

run:
	./build/JetSimulator/JetSimulator

clean:
	cmake --build build --target clean

cleanAll:
	rm -rf build CMakeFiles

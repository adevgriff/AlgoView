all:
	g++ -Wall -o sort sort.cpp ./*.c ./*.h -lm -lX11 -lGL -lGLU 
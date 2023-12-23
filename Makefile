main: ./src/main.cpp ./src/Graphics.cpp ./src/Player.cpp
	g++ ./src/main.cpp ./src/Graphics.cpp ./src/Player.cpp -o main -lncurses -g

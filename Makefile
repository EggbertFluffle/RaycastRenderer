main: ./src/main.cpp ./src/Graphics.cpp ./src/Player.cpp ./src/Eggbug.cpp
	g++ ./src/main.cpp ./src/Graphics.cpp ./src/Player.cpp ./src/Eggbug.cpp -o main -lncursesw

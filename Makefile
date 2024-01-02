main: ./src/main.cpp ./src/Graphics.cpp ./src/Player.cpp ./src/Eggbug.cpp ./src/Textures.cpp
	g++ ./src/main.cpp ./src/Graphics.cpp ./src/Player.cpp ./src/Eggbug.cpp ./src/Textures.cpp -o main -lncursesw

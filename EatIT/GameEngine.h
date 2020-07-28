#pragma comment(lib, "winmm.lib")
#pragma once
#include "windows.h"
#include <thread>
#include <iostream>
#include <stdlib.h>

#define sizeY 20
#define sizeX 20

class GameEngine
{
private:
	int matrix[sizeY][sizeX];
	bool playerAlive;
	int score;
	int level;
	int posX;
	int foodX;
	int foodY;
	int posY;
	int enemyX;
	int enemyY;
	void drawBoundry();
	void goTo(int X, int Y);
	void changeColor(int ColorVal);
	void createPlane();
	void showScore();
	void resetBoard();
	void createEnemy();
	void moveEnemy();
	void createFood();
	void moveFood();
	void draw();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void listenKey();
	bool checkCollision();
	void endingMessage();
public:
	GameEngine();
	void startGame();
};


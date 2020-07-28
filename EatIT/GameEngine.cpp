#include "GameEngine.h"
using namespace std;
/*
In Matrix[][]
1 is for boundry
2 is for plane
3 is for enemy
4 is for food
*/
GameEngine::GameEngine() {
	playerAlive = false;
	//default positions.
	//[3][3] from bottom left
	posX = 1;
	foodX = 18;
	foodY = 0;
	level = 0;
	posY = sizeY-4;
	enemyX = 1;
	enemyY = -3;
	score = 0;
	resetBoard();
}
void GameEngine::startGame() {
	playerAlive = true;
	drawBoundry();
	thread keylistenThread([this] {this->listenKey(); });//Confusion
	string soundfile = "gameRunning.wav";
	PlaySound(TEXT("gameRunning.wav"), NULL, SND_ASYNC);
	while (playerAlive) {
		resetBoard();
		createEnemy();
		moveEnemy();
		createPlane();
		createFood();
		moveFood();
		draw();
		showScore();
		playerAlive = !checkCollision();
		Sleep(100-(7*level));
	}
	keylistenThread.join();
	endingMessage();
}
void GameEngine::endingMessage() {
	system("cls");
	goTo(1, 5);
	changeColor(4);
	cout << "The resistance fought hard against the Empire but her fate was already written";
	goTo(1, 7);
	cout << "Every hope of eradicating evil died with death of last Jedi Master";
	goTo(30, 10);
	changeColor(3);
	cout << "GAME OVER";
	changeColor(7);
	cout << endl << endl;

}
void GameEngine::drawBoundry() {
	changeColor(2);
	for (int current_Y = 0; current_Y < sizeY; current_Y++)
	{
		for (int current_X = 0; current_X < sizeX; current_X++) {
			//Draw Boundry
			if (current_Y == 0 || current_X == 0 || current_X == sizeX - 1 || current_Y == sizeY - 1) {
				matrix[current_Y][current_X] = 1;
				goTo(current_X, current_Y);
				cout << "#";
			}
		}
	}
}
void GameEngine::resetBoard() {
	//Reset to 0 except boundry
	for (int current_Y = 1; current_Y < sizeY-1; current_Y++)
	{
		for (int current_X = 1; current_X < sizeX-1; current_X++) {
			matrix[current_Y][current_X] = 0;
		}
	}
}
void GameEngine::createPlane() {
	//[3][3] plane from current Top Left plane position
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++) {
			if (j == 1 || i == 1){
				matrix[posY + i][posX + j] = 2;
			}
		}
	}
}
void GameEngine::createEnemy() {
	//[3][3] plane from current Top Left plane position
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if ((j == 1 || i == 1) && (enemyY+i>0)) {
				matrix[enemyY + i][enemyX + j] = 3;
			}
		}
	}
}
int enemies_survived = 0;
void GameEngine::moveEnemy() {
	enemyY++;
	//Reset Enemy position if it crosses boundry
	if (enemyY > 17) {
		enemyX = rand() % 16 + 1;
		/*enemyX = rand() % 16 + 1*/
		enemyY = -2;	//0 - enemysizeY
		score++;
		//Max 10 Levels
		if (level < 10) {
			//Increment level if level*level(enemies survived)
			if (++enemies_survived >= level * 2) {
				level++;
				enemies_survived = 0;
			}
		}
	}
}
void GameEngine::createFood(){
	matrix[foodY][foodX] = 4;
}
void GameEngine::moveFood() {
	foodY++;
	bool eaten = false;
	if (foodY == posY) {
		if (foodX == posX || (foodX == posX + 1) || (foodX == posX + 2)) {
			eaten = true;
		}
	}
	else if ((foodY == posY + 1) || (foodY == posY + 2)) {
		if (foodX == posX || foodX == posX + 2) {
			eaten = true;
		}
	}
	if (eaten == true) {
		score += 5;
		foodY = 0;
		foodX = rand() % 18 + 1;
	}
	else if (foodY > 17) {
		foodY = 0;
		foodX = rand() % 18 + 1;
	}
}
void GameEngine::draw() {
	for (int current_Y = 1; current_Y < sizeY-1; current_Y++)
	{
		for (int current_X = 1; current_X < sizeX-1; current_X ++ ) {
			//Draw Plane
			//Blue Color
			goTo(current_X, current_Y);
			if (matrix[current_Y][current_X] == 2) {
				changeColor(1);
				printf("*");
			}
			//Drawing Enemy.
			//Red Color
			else if (matrix[current_Y][current_X] == 3) {
				changeColor(4);
				printf("O");
			}
			else if (matrix[current_Y][current_X] == 4) {
				changeColor(3);
				printf("x");
			}
			else
				printf(" ");
		}
	}
}
void GameEngine::listenKey() {
	while (playerAlive) {
		if (GetAsyncKeyState(VK_LEFT) && (0x8000 != 0)) {
			moveLeft();
		}
		else if (GetAsyncKeyState(VK_RIGHT) && (0x8000 != 0)) {
			moveRight();
		}
		else if (GetAsyncKeyState(VK_UP) && (0x8000 != 0)) {
			moveUp();
		}
		else if (GetAsyncKeyState(VK_DOWN) && (0x8000 != 0)) {
			moveDown();
		}
		Sleep(100);
	}
}
bool GameEngine::checkCollision() {
	bool collided = (posX < 1 || posY < 1 || posX > 16 || posY > 16);//crossed boundry check

	/**collision with ship**/

	//Enemy bottom Right corner Player Top Left Corner and Enemy Right side player left
	if (enemyX + 2 == posX) {
		if (enemyY + 2 == posY || enemyY+1 == posY || enemyY == posY)
			collided = true;
	}
	//Enemy bottom Left corner Player Top Right Corner and Enemy Left side player right
	else if (enemyX == posX + 2) {
		if (enemyY == posY || enemyY + 1 == posY || enemyY == posY) {
			collided = true;
		}
	}
	//Front Collision
	else if (enemyX == posX && enemyY ==posY) {
		collided = true;
	}
	
	if (collided) {
		string soundfile = "collision.wav";
		PlaySound(TEXT("collision.wav"), NULL, SND_FILENAME);
	}
	return collided;
}
void GameEngine::showScore() {
	goTo(25, 1);
	changeColor(6);
	printf("Score:");
	goTo(25, 3);
	printf("Level:");
	goTo(33, 1);
	changeColor(7);
	cout << score;
	goTo(33, 3);
	cout << level;
}
void GameEngine::moveLeft() {
	posX--;
}
void GameEngine::moveRight() {
	posX++;
}
void GameEngine::moveUp() {
	posY--;
}
void GameEngine::moveDown() {
	posY++;
}
void GameEngine::goTo(int X, int Y) {
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void GameEngine::changeColor(int ColorVal) {
	//0 = Black 8 = Gray
	//1 = Blue 9 = Light Blue
	//2 = Green a = Light Green
	//3 = Aqua b = Light Aqua
	//4 = Red c = Light Red
	//5 = Purple d = Light Purple
	//6 = Yellow e = Light Yellow
	//7 = White f = Bright White
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, ColorVal);
}
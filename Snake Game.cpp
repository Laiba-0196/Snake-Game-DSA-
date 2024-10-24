#include <iostream>
#include <conio.h> 
#include <stdlib.h>
#include <map> 
#include <time.h>
#include <windows.h>

using namespace std;

// Global Variables
map<int, map<int, char> > frame; // 2d map
float speed = 7.0; // Speed of snake or frame rate
int i, j; // Counter variables
int width = 50, height = 20; // Map dimensions
int target = 5;	// Target setting
int score; // Score
bool game_over;	// Game over variable
// Directions
bool direction1; // 0 means vertical, 1 means horizontal
bool direction2; // In vertical: 0 means right 1 means left.
// In horizontal: 0 means up and 1 means down

//Definitions
void show1();
void show2();

// Class Scoreboard
class ScoreBoard
{
public:
	int counter;
	friend class Snake;
	void displayScore() {
		cout << "	Score:" << counter << " ";
		cout << "	Target:" << target;
	}
};

// Class Food
class Food
{
public:
	friend class Snake;
	int foodx, foody;
	char food = '@';

	void foodGeneration() {
		foodx = (rand() % (width - 7)) + 3;
		foody = (rand() % (height - 6)) + 3;
		if (frame[foody][foodx] == ' ')  // fruit will not be generated
		{
			frame[foody][foodx] = food;  // where snake body is
		}
		else foodGeneration();
	}
};

// Objects
Food f; // Food object
ScoreBoard s;	// Scoreboard object

// Class Node
class Node
{
	friend class Snake;

private:
	int xpos, ypos;
	Node* nextNode;
};

//Class Snake
class Snake
{
private:
	//Queue implementation
	Node* front;
	Node* rear;
	char head = 'O';		//Head part of the snake
	char body_part = 'o';	//Body part of the snake
public:
	//Food d;
	//ScoreBoard sb;
	int headx, heady;	// Head position
	int bodyx, bodyy;	// Body position
	Snake(int headx = 0, int heady = 0, int bodyx = 0, int bodyy = 0)
	{
		this->headx = headx;
		this->heady = heady;
		this->bodyx = bodyx;
		this->bodyy = bodyy;
		front = new Node();
		rear = new Node();
		front = NULL;
		rear = NULL;
	}

	//Enqueue body
	void enqueue(int x, int y)
	{
		Node* newNode = new Node();
		newNode->xpos = x;
		newNode->ypos = y;
		newNode->nextNode = NULL;

		if (rear == NULL)
		{
			rear = newNode;
			front = newNode;
		}
		else
		{
			rear->nextNode = newNode;
			rear = newNode;
		}
	}

	// Put data in given variables
	void dequeue(int& x, int& y)
	{
		if (!(this->isEmpty()))
		{
			x = front->xpos;
			y = front->ypos;

			Node* temp = front;
			front = front->nextNode;

			delete temp;

			if (this->isEmpty())
			{
				rear = NULL;
			}
		}
		else cout << "Queue is Empty.\n";
	}

	// only return the node data from front of queue
	void getFront(int& x, int& y)
	{
		if (!(this->isEmpty()))
		{
			x = front->xpos;
			y = front->ypos;
		}

		else cout << "Queue is empty.\n";
	}

	bool isEmpty() {
		return (front == NULL);
	}

	// Initializing
	void initialize()
	{
		// outerbox
		for (i = 0; i < width; i++)
		{
			frame[0][i] = '-';
		}
		for (i = 1; i < height - 1; i++)
		{
			frame[i][0] = '|';
			for (j = 1; j < width - 1; j++)
			{
				frame[i][j] = ' ';
			}
			frame[i][j] = '|';
		}
		for (i = 0; i < width; i++)
		{
			frame[height - 1][i] = '-';
		}

		// innerbox
		frame[1][1] = '-';
		for (i = 2; i < width - 2; i++)
		{
			frame[1][i] = frame[height - 2][i] = '-';
		}
		frame[1][i] = '-';
		for (i = 2; i < height - 2; i++)
		{
			frame[i][1] = frame[i][width - 2] = '|';
		}
		frame[i][1] = '-';
		frame[i][width - 2] = '-';


		// Placing head at random position in start
		srand(time(NULL));

		headx = (rand() % (width - 4)) + 4;
		heady = (rand() % (height - 5)) + 2;

		frame[heady][headx] = head;

		enqueue(headx - 2, heady);
		frame[heady][headx - 2] = body_part;
		enqueue(headx - 1, heady);
		frame[heady][headx - 1] = body_part;

		display();

		// initializing snake direction to vertical and right
		direction1 = 0;
		direction2 = 0;

		game_over = false;

		Sleep(1000 / speed);
	}
	void bodyAddition()
	{
		// Add a new body part (this is called when a fruit is eaten)
		moveSnake();

		display();

		Sleep(1000 / speed);
	}
	void updation()
	{
		// Show next frame
		moveSnake();

		dequeue(bodyx, bodyy);
		frame[bodyy][bodyx] = ' ';

		display();

		Sleep(1000 / speed);
	}


	void directionChange(char dir)
	{
		if ((dir == 72 || dir == 80) && direction1 == 0)
		{ // Vertical
			if (dir == 72)
			{
				direction2 = 0;
			}
			else
			{
				direction2 = 1;
			}

			direction1 = 1;
		}
		else if ((dir == 75 || dir == 77) && direction1 == 1)
		{ // Horizontal
			if (dir == 75)
			{
				direction2 = 1;
			}
			else
			{
				direction2 = 0;
			}

			direction1 = 0;
		}
	}
	void moveSnake()
	{   // Move head to its next node
		enqueue(headx, heady);
		frame[heady][headx] = body_part;

		if (direction1 == 0 && direction2 == 0)
		{
			headx++;
		}
		else if (direction1 == 0 && direction2 == 1)
		{
			headx--;
		}
		else if (direction1 == 1 && direction2 == 0)
		{
			heady--;
		}
		else if (direction1 == 1 && direction2 == 1)
		{
			heady++;
		}

		// Add body if head reaches the fruit
		if (headx == f.foodx && heady == f.foody)
		{
			s.counter++;
			bodyAddition();
			f.foodGeneration();
		}

		// Also increase the speed
		for (int con = 45; score == con;)
		{
			speed += 5;
			con += 45;
		}

		// If the score counter reaches the target, the game will be over.
		if (s.counter == target)
		{
			game_over = true;
			cout << " You Win";

		}

		// gameover if head touches the wall or body
		if (frame[heady][headx] == '-' || frame[heady][headx] == '|' || frame[heady][headx] == body_part)
		{
			game_over = true;
		}

		frame[heady][headx] = head;
	}

	void display()
	{
		// print the frame
		system("cls");
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				cout << frame[i][j];
			}
			if (i == 8)
			{
				s.displayScore();
			}
			cout << endl;
		}
	}

};
//void move();

// Snake queue object
Snake snake;


void show1() {
	char get;
	cout << "\t\t\t\t  -----------------------\n";
	cout << "\t\t\t\t\tSnake Game\n";
	cout << "\t\t\t\t  -----------------------\n";
	cout << "\n\n\n\n\n";
	cout << "\t\t\t\t   Welcome to Snake Game\n\n\n\n";
	cout << "Press enter to start the game: ";
	cin.get(get);
	system("cls");
	show2();
}

void show2() {
	// Iniatilizing the variables
	speed = 10;
	width = 50, height = 20;
	target = 30;
	//initialize()
	snake.initialize();
	//generateFruit()
	f.foodGeneration();
	// Game loop
	do
	{
		//update();
		snake.updation();
		if (_kbhit())
		{
			snake.directionChange(_getch());
		}
	} while (game_over == false);
	if (score == target)
	{
		cout << "You win\n\n";
	}
	else
	{
		cout << "Game Over.\n\n";
	}
	system("pause");
}
int main()
{
	show1();
}

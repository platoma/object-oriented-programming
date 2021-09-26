#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

using namespace bangtal;

ScenePtr scene;
ObjectPtr game_board[16], game_original[16];
ObjectPtr start_button;
ObjectPtr end_button;
TimerPtr mix_timer;
TimerPtr loop_timer;
TimerPtr elapsed_timer;
int size = 3;
int blank;
int mix_count;
int time_count;
int best_record = 0;
float animation_time = 0.05f;
bool game_completed;

int indexToX(int index);
int indexToY(int index);
int gameIndex(ObjectPtr piece);
int randomMove();
bool checkMove(int index);
bool checkEnd();
void gameMove(int index);
void initGame();
void startGame();
void completeGame();

int main()
{
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	initGame();

	startGame(scene);
}

int indexToX(int index)
{
	return 415 + 150 * (index % size);
}

int indexToY(int index)
{
	return 435 - 150 * (index / size);
}

int gameIndex(ObjectPtr piece)
{
	for (int i = 0; i < size * size; i++)
	{
		if (game_board[i] == piece)
		{
			return i;
		}
	}

	return -1;
}

int randomMove()
{
	int index = rand() % (size * size);

	while (!checkMove(index))
	{
		index = rand() % (size * size);
	}

	return index;
}

bool checkMove(int index)
{
	if (blank / size > 0 && index == blank - size)
	{
		return true;
	}
	else if (blank / size < size - 1 && index == blank + size)
	{
		return true;
	}
	else if (blank % size > 0 && index == blank - 1)
	{
		return true;
	}
	else if (blank % size < size - 1 && index == blank + 1)
	{
		return true;
	}

	return false;
}

bool checkEnd()
{
	for (int i = 0; i < size * size; i++)
	{
		if (game_board[i] != game_original[i])
		{
			return false;
		}
	}

	return true;
}

void gameMove(int index)
{
	auto piece = game_board[index];

	game_board[index] = game_board[blank];
	game_board[blank] = piece;

	game_board[index]->locate(scene, indexToX(index), indexToY(index));
	game_board[blank]->locate(scene, indexToX(blank), indexToY(blank));

	blank = index;
}

void initGame()
{
	char path[20];

	scene = Scene::create("겨울왕국2 퍼즐", "Images/배경.png");

	for (int i = 0; i < size * size; i++)
	{
		sprintf(path, "Images/%d.png", i + 1);

		game_board[i] = Object::create(path, scene, indexToX(i), indexToY(i));

		game_board[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {
			int index = gameIndex(piece);

			if (checkMove(index) == true)
			{
				gameMove(index);

				if (checkEnd())
				{
					completeGame();
				}
			}

			return true;
			});

		game_original[i] = game_board[i];
	}

	start_button = Object::create("Images/start.png", scene, 590, 100);

	start_button->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		startGame();

		return true;
		});

	end_button = Object::create("Images/end.png", scene, 590, 50);

	end_button->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		endGame();

		return true;
		});

	mix_timer = Timer::create(animation_time);

	mix_timer->setOnTimerCallback([&](auto)->bool {
		gameMove(randomMove());

		mix_count--;
		if (mix_count > 0)
		{
			mix_timer->set(animation_time);

			mix_timer->start();
		}
		else
		{
			showTimer(elapsed_timer);

			loop_timer->set(1.0f);

			loop_timer->start();
		}

		return true;
		});

	loop_timer = Timer::create(1.0f);

	loop_timer->setOnTimerCallback([&](auto)->bool {
		if (!game_completed)
		{
			loop_timer->set(1.0f);

			loop_timer->start();
			
			time_count++;

			elapsed_timer->increase(1.0f);
		}
		
		return true;
		});

	elapsed_timer = Timer::create(0.0f);
}

void startGame()
{
	mix_count = 100;
	time_count = 0;
	blank = size * size - 1;
	game_completed = false;

	game_board[blank]->hide();

	mix_timer->set(animation_time);

	mix_timer->start();

	elapsed_timer->set(0.0f);

	start_button->hide();
	end_button->hide();
}

void completeGame()
{
	game_board[blank]->show();

	showMessage("Completed!");

	if (!best_record || best_record > time_count)
	{
		showMessage("New Record!");

		best_record = time_count;
	}

	game_completed = true;

	start_button->setImage("Images/restart.png");

	start_button->show();
	end_button->show();
}

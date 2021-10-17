#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace bangtal;

ScenePtr none = Scene::create("GameStart", "Images/none.png");
ScenePtr scene = Scene::create("PersonalProject", "Images/background.png");
ObjectPtr start_button = Object::create("Text/start.png", none, 500, 420);
ObjectPtr quit_button = Object::create("Text/quit.png", none, 1200, 40);
ObjectPtr explain_button = Object::create("Text/explain.png", none, 500, 280);
ObjectPtr how_to_play = Object::create("Text/how-to-play.png", none, 0, 0, false);
ObjectPtr game_over = Object::create("Text/game-over.png", none, 370, 450, false);
ObjectPtr clear = Object::create("Text/clear.png", none, 470, 500, false);
ObjectPtr or_object = Object::create("Text/or.png", none, 640, 160, false);
ObjectPtr rank_object = Object::create("Text/rank.png", none, 470, 420, false);
ObjectPtr rank = Object::create("Text/f.png", none, 550, 420, false);

ObjectPtr hurdle;
ObjectPtr cloud1, cloud2;
ObjectPtr main_char;
ObjectPtr life[3];
TimerID timer;

int char_x, char_y;
int move_img_num;
int jump_img_num;
int jump_time;
int collision_time;
int hurdle_x, hurdle_y;
int hurdle_counter;
int hurdle_step;
int cloud1_x, cloud1_y;
int cloud2_x, cloud2_y;
int cloud_step;
int life_index;
char path[40];
const int JUMP_TIME = 8;
const int JUMP_STEP = 60;
const int COLLISION_TIME = 4;
const int TOTAL_LIFE_NUM = 3;
const int TOTAL_HURDLE_NUM = 30;
const float ANIMATION_TIME = 0.1f;
bool isJumping;
bool isDashing;
bool isColliding;
bool isShowing;
bool isPlaying;
time_t start_time;
time_t end_time;

void keyboardCallback(KeyCode code, KeyState state);
void timerCallback(TimerID timer);
void gameStart();
void gameEnd();
void initGame();
void charJump();
void charDash();
void charMove();
void charCollide();
void hurdleMove();
void cloudMove();
char calcScore();
bool isCollision();

int main()
{
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	gameStart();

	startGame(none);
}

void keyboardCallback(KeyCode code, KeyState state)
{ 
	if (!isPlaying)
	{
		return;
	}

	if (code == KeyCode::KEY_SPACE)
	{
		jump_img_num = -1;

		isJumping = true;
	}
	else if (code == KeyCode::KEY_RIGHT_ARROW)
	{
		if (state == KeyState::KEY_PRESSED)
		{
			isDashing = true;
		}
		else
		{
			hurdle_step = 75;

			cloud_step = 30;

			isDashing = false;
		}
	}
}

void timerCallback(TimerID timer)
{
	if (isJumping)
	{
		charJump();
	}
	else
	{
		charMove();
	}

	if (isDashing)
	{
		charDash();
	}

	main_char->setImage(path);
	main_char->setScale(2.f);

	hurdleMove();

	if (isCollision())
	{
		isColliding = true;
	}

	if (isColliding)
	{
		if (collision_time == 0)
		{
			life[life_index++]->hide();
		}

		if (life_index == TOTAL_LIFE_NUM)
		{
			gameEnd();

			return;
		}

		charCollide();
	}

	if (hurdle_counter == TOTAL_HURDLE_NUM)
	{
		gameEnd();

		return;
	}

	cloudMove();

	setTimer(timer, ANIMATION_TIME);

	startTimer(timer);
}

void gameStart()
{
	hurdle = Object::create("Images/hurdle.png", scene);

	sprintf(path, "Images/cloud%d.png", rand() % 2 + 1);
	cloud1 = Object::create(path, scene);

	sprintf(path, "Images/cloud%d.png", rand() % 2 + 1);
	cloud2 = Object::create(path, scene);

	main_char = Object::create("Images/walk1.png", scene);

	for (int i = 0; i < TOTAL_LIFE_NUM; i++)
	{
		sprintf(path, "Images/life.png");

		life[i] = Object::create(path, scene, 1190 + 30 * i, 690);
	}

	quit_button->setScale(0.5f);

	start_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		initGame();

		return true;
		});

	quit_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();

		return true;
		});

	explain_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		how_to_play->show();

		return true;
		});

	how_to_play->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		how_to_play->hide();

		return true;
		});
}

void gameEnd()
{
	end_time = time(NULL);

	none->enter();

	start_button->setImage("Text/replay.png");
	start_button->locate(none, 420, 160);

	quit_button->setScale(1.f);
	quit_button->locate(none, 740, 160);

	explain_button->hide();

	if (life_index == TOTAL_LIFE_NUM)
	{
		game_over->show();
		clear->hide();

		rank_object->hide();
		rank->hide();
	}
	else
	{
		game_over->hide();
		clear->show();

		rank_object->show();

		sprintf(path, "Text/%c.png", calcScore());

		rank->setImage(path);
		rank->setScale(1.25f);
		rank->show();
	}

	or_object->show();
}

void initGame()
{
	scene->enter();

	char_x = 40;
	char_y = 100;

	move_img_num = -1;
	jump_img_num = -1;

	jump_time = 0;
	collision_time = 0;

	hurdle_x = 1280;
	hurdle_y = 80;

	hurdle_counter = 0;

	hurdle_step = 75;

	cloud1_x = 600;
	cloud1_y = 520;

	cloud2_x = 1280;
	cloud2_y = 580;

	cloud_step = 30;

	life_index = 0;

	isJumping = false;
	isDashing = false;
	isColliding = false;
	isShowing = true;
	isPlaying = true;

	main_char->locate(scene, char_x, char_y);

	hurdle->locate(scene, hurdle_x, hurdle_y);

	cloud1->locate(scene, cloud1_x, cloud1_y);

	cloud2->locate(scene, cloud2_x, cloud2_y);

	for (int i = 0; i < TOTAL_LIFE_NUM; i++)
	{
		life[i]->show();
	}

	start_time = time(NULL);

	timer = createTimer(ANIMATION_TIME);

	startTimer(timer);
}

void charJump()
{
	if (jump_time / (JUMP_TIME / 2) == 0)
	{
		jump_img_num = 1;

		char_y += JUMP_STEP;

		jump_time++;
	}
	else if (jump_time / (JUMP_TIME / 2) == 1)
	{
		jump_img_num = 2;

		char_y -= JUMP_STEP;

		jump_time++;
	}
	else if (jump_time / (JUMP_TIME / 2) == 2)
	{
		jump_img_num = 3;

		isJumping = false;

		jump_time = 0;

		move_img_num = -1;
	}

	sprintf(path, "Images/jump%d.png", jump_img_num);

	main_char->locate(scene, char_x, char_y);
}

void charDash()
{
	if (isShowing && (hurdle_x - 350 <= char_x && hurdle_x + 150 >= char_x))
	{
		hurdle_step = 45;

		cloud_step = 20;
	}
	else
	{
		hurdle_step = 150;

		cloud_step = 60;
	}
}

void charMove()
{
	move_img_num++;

	sprintf(path, "Images/walk%d.png", move_img_num % 8 + 1);
}

void charCollide()
{
	if (collision_time == COLLISION_TIME)
	{
		isColliding = false;

		collision_time = 0;

		return;
	}

	if (collision_time % 2 == 0)
	{
		main_char->hide();

		hurdle->hide();

		collision_time++;
	}
	else
	{
		main_char->show();

		hurdle->show();

		collision_time++;
	}
}

void hurdleMove()
{
	hurdle_x -= hurdle_step;

	if (hurdle_x < -500)
	{
		hurdle_x = 1280;

		if (rand() % 5 < 3)
		{
			hurdle->show();

			isShowing = true;
		}
		else
		{
			hurdle->hide();

			isShowing = false;
		}

		hurdle_counter++;
	}

	hurdle->locate(scene, hurdle_x, hurdle_y);
}

void cloudMove()
{
	cloud1_x -= cloud_step;
	cloud2_x -= cloud_step;

	if (cloud1_x < -184)
	{
		cloud1_x = 1280;

		sprintf(path, "Images/cloud%d.png", rand() % 2 + 1);

		cloud1->setImage(path);
	}

	if (cloud2_x < -184)
	{
		cloud2_x = 1280;

		sprintf(path, "Images/cloud%d.png", rand() % 2 + 1);

		cloud2->setImage(path);
	}

	cloud1->locate(scene, cloud1_x, cloud1_y);
	cloud2->locate(scene, cloud2_x, cloud2_y);
}

bool isCollision()
{
	if (!isShowing)
	{
		return false;
	}

	if ((hurdle_x <= char_x && hurdle_x + 130 >= char_x) && (hurdle_y <= char_y && hurdle_y + 170 >= char_y))
	{
		return true;
	}

	return false;
}

char calcScore()
{
	int time = end_time - start_time;

	if (life_index == 0)
	{
		if (time <= 55)
		{
			return 's';
		}
		else
		{
			return 'a';
		}
	}
	else if (life_index == 1)
	{
		if (time <= 55)
		{
			return 'b';
		}
		else
		{
			return 'c';
		}
	}
	else
	{
		if (time <= 55)
		{
			return 'd';
		}
		else
		{
			return 'f';
		}
	}
}
#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace bangtal;

typedef struct piecepos
{
	int x, y;
} PiecePos;

typedef struct piece
{
	ObjectPtr object;
	int correct_pos;
	int current_pos;
} Piece;

bool checkIn(const Piece piece, const Piece empty)
{
	if (abs(piece.current_pos - empty.current_pos) == 3 || abs(piece.current_pos - empty.current_pos) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void changePieces(Piece* piece1, Piece* piece2)
{
	int temp = piece1->current_pos;
	piece1->current_pos = piece2->current_pos;
	piece2->current_pos = temp;
}

bool checkImageCorrect(const Piece puzzle[], int size)
{
	for (int i = 0; i < size * size; i++)
	{
		if (puzzle[i].correct_pos != puzzle[i].current_pos)
		{
			return false;
		}
	}

	return true;
}

int main()
{
	auto scene1 = Scene::create("", "Images/원본.png");
	auto scene2 = Scene::create("", "Images/배경.png");
	auto start = Object::create("Images/start.png", scene1, 590, 70);
	auto end = Object::create("Images/end.png", scene1, 590, 20);
	auto restart = Object::create("Images/play.png", scene2, 590, 70, false);
	const int marginx = 415;
	const int marginy = 135;
	const int interver = 150;
	const int size = 3;
	int empty_piece;
	bool flag[size * size] = { 0 };
	PiecePos piece_poses[size * size] = { 0 };
	Piece puzzle[size * size] = { 0 };

	std::srand(static_cast<unsigned int>(std::time(NULL)));

	empty_piece = rand() % (size * size);

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			piece_poses[i * size + j].x = marginx + interver * j;
			piece_poses[i * size + j].y = marginy + interver * i;
		}
	}

	for (int i = 0; i < size * size; i++)
	{
		int pos;
		char str[20] = "Images/조각_";
		char digit[1 + 1] = { static_cast<char>(i) + '0' };

		do
		{
			pos = rand() % (size * size);
		} while (flag[pos] == true);

		flag[pos] = true;

		strcat(str, digit);
		strcat(str, ".png");

		if (i == empty_piece)
		{
			puzzle[i].object = Object::create(str, scene2, piece_poses[pos].x, piece_poses[pos].y, false);
		}
		else
		{
			puzzle[i].object = Object::create(str, scene2, piece_poses[pos].x, piece_poses[pos].y);
		}
		puzzle[i].correct_pos = i;
		puzzle[i].current_pos = pos;
	}

	start->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene2->enter();

		return true;
		});

	end->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();

		return true;
		});

	puzzle[0].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 0)
		{
			if (checkIn(puzzle[0], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[0], &puzzle[empty_piece]);
				puzzle[0].object->locate(scene2, piece_poses[puzzle[0].current_pos].x, piece_poses[puzzle[0].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[1].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 1)
		{
			if (checkIn(puzzle[1], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[1], &puzzle[empty_piece]);
				puzzle[1].object->locate(scene2, piece_poses[puzzle[1].current_pos].x, piece_poses[puzzle[1].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[2].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 2)
		{
			if (checkIn(puzzle[2], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[2], &puzzle[empty_piece]);
				puzzle[2].object->locate(scene2, piece_poses[puzzle[2].current_pos].x, piece_poses[puzzle[2].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[3].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 3)
		{
			if (checkIn(puzzle[3], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[3], &puzzle[empty_piece]);
				puzzle[3].object->locate(scene2, piece_poses[puzzle[3].current_pos].x, piece_poses[puzzle[3].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[4].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 4)
		{
			if (checkIn(puzzle[4], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[4], &puzzle[empty_piece]);
				puzzle[4].object->locate(scene2, piece_poses[puzzle[4].current_pos].x, piece_poses[puzzle[4].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[5].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 5)
		{
			if (checkIn(puzzle[5], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[5], &puzzle[empty_piece]);
				puzzle[5].object->locate(scene2, piece_poses[puzzle[5].current_pos].x, piece_poses[puzzle[5].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[6].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 6)
		{
			if (checkIn(puzzle[6], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[6], &puzzle[empty_piece]);
				puzzle[6].object->locate(scene2, piece_poses[puzzle[6].current_pos].x, piece_poses[puzzle[6].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[7].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 7)
		{
			if (checkIn(puzzle[7], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[7], &puzzle[empty_piece]);
				puzzle[7].object->locate(scene2, piece_poses[puzzle[7].current_pos].x, piece_poses[puzzle[7].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	puzzle[8].object->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (empty_piece != 8)
		{
			if (checkIn(puzzle[8], puzzle[empty_piece]) == true)
			{
				changePieces(&puzzle[8], &puzzle[empty_piece]);
				puzzle[8].object->locate(scene2, piece_poses[puzzle[8].current_pos].x, piece_poses[puzzle[8].current_pos].y);
				puzzle[empty_piece].object->locate(scene2, piece_poses[puzzle[empty_piece].current_pos].x, piece_poses[puzzle[empty_piece].current_pos].y);
			}

			if (checkImageCorrect(puzzle, size) == true)
			{
				showMessage("성공!");

				end->show();
			}
		}

		return true;
		});

	startGame(scene1);
}
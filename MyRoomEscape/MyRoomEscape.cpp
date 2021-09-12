#include <bangtal>
using namespace bangtal;

int main()
{
	auto scene1 = Scene::create("룸1", "Images/배경.png");
	auto scene2 = Scene::create("룸2", "Images/배경.png");
	auto door1 = Object::create("Images/문-닫힘.png", scene1, 0, 33);
	auto door2 = Object::create("Images/문-닫힘.png", scene1, 900, 33);
	auto bookshelf = Object::create("Images/책장.png", scene1, 30, 110);
	auto desk = Object::create("Images/책상.png", scene1, 700, 50);
	auto safe = Object::create("Images/금고-닫힘.png", scene1, 730, 215);
	auto key = Object::create("Images/열쇠.png", scene1, 875, 285, false);
	auto clock = Object::create("Images/시계.png", scene2, 450, 370);
	auto back = Object::create("Images/이전.png", scene2, 600, 10);
	auto door1_open = false;
	auto door2_open = false;
	auto bookshelf_moved = false;
	auto safe_locked = true;
	auto safe_open = false;

	door1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (door1_open == true)
		{
			endGame();
		}
		else if (key->isHanded())
		{
			door1->setImage("Images/문-열림.png");

			door1_open = true;
		}
		else
		{
			showMessage("문은 잠겨있다.");
		}

		return true;
		});

	door2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (door2_open == true)
		{
			scene2->enter();
		}
		else
		{
			door2->setImage("Images/문-열림.png");

			door2_open = true;
		}

		return true;
		});

	bookshelf->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (bookshelf_moved == false)
		{
			if (action == MouseAction::MOUSE_DRAG_RIGHT)
			{
				bookshelf->locate(scene1, 350, 110);
			}

			bookshelf_moved = true;
		}

		return true;
		});

	safe->setScale(0.8f);

	safe->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showKeypad("1210", safe);

		return true;
		});

	safe->setOnKeypadCallback([&](ObjectPtr object)->bool {
		safe->setImage("Images/금고-열림.png");

		key->show();

		return true;
		});
	
	key->setScale(0.8f);

	key->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		key->pick();

		return true;
		});

	clock->setScale(1.2f);

	clock->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showMessage("고장난 시계이다.");

		return true;
		});

	back->setScale(0.5f);

	back->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene1->enter();

		return true;
		});

	startGame(scene1);
}
// main.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>


// commit practice
// Ŀ�� ����

void printSpaces(int i)
{
	for (int j = 0; j < i; j++)
		printf(" ");
}

void clear(char* canvas, int length)
{
	memset(canvas, ' ', length);
	canvas[length] = '\0';
}

void render(const char* canvas, int lastPosition)
{
	printf("%s\r", canvas);
}

struct Enemy {
	int		pos;
	char	shape[100];

	Enemy(const char* shape, int maxCount)
		: pos{ rand() % (maxCount - (int)strlen(shape)) }
	{	
		strcpy(this->shape, shape);
	}

	bool isInside(int length)
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void moveRight()
	{
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	void draw(char* canvas, int maxCount)
	{
		if (isInside(maxCount) == false) return;
		strncpy(&canvas[pos], shape, strlen(shape));
	}
};

struct Bullet {
	int		pos;
	char	shape[100];
	bool	isFired;
	int		direction;
			
	Bullet(const char* shape = "")
		: pos(-1), isFired(false), direction(0)
	{
		strcpy(this->shape, shape);
	}
	
	bool isInside(int length)
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void moveRight()
	{
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	void update(int enemy_pos, const char* enemy_shape)
	{
		if (isFired == false) return;

		if (direction == 0)
			moveRight();
		else moveLeft();

		if ((direction == 0 && enemy_pos <= pos)
			|| (direction == 1 && pos < enemy_pos + strlen(enemy_shape)))
		{
			isFired = false;
		}
		
	}

	void draw(char* canvas, int maxCount)
	{
		if (isFired == false) return;
		if (isInside(maxCount) == false) return;
		strncpy(&canvas[pos], shape, strlen(shape));
	}
}; // ����ü Bullet ����

struct Player {
	int		pos;
	char	shape[100];

	// constructor ������
	Player(const char* shape, int maxCount)
		: pos(rand() % (maxCount - strlen(shape)))
	{
		strcpy(this->shape, shape);
	}

	bool isInside(int length)
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void fire(int enemy_pos, Bullet* bullet)
	{
		if (bullet == nullptr) return;
		if (bullet->isFired == true) return;

		bullet->isFired = true;
		bullet->pos = pos;
		if (pos < enemy_pos) {
			bullet->pos += (int)strlen(shape) - 1;
			strcpy(bullet->shape, "-->");
			bullet->direction = 0;
		}
		else {
			strcpy(bullet->shape, "<--");
			bullet->direction = 1;
		}
	}

	void moveRight()
	{
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	void draw(char* canvas, int maxCount)
	{
		if (isInside(maxCount) == false) return;
		strncpy(&canvas[pos], shape, strlen(shape));
	}
};


Bullet* findUnusedBullet(Bullet bullets[], int maxBullets)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullets[i].isFired == true) continue;		
		return &bullets[i];
	}
	return nullptr;
}

int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	Player	player{ "(o_o)", maxCount };
	Enemy   enemy{ "(*___*)", maxCount };
	Bullet	bullets[maxCount];

	while (true)		
	{
		clear(canvas, maxCount);
				
		// update game objects (player, enemy ...)
		if (player.isInside(maxCount) == false || enemy.isInside(maxCount) == false)
			break; // check game loop termination condition

		if (_kbhit()) {
			int key = _getch();
			Bullet* bullet = nullptr;

			//printf("\n%c %d\n", key, key);
			switch (key) {
			case 'a':
				player.moveLeft();
				break;
			case 'd':
				player.moveRight();
				break;
			case ' ':
				bullet = findUnusedBullet(bullets, maxCount);
				if (bullet == nullptr) break;				
				player.fire(enemy.pos, bullet);
				break;
			case 'w':
				enemy.moveRight();
				break;
			case 's':
				enemy.moveLeft();
				break;
			}
		}
		for (int i = 0; i < maxCount; i++)
		{
			if (bullets[i].isFired == false) continue;
			bullets[i].update(enemy.pos, enemy.shape);
		}
		
		// draw game objects to a canvas (player, enemy ...)
		player.draw(canvas, maxCount);
		enemy.draw(canvas, maxCount);
		for (int i = 0; i < maxCount; i++)
		{
			if (bullets[i].isFired == false) continue;
			bullets[i].draw(canvas, maxCount);
		}
		
		// display canvas to a monitor
		render(canvas, maxCount);		
		Sleep(100);
	}
	printf("\n���������� ����Ǿ����ϴ�.\n");
	return 0;
}

// Screen.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Screen.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Integer.h"


using namespace std;

int main()
{
	Integer i = 10;
	Integer j = 20;
	Integer k( 0 );
	Integer m(-1);
	Integer l(-10);

	l = k = i + j + m;
	//l.operator=(k.operator=(  operator+(operator+(i, j), m) ));

	l.print(); // 29

	Integer::printStat();





/*	Player player = { 30, "(^_^)" };
	Enemy enemy{ 60, "(*--*)" };
	Bullet bullet(-1, "+");
	const int maxGameObjects = 10;
	GameObject* gameObjects[maxGameObjects];
	for (int i = 0; i < maxGameObjects; i++)
		gameObjects[i] = nullptr;
	gameObjects[0] = &player; //upcast
	gameObjects[1] = &enemy;
	gameObjects[2] = &bullet;

	while (true)
	{
		Screen::getInstance().clear();

		if (_kbhit())
		{
			int c = _getch();
			for (int i = 0; i < maxGameObjects; i++)
			{
				GameObject* obj = gameObjects[i];
				if (!obj) continue;
				obj->process_input(c, gameObjects, maxGameObjects);
			}
		}
		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gameObjects[i];
			if (!obj) continue;
			obj->draw();
		}

		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gameObjects[i];
			if (!obj) continue;
			obj->update(gameObjects, maxGameObjects);
		}

		Screen::getInstance().render();
		Sleep(66);
	}
	*/
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <conio.h>

using namespace std;

/*
1. Enemy hp �ο�, �ʱⰪ hp = 10, hp == 0�� �Ǹ� ���� ������ ȭ�鿡�� ������ ����.
2. ���� ������ ���� �Ұ�
3. ���� ��� ����, �÷��̾�� �ִ� 10���� 1������ �������� �� �� ����.�����̽��ٷ� ���� �� �� ����.źâ�� ����ٸ� 2�ʰ� cool time �����Ͽ� ���� �� �� ����.
4. �Ѿ��� ������ 1�� �������� ��
5. ���� ���� ������, ������ ��ġ���� spawn ������.
6. �ѹ��� spawn�� ���� �ִ� 5����.
7. ���� �÷��̾�� 1�ʿ� 2ĭ��(0.5�ʿ� 1ĭ) �̵���.
8. ���� �÷��̾� ������ 2ĭ �̳��� ���� �÷��̾�� 1�ʴ� 2��ŭ �������� ����(�� �����Ӹ��� ���������� �������� ����)
9. �÷��̾ �������� �޴� ���� �÷��̾� ����� ���ڱ��� �Ÿ�.�������� ���� ������ ���� ����.
10. �÷��̾ ������ ������ ����ǰ� ���� ���� ���� �÷��̾ ����ִ� �ð��� ȭ�鿡 ������

NOTE:
���� �ڵ�� �÷��̾�� ������ �Ÿ��� ���ϴ� ������ �ſ� �ܼ��ϰ� ǥ���Ǿ� ����. ������ �ʿ���.
�Ѿ� �̵��� ���� �Ѿ˰��� �Ÿ��� ���� ����� ����� �Ǿ� ���� ����.

*/

#include "Players.h"
#include "Enemies.h"
#include "Bullets.h"


int main()
{
	Renderer renderer;
	Players players(&renderer);
	Player* main = players.getMainCharacter();
	Enemies enemies(&renderer, main);
	Bullets bullets(&renderer, &players, &enemies);
	
	clock_t current_tick, start_tick;

	start_tick = clock();
	while (true)
	{
		current_tick = clock();
		renderer.clear();

		if (_kbhit()) {
			char key = _getch();
			//printf("%d\n", key);
			if (key == 27) break;
			if (key == -32) {
				key = _getch();
			}

			switch (key) {
			case 'a': case 75:
				main->move(-1);
				break;
			case 'd': case 77:
				main->move(1);
				break;
			case 72:
				break;
			case 80:
				break;
			case ' ':
				bullets.fire(main);
				break;
			}				
		}
		enemies.update();
		bullets.update();
		players.update();
		main = players.getMainCharacter();
		if (main == nullptr) break;
		
		enemies.draw();
		bullets.draw();
		players.draw();
		
		renderer.render();
		Sleep(66 - (clock()- current_tick));
	}
	renderer.clear();
	Borland::gotoxy(0, 1); printf("# of killed enemies = %d, elapsed survival duration = %d seconds\n", 
		enemies.getNumberOfKilled(),
		(clock() - start_tick) / CLOCKS_PER_SEC );

	return 0;
}
// main.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include <iostream>
#include <Windows.h>

void printSpaces(int i)
{
	for (int j = 0; j < i; j++)
		printf(" ");
}

int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	int player_pos = 0;
	char player_shape[6] = "(o_o)";
	int enemy_pos = 75;
	char enemy_shape[6] = "(*_*)";

	while (player_pos < maxCount && enemy_pos >= 0)
	{
		for (int i = 0; i < maxCount; i++)
			canvas[i] = ' ';

		// player �׸��� canvas ������ player ��ġ�� ����
		for (int i = 0; i < 5; i++)
			canvas[player_pos + i] = player_shape[i];
		player_pos++;

		for (int i = 0; i < 5; i++)
			canvas[enemy_pos + i] = enemy_shape[i];
		// enemy �׸��� canvas ������ enemy ��ġ�� ����
		enemy_pos--;

		canvas[maxCount] = '\0';
		printf("%s\r", canvas);
		Sleep(100);
	}
	printf("\n���������� ����Ǿ����ϴ�.\n");
	return 0;
}

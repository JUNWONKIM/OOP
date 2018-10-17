#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <conio.h>

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

struct Position {
	int x;
	int y;
	Position(int x, int y) : x(x), y(y) {}
};

class Borland {

public:
	static int wherex()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}
	static int wherey()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
	static void gotoxy(int x, int y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}
	static void gotoxy(const Position* pos)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)pos->x, (SHORT)pos->y});
	}
};

class Renderer {
	static const int screen_size = 119;
	char display[screen_size + 1 + 1];
	Position origin;
	
public:
	Renderer() : origin( Borland::wherex(), Borland::wherey() ) {
		// ���� ���� �������� �õ尪�� 10���� �����Ͽ� ������ ���� ���ڰ� �����ǵ��� ����.
		// ����� ����.
		// release�ÿ��� time(nullptr)�� �ٲپ� �õ尪�� ���������� �����ؾ� ��.
		srand(10);
	}

	bool checkRange(const char* shape, int pos)
	{
		if (!shape) return false;
		if (pos < 0) {
			if (pos + (int)strlen(shape) >= 0) return true;
		}
		else {
			if (pos + strlen(shape) < screen_size) return true;
		}
		return false;
	}

	bool checkRange(int pos)
	{
		return pos >= 0 && pos < (screen_size - 1);
	}

	void clear() 
	{
		memset( display, ' ', screen_size);
		display[screen_size] = '\n';
		display[screen_size+1] = '\0';
	}
	
	void draw(const char* shape, int pos)
	{
		int dest_pos = pos;
		if (checkRange(shape, pos) == false) return;

		const char* source = shape;
		int len = strlen(shape);
		if (pos < 0) {
			source = shape - pos;
			len += pos;
			dest_pos = 0;
		}
		else if (pos + strlen(shape) > screen_size - 1) {
			len = screen_size - pos;
		}
		memcpy(display + dest_pos, source, len);
	}

	void render()
	{
		// make sure it should end with proper ending characters.
		display[screen_size] = '\n';
		display[screen_size + 1] = '\0';

		Borland::gotoxy(&origin);
		printf("%s", display);
		Borland::gotoxy(&origin);
	}

	int getScreenLength() {	return screen_size;	}
};

class GameObject {
	float pos;
	char shape[100];

public:
	GameObject(int pos, const char* shape) : pos(pos) { strcpy(this->shape, shape); }
	virtual ~GameObject() {}

	void setPosition(float pos) { this->pos = pos; }
	float getPosition() const { return pos;  }
	
	void setShape(const char* shape) { if (!shape) return;  strcpy(this->shape, shape); }
	const char* getShape() const { return shape; }
	int getShapeSize() { return strlen(shape); }

	void move(float inc) { pos += inc;  }
	virtual void update() {}
	virtual void draw() {}
};

class Player : public GameObject {
	float hp;
	int n_blinks;
	float damage_ratio;
	bool isDamanaged;
public:
	Player(int hp = 10, int pos = 20, const char *face="(-_-)") : GameObject(pos, face), hp(hp), n_blinks(0), damage_ratio(2.0f/30) {}
	
	void update() {
		if (n_blinks > 0) n_blinks--;
	}

	bool isBlinking() { return n_blinks > 0;  }

	bool isAlive() { return hp > 0.0f;  }

	void getDamanagedIfIntruded(float enemy_pos) 
	{
		if (fabs(getPosition() - enemy_pos) > 2.0f) return;
		hp -= damage_ratio;
		n_blinks = 2;
	}

	void display_stat() 
	{
		printf("pos(%2.1f) hp(%2.1f), n_blinks(%2d)", getPosition(), hp, n_blinks);
	}
};




class Enemy : public GameObject {
	float hp;
	char face[100];
	char faceAttacked[100];
	int nAnimations;
	Player* target;
	float speed = 2.0f/30;

public:
	Enemy(Player* target, int pos = 50, int hp = 5, const char* face="(*_*)", const char* faceAttacked="(>_<)") : GameObject(pos, face), target(target), nAnimations(0), hp(hp)
	{ 
		strcpy(this->face, face);
		strcpy(this->faceAttacked, faceAttacked);
	}
	
	void update()
	{
		if (!target) return;
		float player_pos = target->getPosition();
		float pos = getPosition();
		if (player_pos < pos) move(-1*speed);
		else if (player_pos > pos) move( 1*speed);
		else { } // do not move

		// attack if in range
		target->getDamanagedIfIntruded(pos);

		if (nAnimations == 0) return;
		nAnimations--;
		if (nAnimations == 0) {
			setShape(face);
		}
	}
	void OnHit()
	{
		hp -= 1.0f;
		nAnimations= 30;
		setShape(faceAttacked);
	}

	bool isAlive() {
		return hp > 0.0f;
	}

	float getHP() { return hp; }
};

class Bullet : public GameObject {
public:
	Bullet(int player_pos = -1, const char* shape = ">") : GameObject(player_pos, shape) {}

	int getDirection() { return strcmp(getShape(), ">") == 0 ? 1 : -1; }

	void update()
	{
		if (isAlive() == false) return;
		if (strcmp(getShape(), ">") == 0) move(1.0f);
		else if (strcmp(getShape(), "<") == 0) move(-1.0f);
	}
	bool isAlive() { return getPosition() != -1.0f; }
};

class Container {
	const int maxItems;
	int nItems;
	GameObject** items;
public:
	Container(int maxItems) : maxItems(maxItems), nItems(0), items(new GameObject*[maxItems]) {
		for (int i = 0; i < maxItems; i++) items[i] = nullptr;
	}
	~Container() {
		while (nItems > 0) {
			remove(items[0]);
		}
		delete[] items;
	}

	int capacity() const { return maxItems; }

	int count() const { return nItems;  }

	GameObject* at(int idx){
		if (idx < 0 || nItems > maxItems) return nullptr;
		return items[idx];
	}

	void add(GameObject* obj)
	{
		if (!obj) return;
		if (nItems >= maxItems) {
			delete obj;
			return;
		}
		for (int i = 0; i < maxItems; i++)
		{
			if (items[i] == nullptr) {
				items[i] = obj;
				nItems++;
				return;
			}
		}
		delete obj;
	}

	void remove(int i)
	{
		if (i == -1 || i < 0 || i >= maxItems) return;
		delete items[i];
		items[i] = nullptr;
		nItems--;
	}

	void remove(GameObject* obj)
	{
		int idx = indexOf(obj);
		if (idx == -1) {
			if (obj != nullptr)
				delete obj;
			return;
		}
		remove(idx);
	}

	int indexOf(GameObject* obj)
	{
		if (!obj) return -1;
		for (int i = 0; i < maxItems; i++) {
			if (items[i] == obj)
				return i;
		}
		return -1;
	}
};

class Players : public Container {
	Renderer* renderer;
	Player* main;

public:
	Players(Renderer* renderer) : Container(1), renderer(renderer) {
		add(new Player());
	}

	Player* getMainCharacter()
	{
		if (count() == 0) return nullptr;
		for (int i = 0; i < capacity(); i++) {
			if (at(i) != nullptr) 
				return static_cast<Player *>(at(i));
		}
		return nullptr;
	}

	void update()
	{
		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			at(i)->update();
		}

		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			auto player = static_cast<Player*>(at(i));
			if (player->isAlive() == false) remove(i);
		}
	}

	void draw()
	{
		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			auto player = static_cast<Player*>(at(i));
			if (player->isBlinking()) {
				renderer->draw(rand() % 2 ? player->getShape() : " ", player->getPosition());
			}
			else {
				renderer->draw(player->getShape(), player->getPosition());
			}
		}
		Borland::gotoxy(0, 1); printf("player "); getMainCharacter()->display_stat();
	}
};


class Enemies : public Container {
Renderer* renderer;
	Player* target;

	int n_killed;
	int n_remainings_for_respawn;

public:
	Enemies(Renderer* renderer, Player* target) 
		: Container(5), renderer(renderer), n_killed(0), n_remainings_for_respawn(30), target(target) {
		add(new Enemy(target, rand() % renderer->getScreenLength() ));
	}

	int getNumberOfKilled() { return n_killed; }

	void update()
	{
		// enemy spawning related code
		if (n_remainings_for_respawn <= 0) {
			// reset the timer for the next enemy spawning
			add(new Enemy(target, rand() % renderer->getScreenLength()));
			n_remainings_for_respawn = 30;
		}
		else
			n_remainings_for_respawn--;

		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			auto item = static_cast<Enemy*>(at(i));
			item->update();
			if (item->isAlive() == false) {
				n_killed++;
				remove(i);
			}
		}
		Borland::gotoxy(0, 2); printf("# of enemies = %2d,  ", count());
		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			auto item = static_cast<Enemy*>(at(i));
			printf(" [%2d] %2.1f %2.1f   ", i, item->getPosition(), item->getHP());
		}
	}

	void draw()
	{
		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			auto enemy = at(i);
			renderer->draw(enemy->getShape(), enemy->getPosition());
		}
	}

	GameObject* findClosest(float pos)
	{
		GameObject* closest = nullptr;
		float dist = 0.0f;
		if (renderer->checkRange(pos) == false) return closest;
		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			auto enemy = at(i);
			float enemy_pos = enemy->getPosition();
			if (renderer->checkRange(enemy_pos) == false) continue;			
			float current_dist = fabs(pos - enemy_pos);
			if (!closest) {
				dist = current_dist;
				closest = enemy;
				continue;
			}
			// closest != nullptr;
			if (dist > current_dist) {
				dist = current_dist;
				closest = enemy;
			}
		}
		return closest;
	}

	bool isShoted(Bullet* bullet)
	{
		if (!bullet) return false;
		float bullet_pos = bullet->getPosition();
		auto enemy = static_cast<Enemy*>(findClosest(bullet_pos));
		if (!enemy) return false;
		float enemy_pos = enemy->getPosition();
		int bullet_direction = bullet->getDirection();
		if ( (bullet_direction == 1 && enemy_pos < bullet_pos && bullet_pos - enemy_pos <= 1.0f) 
			|| (bullet_direction == -1 && bullet_pos < enemy_pos && enemy_pos - bullet_pos <= 1.0f)
			|| (int)enemy_pos == (int)bullet_pos) {
			enemy->OnHit();
			return true;
		}
		return false;
	}
};


class Bullets : public Container {
	Renderer* renderer;
	Players* players;
	Enemies* enemies;
	int n_remaining_cool_time;

public:
	Bullets(Renderer* renderer, Players* players, Enemies* enemies) 
	: Container(10), renderer(renderer), players(players), enemies(enemies), n_remaining_cool_time(0) { }

	void add(GameObject* bullet) {
		if (!bullet) return;
		if (n_remaining_cool_time > 0) {
			n_remaining_cool_time--;
			delete bullet;
			return;
		}
		if (count() >= capacity()) {
			if (n_remaining_cool_time == 0) {
				n_remaining_cool_time = 30*2;
			}
			delete bullet;
			return;
		}
		Container::add(bullet);
	}
	
	void fire(const Player* player)
	{
		if (player == nullptr || enemies == nullptr) return;
		GameObject* target = enemies->findClosest(player->getPosition());
		if (target == nullptr) return;
		float player_pos = player->getPosition();
		float enemy_pos = target->getPosition();
		if (renderer->checkRange(player_pos) == false || renderer->checkRange(enemy_pos) == false) return;
		char shape[2] = ">";
		if (player_pos > enemy_pos) shape[0] = '<';
		add(new Bullet(player_pos, shape));
	}

	void update()
	{
		if (n_remaining_cool_time > 0)
			n_remaining_cool_time--;
		for (int i = 0; i < capacity(); i++) {
			if (!at(i)) continue;
			auto bullet = at(i);
			float pos = bullet->getPosition();
			if (renderer->checkRange(pos) == false || enemies->isShoted(static_cast<Bullet *>(bullet)) == true) {
				remove(bullet);
				continue;
			}
			bullet->update();
		}
		Borland::gotoxy(0, 3); printf("# of bullets = %2d ", count());
		printf("%5s\n", n_remaining_cool_time == 0 ? "ready" : " ");
	}

	void draw()
	{
		for (int i = 0; i < capacity() ; i++)
		{
			if (!at(i)) continue;
			auto bullet = at(i);
			renderer->draw(bullet->getShape(), bullet->getPosition());
		}
	}
};

int main()
{
	Renderer renderer;
	Players players(&renderer);
	Player* main = players.getMainCharacter();
	Enemies enemies(&renderer, main);
	Bullets bullets(&renderer, &players, &enemies);
	
	clock_t current_tick, last_tick;

	last_tick = current_tick = clock();
	while (true)
	{
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
		Sleep(66);
		last_tick = current_tick;
	}
	renderer.clear();
	current_tick = clock();
	Borland::gotoxy(0, 1); printf("# of killed enemies = %d, elapsed survival duration = %d seconds\n", enemies.getNumberOfKilled(),
		(current_tick - last_tick) / CLOCKS_PER_SEC );

	return 0;
}
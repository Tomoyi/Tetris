//����
//1. cell�ϳ� �׷��� �����ֱ�
//2. block�׷��� � �����ֱ�
//3. �¿� ������ �����ֱ�(cx,cy,dx,dy����)
//4. rotation ó���ϱ�
//5. �Ʒ��� �������°� Ÿ�̸ӷ� �����ϱ�
//6. world data�����ϰ� �浹������ �������� ������ �ϰ�, ����׷��ֱ�
//7. ���� ä������ �������°� ó��

#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int block[7][4][4] =
{
	1,0,0,0,
	1,0,0,0,
	1,0,0,0,
	1,0,0,0,

	1,0,0,0,
	1,1,0,0,
	0,1,0,0,
	0,0,0,0,

	0,1,0,0,
	1,1,0,0,
	1,0,0,0,
	0,0,0,0,

	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,

	1,0,0,0,
	1,1,0,0,
	1,0,0,0,
	0,0,0,0,

	1,0,0,0,
	1,0,0,0,
	1,1,0,0,
	0,0,0,0,

	0,1,0,0,
	0,1,0,0,
	1,1,0,0,
	0,0,0,0,
};
const int cell_size = 30;
const int w_cnt = 10;
const int h_cnt = 20;
int world[h_cnt][w_cnt] = { 0 };
int main(void)
{
	//define 7 block colors
	sf::Color color_map[] = {
		Color::Green, Color::Blue, Color::Red, Color::Yellow,
		Color::White, Color::Magenta, Color::Cyan
	};

	int kind;
	int cx, cy;
	auto new_block = [&]()
	{
		kind = rand() % 7;
		cx = w_cnt / 2, cy = 0;//current x, y		
	};
	new_block();
	RectangleShape cell(Vector2f(cell_size, cell_size));


	sf::RenderWindow window(sf::VideoMode(w_cnt * cell_size, h_cnt * cell_size), "Tetris");

	auto check_block = [&]()
	{
		for (int y = 0; y < 4; y++)for (int x = 0; x < 4; x++)
		{
			if (block[kind][y][x] == 0) continue;
			if (x + cx < 0 || x + cx >= w_cnt || y + cy >= h_cnt) return false;
			if (world[cy + y][cx + x]) return false;
		}
		return true;
	};

	auto rotate = [&]()
	{
		int len = 0;
		for (int y = 0; y < 4; y++)for (int x = 0; x < 4; x++)
			if (block[kind][y][x]) len = max(max(x, y) + 1, len);

		int d[4][4] = { 0 };
		for (int y = 0; y < len; y++)for (int x = 0; x < len; x++)
			if (block[kind][y][x]) d[len - 1 - x][y] = 1;
		for (int y = 0; y < 4; y++)for (int x = 0; x < 4; x++)
			block[kind][y][x] = d[y][x];
	};
	Clock clock;
	while (window.isOpen())
	{
		auto clear_line = [&]()
		{
			int to = h_cnt - 1;
			for (int from = h_cnt - 1; from >= 0; from--)
			{
				int cnt = 0;
				for (int x = 0; x < w_cnt; x++)if (world[from][x]) cnt++;
				if (cnt < w_cnt)
				{
					for (int x = 0; x < w_cnt; x++)world[to][x] = world[from][x];
					to--;
				}
			}
		};
		auto go_down = [&]()
		{
			cy++;
			if (check_block() == false)
			{
				cy--;
				for (int y = 0; y < 4; y++)for (int x = 0; x < 4; x++)
					if (block[kind][y][x])
					{
						world[cy + y][cx + x] = kind + 1;//+1 for avoiding 0
					}
				clear_line();
				new_block();
				return false;
			}
			return true;
		};

		static float prev = clock.getElapsedTime().asSeconds();
		if (clock.getElapsedTime().asSeconds() - prev >= 0.5)
		{
			prev = clock.getElapsedTime().asSeconds();
			go_down();
		}
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed) window.close();
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Left)
				{
					cx--;
					if (check_block() == false) cx++;
				}
				if (e.key.code == Keyboard::Right)
				{
					cx++;
					if (check_block() == false) cx--;
				}
				if (e.key.code == Keyboard::Up)
				{
					rotate();
					if (check_block() == false) { rotate(), rotate(), rotate(); }
				}
				if (e.key.code == Keyboard::Down)
				{
					go_down();
				}
				if (e.key.code == Keyboard::Space)
				{
					while (go_down() == true);
				}
			}
		}

		window.clear();
		auto draw_block = [&]()
		{
			cell.setFillColor(color_map[kind]);
			for (int y = 0; y < 4; y++)for (int x = 0; x < 4; x++)
				if (block[kind][y][x])
				{
					cell.setPosition(Vector2f((cx + x)*cell_size, (cy + y)*cell_size));
					window.draw(cell);
				}
		};
		draw_block();
		auto draw_world = [&]()
		{
			for (int y = 0; y < h_cnt; y++)for (int x = 0; x < w_cnt; x++)
				if (world[y][x])
				{
					cell.setFillColor(color_map[world[y][x] - 1]);
					cell.setPosition(Vector2f(x*cell_size, y*cell_size));
					window.draw(cell);
				}
		};
		draw_world();
		window.display();
	}
	return 0;
}
#pragma once
extern float elapsedTime;

struct Cell
{
	Vec2 position = {0.0f, 0.0f};
	Vec2 velocity = { 0.0f, 0.0f };
	float density = 0.0f;
	Vec2 prevVelocity = { 0.0f, 0.0f };
	float prevDensity = 0.0f;
};

class FluidSimulationScene : Scene
{
public:
	std::vector<Cell> grid;
	int gridSizeX = 30;
	int gridSizeY = 30;

	FluidSimulationScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		for (int j = 0; j < gridSizeY; j++)
		{
			for (int i = 0; i < gridSizeX; i++)
			{
				grid.push_back(Cell{ Vec2{(float)i, (float)j}, Vec2{0.25f, 0.0f}, max(0.0f, ((float)i+(float)j)/(5.0f* gridSizeY+gridSizeX)) });
			}
		}
		renderer->camPos = { gridSizeX * 0.5f, gridSizeY * 0.5f };
	}

	

	void drawGrid(std::vector<Cell> grid)
	{
		int index = 0;
		for (int i = 0; i < gridSizeX; i++)
		{
			for (int j = -0; j < gridSizeY; j++)
			{
				renderer->drawBox(grid[index].position, Vec2{ 0.5f,0.5f }, 0, RGBA{1.0f, 1.0f, 1.0f, grid[index].density });
				//renderer->drawLine(grid[index].position, grid[index].position + grid[index].velocity, 0.05f, true, RGBA::GREEN);
				index++;
			}
		}

		index = 0;
		for (int i = 0; i < gridSizeX; i++)
		{
			for (int j = 0; j < gridSizeY; j++)
			{
				//renderer->drawBox(grid[index].position, Vec2{ 0.5f,0.5f }, 0, RGBA{ grid[index].density, grid[index].density , grid[index].density , grid[index].density });
				renderer->drawLine(grid[index].position, grid[index].position + grid[index].velocity, 0.025f, true, RGBA::RED);
				index++;
			}
		}
	}

	int getCellIndexAtLocation(int x, int y)
	{
		if (x < 0 || x >= gridSizeX || y < 0 || y >= gridSizeY)
			return -1;
		int index = x + y * gridSizeX;
		return index;
	}

	void update(float dt)
	{
		moveCamera(1.0f, dt);
		drawGrid(grid);

		if (mouse[MOUSE_LEFT])
		{
			Vec2 worldCoords = screenWorldCoords();
			int x = (int)(worldCoords.x+0.5f);
			int y = (int)(worldCoords.y+0.5f);

			int index = getCellIndexAtLocation(x, y);
			if (index != -1)
				grid[index].density = 20.0f;
		}

		int index = 0;
		float diff = 0.0001f;
		float a = dt * diff * gridSizeX * gridSizeY;

		for (int i = 0; i < gridSizeX; i++)
		{
			for (int j = 0; j < gridSizeY; j++)
			{
				float densityTop = 0.0f, densityBottom = 0.0f, densityRight = 0.0f, densityLeft = 0.0f;

				int top = getCellIndexAtLocation(i, j + 1);
				if (top != -1)
					densityTop = grid[top].density;

				int bottom = getCellIndexAtLocation(i, j - 1);
				if (bottom != -1)
					densityTop = grid[bottom].density;

				int right = getCellIndexAtLocation(i+1, j);
				if (right != -1)
					densityTop = grid[right].density;

				int left = getCellIndexAtLocation(i-1, j);
				if (left != -1)
					densityTop = grid[left].density;

				index = getCellIndexAtLocation(i, j);
				grid[index].density = (grid[index].density + a * (densityTop + densityBottom + densityRight + densityLeft)) / (1 + 4 * a);
			}
		}
	}
};

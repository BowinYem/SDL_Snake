#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Texture.h"
#include "Snake.h"
#include "Text.h"
using namespace std;

class Game
{
	const int WINDOW_HEIGHT = 800;
	const int WINDOW_WIDTH = 800;
	const int GAME_FIELD_HEIGHT = WINDOW_HEIGHT;
	const int GAME_FIELD_WIDTH = 750;
	const int INITIAL_VELOCITY = 2;
	const int INITIAL_GROWTHRATE = 20;
	const int PLAYER_SPAWN_X = WINDOW_HEIGHT / 2;
	const int PLAYER_SPAWN_Y = WINDOW_WIDTH / 2;

	char* currentPath;

	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	SDL_Rect hitbox;
	SDL_Event e;
	
	Snake* player;
	Texture* point;
	Texture* gameField;
	Text* gameoverText;
	Text* gameoverInstructions;
	Text* scoreText;
	Text* startTitle;
	Text* startInstructions;

	bool quit = false;
	bool move_up = true;
	bool move_down = false;
	bool move_left = false;
	bool move_right = false;
	bool game_over = false;
	bool start_screen = true;

	int velocity = INITIAL_VELOCITY;
	int x = PLAYER_SPAWN_X;
	int y = PLAYER_SPAWN_Y;
	int currScore = 0;
	int growthRate = INITIAL_GROWTHRATE;

	void initialize()
	{
		gameWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_HEIGHT, WINDOW_WIDTH, SDL_WINDOW_SHOWN);
		
		if (gameWindow == NULL)
		{
			cout << "SDL_CREATEWINDOW FAILED";
		}

		gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (gameRenderer == NULL)
		{
			cout << "SDL_CREATERENDERER FAILED";
		}

		if (IMG_Init(IMG_INIT_PNG) == 0)
		{
			cout << "IMG_INIT failed";
		}

		if (TTF_Init() == -1)
		{
			cout << "TTF_INIT FAILED";
		}

		if (SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF) == -1)
		{
			cout << "SDL_SETRENDERDRAWCOLOR FAILED";
		}

		player = new Snake(new Texture(".//point.png", gameRenderer, SDL_Rect{ 0, 0, 20, 20 }, SDL_Rect{ x, y, 20, 20 }));
		point = new Texture(".//point.png", gameRenderer, SDL_Rect{ 0, 0, 20, 20 }, SDL_Rect{ 300, 300, 20, 20 });
		gameField = new Texture(".//field.png", gameRenderer, SDL_Rect{ 0, 0, 20, 20 }, SDL_Rect{0, 0, GAME_FIELD_HEIGHT, GAME_FIELD_WIDTH });
		hitbox = SDL_Rect{ x, y, 20, 1 };
		initializeText();
	}

	void initializeText()
	{
		gameoverText = new Text("Game Over", SDL_Color{ 0xFF, 0xFF, 0xFF }, ".//slkscrb.ttf", 28, gameRenderer, SDL_Rect{ 0, 0, 200, 200 }, SDL_Rect{ 300, 300, 200, 100 });
		gameoverInstructions = new Text("Push Enter To Restart", SDL_Color{ 0xFF, 0xFF, 0xFF }, ".//slkscrb.ttf", 20, gameRenderer, SDL_Rect{ 0, 0, 600, 200 }, SDL_Rect{ 200, 400, 400, 50 });
		scoreText = new Text("0", SDL_Color{ 0, 0, 0 }, ".//slkscrb.ttf", 28, gameRenderer, SDL_Rect{ 0, 0, 50, 50 }, SDL_Rect{ 400, WINDOW_HEIGHT - 50, 60, 60 });
		startTitle = new Text("SNAKE", SDL_Color{ 0xFF, 0xFF, 0xFF }, ".//slkscrb.ttf", 28, gameRenderer, SDL_Rect{ 0, 0, 200, 200 }, SDL_Rect{ 300, 300, 200, 100 });
		startInstructions = new Text("PUSH ARROW KEYS TO MOVE, ESC TO QUIT, AND ENTER TO START GAME", SDL_Color{ 0xFF, 0xFF, 0xFF }, ".//slkscrb.ttf", 20, gameRenderer, SDL_Rect{ 0, 0, 3000, 200 }, SDL_Rect{ 155, 400, 500, 50 });
	}

	void cleanup()
	{
		SDL_DestroyWindow(gameWindow);
		SDL_DestroyRenderer(gameRenderer);

		player->free();
		point->freeTexture();
		gameField->freeTexture();
		gameoverText->freeTexture();
		gameoverInstructions->freeTexture();
		scoreText->freeTexture();
		startTitle->freeTexture();
		startInstructions->freeTexture();

		gameWindow = NULL;
		gameRenderer = NULL;
		player = NULL;
		point = NULL;
		gameField = NULL;
		gameoverText = NULL;
		gameoverInstructions = NULL;
		scoreText = NULL;
		startTitle = NULL;
		startInstructions = NULL;
	}

	void getInput()
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		const Uint8* input = SDL_GetKeyboardState(NULL);
		readInput(input);
	}

	void readInput(const Uint8* input)
	{
		if (input[SDL_SCANCODE_ESCAPE])
		{
			quit = true;
		}

		if (input[SDL_SCANCODE_RETURN])
		{
			if (start_screen) start_screen = false;
			else if (game_over)
			{
				resetGame();
				game_over = false;
			}
		}

		if (input[SDL_SCANCODE_UP])
		{
			if (!move_down)
			{
				move_up = true;
				move_left = move_down = move_right = false;
			}
		}

		if (input[SDL_SCANCODE_DOWN])
		{
			if (!move_up)
			{
				move_down = true;
				move_up = move_left = move_right = false;
			}
		}

		if (input[SDL_SCANCODE_LEFT])
		{
			if (!move_right)
			{
				move_left = true;
				move_up = move_down = move_right = false;
			}
		}

		if (input[SDL_SCANCODE_RIGHT])
		{
			if (!move_left)
			{
				move_right = true;
				move_up = move_down = move_left = false;
			}
		}
	}

	void move()
	{
		if (x <= 0 || x >= GAME_FIELD_HEIGHT || y <= 0 || y >= GAME_FIELD_WIDTH)
		{
			game_over = true;
		}

		if (move_up)
		{
			y -= velocity;
			player->setDest(SDL_Rect{ x, y, 20, 20 });
			hitbox = SDL_Rect{ x, y, 20, 1 };
		}

		if (move_down)
		{
			y += velocity;
			player->setDest(SDL_Rect{ x, y, 20, 20 });
			hitbox = SDL_Rect{ x, y + 20, 20, 1 };
		}

		if (move_left)
		{
			x -= velocity;
			player->setDest(SDL_Rect{ x, y, 20, 20 });
			hitbox = SDL_Rect{ x, y, 1, 20 };
		}

		if (move_right)
		{
			x += velocity;
			player->setDest(SDL_Rect{ x, y, 20, 20 });
			hitbox = SDL_Rect{ x + 20, y, 1, 20 };
		}
	}

	void resetGame()
	{
		player->free();
		x = PLAYER_SPAWN_X;
		y = PLAYER_SPAWN_Y;
		velocity = INITIAL_VELOCITY;
		growthRate = INITIAL_GROWTHRATE;
		player = new Snake(new Texture("point.png", gameRenderer, SDL_Rect{ 0, 0, 20, 20 }, SDL_Rect{ x, y, 20, 20 }));
		currScore = 0;
		updateScore();
	}

	void updateScore()
	{
		string s = to_string(currScore);
		const char* c = s.c_str();
		scoreText->setText(c);
		scoreText->createText(gameRenderer);
	}

	void spawnPoint()
	{
		int randY = rand() % GAME_FIELD_WIDTH;
		int randX = rand() % GAME_FIELD_HEIGHT;
		point->setDest(SDL_Rect{ randX, randY, 20, 20 });
	}

	void increaseVelocity()
	{
		if (currScore % 5 == 0) velocity++;
	}

	void lengthen()
	{
		if (currScore % 5 == 0) growthRate += 10;

		for (int i = 0; i < growthRate; i++)
		{
			player->addBody(new Texture("point.png", gameRenderer, player->getSource(), SDL_Rect{ 2000, 2000, 20, 20 }));
		}
	}

public:

	void play()
	{
		initialize();

		while (quit != true)
		{
			getInput();

			if(!game_over && !start_screen)
			{
				move();

				if (SDL_HasIntersection(&hitbox, &point->getDest()))
				{
					currScore++;
					updateScore();
					spawnPoint();
					increaseVelocity();
					lengthen();
				}

				if (player->detectSelfHit(hitbox))
				{
					game_over = true;
				}
			}

			SDL_RenderClear(gameRenderer);
			
			gameField->render(gameRenderer);

			if (start_screen)
			{
				startTitle->render(gameRenderer);
				startInstructions->render(gameRenderer);
			}
			else if(game_over)
			{
				gameoverText->render(gameRenderer);
				gameoverInstructions->render(gameRenderer);
			}
			else
			{
				player->render(gameRenderer);
				point->render(gameRenderer);
			}

			scoreText->render(gameRenderer);
			SDL_RenderPresent(gameRenderer);
		}

		cleanup();

	}
};

int main(int arg, char* argv[])
{
	Game myGame;
	myGame.play();
	return 0;
}
#include "Match.h"

int Match::posToCameraX(double x)
{
	return (int) (x * DISTANCE_TO_PIXELS - camera_x + SCREEN_WIDTH / 2);
}

int Match::posToCameraY(double y)
{
	return (int) (-y * DISTANCE_TO_PIXELS + camera_y + SCREEN_HEIGHT / 2);
}

void Match::DrawObject(GameObject* obj, SDL_Surface* surface, Uint32 color)
{
	display::DrawRectangleCentered(	screen,
									posToCameraX(obj->X()),
									posToCameraY(obj->Y()),
									obj->Width() * DISTANCE_TO_PIXELS,
									obj->Height() * DISTANCE_TO_PIXELS,
									color);
}

void Match::Input()
{
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) quit = 1;
			else if (event->key.keysym.sym == SDLK_w) v_y = 1.0;
			else if (event->key.keysym.sym == SDLK_s) v_y = -1.0;
			else if (event->key.keysym.sym == SDLK_a) v_x = -1.0;
			else if (event->key.keysym.sym == SDLK_d) v_x = 1.0;
			break;
		case SDL_KEYUP:
			if (event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_s) v_y = 0.0;
			else if (event->key.keysym.sym == SDLK_a || event->key.keysym.sym == SDLK_d) v_x = 0.0;
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		};
	};
}

void Match::Update()
{
	// Time delta
	t2 = SDL_GetTicks();
	double delta = (t2 - t1) * 0.001;
	t1 = t2;
	std::cout << delta << "\n";

	// Update Game state
	player.setPosition(	player.X() + delta * v_x,
						player.Y() + delta * v_y);
}

void Match::Display()
{
	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	// Clear frame
	SDL_FillRect(screen, NULL, black);

	// Draw
	// Draw player
	DrawObject(&player, screen, red);

	// Display
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

Match::Match(SDL_Surface* screen, SDL_Renderer* renderer, SDL_Texture* scrtex)
	:screen(screen), renderer(renderer), scrtex(scrtex), event(new SDL_Event())
{
	player = GameObject(0, 0, 1, 2, 0);
}

void Match::Start()
{
	t1 = SDL_GetTicks();

	while (!quit)
	{
		// Read input
		Input();

		// Update game state
		Update();

		// Display game state
		Display();
	}
}

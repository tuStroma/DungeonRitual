#include "Match.h"

int Match::posToCameraX(double x)
{
	return (int) ((x - camera.X()) * DISTANCE_TO_PIXELS + SCREEN_WIDTH / 2);
}

int Match::posToCameraY(double y)
{
	return (int) ((-y + camera.Y()) * DISTANCE_TO_PIXELS + SCREEN_HEIGHT / 2);
}

void Match::DrawObject(GameObject* obj, SDL_Surface* surface, Uint32 color)
{
	Shape* shape = obj->GetShape();

	if (Rectangle* r = dynamic_cast<Rectangle*>(shape))
	{
		display::DrawRectangleCentered(	screen,
										posToCameraX(obj->Position().X()),
										posToCameraY(obj->Position().Y()),
										(int)(r->Width() * DISTANCE_TO_PIXELS),
										(int)(r->Height() * DISTANCE_TO_PIXELS),
										color);
	}
	else if (Segment* s = dynamic_cast<Segment*>(shape))
	{
		for (int i = 0; i < 5; i++)
			display::DrawLine(	surface,
								posToCameraX(s->Position().X()),
								posToCameraY(s->Position().Y()) + i,
								posToCameraX(s->EndPoint().X()),
								posToCameraY(s->EndPoint().Y()) + i,
								color);
	}
	else
	{
		std::cout << "Unknown shape\n";
	}
}

void Match::Input()
{
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) quit = 1;
			else if (event->key.keysym.sym == SDLK_a) player.MoveLeft(true);
			else if (event->key.keysym.sym == SDLK_d) player.MoveRight(true);
			else if (event->key.keysym.sym == SDLK_SPACE) player.Jump(true);
			break;
		case SDL_KEYUP:
			if (event->key.keysym.sym == SDLK_a) player.MoveLeft(false);
			else if (event->key.keysym.sym == SDLK_d) player.MoveRight(false);
			else if (event->key.keysym.sym == SDLK_SPACE) player.Jump(false);
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
	frame_count++;
	if (t2 - time_count >= 1000)
	{
		std::cout << frame_count << "\n";
		frame_count = 0;
		time_count = t2;
	}

	//// Update Game state

	// Move all objects
	player.Move(delta);

	// Check colisions
	for (int i = 0; i < 5; i++)
	{
		Point connection = collisions::contact::RectangleToRectangle(*(Rectangle*)player.GetShape(), *(Rectangle*)walls[i].GetShape());
		player.ResolveCollision(connection, &walls[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		Point connection = object_collisions::contact::ActorToSlope(player, slopes[i]);
		player.ResolveCollision(connection, &slopes[i]);
	}

	// Camera position
	camera = player.Position();
}

void Match::Display()
{
	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	// Clear frame
	SDL_FillRect(screen, NULL, black);

	// Draw
	// Draw environment
	for (int i = 0; i < 5; i++)
		DrawObject(&walls[i], screen, blue);

	for (int i = 0; i < 2; i++)
		DrawObject(&slopes[i], screen, blue);
	
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
	player = Actor(new Rectangle(Point(0, 0), 1, 2));

	walls[0] = GameObject(new Rectangle(Point(0, -7.0), 20, 1));
	walls[1] = GameObject(new Rectangle(Point(0, 7.0), 20, 1));
	walls[2] = GameObject(new Rectangle(Point(10.0, 0), 1, 14));
	walls[3] = GameObject(new Rectangle(Point(-10.0, 0), 1, 14));
	walls[4] = GameObject(new Rectangle(Point(0, -2.5), 4, 1));


	slopes[0] = Slope(new Segment(Point(2, -2), Point(4.5, -4.5)));
	slopes[1] = Slope(new Segment(Point(-2, -2), Point(-4.5, -4.5)));
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

#include "Match.h"

int Match::posToCameraX(double x)
{
	return (int) ((x - camera.X()) * DISTANCE_TO_PIXELS);
}

int Match::posToCameraY(double y)
{
	return (int) ((y - camera.Y()) * DISTANCE_TO_PIXELS);
}

void Match::DrawObject(GameObject* obj, Uint32 color)
{
	Shape* shape = obj->GetShape();

	if (Rectangle* r = dynamic_cast<Rectangle*>(shape))
	{
		window->DrawRectangleCentered(	posToCameraX(obj->Position().X()),
										posToCameraY(obj->Position().Y()),
										(int)(r->Width() * DISTANCE_TO_PIXELS),
										(int)(r->Height() * DISTANCE_TO_PIXELS),
										color);
	}
	else if (Segment* s = dynamic_cast<Segment*>(shape))
	{
		for (int i = 0; i < 5; i++)
			window->DrawLine(	posToCameraX(s->Position().X()),
								posToCameraY(s->Position().Y()) - i,
								posToCameraX(s->EndPoint().X()),
								posToCameraY(s->EndPoint().Y()) - i,
								color);
	}
	else
	{
		std::cout << "Unknown shape\n";
	}
}

void Match::DrawSprite(SDL_Surface* sprite, Rectangle* rectangle)
{
	window->DrawSprite(sprite,
		posToCameraX(rectangle->Left()),
		posToCameraX(rectangle->Right()),
		posToCameraY(rectangle->Up()),
		posToCameraY(rectangle->Down()));
}

void Match::DrawSpriteCentered(SDL_Surface* sprite, Point position, double paralax)
{
	double width = sprite->w / PIXELS_IN_METER;
	double height = sprite->h / PIXELS_IN_METER;

	double x = camera.X() + (position.X() - camera.X()) / paralax;
	double y = camera.Y() + (position.Y() - camera.Y()) / paralax;

	window->DrawSprite(sprite,
		posToCameraX(x - width / 2),
		posToCameraX(x + width / 2),
		posToCameraY(y + height / 2),
		posToCameraY(y - height / 2));
}

void Match::Input()
{
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) quit = 1;
			else if (event->key.keysym.sym == SDLK_a) player->MoveLeft(true);
			else if (event->key.keysym.sym == SDLK_d) player->MoveRight(true);
			else if (event->key.keysym.sym == SDLK_s) player->MoveDown(true);
			else if (event->key.keysym.sym == SDLK_SPACE) player->Jump(true);
			break;
		case SDL_KEYUP:
			if (event->key.keysym.sym == SDLK_a) player->MoveLeft(false);
			else if (event->key.keysym.sym == SDLK_d) player->MoveRight(false);
			else if (event->key.keysym.sym == SDLK_s) player->MoveDown(false);
			else if (event->key.keysym.sym == SDLK_SPACE) player->Jump(false);
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
	player->Move(delta);

	// Check colisions
	for (GameObject* wall : walls)
	{
		Point connection = collisions::contact::RectangleToRectangle(*(Rectangle*)player->GetShape(), *(Rectangle*)wall->GetShape());
		player->ResolveCollision(connection, wall);
	}

	// Slopes
	for (Slope* slope : slopes)
	{
		Point connection = object_collisions::contact::ActorToSlope(*player, *slope);
		player->ResolveCollision(connection, slope);
	}

	// Camera position
	camera = player->Position();
}

void Match::Display()
{
	int red = window->getColor(0xFF, 0x00, 0x00);
	int blue = window->getColor(0x00, 0x00, 0xFF);
	int black = window->getColor(0x00, 0x00, 0x00);

	// Clear frame
	window->ClearFrame();

	// Draw
	// Draw environment
	for (GameObject* wall : walls)
		DrawObject(wall, blue);

	for (Slope* slope : slopes)
		DrawObject(slope, blue);

	for (layer layer : background)
		DrawSpriteCentered(	layer.animation->GetSprite(),
							layer.position,
							layer.depth);
	
	// Draw player
	//DrawObject(player, red);
	DrawSpriteCentered(test, Point(0,0));
	DrawSpriteCentered(test, Point(0,0), 2);
	DrawSprite(player_animation->GetSprite(), player->getRectangle());

	// Display
	window->DisplayFrame();
}

Match::Match(Window* window)
	:window(window), event(new SDL_Event())
{
	player_animation = new Animation("Players/test", "Idle"); 
	test = SDL_LoadBMP("../common/Assets/tests/player.bmp");
	if (test == NULL) {
		printf("test loading error: %s\n", SDL_GetError());
	};
}

void Match::addObject(GameObject* object)
{
	if (Slope* slope = dynamic_cast<Slope*>(object))
		slopes.push_back(slope);
	else
		walls.push_back(object);
}

void Match::addActor(Actor* actor, bool is_player)
{
	if (is_player)
		player = actor;
}

void Match::addLayer(Animation* animation, Point position, double depth)
{
	background.push_back(layer(animation, position, depth));
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

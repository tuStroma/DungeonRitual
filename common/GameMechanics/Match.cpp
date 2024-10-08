#include "Match.h"

int Match::posToCameraX(double x)
{
	return (int) ((x - camera.X()) * DISTANCE_TO_PIXELS);
}

int Match::posToCameraY(double y)
{
	return (int) ((y - camera.Y()) * DISTANCE_TO_PIXELS);
}

void Match::DrawObject(SDL_Surface* surface, GameObject* obj, Uint32 color)
{
	Shape* shape = obj->GetShape();

	if (Rectangle* r = dynamic_cast<Rectangle*>(shape))
	{
		SurfacePainter::DrawRectangleCentered(	surface,
												posToCameraX(obj->Position().X()) / PIXEL_SCALEUP,
												posToCameraY(obj->Position().Y()) / PIXEL_SCALEUP,
												(int)(r->Width() * PIXELS_IN_METER),
												(int)(r->Height() * PIXELS_IN_METER),
												color);
	}
	else if (Segment* s = dynamic_cast<Segment*>(shape))
	{
		for (int i = 0; i < 5; i++)
			SurfacePainter::DrawLine(	surface,
										posToCameraX(s->Position().X()) / PIXEL_SCALEUP,
										posToCameraY(s->Position().Y()) / PIXEL_SCALEUP - i,
										posToCameraX(s->EndPoint().X()) / PIXEL_SCALEUP,
										posToCameraY(s->EndPoint().Y()) / PIXEL_SCALEUP - i,
										color);
	}
	else
	{
		std::cout << "Unknown shape\n";
	}
}

void Match::DrawSpriteCentered(SDL_Texture* sprite, Point position, int width, int height, double paralax)
{
	double match_width = width / PIXELS_IN_METER;
	double match_height = height / PIXELS_IN_METER;

	double x = camera.X() + (position.X() - camera.X()) / paralax;
	double y = camera.Y() + (position.Y() - camera.Y()) / paralax;

	window->DrawSprite(sprite,
		posToCameraX(x),
		posToCameraY(y),
		width / PIXELS_IN_METER * DISTANCE_TO_PIXELS,
		height / PIXELS_IN_METER * DISTANCE_TO_PIXELS);
}

void Match::DrawSprite(SDL_Texture* sprite, Rectangle* rectangle)
{
	window->DrawSprite(sprite,
		posToCameraX(rectangle->Position().X()),
		posToCameraY(rectangle->Position().Y()),
		rectangle->Width() * DISTANCE_TO_PIXELS,
		rectangle->Height() * DISTANCE_TO_PIXELS);
}

void Match::CreateBackgroundTexture()
{
	// Surface size
	int left = 0, right = 0, up = 0, down = 0;
	for (GameObject* go : walls)
	{
		Rectangle* r = static_cast<Rectangle*>(go->GetShape());
		left = fmin(left, r->Left() * PIXELS_IN_METER);
		right = fmax(right, r->Right() * PIXELS_IN_METER);
		up = fmax(up, r->Up() * PIXELS_IN_METER);
		down = fmin(down, r->Down() * PIXELS_IN_METER);
	}

	for (Slope* sl : slopes)
	{
		Segment* s = static_cast<Segment*>(sl->GetShape());
		left = fmin(left, s->LeftPoint().X() * PIXELS_IN_METER);
		right = fmax(right, s->RightPoint().X() * PIXELS_IN_METER);
		up = fmax(up, s->UpperPoint().Y() * PIXELS_IN_METER);
		down = fmin(down, s->LowerPoint().Y() * PIXELS_IN_METER);
	}

	int width = (fmax(-left, right)) * 2;
	int height = (fmax(up, -down)) * 2;

	// Create surface
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	// Paint map
	int black = SDL_MapRGB(surface->format, 0, 0, 0);
	int blue = SDL_MapRGB(surface->format, 0, 0, 0xFF);
	SDL_FillRect(surface, NULL, black);

	for (GameObject* wall : walls)
		DrawObject(surface, wall, blue);

	for (Slope* slope : slopes)
		DrawObject(surface, slope, blue);

	Animation* animation = new Animation(surface, window->GetRenderer());
	addLayer(animation, Point(0, 0), 1);
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
	// Draw
	// Draw environment
	for (Layer layer : background)
		DrawSpriteCentered(layer.animation->GetSprite(),
							layer.position,
							layer.animation->GetWidth(),
							layer.animation->GetHeight(),
							layer.depth);
	
	// Draw player
	DrawSprite(player->getAnimation()->GetSprite(), player->getRectangle());

	// Display
	window->DisplayFrame();
}

Match::Match(Window* window, std::string map)
	:window(window), event(new SDL_Event())
{
	// Load map
	std::string map_path = MAPS_PATH + map + "/map.xml";
	AssetLoader loader(map_path);
	rapidxml::xml_document<>* doc = loader.GetDocument();
	rapidxml::xml_node<>* node = doc->first_node("GameObjects")->first_node();

	// Load objects
	while (node)
	{
		std::string name = node->name();
		if (name == "Wall")
			addObject(AssetLoader::LoadWall(node));
		else if (name == "Slope")
			addObject(AssetLoader::LoadSlope(node));
		else if (name == "Actor")
			addActor(new Actor(node, "Players/test", window->GetRenderer()), true);

		node = node->next_sibling();
	}

	// Load background
	SDL_Renderer* renderer = window->GetRenderer();
	node = doc->first_node("Layers")->first_node();

	while (node)
	{
		Layer layer(node, renderer);
		background.push_back(layer);

		node = node->next_sibling();
	}
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
	background.push_back(Layer(animation, position, depth));
}

Window* Match::GetWindow()
{
	return window;
}

void Match::Start()
{
	if (background.size() == 0)
		CreateBackgroundTexture();

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

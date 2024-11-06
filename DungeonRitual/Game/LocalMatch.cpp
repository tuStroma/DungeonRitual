#include "LocalMatch.h"

int LocalMatch::posToCameraX(double x)
{
	return (int) ((x - camera.X()) * DISTANCE_TO_PIXELS);
}

int LocalMatch::posToCameraY(double y)
{
	return (int) ((y - camera.Y()) * DISTANCE_TO_PIXELS);
}

void LocalMatch::DrawObject(SDL_Surface* surface, GameObject* obj, Uint32 color)
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

void LocalMatch::DrawSpriteCentered(SDL_Texture* sprite, Point position, int width, int height, double paralax)
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

void LocalMatch::DrawSprite(SDL_Texture* sprite, Rectangle* rectangle)
{
	window->DrawSprite(sprite,
		posToCameraX(rectangle->Position().X()),
		posToCameraY(rectangle->Position().Y()),
		rectangle->Width() * DISTANCE_TO_PIXELS,
		rectangle->Height() * DISTANCE_TO_PIXELS);
}

void LocalMatch::CreateBackgroundTexture()
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

void LocalMatch::Input()
{
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) quit = 1;
			else if (event->key.keysym.sym == SDLK_a) player_controller->AddAction(Left);
			else if (event->key.keysym.sym == SDLK_d) player_controller->AddAction(Right);
			else if (event->key.keysym.sym == SDLK_s) player_controller->AddAction(Down);
			else if (event->key.keysym.sym == SDLK_SPACE) player_controller->AddAction(Jump);
			break;
		case SDL_KEYUP:
			if (event->key.keysym.sym == SDLK_a) player_controller->AddAction(StopLeft);
			else if (event->key.keysym.sym == SDLK_d) player_controller->AddAction(StopRight);
			else if (event->key.keysym.sym == SDLK_s) player_controller->AddAction(StopDown);
			else if (event->key.keysym.sym == SDLK_SPACE) player_controller->AddAction(StopJump);
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		};
	};
}

void LocalMatch::Update()
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
	for (Actor* actor : actors)
	{
		actor->TakeAction();

		actor->Move(delta);

		// Check colisions
		for (GameObject* wall : walls)
		{
			Point connection = collisions::contact::RectangleToRectangle(*(Rectangle*)actor->GetShape(), *(Rectangle*)wall->GetShape());
			actor->ResolveCollision(connection, wall);
		}

		// Slopes
		for (Slope* slope : slopes)
		{
			Point connection = object_collisions::contact::ActorToSlope(*actor, *slope);
			actor->ResolveCollision(connection, slope);
		}
	}

	// Camera position
	camera = actors[player_index]->Position();
}

void LocalMatch::Display()
{
	// Draw
	// Draw environment
	for (Layer layer : background)
		DrawSpriteCentered(layer.animation->GetSprite(),
							layer.position,
							layer.animation->GetWidth(),
							layer.animation->GetHeight(),
							layer.depth);
	
	// Draw actors
	for (Actor* actor : actors)
		DrawSprite(animations[actor]->getAnimation()->GetSprite(), actor->getRectangle());

	// Display
	window->DisplayFrame();
}

LocalMatch::LocalMatch(Window* window, std::string map, int player_index)
	:Match(map, player_index), window(window)
{
	// Load map
	std::string map_path = MAPS_PATH + map + "/map.xml";
	AssetLoader loader(map_path);
	rapidxml::xml_document<>* doc = loader.GetDocument();
	rapidxml::xml_node<>* node = doc->first_node("GameObjects")->first_node();


	// Load background
	SDL_Renderer* renderer = window->GetRenderer();
	node = doc->first_node("Layers")->first_node();

	while (node)
	{
		Layer layer(node, renderer);
		background.push_back(layer);

		node = node->next_sibling();
	}

	// Load animations
	for (Actor* actor : actors)
	{
		ActorAnimations* animation_set = new ActorAnimations(actor, "Players/test", window->GetRenderer());
		animations[actor] = animation_set;
	}
}

void LocalMatch::addLayer(Animation* animation, Point position, double depth)
{
	background.push_back(Layer(animation, position, depth));
}

Window* LocalMatch::GetWindow()
{
	return window;
}

void LocalMatch::Start()
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

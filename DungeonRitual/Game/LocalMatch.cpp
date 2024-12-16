#include "LocalMatch.h"
#include "Client.h"

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
	geometry::Shape* shape = obj->GetShape();

	if (geometry::Rectangle* r = dynamic_cast<geometry::Rectangle*>(shape))
	{
		SurfacePainter::DrawRectangleCentered(	surface,
												posToCameraX(obj->Position().X()) / PIXEL_SCALEUP,
												posToCameraY(obj->Position().Y()) / PIXEL_SCALEUP,
												(int)(r->Width() * PIXELS_IN_METER),
												(int)(r->Height() * PIXELS_IN_METER),
												color);
	}
	else if (geometry::Segment* s = dynamic_cast<geometry::Segment*>(shape))
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

void LocalMatch::DrawSpriteCentered(SDL_Texture* sprite, geometry::Point position, int width, int height, double paralax)
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

void LocalMatch::DrawAnimation(Animation* animation, geometry::Rectangle* rectangle)
{
	window->DrawSprite(animation->GetSprite(),
		posToCameraX(rectangle->Position().X()),
		posToCameraY(rectangle->Position().Y()),
		rectangle->Width() * DISTANCE_TO_PIXELS,
		rectangle->Height() * DISTANCE_TO_PIXELS,
		animation->GetRotation(),
		animation->GetFlip());
}

void LocalMatch::CreateBackgroundTexture()
{
	// Surface size
	int left = 0, right = 0, up = 0, down = 0;
	for (GameObject* go : walls)
	{
		geometry::Rectangle* r = static_cast<geometry::Rectangle*>(go->GetShape());
		left = fmin(left, r->Left() * PIXELS_IN_METER);
		right = fmax(right, r->Right() * PIXELS_IN_METER);
		up = fmax(up, r->Up() * PIXELS_IN_METER);
		down = fmin(down, r->Down() * PIXELS_IN_METER);
	}

	for (Slope* sl : slopes)
	{
		geometry::Segment* s = static_cast<geometry::Segment*>(sl->GetShape());
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
	addLayer(animation, geometry::Point(0, 0), 1);
}

void LocalMatch::UserAction(Action action)
{
	player_controller->AddAction(action);

	// Online component
	if (game_client) // Send action to server
	{
		net::common::Message<NetContext> action_msg(MoveAction, sizeof(Action));
		action_msg.put(&action, sizeof(action));
		game_client->Send(action_msg);
	}
}

void LocalMatch::ServerStateUpdate()
{
	server_state_lock.acquire();

	if (server_game_state)
	{
		DeserializeGameState(server_game_state);

		delete server_game_state;
		server_game_state = nullptr;
	}

	server_state_lock.release();
}

void LocalMatch::Input()
{
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) quit = 1;
			else if (event->key.keysym.sym == SDLK_a) UserAction(Left);
			else if (event->key.keysym.sym == SDLK_d) UserAction(Right);
			else if (event->key.keysym.sym == SDLK_s) UserAction(Down);
			else if (event->key.keysym.sym == SDLK_SPACE) UserAction(Jump);
			break;
		case SDL_KEYUP:
			if (event->key.keysym.sym == SDLK_a) UserAction(StopLeft);
			else if (event->key.keysym.sym == SDLK_d) UserAction(StopRight);
			else if (event->key.keysym.sym == SDLK_s) UserAction(StopDown);
			else if (event->key.keysym.sym == SDLK_SPACE) UserAction(StopJump);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				UserAction(BasicAttack);
				std::cout << "Mouse left click at (" << x << ", " << y << ")\n";
			}
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
	double time_delta = UpdateTime();

	// Print frames
	if (TimeDelta(t2, time_count) >= 1000000)
	{
		std::cout << frame_count << "\n";
		frame_count = 0;
		time_count = t2;
	}

	ServerStateUpdate();

	UpdateState(time_delta);

	// Camera position
	camera = actors[player_index]->Position();
}

void LocalMatch::Display()
{
	// Update animations
	for (auto& [actor, animation_set] : animations)
		animation_set->CheckState();

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
		DrawAnimation(animations[actor]->getAnimation(), actor->getRectangle());

	// Display
	window->DisplayFrame();
}

LocalMatch::LocalMatch(Window* window, std::string map, int player_index, Client* game_client)
	:Match(map),
	event(new SDL_Event()),
	window(window),
	player_index(player_index),
	game_client(game_client),
	server_state_lock(std::binary_semaphore(1))
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

	// Get the player controller
	ActorController* pl = actors[player_index]->getController();
	player_controller = dynamic_cast<OutsideController*>(pl);
}

void LocalMatch::addLayer(Animation* animation, geometry::Point position, double depth)
{
	background.push_back(Layer(animation, position, depth));
}

Window* LocalMatch::GetWindow()
{
	return window;
}

void LocalMatch::MakeActionAsPlayer(int player_id, Action action)
{
	OutsideController* controller = dynamic_cast<OutsideController*>(actors[player_id]->getController());

	if (controller)
		controller->AddAction(action);
}

void LocalMatch::LoadServerState(Data* server_state)
{
	server_state_lock.acquire();

	if (server_game_state)
		delete server_game_state;

	server_game_state = server_state;

	server_state_lock.release();
}

void LocalMatch::Start()
{
	if (background.size() == 0)
		CreateBackgroundTexture();

	t1 = std::chrono::system_clock::now();

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

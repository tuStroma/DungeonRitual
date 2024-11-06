#include "Match.h"

void Match::Input()
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
}

Match::Match(std::string map, int player_index)
	:event(new SDL_Event()), player_index(player_index)
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
			addActor(new Actor(node));

		node = node->next_sibling();
	}

	// Get the player controller
	ActorController* pl = actors[player_index]->getController();
	player_controller = dynamic_cast<OutsideController*>(pl);

}

void Match::addObject(GameObject* object)
{
	if (Slope* slope = dynamic_cast<Slope*>(object))
		slopes.push_back(slope);
	else
		walls.push_back(object);
}

void Match::addActor(Actor* actor)
{
	actors.push_back(actor);
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
	}
}

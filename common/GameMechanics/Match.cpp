#include "Match.h"

long long Match::TimeDelta(std::chrono::system_clock::time_point end, std::chrono::system_clock::time_point begin)
{
	return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

void Match::Input()
{
	
}

void Match::Update()
{
	// Time delta
	t2 = std::chrono::system_clock::now();
	double delta = TimeDelta(t2, t1) * 0.000001;
	t1 = t2;
	frame_count++;
	if (TimeDelta(t2, time_count) >= 1000000)
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
			geometry::Point connection = geometry::collisions::contact::RectangleToRectangle(*(geometry::Rectangle*)actor->GetShape(), *(geometry::Rectangle*)wall->GetShape());
			actor->ResolveCollision(connection, wall);
		}

		// Slopes
		for (Slope* slope : slopes)
		{
			geometry::Point connection = object_collisions::contact::ActorToSlope(*actor, *slope);
			actor->ResolveCollision(connection, slope);
		}
	}
}

Match::Match(std::string map, int player_index)
	:player_index(player_index)
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
	t1 = std::chrono::system_clock::now();

	while (!quit)
	{
		// Read input
		Input();

		// Update game state
		Update();
	}
}

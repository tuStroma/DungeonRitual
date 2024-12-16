#include "Match.h"

long long Match::TimeDelta(std::chrono::system_clock::time_point end, std::chrono::system_clock::time_point begin)
{
	return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

void Match::Input()
{
	
}

void Match::UpdateState(double time_delta)
{
	//// Update Game state

	// Move all objects
	for (Actor* actor : environment.actors)
	{
		actor->TakeAction();

		actor->Move(time_delta);

		// Check colisions
		for (GameObject* wall : environment.walls)
		{
			geometry::Point connection = geometry::collisions::contact::RectangleToRectangle(*(geometry::Rectangle*)actor->GetShape(), *(geometry::Rectangle*)wall->GetShape());
			actor->ResolveCollision(connection, wall);
		}

		// Slopes
		for (Slope* slope : environment.slopes)
		{
			geometry::Point connection = object_collisions::contact::ActorToSlope(*actor, *slope);
			actor->ResolveCollision(connection, slope);
		}
	}
}

double Match::UpdateTime()
{
	// Time delta
	t2 = std::chrono::system_clock::now();
	double delta = TimeDelta(t2, t1) * 0.000001;
	t1 = t2;
	frame_count++;

	return delta;
}

Match::Match(std::string map)
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


	// Add basic attack ability
	for (Actor* actor : environment.actors)
		actor->AddAbility(new Hit(&environment, actor));
}

void Match::addObject(GameObject* object)
{
	if (Slope* slope = dynamic_cast<Slope*>(object))
		environment.slopes.push_back(slope);
	else
		environment.walls.push_back(object);
}

void Match::addActor(Actor* actor)
{
	environment.actors.push_back(actor);
}

Data* Match::SerializeGameState()
{
	size_t size = sizeof(size_t) + environment.actors.size() * Actor::SerializationSize();
	Data* serialized = new Data(size);
	size_t actors_num = environment.actors.size();
	serialized->put(&actors_num, sizeof(size_t));

	// Serialize actors
	for (Actor* actor : environment.actors)
	{
		Data* serialized_actor = actor->Serialize();
		serialized->put(serialized_actor);
	}

	return serialized;
}

void Match::DeserializeGameState(Data* data)
{
	size_t actors_num;

	data->get(&actors_num, sizeof(size_t));

	// Deserialize actors
	for (int i = 0; i < actors_num; i++)
	{
		Data* actor_data = data->get(Actor::SerializationSize());
		if (!actor_data)
			continue;

		environment.actors[i]->Deserialize(actor_data);
		delete actor_data;
	}
}

void Match::Start()
{
	t1 = std::chrono::system_clock::now();

	while (!quit)
	{
		// Read input
		Input();

		// Update game state
		UpdateState(UpdateTime());
	}
}

#pragma once
#include "Animation.h"
#include "../../common/GameMechanics/Actor/Actor.h"

class ActorAnimations
{
private:
	Actor* actor = nullptr;

	Animation* idle = nullptr;

	Animation* LoadAnimation(rapidxml::xml_document<>* doc,
		std::string character,
		std::string animation,
		SDL_Renderer* renderer) 
	{
		rapidxml::xml_node<>* character_node = doc->first_node(animation.c_str());
		return new Animation(character_node, ACTORS_PATH + character + "/" + animation + "/", renderer);
	}

public:
	ActorAnimations(Actor* actor, std::string character, SDL_Renderer* renderer)
		:actor(actor)
	{
		std::string animation_path = ACTORS_PATH + character + "/animations.xml";
		AssetLoader loader(animation_path);
		rapidxml::xml_document<>* doc = loader.GetDocument();

		idle = LoadAnimation(doc, character, "Idle", renderer);
	}

	Animation* getAnimation()
	{
		return idle;
	}

	void CheckState()
	{
		if (actor->isMovingLeft() == actor->isMovingRight())
			return;

		if (actor->isMovingLeft())
			idle->SetFlip(SDL_FLIP_HORIZONTAL);

		if (actor->isMovingRight())
			idle->SetFlip(SDL_FLIP_NONE);
	}
};

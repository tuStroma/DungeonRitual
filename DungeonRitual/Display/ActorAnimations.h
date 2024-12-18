#pragma once
#include "Animation.h"
#include "../../common/GameMechanics/Actor/Actor.h"

class ActorAnimations
{
private:
	Actor* actor = nullptr;

	Animation* idle = nullptr;
	Animation* walk = nullptr;
	Animation* hit = nullptr;

	// Variable to track changing animations for proper reset
	Animation* last_played = nullptr;

	Animation* PlayAnimation(Animation* animation)
	{
		if (animation == last_played)
			return animation;

		animation->Reset();
		last_played = animation;
		return animation;
	}

	Animation* LoadAnimation(rapidxml::xml_document<>* doc,
		std::string character,
		std::string animation,
		SDL_Renderer* renderer) 
	{
		rapidxml::xml_node<>* character_node = doc->first_node(animation.c_str());
		return new Animation(character_node, ACTORS_PATH + character + "/" + animation + "/", renderer);
	}

	void SetFlip(SDL_RendererFlip flip)
	{
		idle->SetFlip(flip);
		walk->SetFlip(flip);
		hit->SetFlip(flip);
	}

public:
	ActorAnimations(Actor* actor, std::string character, SDL_Renderer* renderer)
		:actor(actor)
	{
		std::string animation_path = ACTORS_PATH + character + "/animations.xml";
		AssetLoader loader(animation_path);
		rapidxml::xml_document<>* doc = loader.GetDocument();

		idle = LoadAnimation(doc, character, "Idle", renderer);
		walk = LoadAnimation(doc, character, "Walk", renderer);
		hit = LoadAnimation(doc, character, "Hit", renderer);
	}

	Animation* getAnimation()
	{
		if (dynamic_cast<Hit*>(actor->ActiveAbility()))
			return PlayAnimation(hit);

		if (actor->isMoving() && actor->GetStandingOn())
			return PlayAnimation(walk);

		return PlayAnimation(idle);
	}

	void CheckState()
	{
		actor->isFacingRight() ?
			SetFlip(SDL_FLIP_NONE) :
			SetFlip(SDL_FLIP_HORIZONTAL);
	}
};

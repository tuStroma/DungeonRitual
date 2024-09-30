#pragma once

#include <SDL.h>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#define ACTORS_PATH "../common/Assets/Actors/"

class Animation {
private:
	class Frame
	{
	private:
		SDL_Surface* sprite = nullptr;
		SDL_Texture* texture = nullptr;
		// Duration in miliseconds
		int duration = 0;

		Frame* next = nullptr;

		int width = 0;
		int height = 0;

	public:
		Frame() {}
		Frame(std::string sprite_path, int duration, SDL_Renderer* renderer)
			:sprite(SDL_LoadBMP(sprite_path.c_str())),
			duration(duration),
			texture(SDL_CreateTextureFromSurface(renderer, sprite)),
			width(sprite->w),
			height(sprite->h)
		{}

		void SetNext(Frame* next)
		{
			this->next = next;
		}

		SDL_Texture* GetSprite()
		{
			return texture;
		}
		int GetWidth()
		{
			return width;
		}
		int GetHeight()
		{
			return height;
		}
		int GetDuration()
		{
			return duration;
		}
		Frame* Next()
		{
			return next;
		}
	};

	Frame* first = nullptr;
	Frame* current = nullptr;
	int time_counter = 0;

public:
	Animation(std::string character, std::string animation, SDL_Renderer* renderer)
	{
		// Parse XML
		std::string animation_path = ACTORS_PATH + character + "/animations.xml";
		rapidxml::file<> xmlFile(animation_path.c_str());
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		rapidxml::xml_node<>* node = doc.first_node(animation.c_str())->first_node();

		// Load sprites
		while (node)
		{
			std::string sprite_name = node->first_attribute("path")->value();
			int duration = atoi(node->first_attribute("duration")->value());

			std::string sprite_path = ACTORS_PATH + character + "/" + animation + "/" + sprite_name;
			Frame* frame = new Frame(sprite_path, duration, renderer);

			// Insert new frame into animation
			if (!first)
			{
				first = frame;
				current = frame;
			}
			else
			{
				current->SetNext(frame);
				current = frame;
			}

			node = node->next_sibling();
		}

		if (current)
			current->SetNext(first);
		current = first;
	}
	Animation(rapidxml::xml_node<>* node, std::string asset_path, SDL_Renderer* renderer)
	{
		node = node->first_node();

		// Load sprites
		while (node)
		{
			std::string sprite_name = node->first_attribute("path")->value();
			int duration = atoi(node->first_attribute("duration")->value());

			std::string sprite_path = asset_path + sprite_name;
			Frame* frame = new Frame(sprite_path, duration, renderer);

			// Insert new frame into animation
			if (!first)
			{
				first = frame;
				current = frame;
			}
			else
			{
				current->SetNext(frame);
				current = frame;
			}

			node = node->next_sibling();
		}

		if (current)
			current->SetNext(first);
		current = first;
	}

	SDL_Texture* GetSprite()
	{
		int now = SDL_GetTicks();

		SDL_Texture* sprite = current->GetSprite();
		int duration = current->GetDuration();

		if (duration == 0)
			return sprite;

		while (now - time_counter > duration)
		{
			current = current->Next();
			time_counter += duration;

			sprite = current->GetSprite();
			duration = current->GetDuration();
		}

		return sprite;
	}

	int GetWidth()
	{
		return current->GetWidth();
	}

	int GetHeight()
	{
		return current->GetHeight();
	}
};
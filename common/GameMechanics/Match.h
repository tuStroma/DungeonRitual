#pragma once
#include <iostream>
#include <vector>
#include <list>

#include "../Display/Window.h"
#include "../Display/DisplayParameters.h"
#include "../Display/Animation.h"
#include "../Display/SurfacePainter.h"
#include "AssetLoader/AssetLoader.h"
#include "GameObject.h"
#include "Actor/Actor.h"
#include "Actor/OutsideController.h"

#include "ObjectCollisions.h"

class Match
{
private:
	struct Layer {
		Animation* animation;
		Point position;
		double depth;

		Layer(Animation* animation, Point position, double depth)
			:animation(animation), position(position), depth(depth)
		{}
		Layer(rapidxml::xml_node<>* node, SDL_Renderer* renderer)
		{
			double position_x = atof(node->first_attribute("position_x")->value());
			double position_y = atof(node->first_attribute("position_y")->value());
			depth = atof(node->first_attribute("depth")->value());
			std::string path = node->first_attribute("path")->value();
			animation = new Animation(node, ASSETS_PATH + path, renderer);

			position = Point(position_x, position_y);
		}
	};


	bool quit = false;

	// Game status
	int player_index = 0;
	OutsideController* player_controller = nullptr;

	std::vector<Actor*> actors;
	std::vector<GameObject*> walls;
	std::vector<Slope*> slopes;

	double v_x = 1;
	double v_y = 0;

	// Time
	int t1 = 0, t2 = 0;
	int time_count = 0;
	int frame_count = 0;

	// Camera
	Point camera = Point(0, 0);

	int posToCameraX(double x);
	int posToCameraY(double y);

	// Display
	Window* window;
	std::list<Layer> background;

	void DrawObject(SDL_Surface* surface, GameObject* obj, Uint32 color);
	void DrawSpriteCentered(SDL_Texture* sprite, Point position, int width, int height, double paralax = 1);
	void DrawSprite(SDL_Texture* sprite, Rectangle* rectangle);
	void CreateBackgroundTexture();

	// Controle
	SDL_Event* event;

	void Input();
	void Update();
	void Display();

public:
	Match(Window* window, std::string map, int player_index);

	void addObject(GameObject* object);
	void addActor(Actor* actor);
	void addLayer(Animation* animation, Point position, double depth);

	Window* GetWindow();

	void Start();
};


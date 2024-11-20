#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <semaphore>

#include "../../common/GameMechanics/Match.h"

#include "../Display/ActorAnimations.h"
#include "../Display/Window.h"
#include "../Display/DisplayParameters.h"
#include "../Display/Animation.h"
#include "../Display/SurfacePainter.h"

class Client;

class LocalMatch : public Match
{
private:
	struct Layer {
		Animation* animation;
		geometry::Point position;
		double depth;

		Layer(Animation* animation, geometry::Point position, double depth)
			:animation(animation), position(position), depth(depth)
		{}
		Layer(rapidxml::xml_node<>* node, SDL_Renderer* renderer)
		{
			double position_x = atof(node->first_attribute("position_x")->value());
			double position_y = atof(node->first_attribute("position_y")->value());
			depth = atof(node->first_attribute("depth")->value());
			std::string path = node->first_attribute("path")->value();
			animation = new Animation(node, ASSETS_PATH + path, renderer);

			position = geometry::Point(position_x, position_y);
		}
	};

	// Online component
	Client* game_client = nullptr;
	Data* server_game_state = nullptr;

	std::binary_semaphore server_state_lock;

	// Controlls
	int player_index = 0;
	OutsideController* player_controller = nullptr;

	// Camera
	geometry::Point camera = geometry::Point(0, 0);

	int posToCameraX(double x);
	int posToCameraY(double y);

	// Display
	Window* window;
	std::list<Layer> background;
	std::map<Actor*, ActorAnimations*> animations;

	void DrawObject(SDL_Surface* surface, GameObject* obj, Uint32 color);
	void DrawSpriteCentered(SDL_Texture* sprite, geometry::Point position, int width, int height, double paralax = 1);
	void DrawSprite(SDL_Texture* sprite, geometry::Rectangle* rectangle);
	void CreateBackgroundTexture();

	// Controle
	SDL_Event* event;

	void UserAction(Action action);

	void ServerStateUpdate();

	void Input();
	void Update();
	void Display();

public:
	LocalMatch(Window* window, std::string map, int player_index, Client* game_client);

	void addLayer(Animation* animation, geometry::Point position, double depth);

	Window* GetWindow();

	void Start() override;

	// Online component

	void MakeActionAsPlayer(int player_id, Action action);
	void LoadServerState(Data* server_state);
};


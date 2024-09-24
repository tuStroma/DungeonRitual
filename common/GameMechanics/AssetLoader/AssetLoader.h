#pragma once
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <list>

#include "../Match.h"
#include "../Slope.h"


#define ASSETS_PATH "../common/Assets/"
#define MAPS_PATH "../common/Assets/Maps/"

class AssetLoader
{
private:
    static bool BoolFromAttribute(rapidxml::xml_node<>* node, std::string attr)
    {
        rapidxml::xml_attribute<>* attribute = node->first_attribute("left");
        if (!attribute)
            return true;

        return  attribute->value() != "" &&
            attribute->value() != "0";
    }

    static GameObject* ProcessWall(rapidxml::xml_node<>* node)
    {
        float left = atof(node->first_attribute("left")->value());
        float right = atof(node->first_attribute("right")->value());
        float up = atof(node->first_attribute("up")->value());
        float down = atof(node->first_attribute("down")->value());

        float width = right - left;
        float height = up - down;
        Point centre(left + width / 2, down + height / 2);

        std::cout << "Wall"
            << "\n\tLeft:\t" << left
            << "\n\tRight:\t" << right
            << "\n\tUp:\t" << up
            << "\n\tDown:\t" << down
            << "\n";

        return new GameObject(new Rectangle(centre, width, height));
    }

    static Slope* ProcessSlope(rapidxml::xml_node<>* node)
    {
        float begin_x = atof(node->first_attribute("begin_x")->value());
        float begin_y = atof(node->first_attribute("begin_y")->value());
        float end_x = atof(node->first_attribute("end_x")->value());
        float end_y = atof(node->first_attribute("end_y")->value());

        bool penetrable = BoolFromAttribute(node, "penetrable");

        std::cout << "Slope"
            << "\n\tBegin_x:\t" << begin_x
            << "\n\tBegin_y:\t" << begin_y
            << "\n\tEnd_x:\t\t" << end_x
            << "\n\tEnd_y:\t\t" << end_y
            << "\n\tPenetrable:\t" << (penetrable ? "true" : "false")
            << "\n";

        return new Slope(new Segment(Point(begin_x, begin_y), Point(end_x - begin_x, end_y - begin_y)), penetrable);
    }

    static Actor* ProcessActor(rapidxml::xml_node<>* node)
    {
        float position_x = atof(node->first_attribute("position_x")->value());
        float position_y = atof(node->first_attribute("position_y")->value());
        float width = atof(node->first_attribute("width")->value());
        float height = atof(node->first_attribute("height")->value());

        bool is_player = BoolFromAttribute(node, "is_player");

        Point centre(position_x, position_y);

        std::cout << "Actor"
            << "\n\tPosition_x:\t" << position_x
            << "\n\tPosition_y:\t" << position_y
            << "\n\tWidth:\t" << width
            << "\n\tHeight:\t" << height
            << "\n";

        return new Actor(new Rectangle(centre, width, height));
    }

    static void ProcessNode(rapidxml::xml_node<>* node,
                            std::list<GameObject*>& rectangles,
                            std::list<Slope*>& segments,
                            std::list<Actor*>& actors)
    {
        while (node)
        {
            std::string name = node->name();
            if (name == "Wall")
                rectangles.push_back(ProcessWall(node));
            else if (name == "Slope")
                segments.push_back(ProcessSlope(node));
            else if (name == "Actor")
                actors.push_back(ProcessActor(node));

            node = node->next_sibling();
        }
    }

    static Animation* LoadAnimation(rapidxml::xml_node<>* node, std::string asset_path)
    {
        return new Animation(node, asset_path);
    }

public:
	static void LoadMap(Match* match, std::string map_name)
	{
        std::string map_path = MAPS_PATH + map_name + "/map.xml";
        // Parse XML
        rapidxml::file<> xmlFile(map_path.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());
        rapidxml::xml_node<>* node = doc.first_node()->first_node();

        // Get shapes from XML
        std::list<GameObject*> walls;
        std::list<Slope*> slopes;
        std::list<Actor*> actors;
        ProcessNode(node, walls, slopes, actors);

        // Load layers
        node = doc.first_node("Layers")->first_node();

        while (node)
        {
            double position_x = atof(node->first_attribute("position_x")->value());
            double position_y = atof(node->first_attribute("position_y")->value());
            double depth = atof(node->first_attribute("depth")->value());
            std::string path = node->first_attribute("path")->value();
            Animation* animation = new Animation(node, ASSETS_PATH + path);

            match->addLayer(animation, Point(position_x, position_y), depth);

            node = node->next_sibling();
        }


        for (GameObject* wall : walls)
            match->addObject(wall);

        for (Slope* slope : slopes)
            match->addObject(slope);

        for (Actor* actor : actors)
            match->addActor(actor, true);
	}
};

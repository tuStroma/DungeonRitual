#pragma once
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <list>

#include "../Slope.h"


#define ASSETS_PATH "../common/Assets/"
#define MAPS_PATH "../common/Assets/Maps/"


class AssetLoader
{
private:
    rapidxml::xml_document<>* doc = nullptr;
    rapidxml::file<>* xmlFile = nullptr;


public:
    AssetLoader(std::string asset_path)
    {
        doc = new rapidxml::xml_document<>();
        xmlFile = new rapidxml::file<>(asset_path.c_str());
        doc->parse<0>(xmlFile->data());
    }

    rapidxml::xml_document<>* GetDocument()
    {
        return doc;
    }

    // Static loaders
    static GameObject* LoadWall(rapidxml::xml_node<>* node)
    {
        float left = atof(node->first_attribute("left")->value());
        float right = atof(node->first_attribute("right")->value());
        float up = atof(node->first_attribute("up")->value());
        float down = atof(node->first_attribute("down")->value());

        float width = right - left;
        float height = up - down;
        geometry::Point centre(left + width / 2, down + height / 2);

        std::cout << "Wall"
            << "\n\tLeft:\t" << left
            << "\n\tRight:\t" << right
            << "\n\tUp:\t" << up
            << "\n\tDown:\t" << down
            << "\n";

        return new GameObject(new geometry::Rectangle(centre, width, height));
    }

    static Slope* LoadSlope(rapidxml::xml_node<>* node)
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

        return new Slope(new geometry::Segment(geometry::Point(begin_x, begin_y), geometry::Point(end_x - begin_x, end_y - begin_y)), penetrable);
    }

    // Static helpers
    static bool BoolFromAttribute(rapidxml::xml_node<>* node, std::string attr)
    {
        rapidxml::xml_attribute<>* attribute = node->first_attribute("left");
        if (!attribute)
            return true;

        return  attribute->value() != "" &&
            attribute->value() != "0";
    }
};

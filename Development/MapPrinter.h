#pragma once
#include <list>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <opencv2/opencv.hpp>

#include "../common/GameMechanics/geometry/Rectangle.h"
#include "../common/GameMechanics/geometry/Segment.h"
#include "../common/Display/DisplayParameters.h"

#define MARGIN 150
#define MAPS_PATH "../common/Assets/Maps/"

using namespace rapidxml;

bool BoolFromAttribute(rapidxml::xml_node<>* node, std::string attr)
{
    xml_attribute<>* attribute = node->first_attribute("left");
    if (!attribute)
        return true;

    return  attribute->value() != "" &&
            attribute->value() != "0";
}

Rectangle* ProcessWall(rapidxml::xml_node<>* node)
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

    return new Rectangle(centre, width, height);
}

Segment* ProcessSlope(rapidxml::xml_node<>* node)
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

    return new Segment(Point(begin_x, begin_y), Point(end_x - begin_x, end_y - begin_y));
}

void ProcessNode(rapidxml::xml_node<>* node, std::list<Rectangle*>& rectangles, std::list<Segment*>& segments)
{
    while (node)
    {
        std::string name = node->name();
        if (name == "Wall")
            rectangles.push_back(ProcessWall(node));
        else if (name == "Slope")
            segments.push_back(ProcessSlope(node));

        node = node->next_sibling();
    }
}


void MapBorders(int& left, int& right, int& up, int& down, std::list<Rectangle*>& rectangles, std::list<Segment*>& segments)
{
    for (Rectangle* r : rectangles)
    {
        left = fmin(left, r->Left() * PIXELS_IN_METER);
        right = fmax(right, r->Right() * PIXELS_IN_METER);
        up = fmax(up, r->Up() * PIXELS_IN_METER);
        down = fmin(down, r->Down() * PIXELS_IN_METER);
    }

    for (Segment* s : segments)
    {
        left = fmin(left, s->LeftPoint().X() * PIXELS_IN_METER);
        right = fmax(right, s->RightPoint().X() * PIXELS_IN_METER);
        up = fmax(up, s->UpperPoint().Y() * PIXELS_IN_METER);
        down = fmin(down, s->LowerPoint().Y() * PIXELS_IN_METER);
    }
}

int ToImageX(cv::Mat& image, double x)
{
    return image.cols / 2 + x * PIXELS_IN_METER;
}

int ToImageY(cv::Mat& image, double y)
{
    return image.rows / 2 - y * PIXELS_IN_METER;
}

void PrintRectangle(cv::Mat& image, Rectangle* r)
{
    for (int col = ToImageX(image, r->Left()); col <= ToImageX(image, r->Right()); col++)
        for (int row = ToImageY(image, r->Up()); row <= ToImageY(image, r->Down()); row++)
            image.at<cv::Vec3b>(row, col)[0] = 0xFF;
}

void PrintSegment(cv::Mat& image, Segment* s)
{
    int begin_x = ToImageX(image, s->LeftPoint().X());
    int begin_y = ToImageY(image, s->LeftPoint().Y());
    int end_x = ToImageX(image, s->RightPoint().X());
    int end_y = ToImageY(image, s->RightPoint().Y());

    int dx = end_x - begin_x;
    int dy = end_y - begin_y;

    if (dx == 0)
    {
        for (int i = begin_y; i <= end_y; i++)
            image.at<cv::Vec3b>(i, begin_x)[0] = 0xFF;
        return;
    }

    double step = (double)dy / dx;
    double y = begin_y;

    for (int x = begin_x; x <= end_x; x++)
    {
        double new_y = y + step;
        for (int h = fmin(y, new_y); h <= fmax(y, new_y); h++)
            image.at<cv::Vec3b>(h, x)[0] = 0xFF;
        y = new_y;
    }
}

cv::Mat CreateImage(std::list<Rectangle*>& rectangles, std::list<Segment*>& segments)
{
    // Map borders
    int left = 0, right = 0, up = 0, down = 0;
    MapBorders(left, right, up, down, rectangles, segments);

    int width = (fmax(left, right) + MARGIN) * 2;
    int height = (fmax(up, down) + MARGIN) * 2;

    cv::Mat image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

    for (Rectangle* r : rectangles)
        PrintRectangle(image, r);

    for (Segment* s : segments)
        PrintSegment(image, s);

    return image;
}

void PrintMap(std::string map_name)
{
    std::string map_path = MAPS_PATH + map_name + "/map.xml";
    // Parse XML
    file<> xmlFile(map_path.c_str());
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    xml_node<>* node = doc.first_node()->first_node();

    std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

    // Get shapes from XML
    std::list<Rectangle*> rectangles;
    std::list<Segment*> segments;
    ProcessNode(node, rectangles, segments);

    cv::Mat image = CreateImage(rectangles, segments);

    cv::imwrite(MAPS_PATH + map_name + "/layout.png", image);
    
}
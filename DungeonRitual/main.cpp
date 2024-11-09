#include <iostream>

#include "Game/Game.h"

//// Tests
void segment()
{
	geometry::Segment a = geometry::Segment(geometry::Point(-1, 1), geometry::Point(2, -2));
	geometry::Segment b = geometry::Segment(geometry::Point(-1, -1), geometry::Point(2, 2));
	geometry::Segment c = geometry::Segment(geometry::Point(0, -1), geometry::Point(0, 2));
	geometry::Segment d = geometry::Segment(geometry::Point(0, -3), geometry::Point(0, 2));
	geometry::Segment e = geometry::Segment(geometry::Point(1, 1), geometry::Point(1, 1));
	std::cout << "Expected 1: " << geometry::collisions::SegmentToSegment(a, b) << "\n";
	std::cout << "Expected 1: " << geometry::collisions::SegmentToSegment(a, c) << "\n";
	std::cout << "Expected 0: " << geometry::collisions::SegmentToSegment(b, d) << "\n";
	std::cout << "Expected 1: " << geometry::collisions::SegmentToSegment(b, e) << "\n";
	exit(0);
}

int main(int argc, char* argv[])
{
	//segment();

	Game game = Game();
	game.Launch();

	return 0;
}
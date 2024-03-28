#include <iostream>

#include "Game/Game.h"

//// Tests
void segment()
{
	Segment a = Segment(Point(-1, 1), Point(2, -2));
	Segment b = Segment(Point(-1, -1), Point(2, 2));
	Segment c = Segment(Point(0, -1), Point(0, 2));
	Segment d = Segment(Point(0, -3), Point(0, 2));
	Segment e = Segment(Point(1, 1), Point(1, 1));
	std::cout << "Expected 1: " << collisions::SegmentToSegment(a, b) << "\n";
	std::cout << "Expected 1: " << collisions::SegmentToSegment(a, c) << "\n";
	std::cout << "Expected 0: " << collisions::SegmentToSegment(b, d) << "\n";
	std::cout << "Expected 1: " << collisions::SegmentToSegment(b, e) << "\n";
	exit(0);
}

int main(int argc, char* argv[])
{
	//segment();

	Game game = Game();
	game.Launch();

	return 0;
}
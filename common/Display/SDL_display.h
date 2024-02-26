#pragma once
#include <SDL.h>

namespace display
{
	inline void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
		int bpp = surface->format->BytesPerPixel;
		Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
		*(Uint32*)p = color;
	};

	inline void DrawLine(SDL_Surface* surface, int x, int y, int length, int dx, int dy, Uint32 color)
	{
		for (int i = 0; i < length; i++)
		{
			DrawPixel(surface, x, y, color);
			x += dx;
			y += dy;
		}
	}

	inline void DrawRectangle(SDL_Surface* surface, int x_start, int y_start, int x_finish, int y_finish, Uint32 color)
	{
		int x0 = x_start < x_finish ? x_start : x_finish;
		int x1 = x_start > x_finish ? x_start : x_finish;

		int y0 = y_start < y_finish ? y_start : y_finish;
		int y1 = y_start > y_finish ? y_start : y_finish;

		for (int height = y0; height <= y1; height++)
			DrawLine(surface, x0, height, x1 - x0, 1, 0, color);
	}

	inline void DrawRectangleCentered(SDL_Surface* surface, int x, int y, int w, int h, Uint32 color)
	{
		DrawRectangle(surface, x - w / 2, y - h / 2, x + w / 2, y + h / 2, color);
	}

} // display

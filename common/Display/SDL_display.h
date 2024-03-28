#pragma once
#include <SDL.h>

namespace display
{
	inline void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color)
	{
		if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
			return;

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

	inline void DrawLine(SDL_Surface* surface, int x0, int y0, int x1, int y1, Uint32 color)
	{
		// Swap points
		if (x0 > x1)
		{
			int buffer = x0;
			x0 = x1;
			x1 = buffer;

			buffer = y0;
			y0 = y1;
			y1 = buffer;
		}

		double step = (double)(y1 - y0) / (x1 - x0);
		bool step_sign = step > 0;
		int increment = step_sign ? 1 : -1;

		double count = 0;

		while (x0 <= x1)
		{
			count += step;

			int column = round(abs(count)) + 1;
			int h = 0;
			while (h < column && h > -column)
			{
				DrawPixel(surface, x0, y0 + h, color);
				h += increment;
			}

			int y_step = step_sign ? column - 1 : 1 - column;
			count -= y_step;

			y0 += y_step;
			x0++;
		}
	}

	inline void DrawRectangle(SDL_Surface* surface, int x_start, int y_start, int x_finish, int y_finish, Uint32 color)
	{
		int x0 = x_start < x_finish ? x_start : x_finish;
		int x1 = x_start > x_finish ? x_start : x_finish;

		int y0 = y_start < y_finish ? y_start : y_finish;
		int y1 = y_start > y_finish ? y_start : y_finish;

		if (x1 < 0 || y1 < 0 || x0 >= surface->w || y0 >= surface->h)
			return;

		for (int height = y0; height <= y1; height++)
			DrawLine(surface, x0, height, x1 - x0, 1, 0, color);
	}

	inline void DrawRectangleCentered(SDL_Surface* surface, int x, int y, int w, int h, Uint32 color)
	{
		DrawRectangle(surface, x - w / 2, y - h / 2, x + w / 2, y + h / 2, color);
	}

} // display

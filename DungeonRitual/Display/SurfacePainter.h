#pragma once
#include <SDL.h>

class SurfacePainter
{
private:
	static int CentreToSurfacePosition_X(SDL_Surface* surface, int x)
	{
		return (int)(surface->w / 2 + x);
	}
	static int CentreToSurfacePosition_Y(SDL_Surface* surface, int y)
	{
		return (int)(surface->h / 2 - y);
	}

	static void SetSurfaceColor(SDL_Surface* surface, int x, int y, Uint32 color)
	{
		int bpp = surface->format->BytesPerPixel;
		Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
		*(Uint32*)p = color;
	}

	static Uint32 GetSurfaceColor(SDL_Surface* surface, int x, int y)
	{
		int bpp = surface->format->BytesPerPixel;
		Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
		return *(Uint32*)p;
	}

	// Mixes 2 colors by a given ratio
	// Ratio is a number in range 0-255
	// Higher ratio values favours 'a' in cost of 'b' in the result
	static Uint32 MixColors(SDL_Surface* surface, Uint32 a, Uint32 b, int ratio)
	{
		Uint8 a_red, b_red, result_red,
			a_green, b_green, result_green,
			a_blue, b_blue, result_blue;

		SDL_GetRGB(a, surface->format, &a_red, &a_green, &a_blue);
		SDL_GetRGB(b, surface->format, &b_red, &b_green, &b_blue);

		result_red = (a_red * ratio + b_red * (255 - ratio)) / 255;
		result_green = (a_green * ratio + b_green * (255 - ratio)) / 255;
		result_blue = (a_blue * ratio + b_blue * (255 - ratio)) / 255;
		return SDL_MapRGB(surface->format, result_red, result_green, result_blue);
	}

public:
	static void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color)
	{
		x = CentreToSurfacePosition_X(surface, x);
		y = CentreToSurfacePosition_Y(surface, y);

		if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
			return;

		Uint8 r, g, b, a;
		SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);
		if (a != 255)
		{
			Uint32 background = GetSurfaceColor(surface, x, y);
			color = MixColors(surface, color, background, a);
		}

		SetSurfaceColor(surface, x, y, color);
	};
	static void DrawLine(SDL_Surface* surface, int x, int y, int length, int dx, int dy, Uint32 color)
	{
		for (int i = 0; i < length; i++)
		{
			DrawPixel(surface, x, y, color);
			x += dx;
			y += dy;
		}
	}
	static void DrawLine(SDL_Surface* surface, int x0, int y0, int x1, int y1, Uint32 color)
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

			int column = round(fabs(count)) + 1;
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
	static void DrawRectangle(SDL_Surface* surface, int x_start, int y_start, int x_finish, int y_finish, Uint32 color)
	{
		int x0 = x_start < x_finish ? x_start : x_finish;
		int x1 = x_start > x_finish ? x_start : x_finish;

		int y0 = y_start < y_finish ? y_start : y_finish;
		int y1 = y_start > y_finish ? y_start : y_finish;

		if (x1 < -surface->w / 2 || y1 < -surface->h / 2 || x0 >= surface->w / 2 || y0 >= surface->h / 2)
			return;

		for (int height = y0; height <= y1; height++)
			DrawLine(surface, x0, height, x1 - x0, 1, 0, color);
	}
	static void DrawRectangleCentered(SDL_Surface* surface, int x, int y, int w, int h, Uint32 color)
	{
		DrawRectangle(surface, x - w / 2, y - h / 2, x + w / 2, y + h / 2, color);
	}
};
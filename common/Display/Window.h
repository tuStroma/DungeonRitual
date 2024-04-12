#pragma once
#include <SDL.h>

class Window
{
private:
	// Window properties
	int screen_width;
	int screen_height;

	// SDL
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Texture* scrtex;

	// Consts
	int black;


	int CentreToWindowPosition_X(int x)
	{
		return (int)(screen_width / 2 + x);
	}
	int CentreToWindowPosition_Y(int y)
	{
		return (int)(screen_height / 2 - y);
	}

public:
	Window(int width, int height)
		:screen_width(width), screen_height(height)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow("Dungeon Ritual", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, 0);

		surface = SDL_CreateRGBSurface(0, screen_width, screen_height, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			screen_width, screen_height);

		// Set consts
		black = SDL_MapRGB(surface->format, 0x00, 0x00, 0x00);
	}


	// Draw
	void DrawPixel(int x, int y, Uint32 color)
	{
		x = CentreToWindowPosition_X(x);
		y = CentreToWindowPosition_Y(y);

		if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
			return;

		int bpp = surface->format->BytesPerPixel;
		Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
		*(Uint32*)p = color;
	};
	void DrawLine(int x, int y, int length, int dx, int dy, Uint32 color)
	{
		for (int i = 0; i < length; i++)
		{
			DrawPixel(x, y, color);
			x += dx;
			y += dy;
		}
	}
	void DrawLine(int x0, int y0, int x1, int y1, Uint32 color)
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
				DrawPixel(x0, y0 + h, color);
				h += increment;
			}

			int y_step = step_sign ? column - 1 : 1 - column;
			count -= y_step;

			y0 += y_step;
			x0++;
		}
	}
	void DrawRectangle(int x_start, int y_start, int x_finish, int y_finish, Uint32 color)
	{
		int x0 = x_start < x_finish ? x_start : x_finish;
		int x1 = x_start > x_finish ? x_start : x_finish;

		int y0 = y_start < y_finish ? y_start : y_finish;
		int y1 = y_start > y_finish ? y_start : y_finish;

		if (x1 < -surface->w/2 || y1 < -surface->h/2 || x0 >= surface->w/2 || y0 >= surface->h/2)
			return;

		for (int height = y0; height <= y1; height++)
			DrawLine(x0, height, x1 - x0, 1, 0, color);
	}
	void DrawRectangleCentered(int x, int y, int w, int h, Uint32 color)
	{
		DrawRectangle(x - w / 2, y - h / 2, x + w / 2, y + h / 2, color);
	}

	void ClearFrame()
	{
		SDL_FillRect(surface, NULL, black);
	}
	void DisplayFrame()
	{
		SDL_UpdateTexture(scrtex, NULL, surface->pixels, surface->pitch);
		//SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	// Generate color
	int getColor(Uint8 r, Uint8 g, Uint8 b)
	{
		return SDL_MapRGB(surface->format, r, g, b);
	}
};

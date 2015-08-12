#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <gl\glew.h>

#include "shader.h"

class Window
{
public:
	static void Create(int width, int height, const std::string& title, bool fullscreen);
	static void Close();

	static void Clear(float r, float g, float b, float a);
	static void Update();

	static bool IsOpen();
	static void SetCloseRequest();

	static void SetMousePosition(int x, int y);

	static void SetRenderToScreen();

	static int GetWidth();
	static int GetHeight();
	static float GetAspect();
protected:
private:
	static int s_width;
	static int s_height;

	static SDL_Window* s_window;
	static SDL_GLContext s_glContext;
	static bool s_isOpen;
};

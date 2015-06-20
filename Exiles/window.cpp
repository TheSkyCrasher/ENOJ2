#include "window.h"
#include <GL/glew.h>
#include <iostream>

int Window::s_width = 0;
int Window::s_height = 0;

SDL_Window* Window::s_window = nullptr;
SDL_GLContext Window::s_glContext = 0;
bool Window::s_isOpen = true;

void Window::Create(int width, int height, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	s_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	s_glContext = SDL_GL_CreateContext(s_window);

	SDL_GL_SetSwapInterval(1);

	GLenum res = glewInit();
    if(res != GLEW_OK)
    {
		std::cerr << "Glew failed to initialize!" << std::endl;
    }

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	s_isOpen = true;
	s_width = width;
	s_height = height;
}

void Window::Close()
{
	s_isOpen = false;
	SDL_GL_DeleteContext(s_glContext);
	SDL_DestroyWindow(s_window);
	SDL_Quit();
}

void Window::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{
	SDL_GL_SwapWindow(s_window);
}

bool Window::IsOpen()
{
	return s_isOpen;
}

void Window::SetCloseRequest()
{
	s_isOpen = false;
}

void Window::SetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(s_window, x, y);
}

int Window::GetWidth() 
{ 
	return s_width; 
}

int Window::GetHeight() 
{ 
	return s_height; 
}

float Window::GetAspect()
{
	return ((float)s_width / (float)s_height);
}
#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class Display
{
public:
	Display();
	~Display();
	void initDisplay();
	void swapBuffer();
	void clearDisplay(float r, float g, float b, float a);

	float getWidth();
	float getHeight();

private:

	void returnError(std::string errorString);
	
	//Variable for the context
	SDL_GLContext glContext;

	//Pointer that holds thw window
	SDL_Window* sdlWindow;

	//Variables for the screens height and width
	float screenWidth;
	float screenHeight;
};


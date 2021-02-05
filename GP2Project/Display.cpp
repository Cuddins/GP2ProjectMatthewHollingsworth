#include "Display.h"

//Constructor
Display::Display()
{
	//Initialise to null
	sdlWindow = nullptr;

	//Sets screen height and width
	screenWidth = 1024.0f;
	screenHeight = 768.0f; 
}

//Destructor
Display::~Display()
{
	//Deletes the context
	SDL_GL_DeleteContext(glContext);

	//Deletes the window
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}

//Getters
float Display::getWidth()
{
	return screenWidth;
}
float Display::getHeight()
{
	return screenHeight;
}

//Method for debugging
void Display::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void Display::swapBuffer()
{
	//swap buffers
	SDL_GL_SwapWindow(sdlWindow); 
}

void Display::clearDisplay(float r, float g, float b, float a)
{
	//Clear colour and depth buffer
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::initDisplay()
{
	//initalise everything
	SDL_Init(SDL_INIT_EVERYTHING); 

	//Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	// set up double buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);    
	
	//Creates the window
	sdlWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); // create window

	//Checks if window was created
	if (sdlWindow == nullptr)
	{
		returnError("window failed to create");
	}

	//Creates the context
	glContext = SDL_GL_CreateContext(sdlWindow);

	//Checks if the context was created
	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}

	//Initialises GLEW
	GLenum error = glewInit();

	//Checks if GLEW was initialised
	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}

	//Enable z-buffering 
	glEnable(GL_DEPTH_TEST);

	//Dont draw faces that are not pointing at the camera
	glEnable(GL_CULL_FACE); 

	//Sets values used when colours are cleared
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}
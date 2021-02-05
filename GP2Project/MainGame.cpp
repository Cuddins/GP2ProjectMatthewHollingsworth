#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

//Constructor
MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display();
    Mesh* cat();
	Mesh* dog();
	Mesh* hand();
	Audio* audioDevice();
}

//Destructor
MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	//Initialise the display
	_gameDisplay.initDisplay();

	//Load sounds
	meow = audioDevice.loadSound("..\\res\\Meow.wav");
	bark = audioDevice.loadSound("..\\res\\Bark.wav");
	music = audioDevice.loadSound("..\\res\\ukulele.wav");
	
	//Load models
	cat.loadModel("..\\res\\Cat.obj");
	dog.loadModel("..\\res\\Dog.obj");
	hand.loadModel("..\\res\\Hand.obj");
	
	//Initialise the camera
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	//initlialise the shader
	shader.init("..\\res\\shader");

	//initialise variables
	counter = 1.0f;
	handX = 0.0f;
	handY = 0.0f;
}

void MainGame::gameLoop()
{
	//Keeps game running while the gamestate is not EXIT
	while (_gameState != GameState::EXIT)
	{
		//Allows user input
		processInput();

		//Displays the game
		drawGame();

		//Collision detection for cat and dog models
		collisionCat(cat.getSpherePos(), cat.getSphereRadius(), hand.getSpherePos(), hand.getSphereRadius());
		collisionDog(dog.getSpherePos(), dog.getSphereRadius(), hand.getSpherePos(), hand.getSphereRadius());

		//Starts background music
		playAudio(music, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	//processes events
	while(SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
			//Quits the game
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			//Detects if a key is being pressed
			case SDL_KEYDOWN:
				//Detects which key was pressed
				switch (evnt.key.keysym.sym)
				{
					case SDLK_LEFT:
						handX += 0.1f;
						break;
					case SDLK_RIGHT:
						handX -= 0.1f;
						break;
					case SDLK_UP:
						handY += 0.1f;
						break;
					case SDLK_DOWN:
						handY -= 0.1f;
						break;
				}
				break;
		}
	}
	
}

//Detects collision for the cat
bool MainGame::collisionCat(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	//Calculates distance between the mesh's
	float distance = ((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));
	
	//Checks if calculated distance is less than the sum of mesh radius'
	if (distance*distance < (m1Rad + m2Rad))
	{
		audioDevice.setlistener(myCamera.getPos(), m1Pos);
		playAudio(meow, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

//Detects collision for the dog
bool MainGame::collisionDog(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	//Calculates distance between the mesh's
	float distance = ((m2Pos.x - m1Pos.x) * (m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) * (m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z) * (m2Pos.z - m1Pos.z));

	//Checks if calculated distance is less than the sum of mesh radius'
	if (distance * distance < (m1Rad + m2Rad))
	{
		audioDevice.setlistener(myCamera.getPos(), m1Pos);
		playAudio(bark, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);

	//Checks if sound is already playing
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(Source, pos);
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Update Transform
	transform.SetPos(glm::vec3(-1.5f, sinf(counter), 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	//load Textures
	Texture catTexture("..\\res\\CatFur.jpg");
	Texture dogTexture("..\\res\\DogFur.jpg");
	Texture handTexture("..\\res\\Skin.jpg");

	shader.Bind();
	shader.Update(transform, myCamera);
	catTexture.Bind(0);
	cat.draw();
	cat.updateSphereData(*transform.GetPos(), 0.62f);
	
	//Update Transform
	transform.SetPos(glm::vec3(1.5f, -sinf(counter), 0.0f));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shader.Bind();
	shader.Update(transform, myCamera);
	dogTexture.Bind(0);
	dog.draw();
	dog.updateSphereData(*transform.GetPos(), 0.62f);

	//Update Transform
	transform.SetPos(glm::vec3(handX, handY, 0.0));
	transform.SetRot(glm::vec3(0.0, 90.0, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shader.Bind();
	shader.Update(transform, myCamera);
	handTexture.Bind(0);
	hand.draw();
	hand.updateSphereData(*transform.GetPos(), 0.62f);
	
	
	counter = counter + 0.05f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 
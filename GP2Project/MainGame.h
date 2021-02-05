#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collisionCat(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	bool collisionDog(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);

	Display _gameDisplay;
	GameState _gameState;
	Mesh cat;
	Mesh dog;
	Mesh hand;
	Camera myCamera;
	Shader shader;
	Audio audioDevice;

	//Variables for altering transforms
	float counter;
	float handX;
	float handY;

	//Holds audio
	unsigned int meow;
	unsigned int bark;
	unsigned int music;
};


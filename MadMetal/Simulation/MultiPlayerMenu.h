#pragma once

#include "Global\Definitions.h"
#include "Scene Manager\Scene.h"
#include "Objects\Text3D.h"
#include "Objects\TexturedObject2D.h"

class PlayerSelection {
private:
	World *m_world;
	GamePad *m_gamePad;
	Audio *m_audio;
	ControllableTemplate *playerTemplate;
	Characters tempSelection;
	Object3D *selectionIndicator;
	Object3D *selectedCar;
	Object3D *background;
	TexturedObject2D *bToQuit;
	glm::vec3 selectedCarPosition;

	glm::vec3 selectionIndicatorInitialPosition;
	glm::vec3 selectionIndicatorOffset = glm::vec3(7, 0, 0);

	int assignedPosition = -1;

public:
	PlayerSelection(GamePad *gamePad, Audio *audio, World *world);

	void joinGame(int position, glm::vec3 selectionIndicatorInitialPosition, glm::vec3 carPosition);

	bool joinedGame();

	void unjoinGame();

	void selectCar();

	void unselectCar();

	bool carSelected();

	void selectNextCar();


	void selectPrevCar();

	ControllableTemplate *getControllableTemplate();

	GamePad *getGamePad();
	int getAssignedPosition();
};

class MultiPlayerMenu : public Scene
{
private:
	std::vector<GamePad *> m_gamePads;
	std::vector<PlayerSelection *> m_players;
	std::vector<Object3D *> playerBoxes;
	std::vector<TexturedObject2D *> aToJoins;
	TexturedObject2D *aToStart;
	Object3D *car1,
		*car2,
		*car3,
		*numberOfAIsButton,
		*startButton;
	Text3D *numberOfAIsString;
	Text3D *loadingInfoString;
	SceneMessage::SceneEnum messageToReturn = SceneMessage::eNone;
	int numberOfAIs = 4;
	Audio *m_audio;
public:
	MultiPlayerMenu(Input * input, Audio *audio);
	~MultiPlayerMenu();
	bool simulateScene(double dt, SceneMessage &newMessage);
	void setupSceneLights();
};


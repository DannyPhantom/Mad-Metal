#include "MainMenu.h"
#include "Objects\Text3D.h"
#include "Global\Assets.h"
#include "Objects\ObjectUpdaters\ObjectPositionUpdater.h"
#include "Objects\ObjectUpdaters\ObjectRotationUpdater.h"
#include "Objects\ObjectUpdaters\ObjectScaleUpdater.h"
#include "Objects\ObjectUpdaters\ObjectUpdaterSequence.h"
#include "Objects\ObjectUpdaters\ObjectUpdaterParallel.h"

MainMenu::MainMenu(Input * input, Audio *audio)
{
	m_gamePad = input->getGamePadHandle();
	m_audio = audio;
	m_audio->clearListeners();
	m_audio->setMusicVolume(128);

	m_defaultSceneCamera->setLookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -3), glm::vec3(0, 1, 0));
	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, 1, -15));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Singleplayer.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		singlePlayerButton = new Object3D(1, au, p, a, r, NULL);
		m_world->addGameObject(singlePlayerButton);
		selectedButton = singlePlayerButton;
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, -1, -25));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Multiplayer.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		multiPlayerButton = new Object3D(2, au, p, a, r, NULL);
		m_world->addGameObject(multiPlayerButton);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, -3, -25));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Controls.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		controlsButton = new Object3D(2, au, p, a, r, NULL);
		m_world->addGameObject(controlsButton);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, -5, -25));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Quit.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		exitButton = new Object3D(3, au, p, a, r, NULL);
		m_world->addGameObject(exitButton);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, 0, -30));
		a->setScale(glm::vec3(33, 24.5, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/MainMenuBackground.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		background = new Object3D(3, au, p, a, r, NULL);
		m_world->addGameObject(background);
	}

	setupSceneLights();
}

MainMenu::~MainMenu() {
}


void MainMenu::upPressed() {
	m_audio->queAudioSource(NULL, MenuButtonChangeSound());
	unselectMenuItem(selectedButton);
	if (selectedButton == singlePlayerButton) {
		selectedButton = exitButton;
	}
	else if (selectedButton == multiPlayerButton) {
		selectedButton = singlePlayerButton;
	}
	else if (selectedButton == controlsButton) {
		selectedButton = multiPlayerButton;
	}
	else if (selectedButton == exitButton) {
		selectedButton = controlsButton;
	}
	selectMenuItem(selectedButton);
}

void MainMenu::downPressed() {
	m_audio->queAudioSource(NULL, MenuButtonChangeSound());
	unselectMenuItem(selectedButton);
	if (selectedButton == singlePlayerButton) {
		selectedButton = multiPlayerButton;
	}
	else if (selectedButton == multiPlayerButton) {
		selectedButton = controlsButton;
	}
	else if (selectedButton == controlsButton) {
		selectedButton = exitButton;
	}
	else if (selectedButton == exitButton) {
		selectedButton = singlePlayerButton;
	}
	selectMenuItem(selectedButton);
}

void MainMenu::aPressed() {
	m_audio->queAudioSource(NULL, MenuButtonClickSound());
	if (selectedButton == singlePlayerButton) {
		messageToReturn = SceneMessage::eSingleCharSelect;
	}
	else if (selectedButton == multiPlayerButton) {
		messageToReturn = SceneMessage::eMultiCharSelect;
	}
	else if (selectedButton == controlsButton) {
		messageToReturn = SceneMessage::eControls;
	}
	else if (selectedButton == exitButton) {
		messageToReturn = SceneMessage::eExit;
	}
}


void MainMenu::selectMenuItem(Object3D *menuItem) {
	{
		ObjectPositionUpdater *upd = new ObjectPositionUpdater(menuItem, glm::vec3(0, 0, 10), .5);
		m_world->addObjectUpdater(upd);
	}
}

void MainMenu::unselectMenuItem(Object3D *menuItem) {
	{
		ObjectPositionUpdater *upd = new ObjectPositionUpdater(menuItem, glm::vec3(0, 0, -10), .5);
		m_world->addObjectUpdater(upd);
	}
}

bool MainMenu::simulateScene(double dt, SceneMessage &message)
{
	m_sceneGameTimeSeconds += dt;
	m_world->update(dt);
	if (messageToReturn != SceneMessage::eNone) {
		message.setTag(messageToReturn);
		messageToReturn = SceneMessage::eNone;
		return true;
	}

	//music
	if (m_audio->getMusicFinished()) {
		m_audio->playMusic(CrysisTwoThemeSong(), 1);
	}

	//check gamepad stuff
	if (m_gamePad->checkConnection() && m_sceneGameTimeSeconds > 1)
	{
		
		if (m_gamePad->isPressed(GamePad::DPadUp) || m_gamePad->isPressed(GamePad::LJoyUp)) {
			upPressed();
		} 
		else if (m_gamePad->isPressed(GamePad::DPadDown) || m_gamePad->isPressed(GamePad::LJoyDown)) {
			downPressed();
		}
		else if (m_gamePad->isPressed(GamePad::AButton)) {
			aPressed();
		}

	}
	return false;
}

void MainMenu::setupSceneLights() {

	{
		Animatable *anable = new Animatable();
		Light *light = new Light(1, anable);
		anable->setPosition(glm::vec3(100, 100, 100));
		light->colour = glm::vec3(1, 1, 1);
		light->constant = .8f;
		light->linear = 0;
		light->quad = 0;
		light->cutoff = 500.0;
		m_world->addLightObject(light);
	}
}
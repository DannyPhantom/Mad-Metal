#include "PauseMenu.h"
#include "Objects\Text3D.h"
#include "Global\Assets.h"
#include "Objects\Cars\MeowMix.h"
#include "Objects\ObjectUpdaters\ObjectPositionUpdater.h"
#include "Objects\ObjectUpdaters\ObjectRotationUpdater.h"
#include "Objects\ObjectUpdaters\ObjectScaleUpdater.h"
#include "Objects\ObjectUpdaters\ObjectUpdaterSequence.h"
#include "Objects\ObjectUpdaters\ObjectUpdaterParallel.h"
#include "Settings.h"
#include <sstream>


PauseMenu::PauseMenu(std::vector<ControllableTemplate *> players, Audio *audio)
{
	m_audio = audio;
	m_audio->clearListeners();
	m_gamePad = players.at(players.size()-1)->getGamePad();
	m_playerTemplates = players;
	m_playerTemplates.pop_back();
	m_defaultSceneCamera->setLookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -3), glm::vec3(0, 1, 0));
	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, 2, -15));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Resume.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		resumeButton = new Object3D(1, au, p, a, r, NULL);
		m_world->addGameObject(resumeButton);
		selectedButton = resumeButton;
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, 0, -25));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Restart.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		restartButton = new Object3D(2, au, p, a, r, NULL);
		m_world->addGameObject(restartButton);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, -2, -25));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/MainMenu.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		exitToMainMenuButton = new Object3D(3, au, p, a, r, NULL);
		m_world->addGameObject(exitToMainMenuButton);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, -4, -25));
		a->setScale(glm::vec3(5, 1, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Quit.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		exitToWindowsButton = new Object3D(4, au, p, a, r, NULL);
		m_world->addGameObject(exitToWindowsButton);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->updatePosition(glm::vec3(0, 0, -30));
		a->setScale(glm::vec3(33, 24.5, 1));
		Audioable *au = new Audioable(*audio);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/PauseScreenBackground.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		background = new Object3D(3, au, p, a, r, NULL);
		m_world->addGameObject(background);
	}

	setupSceneLights();
}


PauseMenu::~PauseMenu()
{
}

void PauseMenu::upPressed() {
	m_audio->queAudioSource(NULL, MenuButtonChangeSound());
	unselectMenuItem(selectedButton);
	if (selectedButton == resumeButton) {
		selectedButton = exitToWindowsButton;
	}
	else if (selectedButton == restartButton) {
		selectedButton = resumeButton;
	}
	else if (selectedButton == exitToMainMenuButton) {
		selectedButton = restartButton;
	}
	else if (selectedButton == exitToWindowsButton) {
		selectedButton = exitToMainMenuButton;
	}
	selectMenuItem(selectedButton);
}

void PauseMenu::downPressed() {
	m_audio->queAudioSource(NULL, MenuButtonChangeSound());
	unselectMenuItem(selectedButton);
	if (selectedButton == resumeButton) {
		selectedButton = restartButton;
	}
	else if (selectedButton == restartButton) {
		selectedButton = exitToMainMenuButton;
	}
	else if (selectedButton == exitToMainMenuButton) {
		selectedButton = exitToWindowsButton;
	}
	else if (selectedButton == exitToWindowsButton) {
		selectedButton = resumeButton;
	}
	selectMenuItem(selectedButton);
}

void PauseMenu::aPressed() {
	if (selectedButton == resumeButton) {
		m_audio->queAudioSource(NULL, MenuButtonClickSound());
		messageToReturn = SceneMessage::ePop;
		if (stopMusic)
			m_audio->stopMusic();
	}
	else if (selectedButton == exitToMainMenuButton) {
		m_audio->queAudioSource(NULL, MenuBackButtonSound());
		m_audio->clearListeners();
		messageToReturn = SceneMessage::eMainMenu;
		m_audio->stopMusic();
	}
	else if (selectedButton == exitToWindowsButton) {
		m_audio->queAudioSource(NULL, MenuBackButtonSound());
		messageToReturn = SceneMessage::eExit;
	}
	else if (selectedButton == restartButton) {
		m_audio->queAudioSource(NULL, MenuButtonClickSound());
		messageToReturn = SceneMessage::eGameSimulation;
		m_audio->stopMusic();
	}
}


void PauseMenu::selectMenuItem(Object3D *menuItem) {
	{
		ObjectPositionUpdater *upd = new ObjectPositionUpdater(menuItem, glm::vec3(0, 0, 10), .5);
		m_world->addObjectUpdater(upd);
	}
}

void PauseMenu::unselectMenuItem(Object3D *menuItem) {
	{
		ObjectPositionUpdater *upd = new ObjectPositionUpdater(menuItem, glm::vec3(0, 0, -10), .5);
		m_world->addObjectUpdater(upd);
	}
}

bool PauseMenu::simulateScene(double dt, SceneMessage &message)
{
	m_sceneGameTimeSeconds += dt;
	m_world->update(dt);
	if (messageToReturn != SceneMessage::eNone) {
		message.setTag(messageToReturn);
		if (messageToReturn == SceneMessage::eGameSimulation) {
			message.setPlayerTemplates(m_playerTemplates);
		}
		messageToReturn = SceneMessage::eNone;
		return true;
	}

	//music
	if (m_audio->getMusicFinished()) {
		stopMusic = true;
		m_audio->playMusic(CrysisTwoThemeSong(), 1);
	}

	//check gamepad stuff
	if (m_gamePad->checkConnection() && m_sceneGameTimeSeconds > 1)
	{

		if (m_gamePad->isPressed(GamePad::DPadUp)) {
			upPressed();
		}

		if (m_gamePad->isPressed(GamePad::DPadDown)) {
			downPressed();
		}

		if (m_gamePad->isPressed(GamePad::AButton)) {
			aPressed();
		}

		if (m_gamePad->isPressed(GamePad::BButton)) {
			m_audio->queAudioSource(NULL, MenuButtonClickSound());
			messageToReturn = SceneMessage::ePop;
		}

	}
	return false;
}

void PauseMenu::setupSceneLights() {

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
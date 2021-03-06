#include "EndingScene.h"
#include "Global\Assets.h"
#include "Global\Definitions.h"
#include "Libraries\freeglut\freeglut.h"
#include "Objects\TexturedObject2D.h"

EndingScene::EndingScene(std::vector<ControllableTemplate *> playerTemplates, Audio& audioHandle) : m_audio(audioHandle), m_players(playerTemplates), m_playersToReturn(playerTemplates)
{
	//set player number to show later
	for (int i = 0; i < playerTemplates.size(); i++) {
		m_players.at(i)->setPlayerNumber(i + 1);
	}
	sortPlayers();

	m_audio.clearListeners();
	m_audio.setMusicVolume(128);

	m_defaultSceneCamera->setLookAt(glm::vec3(0, 0, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->setPosition(glm::vec3(0, 0, 0));
		Audioable *au = new Audioable(audioHandle);
		Model3D *model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/finishScreen.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, false, false);
		map = new Object3D(1, au, p, a, r, NULL);
		m_world->addGameObject(map);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		a->setPosition(glm::vec3(0, 2, 0));
		a->setScale(glm::vec3(5, 4, 7));
		Audioable *au = new Audioable(audioHandle);
		Model3D *model = NULL;
		if (m_players.at(0)->getCarSelection() == Characters::CHARACTER_MEOW_MIX)
			model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Meowmix.obj"));
		else if (m_players.at(0)->getCarSelection() == Characters::CHARACTER_GARGANTULOUS)
			model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Gargantulous.obj"));
		else if (m_players.at(0)->getCarSelection() == Characters::CHARACTER_EXPLOSIVELY_DELICIOUS)
			model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/twisted1.obj"));
		model->setupVAOs();
		Renderable3D *r = new Renderable3D(model, true, true);
		car1 = new Object3D(1, au, p, a, r, NULL);
		m_world->addGameObject(car1);
	}

	{
		Physicable *p = new Physicable(NULL);
		Animatable *a = new Animatable();
		Audioable *au = new Audioable(audioHandle);
		Renderable3D *r = new Renderable3D(NULL);
		car1Manager = new EndingScreenCarInfoManager(1, au, p, a, r, NULL, car1, m_players.at(0)->getPlayerNumber());
		m_world->addGameObject(car1Manager);
	}

	if (m_players.size() >= 2) {
		{
			Physicable *p = new Physicable(NULL);
			Animatable *a = new Animatable();
			a->setPosition(glm::vec3(-7.4f, 0.4f, 3.6f));
			a->setScale(glm::vec3(5, 4, 7));
			Audioable *au = new Audioable(audioHandle);
			Model3D *model = NULL;
			if (m_players.at(1)->getCarSelection() == Characters::CHARACTER_MEOW_MIX)
				model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Meowmix.obj"));
			else if (m_players.at(1)->getCarSelection() == Characters::CHARACTER_GARGANTULOUS)
				model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Gargantulous.obj"));
			else if (m_players.at(1)->getCarSelection() == Characters::CHARACTER_EXPLOSIVELY_DELICIOUS)
				model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/twisted1.obj"));
			model->setupVAOs();
			Renderable3D *r = new Renderable3D(model, true, true);
			car2 = new Object3D(1, au, p, a, r, NULL);
			m_world->addGameObject(car2);
		}

		{
			Physicable *p = new Physicable(NULL);
			Animatable *a = new Animatable();
			Audioable *au = new Audioable(audioHandle);
			Renderable3D *r = new Renderable3D(NULL);
			car2Manager = new EndingScreenCarInfoManager(1, au, p, a, r, NULL, car2, m_players.at(1)->getPlayerNumber());
			m_world->addGameObject(car2Manager);
		}
	}

	if (m_players.size() >= 3) {
		{
			Physicable *p = new Physicable(NULL);
			Animatable *a = new Animatable();
			a->setPosition(glm::vec3(7.5f, -.4f, 3.63f));
			a->setScale(glm::vec3(5, 4, 7));
			Audioable *au = new Audioable(audioHandle);
			Model3D *model = NULL;
			if (m_players.at(2)->getCarSelection() == Characters::CHARACTER_MEOW_MIX)
				model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Meowmix.obj"));
			else if (m_players.at(2)->getCarSelection() == Characters::CHARACTER_GARGANTULOUS)
				model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/Gargantulous.obj"));
			else if (m_players.at(2)->getCarSelection() == Characters::CHARACTER_EXPLOSIVELY_DELICIOUS)
				model = static_cast<Model3D *>(Assets::loadObjFromDirectory("Assets/Models/twisted1.obj"));
			model->setupVAOs();
			Renderable3D *r = new Renderable3D(model, true, true);
			car3 = new Object3D(1, au, p, a, r, NULL);
			m_world->addGameObject(car3);
		}

		{
			Physicable *p = new Physicable(NULL);
			Animatable *a = new Animatable();
			Audioable *au = new Audioable(audioHandle);
			Renderable3D *r = new Renderable3D(NULL);
			car3Manager = new EndingScreenCarInfoManager(1, au, p, a, r, NULL, car3, m_players.at(2)->getPlayerNumber());
			m_world->addGameObject(car3Manager);
		}
	}

	{
		Animatable *a = new Animatable();
		a->setPosition(glm::vec3(0, glutGet(GLUT_WINDOW_HEIGHT), 0));
		Audioable *au = new Audioable(audioHandle);
		Renderable2D *r = new Renderable2D(NULL);
		scoreTable = new EndingScreenUIScoreTable(1, au, a, r, m_players);
		m_world->addGameObject(scoreTable);
	}

	{
		Animatable *a = new Animatable();
		a->setPosition(glm::vec3(0.8f, -0.7f, 0));
		a->setScale(glm::vec3(0.35f, 0.15f, 0));
		Audioable *au = new Audioable(audioHandle);
		Model2D *model;
		model = new Model2D(Assets::loadTextureFromDirectory("Assets/Textures/restart_x.png"));
		model->getTexture()->Load();
		Renderable2D *r = new Renderable2D(model);
		m_world->addGameObject(new TexturedObject2D(1, au, a, r));
	}

	{
		Animatable *a = new Animatable();
		a->setPosition(glm::vec3(0.8f, -0.9f, 0));
		a->setScale(glm::vec3(0.35f, 0.15f, 0));
		Audioable *au = new Audioable(audioHandle);
		Model2D *model;
		model = new Model2D(Assets::loadTextureFromDirectory("Assets/Textures/main_menu_a.png"));
		model->getTexture()->Load();
		Renderable2D *r = new Renderable2D(model);
		m_world->addGameObject(new TexturedObject2D(1, au, a, r));
	}

	setupSceneLights();
}


EndingScene::~EndingScene()
{
}

void EndingScene::sortPlayers() {
	for (int i = 0; i < m_players.size(); i++) {
		for (int j = 0; j < m_players.size() - 1; j++) {
			if (m_players.at(j)->getFinalScore() < m_players.at(j + 1)->getFinalScore()) {
				std::iter_swap(m_players.begin() + j, m_players.begin() + j + 1);
			}
		}
	}
}

bool EndingScene::simulateScene(double dt, SceneMessage &message)
{
	m_sceneGameTimeSeconds += dt;
	m_world->update(dt);

	//music
	if (m_audio.getMusicFinished()) {
		m_audio.playMusic(CrysisTwoThemeSong(), 1);
	}

	if (m_sceneGameTimeSeconds < 1)
		return false;

	for (ControllableTemplate *ct : m_players) {
		if (ct->getGamePad() == NULL)
			continue;

		if (ct->getGamePad()->isPressed(GamePad::AButton)) {
			m_audio.stopMusic();
			message.setTag(SceneMessage::eMainMenu);
			return true;
		}
		else if (ct->getGamePad()->isPressed(GamePad::XButton)) {
			m_audio.stopMusic();
			message.setTag(SceneMessage::eGameSimulation);
			message.setPlayerTemplates(m_playersToReturn);
			return true;
		}
	}

	return false;
}

void EndingScene::setupSceneLights() {

	{
		Animatable *anable = new Animatable();
		Light *light = new Light(1, anable);
		anable->setPosition(glm::vec3(0, -1.5, 1.70));
		light->colour = glm::vec3(0.8549, 0.6471, 0.1255);
		light->constant = 0.0f;
		light->linear = 0.0f;
		light->quad = 0.3f;
		light->cutoff = 500.0;
		m_world->addLightObject(light);
	}
	{
		Animatable *anable = new Animatable();
		Light *light = new Light(1, anable);
		anable->setPosition(glm::vec3(-7.6, -4.0, 3.9));
		light->colour = glm::vec3(0.2229, 0.3229, 0.80);
		light->constant = 0.0f;
		light->linear = 0.0f;
		light->quad = 0.3f;
		light->cutoff = 500.0;
		m_world->addLightObject(light);
	}
	{
		Animatable *anable = new Animatable();
		Light *light = new Light(1, anable);
		anable->setPosition(glm::vec3(7.7, -4.0, 4.0));
		light->colour = glm::vec3(0.9015, 0.45, 0.2);
		light->constant = 0.0f;
		light->linear = 0.0f;
		light->quad = 0.3f;
		light->cutoff = 500.0;
		m_world->addLightObject(light);
	}
}
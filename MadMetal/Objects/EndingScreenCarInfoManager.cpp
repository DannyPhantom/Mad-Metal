#include "EndingScreenCarInfoManager.h"
#include "Libraries\ftgl\FTGL\ftgl.h"
#include "Libraries\freeglut\freeglut.h"
#include <sstream>

EndingScreenCarInfoManager::EndingScreenCarInfoManager(long id, Audioable *aable, Physicable *pable, Animatable *anable, Renderable3D *rable, Animation *aniable, Object3D *car, int playerNumber) : Object3D(id, aable, pable, anable, rable, aniable)
{
	this->car = car;
	font = new FTPolygonFont("Assets/Fonts/OpenSans-Bold.ttf");
	fontSize = 1;
	font->FaceSize(fontSize);
	std::stringstream ss;
	ss << "Player " << playerNumber;
	this->name = ss.str();
}


EndingScreenCarInfoManager::~EndingScreenCarInfoManager()
{
	delete font;
}

bool EndingScreenCarInfoManager::draw(Renderer *renderer, Renderer::ShaderType type, int passNumber) {
	if (type != Renderer::ShaderType::SHADER_TYPE_NONE || passNumber > 1)
		return false;

	glMatrixMode(GL_PROJECTION);
	glm::mat4x4 proj = renderer->getProjectionMatrix();
	glLoadMatrixf(glm::value_ptr(proj));
	glMatrixMode(GL_MODELVIEW);
	glm::mat4x4 view = renderer->getViewMatrix();
	glLoadMatrixf(glm::value_ptr(view));
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);

	glColor3f(1.0, 1.0, 1.0);
	// Set the font size and render a small text.

	//FTPo
	font->Render(name.c_str(), -1, FTPoint(car->getAnimatablePos().x - 2, car->getAnimatablePos().y + 3, car->getAnimatablePos().z));

	glPopAttrib();

	return false;
}

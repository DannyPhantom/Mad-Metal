#pragma once

#include "Car.h"



class Gargantulous : public Car
{
protected:
	void unuseSuper();
public:
	Gargantulous(long id, DrivingStyle* style, PxVehicleDrive4W &car, Audioable *aable, Physicable *pable, Animatable *anable, Renderable3D *rable);

	~Gargantulous();

	void fire();
	void update(float dt);
	void useSuper();
private:

	
};


#pragma once
#include "DrivingStyle.h"

class DrivingStyleExplosivelyDelicious : public DrivingStyle
{
private:

public:
	DrivingStyleExplosivelyDelicious(PxMaterial *chassisMaterial, PxMaterial *wheelMaterial);
	virtual ~DrivingStyleExplosivelyDelicious();
};

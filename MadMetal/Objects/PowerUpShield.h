#pragma once
#include "PowerUpVolume.h"
#include "ObjectUpdaters\ObjectRotationUpdater.h"
class PowerUpShield : public PowerUpVolume
{
public:
	
	PowerUpShield(long id, Audioable *aable, Physicable *pable, Animatable *anable, Renderable3D *rable, Car * owner);
	virtual ~PowerUpShield();
	
private://members

private:

};
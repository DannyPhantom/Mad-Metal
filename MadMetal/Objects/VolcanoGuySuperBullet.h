#pragma once

#include "Bullet.h"

class VolcanoGuySuperBullet : public Bullet
{
public:
	VolcanoGuySuperBullet(long id, Audioable &aable, Physicable &pable, Animatable &anable, Renderable &rable, Audio* audio, Car *owner);
	~VolcanoGuySuperBullet();
};

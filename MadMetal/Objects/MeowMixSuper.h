#pragma once
#include "Object3D.h"
#include "PxQueryReport.h"
#include "Factory\GameFactory.h"

#define BEAM_DISTANCE 300
#define BEAM_RADIUS 3
class MeowMixSuper : public Object3D
{
public:
	MeowMixSuper(long id, Audioable *aable, Physicable *pable, Animatable *anable, Renderable3D *rable, Car *owner) : Object3D(id, aable, pable, anable, rable, NULL)
	{
		m_owner = owner;
		m_animatable->setScale(glm::vec3(BEAM_RADIUS, BEAM_RADIUS, BEAM_DISTANCE));
	}

	virtual ~MeowMixSuper()
	{

	}

	virtual void update(float dtMillis)
	{

		if (m_owner->getSuperDurationRemaining() <= 0)
		{
			setHasToBeDeleted(true);
			return;
		}
		
		PxVec3 initialPosition = m_owner->getCar().getRigidDynamicActor()->getGlobalPose().p;
		glm::vec3 forward = glm::normalize(m_owner->getForwardVector());
		PxVec3 newPosition = initialPosition + PxVec3(forward.x, forward.y, forward.z) * BEAM_DISTANCE /2;
		getActor().setGlobalPose(PxTransform(newPosition, m_owner->getCar().getRigidDynamicActor()->getGlobalPose().q));
		m_animatable->setRotation(glm::vec3(0, 0, 0));



		PxRaycastBuffer hit;              // [out] Sweep results
		PxQueryFilterData fd = PxQueryFilterData(PxQueryFlag::eDYNAMIC);
		PxVec3 rayDirection = PxVec3(forward.x, forward.y, forward.z);    // [in] normalized sweep direction
		initialPosition += rayDirection * 5;
		initialPosition.y += 1;
		GameFactory::instance()->sceneRayCast(initialPosition, rayDirection, BEAM_DISTANCE, hit, PxHitFlag::eDEFAULT, fd);
		
		if (hit.hasBlock)
		{
			
			if (hit.block.actor != NULL)
			{
				
				PxShape * shapes[1];
				hit.block.actor->getShapes(shapes, 1);
				//std::cout << shapes[0]->getSimulationFilterData().word0 << std::endl;
				switch (shapes[0]->getSimulationFilterData().word0)
				{
				case (2) : //COLLISION_FLAG_CHASSIS
					//std::cout << "hit a car" << std::endl;
					Car * car = static_cast<Car *>(GameFactory::instance()->getWorld().findObject(shapes[0]->getSimulationFilterData().word2));
					if (car->isAlive())
					{
						m_owner->addDamageDealt(car->getHealthRemaining());
						car->takeDamage(car->getHealthRemaining());
					}
				}
			}
			
			

		}
		
	}

private:
	Car * m_owner;
	
};
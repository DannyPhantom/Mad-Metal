#pragma once 
#include "Simulation\PhysicsManager.h"
#include "Objects\ObjectCreators\VehicleCreator.h"

class PhysicsFactory
{
public:
	enum PhysicalObjects {
		PHYSICAL_OBJECT_CAR,
		PHYSICAL_OBJECT_WALL,
		PHYSICAL_OBJECT_DRIVING_BOX,
	};

public:
	PhysicsFactory(){}
	~PhysicsFactory(){}

	PxRigidStatic* createDrivingBox(physx::PxMaterial* material, PxTransform position, PxGeometry *box)
	{
		//Add a plane to the scene.
		PxRigidStatic* collisionBox = PhysicsManager::getPhysicsInstance().createRigidStatic(position);
		collisionBox->createShape(*box, *material);

		//Get the plane shape so we can set query and simulation filter data.
		PxShape* shapes[1];
		collisionBox->getShapes(shapes, 1);

		//Set the query filter data of the ground plane so that the vehicle raycasts can hit the ground.
		physx::PxFilterData qryFilterData;
		setupDrivableSurface(qryFilterData);
		shapes[0]->setQueryFilterData(qryFilterData);

		//Set the simulation filter data of the ground plane so that it collides with the chassis of a vehicle but not the wheels.
		PxFilterData simFilterData;
		simFilterData.word0 = COLLISION_FLAG_GROUND;
		simFilterData.word1 = COLLISION_FLAG_GROUND_AGAINST;
		shapes[0]->setSimulationFilterData(simFilterData);

		return collisionBox;
	}

	PxBase *makePhysicsObject(PhysicalObjects actorToMake, PxTransform *pos, PxGeometry *geom, PxMaterial *material, DrivingStyle *style)
	{
		PxBase *toReturn = NULL;

		switch (actorToMake) {
		case PHYSICAL_OBJECT_WALL:
		{
			PxRigidStatic * wall = PhysicsManager::getPhysicsInstance().createRigidStatic(*pos);
			wall->createShape(*geom, *material);
			toReturn = wall;
			break;
		}
		case PHYSICAL_OBJECT_DRIVING_BOX:
		{
			PxRigidStatic * plane = createDrivingBox(material, *pos, geom);
			toReturn = plane;
			break;
		}
		case PHYSICAL_OBJECT_CAR:
		{
			VehicleCreator *vc = new VehicleCreator(&PhysicsManager::getPhysicsInstance(), &PhysicsManager::getCookingInstance());
			PxVehicleDrive4W *car = vc->create(style);
			PxTransform startTransform(PxVec3(0, 3 + (style->getChassisDimensions().y*0.5f + style->getWheelRadius() + 1.0f), 0), PxQuat(PxIdentity));
			car->getRigidDynamicActor()->setGlobalPose(startTransform);
			toReturn = car;
			break;
		}
		}

		return toReturn;
	}

	
private: //members
	//enum of physics objects to make

	
};
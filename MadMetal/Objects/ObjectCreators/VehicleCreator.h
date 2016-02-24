#pragma once

#include "SnippetVehicleCreate.h"
#include "SnippetVehicleTireFriction.h"
#include "SnippetVehicleRaycast.h"
#include "../DrivingStyle.h"

class VehicleCreator
{
private:
	PxPhysics* physics;
	PxCooking* cooking;
	void setupWheelsSimulationData (DrivingStyle* style, const PxVec3* wheelCenterActorOffsets, PxVehicleWheelsSimData* wheelsSimData);
	void computeWheelCenterActorOffsets4W(DrivingStyle *style, const PxF32 wheelFrontZ, const PxF32 wheelRearZ, PxVec3* wheelCentreOffsets);
public:
	VehicleCreator(PxPhysics *physics, PxCooking *cooking);
	~VehicleCreator();
	PxVehicleDrive4W* create(DrivingStyle* style);

};


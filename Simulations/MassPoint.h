#pragma once
#include "Simulator.h"


class MassPoint {
public:

	MassPoint(Vec3 newPosition, Vec3 newVelocity, Vec3 newForce, bool newFixed, Vec3 newTempForce);

	void setPostion(Vec3 newPostion);

	void setVelocity(Vec3 newVelocity);

	void setForce(Vec3 newForce);


	Vec3 position;
	Vec3 velocity;
	Vec3 force;
	Vec3 tempForce;
	bool isFixed;
};


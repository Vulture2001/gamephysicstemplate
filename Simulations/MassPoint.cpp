#include "MassPoint.h"

MassPoint::MassPoint(Vec3 newPosition, Vec3 newVelocity, Vec3 newForce, bool newFixed, Vec3 newTempForce) {
	this->position = newPosition;
	this->velocity = newVelocity;
	this->force = newForce;
	this->isFixed = newFixed;
	this->tempForce = newTempForce;
}

void MassPoint::setPostion(Vec3 newPostion) {
	this->position = newPostion;
}

void MassPoint::setVelocity(Vec3 newVelocity) {
	this->velocity = newVelocity;
}


void MassPoint::setForce(Vec3 newForce) {
	this->force = newForce;
}

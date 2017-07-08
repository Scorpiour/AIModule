#include "RigidBody.h"

using namespace std;

VirtualAttractivePoint::VirtualAttractivePoint():RigidBody(),position(Point2F(0,0)){ 
    this->id = RigidTypeID::RigidType_VirtualObject;
}

VirtualAttractivePoint::~VirtualAttractivePoint(){  }


float VirtualAttractivePoint::getX()const {  
	return position.x;
}
float VirtualAttractivePoint::getY()const {  
	return position.y;
}
void VirtualAttractivePoint::setSX(float) {  

}
void VirtualAttractivePoint::setSY(float) { 

}
float VirtualAttractivePoint::getSX()const { 
	return 0.f;
}
float VirtualAttractivePoint::getSY()const { 
	return 0.f;
}
float VirtualAttractivePoint::getOuterRadius()const {
	return 0.f;
}
float VirtualAttractivePoint::getInnerRadius()const{
    return 0.f;
}

void VirtualAttractivePoint::move(double dt) {

}

bool VirtualAttractivePoint::calculateVirtualForce(RigidBody* dest,Point2F& result,double dt) {

	return false;
}

bool VirtualAttractivePoint::calculateForce(RigidBody* dest,Point2F& result,double dt) { 

	if((dest->getID() != RigidTypeID::RigidType_Robot)&&(dest->getID() != RigidTypeID::RigidType_Keeper)){
		result.x = 0.f;
		result.y = 0.f;
		return true;
	}

	float dx = position.x - dest->getX();
	float dy = position.y - dest->getY();

	//float mr = dest->getRadius();

	float arc = GlobalVariables::Atan2(dy,dx);

	float distance = sqrt(dx*dx + dy*dy);

	//float value = (distance > mr) ? 10.f:0.f;
    float value = distance  + 500;
	result.x = value * cos(arc);
	result.y = value * sin(arc);

	return true;
}

bool VirtualAttractivePoint::checkLOS(const Point2F& p1, const Point2F& p2){
	return true;
}

float VirtualAttractivePoint::calculateDistance(const Point2F& point, float rad) {

	if(!activeDistance){
		return FLT_MAX;
	}

	float dx = point.x - position.x;
	float dy = point.y - position.y;

	return sqrt(dx*dx + dy*dy);
}

void VirtualAttractivePoint::setPosition(const Point2F& pos){
	this->position = pos;
}

const Point2F& VirtualAttractivePoint::getPosition()const{
	return this->position;
}

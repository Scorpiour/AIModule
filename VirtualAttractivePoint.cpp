#include "RigidBody.h"

using namespace std;

VirtualAttractivePoint::VirtualAttractivePoint():RigidBody(),position(Point2F(0,0)){ 
	this->id = -4;
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
float VirtualAttractivePoint::getRadius()const {
	return 0.f;
}

void VirtualAttractivePoint::move(double dt) {

}

bool VirtualAttractivePoint::calculateForce(RigidBody* dest,Point2F& result,double dt) { 

	if(dest->getID() != 1){
		result.x = 0.f;
		result.y = 0.f;
		return true;
	}

	float dx = position.x - dest->getX();
	float dy = position.y - dest->getY();

	float mr = dest->getRadius();

	float arc = atan2(dy,dx);

	float distance = sqrt(dx*dx + dy*dy);

	//float value = (distance > mr) ? 10.f:0.f;
	float value = 200.f;
	result.x = value * cos(arc);
	result.y = value * sin(arc);

	return true;
}

float VirtualAttractivePoint::calculateDistance(const Point2F& point, float rad) {

	float dx = point.x - position.x;
	float dy = point.y - position.y;

	return sqrt(dx*dx + dy*dy);
}

void VirtualAttractivePoint::setPosition(const Point2F& pos){
	this->position = pos;
}
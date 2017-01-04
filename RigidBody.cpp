#include "RigidBody.h"

using namespace std;

RigidController RigidController::controller;

const float RigidBody::speedlimit = 0.2f;


RigidController& RigidController::getInstance(){
	return RigidController::controller;
}

RigidController::RigidController(){}

RigidController::~RigidController(){}


void RigidController::addToController(RigidBody* body){
	this->rigids.insert(body);
}

void RigidController::removeFromController(RigidBody* body){
	this->rigids.erase(body);
}

void RigidController::calculateVirtualForce(RigidBody* dest, Point2F& result, double dt){
	Point2F netForce;
	netForce.x = 0.f;
	netForce.y = 0.f;
	for(auto& src : this->rigids){
		if(src == dest){
			continue;
		}
		if(src->getID() == -4){
			continue;
		}

		Point2F force;
		force.x = 0.f;
		force.y = 0.f;

		if(src->calculateVirtualForce(dest,force,dt)){
			netForce.x += force.x;
			netForce.y += force.y;
		}
	}
	result.x = netForce.x;
	result.y = netForce.y;
}

void RigidController::calculateCollisionForce(double dt){

	for(auto& agent : this->rigids){

		Point2F netForce;
		netForce.x = 0.f;
		netForce.y = 0.f;

		for(auto& src : this->rigids){
			if(src == agent){
				continue;
			}
			if(src->getID() == -4){
				continue;
			}

			Point2F force;
			force.x = 0.f;
			force.y = 0.f;

			if(src->calculateForce(agent,force,dt)){
				netForce.x += force.x;
				netForce.y += force.y;
			}
		}

		agent->setCollisionForce(netForce.x,netForce.y);
	}

	for(auto& agent : this->rigids){
		agent->move(dt);
	}
}

float RigidController::calculateDistanceLevel(const Point2F& pt){
	float dv = 50000;
	
	for(auto& target : this->rigids){
		
		
		int id = target->getID();
		if(/*(id == -2)||(id == 0)*/ id != -4){
			float distance = target->calculateDistance(pt,0);

			if(distance < dv){
				dv = distance;
			}
		}
	}

	return dv;
}

int RigidBody::getID()const{
	return this->id;
}

void RigidBody::setID(int _id){
	this->id = _id;
}

void RigidBody::setMovable(bool isMovable){
	this->movable = isMovable;
}

bool RigidBody::isMovable()const{
	return this->movable;
}

void RigidBody::setCollisionForce(float fx,float fy){
	this->collisionForce.x = fx;
	this->collisionForce.y = fy;
}

void RigidBody::setForceFunc(std::function<bool(RigidBody*,Point2F&)> _forceFunc){
	this->forceFunc = _forceFunc;
}

bool RigidBody::processForceFunc(RigidBody* bdy,Point2F& force){
	if(this->forceFunc != nullptr){
		return forceFunc(bdy,force);
	}
	return false;
}

void RigidBody::removeForceFunc(){
	this->forceFunc = nullptr;
}

void RigidBody::activeDistanceCalculate(bool b){
	this->activeDistance = b;
}

RigidBody::RigidBody(){
	mass = 0;
	collisionForce.x = 0;
	collisionForce.y = 0;
	sx = 0;
	sy = 0;
	forceFunc = nullptr;
	id = -1;
	movable = true;
	activeDistance = true;

	RigidController::getInstance().addToController(this);
}

RigidBody::~RigidBody(){

}

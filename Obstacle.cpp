#include "Sprite.h"

using namespace std;

Obstacle::Obstacle():Robot(){
    id = RigidTypeID::RigidType_Obstacle;
    this->activeDistance = true;
}

Obstacle::~Obstacle(){

}

void Obstacle::setScale(glm::vec3 _scale){

	Sprite::setScale(_scale);
	float scalex = this->scaleMatrix[0];
	float scalez = this->scaleMatrix[10];

	scalex *= 10.f;
	scalez *= 10.f;

	this->width = scalez;
	this->length = scalex;

	this->radius = sqrt(scalex*scalex + scalez*scalez)/2;
}

void Obstacle::move(double dt){
	if(!movable){
		return;
	}

	float ax = collisionForce.x / mass;
	float ay = collisionForce.y / mass;

	float dx = 0.5 * ax * dt;
	float dy = 0.5 * ay * dt;

	this->position.x += dx;
	this->position.z += dy;

	//float arc = GlobalVariables::Atan2(ay,ax);
	//this->setAngle(glm::vec3(0,arc,0));
}

bool Obstacle::checkLOS(const Point2F& p1, const Point2F& p2){
	if(!this->isEnable){
		return true;
	}


	float angle = this->angles.y;
	float length = this->scaleMatrix[0] * 10;
	float height = this->scaleMatrix[10] * 10;
	float x = this->getX();
	float y = this->getY();
	Point2F pivot(x,y);
	float r = sqrt(length*length + height*height)/4;

	if( GlobalVariables::distancePointToLine(pivot,p1,p2) > r){
		return true;
	}

	float arc = -angle; //rotate them back;

	Point2F np1(GlobalVariables::rotatePointByArcPivot(p1,pivot,arc));
	Point2F np2(GlobalVariables::rotatePointByArcPivot(p2,pivot,arc));

	Point2F rpmin;
	Point2F rpmax;

	float rl = length / 2;
	float rh = height / 2;

	float ca = cos(angle);
	float sa = sin(angle);

	rpmin.x = x-rl*ca;
	rpmin.y = y-rl*sa;
	rpmax.x = x+rl*ca;
	rpmax.y = y+rl*sa;

	rpmin = GlobalVariables::rotatePointByArcPivot(rpmin,pivot,arc);
	rpmax = GlobalVariables::rotatePointByArcPivot(rpmax,pivot,arc);

	if(rpmin.x > rpmax.x){
		Point2F tmp(rpmax);
		rpmax = rpmin;
		rpmin = tmp;
	}

	rpmin.y -= rh;
	rpmax.y += rh;

	return !GlobalVariables::Cohen_Sutherland(np1,np2,rpmin,rpmax);

}

bool Obstacle::calculateVirtualForce(RigidBody* dest, Point2F& result,double dt){
	if(!this->isEnable){
		result.x = 0.f;
		result.y = 0.f;
		return 0.f;
	}

    /*
    result.x = 0;
    result.y = 0;
    return true;
    */
    
    if(dest->getID() == RigidTypeID::RigidType_Obstacle){
		return false;
	}

	//float ix = this->getX();
	//float iy = this->getY();
	//float ir = this->getRadius();
	
	//float tx = dest->getX();
	//float ty = dest->getY();
	//float tr = dest->getRadius();

	//RigidTypeID id = dest->getID();
    
    
    //float dx = tx - ix;
    //float dy = ty - iy;
    
    /*
	float dx = tx - ix;
	float dy = ty - iy;

	float distance = sqrt(dy*dy + dx*dx);
	
	if(distance > ir + tr){
		result.x = 0;
		result.y = 0;
		return true;
	}

	float phi = GlobalVariables::Atan2(dy,dx);
	float theta = this->angles.y;

	while(phi < 0){
		phi += 2*M_PI;
	}
	while(theta <0){
		theta += 2*M_PI;
	}
	while(phi < theta){
		phi += 2*M_PI;
	}

	float delta = phi - theta;
	float denom = GlobalVariables::Atan2(length,width);
	
	Point2F forceDirection;
	forceDirection.x = 0;
	forceDirection.y = 0;
	float r = 0;

	if(delta < denom){
		forceDirection.x = cos(theta);
		forceDirection.y = sin(theta);
		distance *= cos(delta);
		r = width/2;
		distance -= r;
	}else if(delta == denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else if(delta < M_PI-denom){
		forceDirection.x = -sin(theta);
		forceDirection.y = cos(theta);
		distance *= sin(delta);
		r = length/2;
		distance -= r;
	}else if(delta == M_PI-denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else if(delta < M_PI+denom){
		forceDirection.x = -cos(theta);
		forceDirection.y = -sin(theta);
		distance *= cos(delta);
		r = width/2;
		distance -= r;
	}else if(delta == M_PI+denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else if(delta < 2*M_PI-denom){
		forceDirection.x = sin(theta);
		forceDirection.y = -cos(theta);
		distance *= sin(delta);
		r = length/2;
		distance -= r;
	}else if(delta == 2*M_PI-denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else{
		forceDirection.x = cos(theta);
		forceDirection.y = sin(theta);
		distance *= cos(delta);
		r = width/2;
		distance -= r;
	}

	distance = abs(distance);

	if(distance > r + tr + 20){
		result.x = 0;
		result.y = 0;
		return false;
	}

	float value = 1500.f/(1+exp(distance-20));//1000.f/distance;

	float arc = GlobalVariables::Atan2(forceDirection.y, forceDirection.x);

	result.x = value*cos(arc);
	result.y = value*sin(arc);
	*/
	return true;

}

bool Obstacle::calculateForce(RigidBody* dest,Point2F& result,double dt){

	if(!this->isEnable){
		result.x = 0.f;
		result.y = 0.f;
		return 0.f;
	}

	
    if(dest->getID() == RigidTypeID::RigidType_Obstacle){
		return false;
	}

	float ix = this->getX();
	float iy = this->getY();
	//float ir = this->getRadius();
	
	float tx = dest->getX();
	float ty = dest->getY();
	//float tr = dest->getRadius();
    
	//RigidTypeID id = dest->getID();
    
    float dx = tx - ix;
    float dy = ty - iy;
    
    float phi = GlobalVariables::Atan2(dy,dx);
    float theta = this->angles.y;//GlobalVariables::Atan2(this->getSY(),this->getSX());
    
    while(theta < 0){
        theta += 2*M_PI;
    }
    while(theta > 2*M_PI){
        theta -= 2*M_PI;
    }
    
    while(phi < 0){
        phi += 2*M_PI;
    }
    while(phi > 2*M_PI){
        phi -= 2*M_PI;
    }
    
    float psi = phi - theta;
    
    float d1 = sqrt(dy*dy + dx*dx);
    float d2 = fabs(d1 * cos(psi)) - length/2;
    float d3 = fabs(d1 * sin(psi)) - width/2;
    
    float dist = 0;
    
    bool inside = true;
    
    if(d2 < 0 && d3 < 0){
        dist = 0;   //inside
    }else if(d2 < 0){
        dist = d3;
        inside = false;
    }else if(d3 < 0){
        dist = d2;
        inside = false;
    }else{
        dist = sqrt(d2*d2 + d3*d3);
        inside = false;
    }
    
    if(!inside){
        
        result.x = 0;
        result.y = 0;
        
        return true;
    }
    
    //inside
    
    float eta = GlobalVariables::Atan2(width,length);
    while(eta < 0){
        eta += 2*M_PI;
    }
    while(eta > 2*M_PI){
        eta -= 2*M_PI;
    }
    
    result.x = 0;
    result.y = 0;
    
    float value = 1000.f;
    
    if(psi < eta){
        result.x = value * cos(theta);
        result.y = value * sin(theta);
    }else if(psi < M_PI - eta){
        result.x = value * cos(theta + 0.5*M_PI);
        result.y = value * sin(theta + 0.5*M_PI);
    }else if (psi < M_PI + eta){
        result.x = value * cos(theta + M_PI);
        result.y = value * sin(theta + M_PI);
    }else if (psi < 2*M_PI - eta){
        result.x = value * cos(theta + 1.5*M_PI);
        result.y = value * sin(theta + 1.5*M_PI);
    }else{
        result.x = value * cos(theta);
        result.y = value * sin(theta);
    }
    
    /*
	float dx = tx - ix;
	float dy = ty - iy;

	float distance = sqrt(dy*dy + dx*dx);
	
	
	if(distance > ir + tr){
		result.x = 0;
		result.y = 0;
		return true;
	}

	float phi = GlobalVariables::Atan2(dy,dx);
	float theta = this->angles.y;

	while(phi < 0){
		phi += 2*M_PI;
	}
	while(theta <0){
		theta += 2*M_PI;
	}
	while(phi < theta){
		phi += 2*M_PI;
	}

	float delta = phi - theta;
	float denom = GlobalVariables::Atan2(length,width);
	
	Point2F forceDirection;
	forceDirection.x = 0;
	forceDirection.y = 0;
	float r = 0;

	if(delta < denom){
		forceDirection.x = cos(theta);
		forceDirection.y = sin(theta);
		distance *= cos(delta);
		r = width/2;
		distance -= r;
	}else if(delta == denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else if(delta < M_PI-denom){
		forceDirection.x = -sin(theta);
		forceDirection.y = cos(theta);
		distance *= sin(delta);
		r = length/2;
		distance -= r;
	}else if(delta == M_PI-denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else if(delta < M_PI+denom){
		forceDirection.x = -cos(theta);
		forceDirection.y = -sin(theta);
		distance *= cos(delta);
		r = width/2;
		distance -= r;
	}else if(delta == M_PI+denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else if(delta < 2*M_PI-denom){
		forceDirection.x = sin(theta);
		forceDirection.y = -cos(theta);
		distance *= sin(delta);
		r = length/2;
		distance -= r;
	}else if(delta == 2*M_PI-denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
		distance -= r;
	}else{
		forceDirection.x = cos(theta);
		forceDirection.y = sin(theta);
		distance *= cos(delta);
		r = width/2;
		distance -= r;
	}

	distance = abs(distance);
	float value = 1000.f;
	float arc = GlobalVariables::Atan2(forceDirection.y, forceDirection.x);

	result.x = value*cos(arc);
	result.y = value*sin(arc);

	
	if(distance > r + tr){
		result.x = 0;
		result.y = 0;
	}

     */
	/*
	if(id == 1){
		dest->setForceFunc([=](RigidBody* rbdy,Point2F& force)->bool{
			auto robot = dynamic_cast<Robot*>(rbdy);
			robot->setDistanceHeuristic(distance);
			return true;
		});

		dest->processForceFunc(dest,result);

		dest->removeForceFunc();
	}*/

	return true;
}

float Obstacle::calculateDistance(const Point2F& point,float rad){
	if(!this->isEnable){
		return FLT_MAX;
	}
	if(!this->activeDistance){ //wtf???
		return FLT_MAX;
	}
	
    float ix = this->getX();
    float iy = this->getY();
    //float ir = this->getRadius();
    
    float tx = point.x;
    float ty = point.y;
    float tr = rad;
    
    //int id = dest->getID();
    
    float dx = tx - ix;
    float dy = ty - iy;
    
    float phi = GlobalVariables::Atan2(dy,dx);
    float theta = this->angles.y;//GlobalVariables::Atan2(this->getSY(),this->getSX());
    
    while(theta < 0){
        theta += 2*M_PI;
    }
    while(theta > 2*M_PI){
        theta -= 2*M_PI;
    }
    
    while(phi < 0){
        phi += 2*M_PI;
    }
    while(phi > 2*M_PI){
        phi -= 2*M_PI;
    }
    
    float psi = phi - theta;
    
    float d1 = sqrt(dy*dy + dx*dx);
    float d2 = fabs(d1 * cos(psi)) - length/2;
    float d3 = fabs(d1 * sin(psi)) - width/2;
    
    float dist = 0;
    
    bool inside = true;
    
    if(d2 < 0 && d3 < 0){
        dist = 0;   //inside
    }else if(d2 < 0){
        dist = d3;
        inside = false;
    }else if(d3 < 0){
        dist = d2;
        inside = false;
    }else{
        dist = sqrt(d2*d2 + d3*d3);
        inside = false;
    }
    
    if(inside){
        return -1.f;
    }
    
    return dist - tr;
    
    /*
     float phi = GlobalVariables::Atan2(dy,dx);
     float theta = this->angles.y;

	while(phi < 0){
		phi += 2*M_PI;
	}
	while(theta <0){
		theta += 2*M_PI;
	}
	while(phi < theta){
		phi += 2*M_PI;
	}

	float delta = phi - theta;
	float denom = GlobalVariables::Atan2(length,width);
	
	Point2F forceDirection;
	forceDirection.x = 0;
	forceDirection.y = 0;
	float r = 0;

	if(delta < denom){
		forceDirection.x = cos(theta);
		forceDirection.y = sin(theta);
		distance *= cos(delta);
		r = width/2;
	}else if(delta == denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
	}else if(delta < M_PI-denom){
		forceDirection.x = -sin(theta);
		forceDirection.y = cos(theta);
		distance *= sin(delta);
		r = length/2;
	}else if(delta == M_PI-denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
	}else if(delta < M_PI+denom){
		forceDirection.x = -cos(theta);
		forceDirection.y = -sin(theta);
		distance *= cos(delta);
		r = width/2;
	}else if(delta == M_PI+denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
	}else if(delta < 2*M_PI-denom){
		forceDirection.x = sin(theta);
		forceDirection.y = -cos(theta);
		distance *= sin(delta);
		r = length/2;
	}else if(delta == 2*M_PI-denom){
		forceDirection.x = cos(phi);
		forceDirection.y = sin(phi);
		r = ir;
	}else{
		forceDirection.x = cos(theta);
		forceDirection.y = sin(theta);
		distance *= cos(delta);
		r = width/2;
	}

	distance = abs(distance);
	
	return distance - r;
     */
    
    
}

float Obstacle::calculateDistance(RigidBody* dest){
    if(!this->isEnable){
        return FLT_MAX;
    }
    if(!this->activeDistance){
        return FLT_MAX;
    }
    
    float ix = this->getX();
    float iy = this->getY();
    //float ir = this->getRadius();
    
    float tx = dest->getX();
    float ty = dest->getY();;
    float tr = dest->getRadius();
    
    //int id = dest->getID();
    
    float dx = tx - ix;
    float dy = ty - iy;
    
    float phi = GlobalVariables::Atan2(dy,dx);
    float theta = this->angles.y;//GlobalVariables::Atan2(this->getSY(),this->getSX());
    
    while(theta < 0){
        theta += 2*M_PI;
    }
    while(theta > 2*M_PI){
        theta -= 2*M_PI;
    }
    
    while(phi < 0){
        phi += 2*M_PI;
    }
    while(phi > 2*M_PI){
        phi -= 2*M_PI;
    }
    
    float psi = phi - theta;
    
    float d1 = sqrt(dy*dy + dx*dx);
    float d2 = fabs(d1 * cos(psi)) - length/2;
    float d3 = fabs(d1 * sin(psi)) - width/2;
    
    float dist = 0;
    
    bool inside = true;
    
    if(d2 < 0 && d3 < 0){
        dist = -1;   //inside
    }else if(d2 < 0){
        dist = d3;
        inside = false;
    }else if(d3 < 0){
        dist = d2;
        inside = false;
    }else{
        dist = sqrt(d2*d2 + d3*d3);
        inside = false;
    }
    
    if(inside){
        return -1.f;
    }
    
    return dist - tr;
    
    
	
}

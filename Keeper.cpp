#include "Sprite.h"


Keeper::Keeper():Robot(){
    this->id = RigidTypeID::RigidType_Keeper;
    this->activeDistance = true;
    
    this->currentStatus = KeeperStatus::Keeper_Parking;
}

Keeper::~Keeper(){
    
}

void Keeper::move(double dt){
    Robot::move(dt);
}

void Keeper::setScale(glm::vec3 _scale){
    Robot::setScale(_scale);
}

bool Keeper::calculateForce(RigidBody* dest,Point2F& result,double dt){
    
	assert(this->path != nullptr);
	assert(this->pTargetPoint != nullptr);

  float ix = this->getX();
	float iy = this->getY();
	float ir = this->getRadius();
	
	float tx = dest->getX();
	float ty = dest->getY();
	float tr = dest->getRadius();

	RigidTypeID id = dest->getID();
	

	float dx = tx - ix;
	float dy = ty - iy;

	float distance = sqrt(dy*dy + dx*dx);

	float phi = atan2(dy,dx);
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
	float denom = atan2(length,width);
	
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
	float value = 100.f;
	float arc = atan2(forceDirection.y, forceDirection.x);

	result.x = value*cos(arc);
	result.y = value*sin(arc);

	
	if(distance > r + tr){
		result.x = 0;
		result.y = 0;
	}

	if(result){
		float cspeed = sqrt(sx*sx + sy*sy);
		dest->forceAccel(cspeed*2);
	}
    
	bool inside = (distance < r);

	do{

		if(id == RigidTypeID::RigidType_Ball){
        
			Point2F ballpos;
			ballpos.x = tx;
			ballpos.y = ty;
        
			auto ballPosFunc = [](const Point2F& ballpos)->BallPosStatus{
            
				BallPosStatus ret = BallPosStatus::BallPos_Inside;
            
				if((ballpos.y < 40) && (ballpos.y > -40)){
                
					if(ballpos.x > 110){
						ret = BallPosStatus::BallPos_Inside;
					}else if(ballpos.x > 85){
						ret = BallPosStatus::BallPos_Near;
					}else if(ballpos.x > 0){
						ret = BallPosStatus::BallPos_Middle;
					}else{
						ret = BallPosStatus::BallPos_Far;
					}
                
				}else if(ballpos.x > 0){
					ret = BallPosStatus::BallPos_Middle;
				}else{
					ret = BallPosStatus::BallPos_Far;
				}
            
				return ret;
			};
        
			BallPosStatus ballStatus = ballPosFunc(ballpos);
			KeeperStatus nextStatus = KeeperStatus::Keeper_Waiting;
        
			switch(currentStatus){
				case KeeperStatus::Keeper_Parking:{
					if( ballStatus & BallPosStatus::BallPos_Dangerous){
						nextStatus = KeeperStatus::Keeper_Persuing;
					}else{
						Point2F parkpos;
						parkpos.y = 0;
						parkpos.x = 110;
                    
						dx = parkpos.x - ix;
						dy = parkpos.y - iy;
                    
						float parkdist = sqrt(dx*dx + dy*dy);
                    
						if(parkdist < ir+3){
							nextStatus = KeeperStatus::Keeper_Waiting;
						}else{
							nextStatus = KeeperStatus::Keeper_Parking;
						}
					}
            
				}break;
            
				case KeeperStatus::Keeper_Waiting:{
					if(ballStatus == BallPosStatus::BallPos_Near){
						nextStatus = KeeperStatus::Keeper_Persuing;
					}else if(ballStatus == BallPosStatus::BallPos_Middle){
						nextStatus = KeeperStatus::Keeper_Blocking;
					}else{
						nextStatus = KeeperStatus::Keeper_Waiting;
					}
				}break;
            
				case KeeperStatus::Keeper_Blocking:{
					if(ballStatus & BallPosStatus::BallPos_Dangerous){
						nextStatus = KeeperStatus::Keeper_Blocking;
					}else{
						nextStatus = KeeperStatus::Keeper_Parking;
					}
				}break;
            
				case KeeperStatus::Keeper_Persuing:{
					if(!inside && distance < 10){
						nextStatus = KeeperStatus::Keeper_Blocking;
					}else if(ballStatus == BallPosStatus::BallPos_Far){
						nextStatus = KeeperStatus::Keeper_Parking;
					}else {
						nextStatus = KeeperStatus::Keeper_Blocking;
					}
				}break;
			}
		
			if(currentStatus == nextStatus){
				currentStatus = nextStatus;
				if(this->path->size() > 0){
					break;
				}
			}
			
			//do replaning

			switch(nextStatus){
				case KeeperStatus::Keeper_Blocking:{

					}break;
				case KeeperStatus::Keeper_Parking:{
						
					}break;
				case KeeperStatus::Keeper_Persuing:{
					
					}break;
				case KeeperStatus::Keeper_Waiting:{

					}break;
			}

			currentStatus = nextStatus;


		}

	}while(false);

	//Calculate Force Here

	this->pTargetPoint->calculateForce(this,this->virtualForce,dt);
	
    //return Robot::calculateForce(dest,result,dt);
    
    return true;
}

bool Keeper::calculateVirtualForce(RigidBody* dest, Point2F& result,double dt){
    
    return Robot::calculateVirtualForce(dest,result,dt);
}

float Keeper::calculateDistance(RigidBody* dest){
    Point2F pt;
    pt.x = dest->getX();
    pt.y = dest->getY();
    float rad = dest->getRadius();
    
    return Robot::calculateDistance(pt,rad);
}

float Keeper::calculateDistance(const Point2F& point,float rad){
    
    return Robot::calculateDistance(point,rad);
}

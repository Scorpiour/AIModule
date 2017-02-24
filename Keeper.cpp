#include "Sprite.h"


Keeper::Keeper():Robot(){
    this->id = RigidTypeID::RigidType_Keeper;
    this->activeDistance = false;
    
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
    
    float ix = this->getX();
    float iy = this->getY();
    float ir = this->getRadius();
    
    float tx = dest->getX();
    float ty = dest->getY();
    float tr = dest->getRadius();
    
    RigidTypeID id = dest->getID();
    
    
    float dx = tx - ix;
    float dy = ty - iy;
    
    float phi = atan2(dy,dx);
    float theta = this->angles.y;//atan2(this->getSY(),this->getSX());
    
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
        
        
    }else{
    
    //inside
    
        float eta = atan2(width,length);
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
    }
    
    
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
					if(!inside && dist < 10){
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
				break;
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

#include "Sprite.h"

using namespace std;

Keeper::Keeper():Robot(){
    this->id = RigidTypeID::RigidType_Keeper;
    this->activeDistance = true;
    
    this->currentStatus = KeeperStatus::Keeper_Parking;
}

Keeper::~Keeper(){
    
}

void Keeper::move(double dt){
	if(this->pts != nullptr){
		Point2F pos;
		pos.x = this->position.x;
		pos.y = this->position.z;
        
		this->pts->addPoint(pos);
	}
    
    
	if(!movable){
		return;
	}

	if(currentStatus == KeeperStatus::Keeper_Waiting){
		return;
	}

	if(!collisionForce){
		inTouch = false;
	}else if(!inTouch){
		inTouch = true;
		touchCount++;
		cout<<"Touch Count : "<<touchCount<<endl;
	}


	float ax,ay;
	
	if(collisionForce){
		ax = collisionForce.x / mass;
		ay = collisionForce.y / mass;
	}else{
		ax = virtualForce.x / mass;
		ay = virtualForce.y / mass;
	}

	float baseSpeed = maxSpeed;

	if(collisionForce){

		baseSpeed = min(sqrt(sx*sx + sy*sy),maxSpeed);

		sx = 0;
		sy = 0;
	}
    

	sx += ax * dt;
	sy += ay * dt;

	float s = sqrt(sx*sx + sy*sy);

	float arc = GlobalVariables::Atan2(sy,sx);

	if(nextVelo >= 0.f){
		sx = nextVelo * cos(arc);
		sy = nextVelo * sin(arc);
		nextVelo = -1.f;
	}else if(s > baseSpeed){
		sx = baseSpeed * cos(arc);
		sy = baseSpeed * sin(arc);
		
	}


	float dx = sx + 0.5*dt*ax;
	float dy = sy + 0.5*dt*ay;


	this->position.x += dx;
	this->position.z += dy;

	this->setAngle(glm::vec3(0,arc,0));
}

void Keeper::setScale(glm::vec3 _scale){
    Robot::setScale(_scale);
}

bool Keeper::calculateForce(RigidBody* dest,Point2F& result,double dt){
    
	assert(this->path != nullptr);
	assert(this->pTargetPoint != nullptr);

	this->setMovable(true);

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
	float value = 100.f;
	float arc = GlobalVariables::Atan2(forceDirection.y, forceDirection.x);

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
            
				if(abs(ballpos.y)<40){
                
					if(ballpos.x > 110){
						ret = BallPosStatus::BallPos_Inside;
					}else if(ballpos.x > 85){
						ret = BallPosStatus::BallPos_Near;
					}else if(ballpos.x > 55){
						ret = BallPosStatus::BallPos_Middle;
					}else{
						ret = BallPosStatus::BallPos_Far;
					}
                
				}else if(ballpos.x > 55 && abs(ballpos.y) < 55){
					ret = BallPosStatus::BallPos_Middle;
				}else{
					ret = BallPosStatus::BallPos_Far;
				}
            
				return ret;
			};
        
			BallPosStatus ballStatus = ballPosFunc(ballpos);
			KeeperStatus nextStatus = KeeperStatus::Keeper_Waiting;
        

			Point2F targetPos = ballpos;

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
						cout<<"Keeper : Switch from Waiting to Persuing"<<endl;
					}else if(ballStatus == BallPosStatus::BallPos_Middle){
						nextStatus = KeeperStatus::Keeper_Blocking;
						cout<<"Keeper : Switch from Waiting to Blocking"<<endl;
					}else{
						nextStatus = KeeperStatus::Keeper_Waiting;
					}
				}break;
            
				case KeeperStatus::Keeper_Blocking:{
					if(ballStatus & BallPosStatus::BallPos_Dangerous){
						nextStatus = KeeperStatus::Keeper_Blocking;
					}else{
						nextStatus = KeeperStatus::Keeper_Parking;
						cout<<"Keeper : Switch from Bloking to Parking"<<endl;
					}
				}break;
            
				case KeeperStatus::Keeper_Persuing:{
					if(!inside && distance < 10){
						nextStatus = KeeperStatus::Keeper_Blocking;
						cout<<"Keeper : Switch from Persuing to Blocking"<<endl;
					}else if(ballStatus == BallPosStatus::BallPos_Far){
						nextStatus = KeeperStatus::Keeper_Parking;
						cout<<"Keeper : Switch from Persuing to Parking"<<endl;
					}else {
						nextStatus = KeeperStatus::Keeper_Blocking;
						cout<<"Keeper : Switch from Persuing to Blocking"<<endl;
					}
				}break;
			}
		
			if(currentStatus == nextStatus){
				
				if(this->path->size() > 0){

					Point2F nextTarget = this->path->getBackPoint();
					nextTarget.x *= 10.f;
					nextTarget.y *= 10.f;
					this->pTargetPoint->setPosition(nextTarget);
					this->path->removeBackPoint();

					break;
				}
			}

			//do replaning

			bool doReplan = true;;

			switch(nextStatus){
				case KeeperStatus::Keeper_Blocking:{
					//Calculate Blocking Point


					//This is to find out the block pos, but seems not that really needed
					//But it is not works well
					/*
					ix = this->getX();
					iy = this->getY();

					tx = ballpos.x;
					ty = ballpos.y;

					float rx = 110;
					float ry = 0;

					float a,b,c;

					a = ry - ty;
					b = tx - rx;

					c = ty * (rx - tx) + tx * (ty - ry);

					if( a==0 && b == 0){
						targetPos.x = ix;
						targetPos.y = ty;
					}else{

						float denom = 1/sqrt(a*a+b*b);
						float dist = abs(a*ix + b*iy + c)*denom;

						targetPos.x = (b*b*ix - a*b*iy - a*c)*denom*denom;
						targetPos.y = (a*a*iy - a*b*ix - b*c)*denom*denom;
					}
					*/

					}break;
				case KeeperStatus::Keeper_Parking:{
						targetPos.x = 110;
						targetPos.y = 0;
					}break;
				case KeeperStatus::Keeper_Persuing:{
					
					}break;
				case KeeperStatus::Keeper_Waiting:{
						this->setSX(0);
						this->setSY(0);
						doReplan = false;
					}break;
			}


			//Search Path

			if(doReplan){
				if(this->activeModule != nullptr){

					auto pAStar = dynamic_cast<AIAStarSearch*>(this->activeModule);

					AStarSearchNode startNode;
					AStarSearchNode goalNode;

					startNode.position.x = this->getX();
					startNode.position.y = this->getY();

					goalNode.position = targetPos;

					this->data.clear();

					dx = abs(goalNode.position.x - startNode.position.x);
					dy = abs(goalNode.position.y - startNode.position.y);

					int basexLevel = 5;
					basexLevel += dx / 11;

					int baseyLevel = 5;
					baseyLevel += dy / 9;

					pAStar->init(&startNode, &goalNode, basexLevel,baseyLevel);
					pAStar->loadAIData(&(this->data));
					pAStar->processAIData(0);
					pAStar->outputAIData(&(this->data));

					if(this->data.idxSize != 0 && this->data.dataSize != 0){
						this->path->clear();

						Point2F pt;

						//float rd = this->getRadius() * 4.f;

						Point2F tarp;
						pt.x = tarp.x = data.dataList[0];
						pt.y = tarp.y = data.dataList[1];

						pt.x *= 0.1f;
						pt.y *= 0.1f;

						this->path->addPoint(pt);
					
						if(data.dataSize > 2){ 

							for(int i=1;i<data.dataSize/2 - 1;i++){
								pt.x = data.dataList[i*2];
								pt.y = data.dataList[i*2+1];

								tarp = pt;
								pt.x *= 0.1f;
								pt.y *= 0.1f;
								this->path->addPoint(pt);

							}	
						}

						this->pTargetPoint->setPosition(tarp);			
					}

				}

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

#include "Sprite.h"

using namespace std;

Robot::Robot():Sprite(),RigidBody(){
	mass = 50.f;
    this->pts = nullptr;
	this->path = nullptr;
	this->pTargetPoint = nullptr;
	this->id = 1;
	this->virtualForce = Point2F(0,0);
}

Robot::~Robot(){

}

void Robot::update(double dt){

	for(int i=0;i<16;i++){
		modelMatrix[i] = 0.f;
	}

	modelMatrix[0] = 1.f;
	modelMatrix[5] = 1.f;
	modelMatrix[10] = 1.f;
	modelMatrix[15] = 1.f;

	GlobalVariables::Mat4Multi(rotateMatrix,modelMatrix,modelMatrix);
	GlobalVariables::Mat4Multi(scaleMatrix,modelMatrix,modelMatrix);
}

void Robot::draw(double dt){
	this->program->use();
	glBindVertexArray(this->vao);

	glUniformMatrix4fv(this->program->getLoc("ModelMatrix"),1,GL_FALSE,modelMatrix);
	glUniform3fv(this->program->getLoc("translateOffset"),1,glm::value_ptr(position));
	glUniform3fv(this->program->getLoc("starColor"),1,glm::value_ptr(colourValue));
	glUniform3fv(this->program->getLoc("starIntensity"),1,glm::value_ptr(colourIntensity));

	this->camera->draw(this->program);

	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

float Robot::getX()const{
	return this->position.x * 10;
}

float Robot::getY()const{
	return this->position.z * 10;
}

void Robot::setSX(float _sx){
	this->sx = _sx/10;
}

void Robot::setSY(float _sy){
	this->sy = _sy/10;
}

float Robot::getSX()const{
	return this->sx * 10;
}

float Robot::getSY()const{
	return this->sy * 10;
}

float Robot::getRadius()const{
	return this->radius;
}

void Robot::setScale(glm::vec3 _scale){

	Sprite::setScale(_scale);
	float scalex = this->scaleMatrix[0];
	float scalez = this->scaleMatrix[10];

	scalex *= 10.f;
	scalez *= 10.f;

	this->width = scalex;
	this->length = scalez;

	this->radius = sqrt(scalex*scalex + scalez*scalez)/2;
}


void Robot::move(double dt){

	if(!movable){
		return;
	}


	//float rsx = this->sx;
	//float rsy = this->sy;
	
	float ax = (virtualForce.x + collisionForce.x )/ mass;
	float ay = (virtualForce.y + collisionForce.y )/ mass;
    
    /*
	if(this->collisionForce.y * sy < 0){
		
        //ay = 0.f;//collisionForce.y / mass;
		sy = 0.f;
	}
	if(this->collisionForce.x * sx < 0){
		//ax = 0.f;//collisionForce.x / mass;
		sx = 0.f;
	}*/

	sx += ax * dt;
	sy += ay * dt;

	float s = sqrt(sx*sx + sy*sy);

	float arc = atan2(sy,sx);

	if(s > speedlimit){
		sx = speedlimit * cos(arc);
		sy = speedlimit * sin(arc);
	}


	float dx = sx + 0.5*dt*ax;
	float dy = sy + 0.5*dt*ay;


	this->position.x += dx;
	this->position.z += dy;

	this->setAngle(glm::vec3(0,arc,0));

	/*
	float angle = this->data.dataList[this->data.dataSize -1];
	angle = angle / 180.f * M_PI;
	
	if(angle != 0){
		float speed = sqrt(sx*sx + sy*sy);

		sy = speed * sin(angle);
		sx = speed * cos(angle);
	
		this->position.x += sx;
		this->position.z += sy;
	
		this->setAngle(glm::vec3(0,-angle,0));
	}*/
    if(this->pts != nullptr){
        Point2F pos;
        pos.x = this->position.x;
        pos.y = this->position.z;
    
        this->pts->addPoint(pos);
    }
}

bool Robot::calculateForce(RigidBody* dest,Point2F& result,double dt){

	float ix = this->getX();
	float iy = this->getY();
	float ir = this->getRadius();
	
	float tx = dest->getX();
	float ty = dest->getY();
	float tr = dest->getRadius();

	int id = dest->getID();
	

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


	if(id==0){

		if(result){

			dynamic_cast<Ball*>(dest)->forceAccel(speedlimit*1.25);

		}


		float dx = tx - ix;
		float dy = ty - iy;
		float distance = sqrt(dx*dx + dy*dy);

		if(distance > tr + ir + 10){

		

		/*
		std::string robot_module_name = "Robot Fuzzy";

		auto iter  = this->modules.find(robot_module_name);

		if(iter != modules.end()){
			auto fuzzy = iter->second;

			this->data.dataList[0] = tx;
			this->data.dataList[1] = ty;

			fuzzy->loadAIData(&(this->data));
			fuzzy->processAIData(dt);
			fuzzy->outputAIData(&(this->data));
		}*/

		//if(this->data.idxSize == 0){
			
			std::string robot_module_name = "Robot AStar Search";
			auto iter  = this->modules.find(robot_module_name);

			if(iter != modules.end()){
				auto pAStar = dynamic_cast<AIAStarSearch*>(iter->second);

				AStarSearchNode startNode;
				AStarSearchNode goalNode;

				startNode.position.x = this->getX();
				startNode.position.y = this->getY();

				goalNode.position.x = dest->getX();
				goalNode.position.y = dest->getY();

				//clear old datas
				this->data.clear();

				int baseLevel = 2;
				baseLevel += distance / 55;


				pAStar->init(&startNode, &goalNode, baseLevel);
				pAStar->loadAIData(&(this->data));
				pAStar->processAIData(0);
				pAStar->outputAIData(&(this->data));

				if(this->data.idxSize != 0 && this->data.dataSize != 0){
					this->path->clear();

					Point2F pt;
                    //GMMPriorityQueue<float, Point2F> pq;
                    
					for(int i=0;i<data.dataSize/2;i++){
						pt.x = data.dataList[i*2] / 10.f;
						pt.y = data.dataList[i*2+1] / 10.f;
						this->path->addPoint(pt);
                        
                        //float dist = RigidController::getInstance().calculateDistanceLevel(pt);
                        
                        //pq.join(dist,pt);
					}
					


					pt.x = data.dataList[data.dataSize-4];
					pt.y = data.dataList[data.dataSize-3];
                    
                   

					this->pTargetPoint->setPosition(pt);			
				}

				this->pTargetPoint->calculateForce(this,this->virtualForce,dt);
			}



		}

	}



	return true;
}

void Robot::clearAIData(){
	this->data.clear();
}

void Robot::addTrailer(pTrailPoints _pts){
    this->pts = _pts;
}

void Robot::addPathview(pTrailPoints _path){
	this->path = _path;
}


float Robot::calculateDistance(const Point2F& point,float rad){
	float ix = this->getX();
	float iy = this->getY();
	float ir = this->getRadius();
	
	float tx = point.x;
	float ty = point.y;

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
}

void Robot::setTargetPoint(pVirtualAttractivePoint pTP){
	this->pTargetPoint = pTP;
}
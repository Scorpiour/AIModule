#include "Sprite.h"

using namespace std;

Robot::Robot():Sprite(),RigidBody(){
	mass = 50.f;
    this->pts = nullptr;
	std::string robot_module_name = "Robot Fuzzy";
	auto fuzzy_logic = new AIFuzzyLogic(robot_module_name);

	fuzzy_logic->init("Fuzzy_Angle.fuzzy");

	if(fuzzy_logic->isInit()){
		data.dataSize = fuzzy_logic->getInputSize() + 1;
		data.dataList = new double[data.dataSize];
	}

	this->addModule(robot_module_name,fuzzy_logic);
    
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

	scalex *= 5.f;
	scalez *= 5.f;

	this->width = scalex;
	this->length = scalez;

	this->radius = sqrt(scalex*scalex + scalez*scalez);
}


void Robot::move(double dt){
	
	float rsx = this->sx;
	float rsy = this->sy;
	
	float ax = collisionForce.x / mass;
	float ay = collisionForce.y / mass;

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
	if(id==0){
		std::string robot_module_name = "Robot Fuzzy";

		auto iter  = this->modules.find(robot_module_name);

		if(iter != modules.end()){
			auto fuzzy = iter->second;

			this->data.dataList[0] = tx;
			this->data.dataList[1] = ty;

			fuzzy->loadAIData(&(this->data));
			fuzzy->processAIData(dt);
			fuzzy->outputAIData(&(this->data));
		}
	}

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

	if(-2 == id){
		Obstacle* obs = dynamic_cast<Obstacle*>(dest);
	}


	return true;
}

void Robot::addTrailer(pTrailPoints _pts){
    this->pts = _pts;
}

void Robot::setDistanceHeuristic(float h){

}
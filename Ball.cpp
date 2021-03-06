#include "Sprite.h"

using namespace std;

Ball::Ball():Sprite(),RigidBody(){
	mass = 1.f;
	friction = 100.f;
	data.dataSize = 10;
	data.dataList = new double[10];
	data.idxSize = 6;
	data.idxList = new int[6];
	//nextVelo = 0.f;
	startTime = clock();

    this->id = RigidTypeID::RigidType_Ball;

	/*
	std::string ball_module_name = "Ball Potential Field";

	auto potentialfield = new AIPotentialField(ball_module_name);
	auto pffunc = [this](pAIData input)->GlobalFlag{

			float dstx = input->dataList[0];
			float dsty = input->dataList[1];
			float dstr = input->dataList[2];

			float srcx = input->dataList[3];
			float srcy = input->dataList[4];
			float srcr = input->dataList[5];

			input->dataList[6] = 0;
			input->dataList[7] = 0;

			float dx = srcx - dstx;
			float dy = srcy - dsty;

			float arc = GlobalVariables::Atan2(dy,dx);

			float distance = sqrt(dx*dx + dy*dy);
			float value = 0.f;
			if(distance > (dstr + srcr)){
				value = 2.5f;
			}

			input->dataList[6] = value*cos(arc);
			input->dataList[7] = value*sin(arc);

			return GlobalFlag::GlobalFlag_Success;
	};

	potentialfield->setProcessFunction(pffunc);

	this->addModule(ball_module_name, potentialfield);
	*/
}

Ball::~Ball(){

}

void Ball::update(double dt){

}

void Ball::draw(double dt){
	this->program->use();
	glBindVertexArray(this->vao);

	float additionIntensity = 0.1f;

	GLint modelMatrixLoc = this->program->getLoc("ModelMatrix");
	GLint translateLoc = this->program->getLoc("translateOffset");
	GLint starRadiusLoc = this->program->getLoc("radius");
	GLint starColorLoc = this->program->getLoc("starColor");
	GLint starIntensityLoc = this->program->getLoc("starIntensity");
	GLint highlightLoc = this->program->getLoc("additionIntensity");
	
	glUniform1f(program->getLoc("divideOuterLevel"),3.0);
	glUniform1f(program->getLoc("divideInnerLevel"),3.0);
	glUniformMatrix4fv(modelMatrixLoc,1,GL_FALSE,modelMatrix);
	glUniform3fv(translateLoc,1,glm::value_ptr(position));
	glUniform1f(starRadiusLoc,this->innerRadius);
	glUniform3fv(starColorLoc,1,glm::value_ptr(colourValue));
	glUniform3fv(starIntensityLoc,1,glm::value_ptr(colourIntensity));
	glUniform1f(highlightLoc,additionIntensity);

	this->camera->draw(this->program);

	glDrawElements(GL_PATCHES,60,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}


float Ball::getX()const{
	return this->position.x * 10;
}

float Ball::getY()const{
	return this->position.z * 10;
}

void Ball::setSX(float _sx){
	this->sx = _sx/10;
}

void Ball::setSY(float _sy){
	this->sy = _sy/10;
}

float Ball::getSX()const{
	return this->sx * 10;
}

float Ball::getSY()const{
	return this->sy * 10;
}

float Ball::getOuterRadius()const{
	return this->outerRadius * 10;
}

float Ball::getInnerRadius()const{
    return this->innerRadius * 10;
}

void Ball::setFriction(float f){
	this->friction = f;
}

Point2F Ball::getKickingPos(float gap){
	
	Point2F ret;

	
	float ix = this->getX();
	float iy = this->getY();
	float ir = this->getInnerRadius();

	Point2F bpos;
	bpos.x = ix;
	bpos.y = iy;

	float distLevel = RigidController::getInstance().calculateDistanceLevel(bpos);

	if(distLevel < gap){
		gap = distLevel/2;
	}

	if(gap < 5){
		ret.x = ix;
		ret.y = iy;
		return ret;
	}


	float tx = 110;
	float ty = 0;

	float dx = tx - ix;
	float dy = ty - iy;

	float distance = sqrt(dx*dx + dy*dy);

	if((distance < ir)||gap < 1e-6){
		ret.x = ix - gap;
		ret.y = iy;
		return ret;
	}

	ret.x = ix - dx * gap/distance;
	ret.y = iy - dy * gap/distance;

	return ret;
	
}

void Ball::resetTimer(){
	this->startTime = clock();
}

void Ball::move(double dt){

	float speed = sqrt(sx*sx + sy*sy);

	float reduce = friction * dt / 1000 / mass;

	float varc = GlobalVariables::Atan2(sy,sx);
	if(speed > reduce){
		speed -= reduce;
	}else{
		speed = 0;
	}
	sx = speed*cos(varc);
	sy = speed*sin(varc);
	

	if(collisionForce.x == 0 && collisionForce.y == 0){
		this->position.x += sx;
		this->position.z += sy;
	}else{

		float speed = 0.f;
		if(nextVelo >= 0){
			float ax = collisionForce.x / mass;
			float ay = collisionForce.y / mass;

			sx += ax;
			sy += ay;

			speed = nextVelo;
			nextVelo = -1.f;

			cout<<"Timer : "<<clock() - startTime <<" ms"<<endl;

		}else{
			speed = sqrt(sy*sy + sx*sx);	
		}

		float arc = GlobalVariables::Atan2(sy,sx);//GlobalVariable::Atan2(sy,sx);
		
		sx = speed*cos(arc);
		sy = speed*sin(arc);



		float phi = GlobalVariables::Atan2(collisionForce.y, collisionForce.x);
		while(phi < 0){
			phi += M_PI * 2.f;
		}

		float theta = GlobalVariables::Atan2(sy,sx);
		while(theta < 0){
			theta += M_PI * 2.f;
		}

		float delta = theta - phi;
		if(abs(delta) <= M_PI/2){
			this->position.x += sx;
			this->position.z += sy;
		}else{
			float theta2 = theta + M_PI;
			while(theta2 >= M_PI * 2.f){
				theta2 -= M_PI * 2.f;
			}
			theta2 = phi*2 - theta2;

			
			//float speed = sqrt(sy*sy + sx*sx);
			//speed = (speed < speedlimit)?speed:speedlimit;

			sx = speed * cos(theta2);
			sy = speed * sin(theta2);
			this->position.x += sx;
			this->position.z += sy;
		}
	}
}

bool Ball::calculateVirtualForce(RigidBody* dest, Point2F& result,double dt){

	//result.x = 0;
	//result.y = 0;
	//return true;

	float tx = dest->getX();
	float ty = dest->getY();

	float ix = this->getX();
	float iy = this->getY();

	float dx = ix - tx;
	float dy = iy - ty;

	float arc = GlobalVariables::Atan2(dy,dx);

	float distance = sqrt(dx*dx + dy*dy);

	float value = distance * 0.668;//distance;

	result.x = value * cos(arc);
	result.y = value * sin(arc);


	return true;
}

bool Ball::checkLOS(const Point2F& p1, const Point2F& p2){

	float x = this->getX();
	float y = this->getY();
	float r = this->getInnerRadius();

	float margin = 3.5;

	Point2F pivot(x,y);

	if( GlobalVariables::distancePointToLine(pivot,p1,p2) > r+margin){
		return true;
	}

	return false;
}

bool Ball::calculateForce(RigidBody* dest,Point2F& result,double dt){

	result.x = 0.f;
	result.y = 0.f;
	return true;

    if(dest->getID() != RigidTypeID::RigidType_Robot){
		result.x = 0.f;
		result.y = 0.f;
		return true;
	}

	result.x = 0;
	result.y = 0;

	float dstx = dest->getX();
	float dsty = dest->getY();
	float dstr = dest->getOuterRadius();

	float srcx = this->getX();
	float srcy = this->getY();
	float srcr = this->getOuterRadius();
	
	data.dataList[0] = dstx;
	data.dataList[1] = dsty;
	data.dataList[2] = dstr;

	data.dataList[3] = srcx;
	data.dataList[4] = srcy;
	data.dataList[5] = srcr;

	for(auto& p : this->modules){
		auto m = p.second;
		if(m!=nullptr){
			m->loadAIData(&data);
			m->processAIData(dt);
			m->outputAIData(&data);
		}
	}

	result.x = data.dataList[6];
	result.y = data.dataList[7];

	return true;
}

float Ball::calculateDistance(const Point2F& point, float rad){
	if(this->activeDistance){
		float ix = this->getX();
		float iy = this->getY();
		float ir = this->getOuterRadius();

		float tx = point.x;
		float ty = point.y;

		float dx = tx-ix;
		float dy = ty-iy;

		float distance = sqrt(dx*dx + dy*dy);

		distance = distance - rad - ir;

		return distance;
	}
	return FLT_MAX;
}

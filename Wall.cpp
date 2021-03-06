#include "Sprite.h"

using namespace std;

Wall::Wall():Sprite(),RigidBody(){
    this->id = RigidTypeID::RigidType_Obstacle;
}

Wall::~Wall(){

}

void Wall::update(double dt){

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

void Wall::draw(double dt){
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


float Wall::getX()const{
	return this->position.x * 10;
}

float Wall::getY()const{
	return this->position.z * 10;
}

void Wall::setSX(float _sx){
	
}

void Wall::setSY(float _sy){

}

float Wall::getSX()const{
	return 0.f;
}

float Wall::getSY()const{
	return 0.f;
}

float Wall::getOuterRadius()const{
	return 0.f;
}

float Wall::getInnerRadius()const{
    return 0.f;
}

void Wall::move(double){

}

bool Wall::calculateForce(RigidBody* dest,Point2F& result,double dt){
	if(this->forceFunc != nullptr){

		return this->forceFunc(dest,result);
	}
	return false;
}

bool Wall::checkLOS(const Point2F& p1, const Point2F& p2){

	float angle = this->angles.y;
	float length = this->scaleMatrix[0] * 10;
	float x = this->getX();
	float y = this->getY();

	float r = length /  2;
	float cosr = r*cos(angle);
	float sinr = r*sin(angle);

	Point2F p3(x-cosr,y-sinr);
	Point2F p4(x+cosr,y+sinr);

	return !GlobalVariables::linesIntersect(p1,p2,p3,p4);
}

bool Wall::calculateVirtualForce(RigidBody* dest, Point2F& result,double dt){

	if(this->forceFunc != nullptr){

		float angle = this->angles.y;
		float length = this->scaleMatrix[0] * 10;
		float x = this->getX();
		float y = this->getY();

		float r = length / 2;

		Point2F p1;
		Point2F p2;

		float cost = r*cos(angle);
		float sint = r*sin(angle);

		p1.x = x - cost;
		p1.y = y - sint;
		p2.x = x + cost;
		p2.y = y + sint;

		if(p1.x > p2.x){
			std::swap(p1.x,p2.x);
			std::swap(p1.y,p2.y);
		}

		float tx = dest->getX();
		float ty = dest->getY();

		float a = (p2.y - p1.y);
		float b = (p1.x - p2.x);
		float c = p1.y*(p2.x-p1.x) + p1.x*(p1.y-p2.y);

		if(a==0&&b==0){
			result.x = 0;
			result.y = 0;
			return false;
		}

		float denom = 1/sqrt(a*a+b*b);
		
		Point2F foot;
		foot.x = (b*b*tx - a*b*ty - a*c)*denom*denom;
		foot.y = (a*a*ty - a*b*tx - b*c)*denom*denom;

		float distance = -1.f;
		float margin = 7.f;

		if(foot.x < p1.x){
			result.x = 0;
			result.y = 0;
			return false;
		}else if(foot.x > p2.x){
			result.x = 0;
			result.y = 0;
			return false;
		}else{
			distance = abs(a*tx + b*ty + c)*denom;
		}
		
		if(distance > margin + dest->getOuterRadius()){
			result.x = 0;
			result.y = 0;
			return false;
		}

		float dx = tx - foot.x;
		float dy = ty - foot.y;
		float arc = GlobalVariables::Atan2(dy,dx);

		float value =  1500.f/(1+exp(distance-7));//1000.f/distance;
		result.x = value * cos(arc);
		result.y = value * sin(arc);
		return true;

		//return distance;
	}


	return false;
}

float Wall::calculateDistance(const Point2F& point, float rad){

	if(this->forceFunc != nullptr){

		float angle = this->angles.y;
		float length = this->scaleMatrix[0] * 10;
		float x = this->getX();
		float y = this->getY();

		float r = length / 2;

		Point2F p1;
		Point2F p2;

		float cost = r*cos(angle);
		float sint = r*sin(angle);

		p1.x = x - cost;
		p1.y = y - sint;
		p2.x = x + cost;
		p2.y = y + sint;

		if(p1.x > p2.x){
			std::swap(p1.x,p2.x);
			std::swap(p1.y,p2.y);
		}

		float tx = point.x;
		float ty = point.y;

		float a = (p2.y - p1.y);
		float b = (p1.x - p2.x);
		float c = p1.y*(p2.x-p1.x) + p1.x*(p1.y-p2.y);

		if(a==0&&b==0){
			return 0.f;
		}

		float denom = 1/sqrt(a*a+b*b);
		
		Point2F foot;
		foot.x = (b*b*tx - a*b*ty - a*c)*denom*denom;
		foot.y = (a*a*ty - a*b*tx - b*c)*denom*denom;

		float distance = -1.f;

		if(foot.x < p1.x){
			float dx = tx-p1.x;
			float dy = ty-p1.y;
			distance = sqrt(dx*dx + dy*dy);
		}else if(foot.x > p2.x){
			float dx = tx-p2.x;
			float dy = ty-p2.y;
			distance = sqrt(dx*dx + dy*dy);
		}else{
			distance = abs(a*tx + b*ty + c)*denom;
		}
		
		return distance;
	}
	return FLT_MAX;
}

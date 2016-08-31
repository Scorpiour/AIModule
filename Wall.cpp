#include "Sprite.h"

using namespace std;

Wall::Wall():Sprite(),RigidBody(){

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

float Wall::getRadius()const{
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
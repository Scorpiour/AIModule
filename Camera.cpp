#include "Camera.h"

using namespace std;

Camera::Camera(glm::vec3 _pos, glm::vec3 _tar, glm::vec3 _up){
	position = _pos;
	target = _tar;
	up = _up;
}

Camera::~Camera(){

}

void Camera::setTarget(glm::vec3 _tar){
	this->target = _tar;
}

void Camera::setPosition(glm::vec3 _pos){
	this->position = _pos;
}

void Camera::setUp(glm::vec3 _up){
	this->up = _up;
}


const glm::vec3 Camera::getTarget()const{
	return this->target;
}

const glm::vec3 Camera::getPosition()const{
	return this->position;
}

const glm::vec3 Camera::getUp()const{
	return this->up;
}


void Camera::update(double dt, double radius, double persp_angle,double ratio){	

	GLfloat camX;
	GLfloat camY;
	GLfloat camZ;

	glm::vec3 tar = this->target;

	camX = position.x;
	camY = position.y;
	camZ = position.z;

	tar.x = target.x - sin(persp_angle)*radius;
	tar.z = position.z - radius * cos(persp_angle);

	glm::vec3 cameraPosition = glm::vec3(camX,camY,camZ);

	GlobalVariables::calculateLookat(cameraPosition, tar, up, lookat);
	GlobalVariables::calculateProjection(45.f, ratio, 0.1f,100.f,project);

	GlobalVariables::Mat4Transpose(project);

	GlobalVariables::Mat4Multi(project,lookat,view);

}

void Camera::draw(pShaderProgram _program){

	glUniformMatrix4fv(_program->getLoc("ViewMatrix"),1,GL_TRUE,view);
	glUniform3fv(_program->getLoc("lightPos"),1,glm::value_ptr(this->position));
	glUniform1f(_program->getLoc("worldW"),1.f);
}
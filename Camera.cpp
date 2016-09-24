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

bool Camera::rayClip(Point2F& worldCoord,GLFWwindow* window){
	memcpy(invLookat,lookat,16*sizeof(GLfloat));
	memcpy(invProject,project,16*sizeof(GLfloat));

	if(GlobalVariables::Mat4Inverse(invProject) && GlobalVariables::Mat4Inverse(invLookat)){

		double x;
		double y;
		glfwGetCursorPos(window,&x,&y);

		int w,h;
		glfwGetWindowSize(window,&w,&h);

		float x_nds = (2.f * x)/(float)w - 1.f;
		float y_nds = 1.f - (2.f * y)/(float)h;

		glm::vec4 rayClip = glm::vec4(x_nds, y_nds, -1.f, 3.f);

		glm::vec4 rayEye = GlobalVariables::Mat4MultiVec4(invProject,rayClip);
		rayEye.z = -1.f;//((or>0)?or:1.f) ;//-camZ;	
		rayEye.w = 0.f;

		glm::vec4 rayWorld =  GlobalVariables::Mat4MultiVec4(invLookat,rayEye);
		glm::vec3 ray = glm::vec3(rayWorld);
		ray = glm::normalize(ray);

		float t = -position.y / ray.y;

		worldCoord.x = position.x + t*ray.x;
		worldCoord.y = position.z + t*ray.z;
		

		return true;
	}
	return false;
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
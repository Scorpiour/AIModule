#include "Star.h"

using namespace std;

Star::Star(GLuint program,GLuint id){
	rotationVelocity = 1.0;
	revolutionVelocity = 1.0;
	orbitRadius = 1.0;
	orbitCentre = glm::vec2(0.f, 0.f);
	radius = 0.3;

	ID = id;

	isHighlighted = false;

	colourValue = glm::vec3(1.f, 1.f, 1.f);
	colourIntensity = glm::vec3(0.1, 0.5, 0.8);
	position = glm::vec3(0,0,0);

	modelMatrixLoc = glGetUniformLocation(program,"ModelMatrix");
	translateLoc = glGetUniformLocation(program,"translateOffset");
	starRadiusLoc = glGetUniformLocation(program,"radius");
	starColorLoc = glGetUniformLocation(program,"starColor");
	starIntensityLoc = glGetUniformLocation(program,"starIntensity");
	highlightLoc = glGetUniformLocation(program,"additionIntensity");
}

Star::~Star(){}

//Get
glm::vec3 Star::getColor(){
	return this->colourValue;
}

glm::vec3 Star::getIntensity(){
	return this->colourIntensity;
}

glm::vec2 Star::getOrbitCentre(){
	return this->orbitCentre;
}

float Star::getRotationVelo(){
	return this->rotationVelocity;
}

float Star::getOrbitRadius(){
	return this->orbitRadius;
}

float Star::getRevolutionVelo(){
	return this->revolutionVelocity;
}

float Star::getRadius(){
	return this->radius;
}

glm::vec3 Star::getPosition(){
	return this->position;
}

GLuint Star::getID(){
	return this->ID;
}

//Set
void Star::setColour(glm::vec3 colour){
	this->colourValue = colour;
}

void Star::setIntensity(glm::vec3 intensity){
	this->colourIntensity = intensity;
}

void Star::setRotationVelo(float value){
	this->rotationVelocity = value;
}

void Star::setRevolutionVelo(float value){
	this->revolutionVelocity = value;
}

void Star::setOrbitRadius(float value){
	this->orbitRadius = value;
}

void Star::setOrbitCentre(glm::vec2 centre){
	this->orbitCentre = centre;
}

void Star::setRadius(float value){
	this->radius = value;
}

void Star::setHighlight(bool b){
	this->isHighlighted = b;
}

void Star::update(double dt){
	GLfloat theta = dt*rotationVelocity*20*M_PI/180.f;
	GLfloat cost = cos(theta);
	GLfloat sint = sin(theta);
	

	/*
	modelMatrix = 
	{
		cost,	0,		sint,	0,
		0,		1,		0,		0,
		-sint,	0,		cost,	0,
		0,		0,		0,		1
	};*/

	for(int i=0;i<16;i++){
		modelMatrix[i] = 0.f;
	}

	modelMatrix[0] = cost;
	modelMatrix[2] = sint;
	modelMatrix[5] = 1.f;
	modelMatrix[8] = -sint;
	modelMatrix[10] = cost;
	modelMatrix[15] = 1.f;

	GLfloat radius = GLfloat(this->orbitRadius);
	GLfloat xoffset = radius * cos(dt*revolutionVelocity);
	GLfloat zoffset = radius * sin(dt*revolutionVelocity);
	position = glm::vec3(xoffset, 0, zoffset);
}


void Star::draw(double dt){

	float additionIntensity = 0.f;

	if(isHighlighted){
		additionIntensity = 0.6f;
	}

	glUniformMatrix4fv(modelMatrixLoc,1,GL_FALSE,modelMatrix);
	glUniform3fv(translateLoc,1,glm::value_ptr(position));
	glUniform1f(starRadiusLoc,this->radius);
	glUniform3fv(starColorLoc,1,glm::value_ptr(colourValue));
	glUniform3fv(starIntensityLoc,1,glm::value_ptr(colourIntensity));
	glUniform1f(highlightLoc,additionIntensity);

}
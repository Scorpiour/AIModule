#include "Sprite.h"

using namespace std;

SpriteManager SpriteManager::manager;

SpriteManager::SpriteManager(){

}

SpriteManager::~SpriteManager(){
	for(auto ptr : this->spriteList){
		SAFE_DEL(ptr);
	}
}

SpriteManager* SpriteManager::getInstance(){
	return &(SpriteManager::manager);
}

void SpriteManager::addToManager(pSprite p){
	this->spriteList.insert(p);
}

void SpriteManager::removeFromManager(pSprite & p){
	size_t count = this->spriteList.erase(p);
	if(count){
		SAFE_DEL(p);
	}
}

void SpriteManager::updateAll(double dt){
	for(auto ptr : this->spriteList){
		if(ptr){
			ptr->update(dt);
		}
	}
}

void SpriteManager::drawAll(double dt){
	for(auto ptr : this->spriteList){
		if(ptr){
			ptr->draw(dt);
		}
	}
}


Sprite::Sprite(){
	for(int i=0;i<16;i++){
		modelMatrix[i] = 0;
		scaleMatrix[i] = 0;
		rotateMatrix[i] = 0;
	}

	modelMatrix[0] = scaleMatrix[0] = rotateMatrix[0] = 1.f;
	modelMatrix[5] = scaleMatrix[5] = rotateMatrix[5] = 1.f;
	modelMatrix[10] = scaleMatrix[10] = rotateMatrix[10] = 1.f;
	modelMatrix[15] = scaleMatrix[15] = rotateMatrix[15] = 1.f;

	program = nullptr;
	camera = nullptr;
	vao = 0;

	angles = glm::vec3(0,0,0);
	radius = 1.f;

	auto manager = SpriteManager::getInstance();
	manager->addToManager(this);
}

Sprite::~Sprite(){

}

void Sprite::setProgram(pShaderProgram _program){
	this->program = _program;
}

void Sprite::setVAO(GLuint _vao){
	this->vao = _vao;
}

void Sprite::setCamera(pCamera _camera){
	this->camera = _camera;
}

void Sprite::setColour(glm::vec3 _colour){
	this->colourValue = _colour;
}

void Sprite::setColourIntensity(glm::vec3 _intensity){
	this->colourIntensity = _intensity;
}

void Sprite::setPosition(glm::vec3 _pos){
	this->position = _pos;
}

void Sprite::setScale(glm::vec3 _scale){
	for(int i=0;i<16;i++){
		scaleMatrix[i] = 0.f;
	}

	scaleMatrix[0] = _scale.x;
	scaleMatrix[5] = _scale.y;
	scaleMatrix[10] = _scale.z;
	scaleMatrix[15] = 1.f;
}

void Sprite::setAngle(glm::vec3 _angle){
	GLfloat rotateX[16];
	GLfloat rotateY[16];
	GLfloat rotateZ[16];
	
	for(int i=0;i<16;i++){
		rotateMatrix[i] = 0.f;
		rotateX[i] = 0.f;
		rotateY[i] = 0.f;
		rotateZ[i] = 0.f;
	}
	rotateMatrix[0] = rotateMatrix[5] = rotateMatrix[10] = rotateMatrix[15] = 1.f;
	rotateX[15] = rotateY[15] = rotateZ[15] = 1.f;

	rotateX[0] = 1.f;
	rotateY[5] = 1.f;
	rotateZ[10] = 1.f;

	GLfloat cosX = cos(_angle.x);
	GLfloat sinX = sin(_angle.x);

	GLfloat cosY = cos(_angle.y);
	GLfloat sinY = sin(_angle.y);
	
	GLfloat cosZ = cos(_angle.z);
	GLfloat sinZ = sin(_angle.z);

	rotateX[5] = cosX;
	rotateX[6] = sinX;
	rotateX[9] = -sinX;
	rotateX[10] = cosX;

	rotateY[0] = cosY;
	rotateY[2] = sinY;
	rotateY[8] = -sinY;
	rotateY[10] = cosY;

	rotateZ[0] = cosZ;
	rotateZ[1] = sinZ;
	rotateZ[4] = -sinZ;
	rotateZ[5] = cosZ;

	GlobalVariables::Mat4Multi(rotateY,rotateX,rotateMatrix);
	GlobalVariables::Mat4Multi(rotateZ,rotateMatrix,rotateMatrix);

	this->angles = _angle;
}

void Sprite::setRadius(double r){
	this->radius = r;
}

void Sprite::addModule(const std::string& name, AIModuleBase* pModule){
	auto iter = this->modules.find(name);
	if(iter == modules.end()){
		modules.insert(make_pair(name,pModule));
	}
}

void Sprite::removeModule(const std::string& name){
	auto iter = this->modules.find(name);
	if(iter != modules.end()){
		modules.erase(iter);
	}
}
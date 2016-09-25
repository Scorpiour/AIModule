#include "Model.h"

using namespace std;

Model::Model(const std::string& _name):modelName(_name){

}

Model::~Model(){

}

const std::string& Model::getModelName()const{
	return this->modelName;
}

void Model::addSprite(Sprite* _sp){
	this->spriteList.insert(_sp);
}

void Model::enable(bool _e){
	for(auto& ptr : spriteList){
		ptr->enable(_e);
	}
}

void Model::visible(bool _v){
	for(auto& ptr : spriteList){
		ptr->visible(_v);
	}
}



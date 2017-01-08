#include "Model.h"

using namespace std;

Model::Model(const std::string& _name):modelName(_name){
	active = true;
    ballPos.x = 50;
    ballPos.y = 50;
    robotPos.x = -50;
    robotPos.y = -50;
}

Model::Model(const std::string& _name,const Point2F& robot,const Point2F& ball):modelName(_name),robotPos(robot),ballPos(ball){
    active = true;
}

Model::Model(const std::string& _name,Point2F && robot,Point2F && ball):modelName(_name),robotPos(robot),ballPos(ball){
    active = true;
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

void Model::swithActive(){
	active = !active;
	this->enable(active);
	this->visible(active);
}

bool Model::getActive()const{
	return this->active;
}

void Model::setActive(bool _a){
	this->active = _a;
	this->enable(_a);
	this->visible(_a);
}

void Model::setRobotPos(const Point2F& pos){
    this->robotPos = pos;
}
void Model::setBallPos(const Point2F& pos){
    this->ballPos = pos;
}

void Model::setRobotPos(Point2F && pos){
    this->robotPos = pos;
}
void Model::setBallPos(Point2F && pos){
    this->ballPos = pos;
}

const Point2F& Model::getRobotPos(void)const{
    return this->robotPos;
}
const Point2F& Model::getBallPos(void)const{
    return this->ballPos;
}

#include "ModuleBase.h"

using namespace std;

AIAStarSearch::AIAStarSearch(const std::string& name):AIModuleBase(name){
	pInternalData = nullptr;
	hasInit = false;
}

AIAStarSearch::~AIAStarSearch(){

}

GlobalFlag AIAStarSearch::loadAIData(const pAIData pdata){
	if(this->hasInit){
		pInternalData = pdata;
		return GlobalFlag_Success;
	}
	return GlobalFlag_Uninitialized;
}

GlobalFlag AIAStarSearch::processAIData(double dt){
	if(this->hasInit){



		return GlobalFlag_Success;
	}
	return GlobalFlag_Uninitialized;
}

GlobalFlag AIAStarSearch::outputAIData(pAIData pdata){
	if(this->hasInit){


		return GlobalFlag::GlobalFlag_Success;
	}
	return GlobalFlag_Uninitialized;
}

GlobalFlag AIAStarSearch::releaseAIData(void){
	pInternalData = nullptr;
	return GlobalFlag::GlobalFlag_Success;
}

void AIAStarSearch::setProcessFunction(ProcessFunction func){
	
}


void AIAStarSearch::init(pAStarSearchNode startNode, pAStarSearchNode goalNode){


	this->hasInit = true;
}

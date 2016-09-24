#include "GlobalMacro.h"
#include "ModuleBase.h"

using namespace std;

AIPotentialField::AIPotentialField(const std::string& name):AIModuleBase(name){
	pInternalData = nullptr;
	quickMode = true;
	pFunc = nullptr;
}

AIPotentialField::~AIPotentialField(){
	if(!quickMode){
		SAFE_DEL(pInternalData);
	}
}

//Interface Implementation
GlobalFlag AIPotentialField::loadAIData(const pAIData pdata){
	if(quickMode){
		this->pInternalData = pdata;
	}else{
		SAFE_DEL(pInternalData);
		pInternalData = new AIData(*pdata);
	}
	return GlobalFlag_Success;
}

GlobalFlag AIPotentialField::processAIData(double dt){
	if(this->pFunc != nullptr){
		return pFunc(pInternalData);
	}
	return  GlobalFlag_Success;
}

GlobalFlag AIPotentialField::outputAIData(pAIData pdata){
	if(!quickMode){
		*pdata = *pInternalData;
	}
	return GlobalFlag_Success;
}

GlobalFlag AIPotentialField::releaseAIData(void){
	if(quickMode){
		this->pInternalData = nullptr;
	}else{
		SAFE_DEL(pInternalData);
	}
	return GlobalFlag_Success;
}

void AIPotentialField::setProcessFunction(ProcessFunction func){
	pFunc = func;
}


//Other Public Methods
void AIPotentialField::setQuickMode(bool b){
	this->quickMode = b;
}
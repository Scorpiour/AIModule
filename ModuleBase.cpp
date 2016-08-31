#include "ModuleBase.h"
#include "GlobalMacro.h"

AIModuleController AIModuleController::controller;

AIModuleController& AIModuleController::getInstance(){
	return AIModuleController::controller;
}


AIModuleController::AIModuleController(){

}

AIModuleController::~AIModuleController(){
	for(auto& p : this->modules){
		if(p.second != nullptr){
			SAFE_DEL(p.second);
		}
	}
}

void AIModuleController::addToController(const std::string& name, AIModuleBase* module){
	auto iter = this->modules.find(name);
	if(iter == modules.end()){
		modules.insert(make_pair(name,module));
	}else{
		//do somethinig with duplicate modules
	}
}



AIModuleBase::AIModuleBase(const std::string& name){
	AIModuleController::getInstance().addToController(name,this);
}

AIModuleBase::~AIModuleBase(){}

AIData::AIData(){
	idxList = nullptr;
	idxSize = 0;

	dataList = nullptr;
	dataSize = 0;
}

AIData::AIData(const AIData& A){
	if(A.dataList !=nullptr && A.dataSize!=0){
		this->dataSize = A.dataSize;
		SAFE_COPY_TYPE_LEN(double,dataList,A.dataList,dataSize);
	}
}

AIData::~AIData(){
	SAFE_DELS(idxList);
	SAFE_DELS(dataList);
}

const AIData& AIData::operator = (const AIData& A){
	if(&A != this){
		if(A.dataList !=nullptr && A.dataSize!=0){
			this->dataSize = A.dataSize;
			SAFE_COPY_TYPE_LEN(double,dataList,A.dataList,dataSize);
		}
	}
	return *this;
}


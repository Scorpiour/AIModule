#include "ModuleBase.h"

using namespace std;


FuzzyRule::FuzzyRule(){
	inp_index = nullptr;
	inp_fuzzy_set = nullptr;
	out_fuzzy_set = 0;
}

FuzzyRule::~FuzzyRule(){
	SAFE_DELS(inp_index);
	SAFE_DELS(inp_fuzzy_set);
}

AIFuzzyLogic::AIFuzzyLogic(const std::string& name):AIModuleBase(name){
	has_init = false;
	noInputs = 0;
	noRules = 0;
	noRegions = 0;
	noOutputs = 0;
	result = 0.f;

	output_values = nullptr;
	tmpValues = nullptr;
	impMemFns =nullptr;
	rules = nullptr;

	pInternalData = nullptr;
}

AIFuzzyLogic::~AIFuzzyLogic(){

	SAFE_DELS(output_values);
	SAFE_DELS(tmpValues);
	SAFE_DELS(rules);
	SAFE_DELS(impMemFns);
}

void AIFuzzyLogic::init(
							size_t no_of_input, 
							size_t no_of_rules,	
							size_t no_of_regions,
							size_t no_of_outputs,

							double outputVals[]
							){

	noInputs = no_of_input;
	noRules = no_of_rules;
	noRegions = no_of_regions;
	noOutputs = no_of_outputs;

	SAFE_DELS(output_values);
	output_values = new double[noOutputs];
	SAFE_DELS(tmpValues);
	tmpValues = new float[noInputs];

	for(size_t i=0;i<noOutputs;i++){
		output_values[i] = outputVals[i];
	}

	rules = new FuzzyRule[noRules];

	has_init = false;
}

void AIFuzzyLogic::initFuzzyRules(){
	
}

void AIFuzzyLogic::init(const std::string& fuzzyFileName){
	ifstream fin;
	fin.open(fuzzyFileName);

	if(!fin){
		this->has_init = false;
		return;
	}

	fin>>noInputs>>noRules>>noRegions>>noOutputs;

	SAFE_DELS(output_values);
	output_values = new double[noOutputs];
	SAFE_DELS(tmpValues);
	tmpValues = new float[noInputs];

	for(size_t i=0;i<noOutputs;i++){
		fin>>output_values[i];
	}
	SAFE_DELS(this->rules);
	this->rules = new FuzzyRule[noRules];

	auto initRules = [=](ifstream& fin)->void{
		for(size_t i=0;i<noRules;i++){
			auto r = &(this->rules[i]);
			SAFE_DELS(r->inp_index);
			r->inp_index = new int[noInputs];
			
			for(size_t j=0;j<noInputs;j++){
				fin>>r->inp_index[j];
			}
		}

		for(size_t i=0;i<noRules;i++){
			auto r = &(this->rules[i]);
			SAFE_DELS(r->inp_fuzzy_set);
			r->inp_fuzzy_set = new int[noInputs];
			
			for(size_t j=0;j<noInputs;j++){
				fin>>r->inp_fuzzy_set[j];
			}
			fin>>r->out_fuzzy_set;
		}
	};

	auto initMembershipFunctions = [=](ifstream& fin)->void{
		
		auto initTrapz = [](float x1,float x2,float x3,float x4, TrapzType type)->Trapezoid{
			Trapezoid trz;
			trz.a = x1;
			trz.b = x2;
			trz.c = x3;
			trz.d = x4;
			trz.type = type;
			switch (trz.type)
			{
			case regular_trapezoid:
			{
				trz.l_slope = 1.0/(trz.b - trz.a);
				trz.r_slope = 1.0/(trz.c - trz.d);
				break;
			}
			case left_trapezoid:
			{
				trz.r_slope = 1.0/(trz.a - trz.b);
				trz.l_slope = 0.0;
				break;
			}
			case right_trapezoid:
			{
				trz.l_slope = 1.0/(trz.b - trz.a);
				trz.r_slope = 0.0;
				break;
			}
			}
			return trz;
		};
		SAFE_DELS(this->impMemFns);
		this->impMemFns = new Trapezoid[noRegions * noInputs];

		for(size_t i=0;i<noInputs;i++){
			for(size_t j=0;j<noRegions;j++){

				float a,b,c,d;
				int type;

				fin>>a>>b>>c>>d>>type;
				TrapzType t = static_cast<TrapzType>(type);

				this->impMemFns[i*noRegions+j] = initTrapz(a,b,c,d,t);
			}
		}
	};

	initRules(fin);
	initMembershipFunctions(fin);

	fin.close();
	
	this->has_init = true;

	return;
}

//Interface Implementation
GlobalFlag AIFuzzyLogic::loadAIData(const pAIData pdata){
	if(has_init = false){
		return GlobalFlag_Uninitialized;
	}
	if(pdata->dataSize == 0 || pdata->dataList == nullptr){
		return GlobalFlag_InvalidArguments;
	}
	pInternalData = pdata;

	return GlobalFlag_Success;
}

GlobalFlag AIFuzzyLogic::processAIData(double dt){
	if(pInternalData == nullptr){
		return GlobalFlag::GlobalFlag_InvalidArguments;
	}

	auto trapz = [=](float x, const Trapezoid& trz)->float{
		switch (trz.type)
		{
			case left_trapezoid:
			{
				if(x<=trz.a)return 1.0;
				if(x>=trz.b)return 0.0;
				return trz.r_slope*(x-trz.b);
			}
			case right_trapezoid:
			{
				if(x<=trz.a)return 0.0;
				if(x>=trz.b)return 1.0;
				return trz.l_slope*(x-trz.a);
			}
			case regular_trapezoid:
			{
				if((x<=trz.a)||(x>=trz.d))return 0.0;
				if((x>=trz.b)&&(x<=trz.c))return 1.0;
				if((x>=trz.a)&&(x<=trz.b))return trz.l_slope*(x-trz.a);
				if((x>=trz.c)&&(x<=trz.d))return trz.r_slope*(x-trz.d);
			}	    
		}
		return 0.0;

	};

	auto minof = [=](float values[], int size)->float{
		float val = values[0];
		for(int i=1;i<size;i++){
			if(val > values[i]){
				val = values[i];
			}
		}
		return val;
	};

	auto inputs = pInternalData->dataList;

	int idx,fuzzy_set;

	float sum1 = 0.f;
	float sum2 = 0.f;
	float weight = 0.f;

	for(size_t i=0;i<noRules;i++){
		for(size_t j=0;j<noInputs;j++){

			idx = rules[i].inp_index[j];
			fuzzy_set = rules[i].inp_fuzzy_set[j];
			tmpValues[j] = trapz(float(inputs[idx]),this->impMemFns[idx * noRegions + fuzzy_set]);
		}

		weight = minof(tmpValues,noInputs);
		float out = output_values[rules[i].out_fuzzy_set];
		sum1 += weight * out;
		sum2 += weight;
	}

	if(fabs(sum2) < 1E-16){
		result = 0.0;
		return GlobalFlag_GeneralError;
	}

	result = sum1/sum2;

	return GlobalFlag_Success;
}

GlobalFlag AIFuzzyLogic::outputAIData(pAIData pdata){
	pdata->dataList[pdata->dataSize - 1] = result;
	return GlobalFlag_Success;
}

GlobalFlag AIFuzzyLogic::releaseAIData(void){
	pInternalData = nullptr;

	return GlobalFlag_Success;
}

void AIFuzzyLogic::setProcessFunction(ProcessFunction func){

}

bool AIFuzzyLogic::isInit(void)const{
	return this->has_init;
}

size_t AIFuzzyLogic::getInputSize(void)const{
	return this->noInputs;
}
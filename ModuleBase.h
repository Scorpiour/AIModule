#ifndef __INCLUDE_MODULE_BASE_H__
#define __INCLUDE_MODULE_BASE_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <algorithm>

#include "GlobalMacro.h"
#include "GMMHeap.h"

typedef enum{
	GlobalFlag_Success					= -0x0,
	GlobalFlag_GeneralError				= -0x1,
	GlobalFlag_IOError					= -0x2,
	GlobalFlag_UnknownError				= -0x3,
	GlobalFlag_Uninitialized			= -0x4,
	GlobalFlag_InvalidArguments			= -0x5
}GlobalFlag;

typedef class AIData{
public:
	int* idxList;
	size_t idxSize;
	double* dataList;
	size_t dataSize;

	AIData();
	AIData(const AIData& A);
	virtual ~AIData();

	const AIData& operator = (const AIData& A);
}*pAIData;


typedef std::function<GlobalFlag(pAIData)> ProcessFunction;

class AIModuleBase;
class AIModuleController;

typedef class AIModuleBase{
	friend class AIModuleController;
protected:
	virtual ~AIModuleBase();
public:
	explicit AIModuleBase(const std::string& name);
	virtual GlobalFlag loadAIData(const pAIData pdata) = 0;
	virtual GlobalFlag processAIData(double dt) = 0;
	virtual GlobalFlag outputAIData(pAIData pdata) = 0;
	virtual GlobalFlag releaseAIData(void) = 0;
	virtual void setProcessFunction(ProcessFunction func) = 0;
}*pAIModuleBase;

class AIModuleController{
	friend class AIModuleBase;
protected:
	static AIModuleController controller;
	std::map<std::string, AIModuleBase*> modules;
	AIModuleController();
	~AIModuleController();
	static AIModuleController& getInstance();
	void addToController(const std::string& name, AIModuleBase* module);
};

typedef class AIPotentialField : public AIModuleBase{
protected:
	pAIData pInternalData;
	bool quickMode;
	ProcessFunction pFunc;
protected:
	virtual ~AIPotentialField();
public:
	explicit AIPotentialField(const std::string& name);
	virtual GlobalFlag loadAIData(const pAIData pdata)override;
	virtual GlobalFlag processAIData(double dt)override;
	virtual GlobalFlag outputAIData(pAIData pdata)override;
	virtual GlobalFlag releaseAIData(void)override;
	virtual void setProcessFunction(ProcessFunction func)override;

	//It does not recommend to disable quick mode
	void setQuickMode(bool b);

}*pAIPotentialField;

typedef enum {regular_trapezoid, left_trapezoid, right_trapezoid} TrapzType;

typedef struct{
   TrapzType type;
   float a,b,c,d,l_slope,r_slope;
}Trapezoid;

typedef struct FuzzyRule{
	int*	inp_index;
	int*	inp_fuzzy_set;
	int		out_fuzzy_set;

	FuzzyRule();
	~FuzzyRule();
}*pFuzzyRule;

typedef class AIFuzzyLogic : public AIModuleBase{
protected:
	bool has_init;
	size_t noInputs;
	size_t noRules;
	size_t noRegions;
	size_t noOutputs;

	float* tmpValues;
	double* output_values;
	pFuzzyRule rules;
	Trapezoid* impMemFns;

	float result;

	pAIData pInternalData;

protected:
	virtual ~AIFuzzyLogic();
public:
	explicit AIFuzzyLogic(const std::string& name);
	
	virtual GlobalFlag loadAIData(const pAIData pdata)override;
	virtual GlobalFlag processAIData(double dt)override;
	virtual GlobalFlag outputAIData(pAIData pdata)override;
	virtual GlobalFlag releaseAIData(void)override;
	virtual void setProcessFunction(ProcessFunction func)override;

	void init(	size_t no_of_input, 
				size_t no_of_rules,
				size_t no_of_regions,
				size_t no_of_outputs,
				
				double outputVals[]
				
			);

	void initFuzzyRules();

	void init(const std::string& fuzzyFileName);

	bool isInit(void)const;
	size_t getInputSize(void)const;

}*pAIFuzzyLogic;


typedef struct AStarSearchNode{
	float gValue;
	float heuristic;

}*pAStarSearchNode;

typedef std::deque<pAStarSearchNode> pathNode;

typedef class AIAStarSearch : public AIModuleBase{
protected:
	pAIData pInternalData;
	bool hasInit;

	pAStarSearchNode startNode;
	pAStarSearchNode goalNode;
    
    std::set<pAStarSearchNode> expandList;
    
    
    
protected:
	~AIAStarSearch();
public:
	explicit AIAStarSearch(const std::string& name);

	virtual GlobalFlag loadAIData(const pAIData pdata)override;
	virtual GlobalFlag processAIData(double dt)override;
	virtual GlobalFlag outputAIData(pAIData pdata)override;
	virtual GlobalFlag releaseAIData(void)override;
	virtual void setProcessFunction(ProcessFunction func)override;

	virtual void init(pAStarSearchNode startNode, pAStarSearchNode goalNode);
}*pAIAStarSearch;
#endif
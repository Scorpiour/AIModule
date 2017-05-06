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

	void clear();
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



//Potential Field

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




//Fuzzy Logic

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




//AStar Search

typedef struct AStarSearchNode{
	float gValue;
	float heuristic;
	Point2F position;
	Point2I coord;

	AStarSearchNode* next;

	AStarSearchNode(){
		gValue = -1.f;
		heuristic = -1.f;
		next = nullptr;
	}
	AStarSearchNode(const AStarSearchNode& node){
		gValue = node.gValue;
		heuristic = node.heuristic;
		position = node.position;
		coord = node.coord;
		next = node.next;
	}

	bool operator < (const AStarSearchNode& node)const{
		return this->coord < node.coord;
	}

	bool operator == (const AStarSearchNode& node){
		return !( (this->coord < node.coord) && ( node.coord < this->coord) ) ;
	}

}AStarNode,*pAStarNode,*pAStarSearchNode;


typedef class AIAStarSearch : public AIModuleBase{
protected:
	pAIData pInternalData;
	bool hasInit;
	uint32_t xlevel;
	uint32_t ylevel;

	pAStarSearchNode startNode;
	pAStarSearchNode goalNode;

protected:
	virtual ~AIAStarSearch();
public:
	explicit AIAStarSearch(const std::string& name);

	virtual GlobalFlag loadAIData(const pAIData pdata)override;
	virtual GlobalFlag processAIData(double dt)override;
	virtual GlobalFlag outputAIData(pAIData pdata)override;
	virtual GlobalFlag releaseAIData(void)override;
	virtual void setProcessFunction(ProcessFunction func)override;

	virtual void init(pAStarSearchNode _startNode, pAStarSearchNode _goalNode,uint32_t _xlevel, uint32_t _ylevel);
}*pAIAStarSearch;


typedef class AIAngularAStar:public AIModuleBase{
protected:
	typedef struct Node{
		Point2F pos;
		Point2F direction;
		double bendArc;
		double actualDist;
		Point2I coord;
		//double heuristic;

		Node* parent;

		explicit Node(const Point2F& _p, const Point2F& _d,const Point2I _c){
			pos = _p;
			direction = _d;
			bendArc = 0.f;
			actualDist = 0.f;
			coord = _c;
			parent = nullptr;
			//heuristic = 0.f;
		}
		Node(const Node& N){
			pos = N.pos;
			direction = N.direction;
			bendArc = N.bendArc;
			actualDist = N.actualDist;
			coord = N.coord;
			parent = N.parent;
			//heuristic = N.heuristic;
		}

		friend bool operator < (const Node& n1, const Node& n2){
			if(n1.coord < n2.coord){
				return true;
			}else if(n1.coord == n2.coord){
				return n1.direction < n2.direction;
			}return false;
		}

		friend bool operator == (const Node& n1, const Node& n2){
			return (!(n1<n2))&&(!(n2<n1)); 
		}

	}*pNode;
protected:
	pAIData pInternalData;
	bool hasInit;
	uint32_t xlevel;
	uint32_t ylevel;

	Point2F startNode;
	Point2F goalNode;
	Point2F initD;
protected:
	virtual ~AIAngularAStar();
public:
	explicit AIAngularAStar(const std::string& name);
	
	virtual GlobalFlag loadAIData(const pAIData pdata);
	virtual GlobalFlag processAIData(double dt);
	virtual GlobalFlag outputAIData(pAIData pdata);
	virtual GlobalFlag releaseAIData(void);
	virtual void setProcessFunction(ProcessFunction func);

	virtual void init(const Point2F& _startNode, const Point2F& _goalNode,const Point2F& initDirec,uint32_t _xlevel, uint32_t _ylevel);



}*pAIAngularAStar;

#endif
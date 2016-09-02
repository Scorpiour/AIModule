#include "ModuleBase.h"
#include "GMMHeap.h"
#include "RigidBody.h"

using namespace std;

AIAStarSearch::AIAStarSearch(const std::string& name):AIModuleBase(name){
	pInternalData = nullptr;
	hasInit = false;
	goalNode = nullptr;
	startNode = nullptr;
}

AIAStarSearch::~AIAStarSearch(){

}

GlobalFlag AIAStarSearch::loadAIData(const pAIData pdata){
	if(this->hasInit){
		pInternalData = pdata;
		pInternalData->clear();
		return GlobalFlag_Success;
	}
	return GlobalFlag_Uninitialized;
}

GlobalFlag AIAStarSearch::processAIData(double dt){
	if(this->hasInit){

		//divide level

		float div_x = abs( ( goalNode->position.x - startNode->position.x ) / (float)(level) ) ;
		float div_y = abs( ( goalNode->position.y - startNode->position.y ) / (float)(level) ) ;

		startNode->coord.x = 0;
		startNode->coord.y = 0;

		goalNode->coord.x = level;
		goalNode->coord.y = level;

		auto reachFunc = [=](Point2F& p1, Point2F& p2)->bool{
			float dx = abs(p1.x - p2.x);
			float dy = abs(p1.y - p2.y);

			if( (dx < div_x/2.f) && (dy < div_y/2.f) ){
				return true;
			}
			return false;
		};

		auto distanceFunc = [=](Point2F& p1, Point2F& p2)->float{
			float dx = abs(p1.x - p2.x);
			float dy = abs(p1.y - p2.y);

			return sqrt(dx*dx + dy*dy);
		};

		Point2F gp;
		gp.x = startNode->position.x;
		gp.y = startNode->position.y;

		float distLevel = RigidController::getInstance().calculateDistanceLevel(gp);

		float distance = distanceFunc(startNode->position, goalNode->position);
			
		startNode->heuristic = 1000/distLevel;
		startNode->gValue = 0.f;

		auto visitNode = startNode;
		std::set<Point2I> expandList;

		GMMPriorityQueue<float, std::deque<pAStarSearchNode>*> searchList;
		
		auto expand = [&](pAStarSearchNode pNode)->bool{
			Point2I pt = pNode->coord;
			size_t sz = expandList.size();
			expandList.insert(pt);
			size_t nsz = expandList.size();

			if(nsz == sz){
				return false;
			}
			return true;
		};

		while(true){

			bool hasVisited = expand(visitNode);
			if(hasVisited){
				continue;
			}

			for(int i=-1;i<=1;i++){
				for(int j=-1;j<=1;j++){

				}
			}
		}
		

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


void AIAStarSearch::init(pAStarSearchNode _startNode, pAStarSearchNode _goalNode,uint32_t _level){
	this->startNode = _startNode;
	this->goalNode = _goalNode;
	this->level = _level;

	if(this->level == 0){
		this->level = 1;
	}

	this->hasInit = true;
}

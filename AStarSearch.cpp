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

		std::set<pAStarSearchNode> nodeBuffer;

		//divide level

		float div_x = abs( ( goalNode->position.x - startNode->position.x ) / (float)(level) ) ;
		float div_y = abs( ( goalNode->position.y - startNode->position.y ) / (float)(level) ) ;

		startNode->coord.x = 0;
		startNode->coord.y = 0;

		goalNode->coord.x = level;
		goalNode->coord.y = level;

		//To Determine if the search has reached certain position
		auto reachFunc = [=](Point2F& p1, Point2F& p2)->bool{
			float dx = abs(p1.x - p2.x);
			float dy = abs(p1.y - p2.y);

			if( (dx < div_x/2.f) && (dy < div_y/2.f) ){
				return true;
			}
			return false;
		};

		//To Calculate the distance between two points
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

		auto initNode = new AStarSearchNode(*startNode);
		nodeBuffer.insert(initNode);

		std::set<Point2I> expandList;

		GMMPriorityQueue<float, std::deque<pAStarSearchNode>*> searchList;
		
		//Expand a node to expansion list
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

		typedef std::deque<pAStarSearchNode> SearchNode, *pSearchNode;

		//Calculate the key value of path
		auto calculateKey = [&](pSearchNode ps)->float{
			if(ps->empty()){
				return FLT_MAX;
			}

			float result = 0.f;
			result += ps->front()->heuristic;

			for(auto& ptr : *ps){
				result += ptr->gValue;
			}

			return result;
		};

		pSearchNode currentNode = new SearchNode;
		currentNode->push_front(initNode);
		
		float key = calculateKey(currentNode);
		searchList.join(key,currentNode);
		
		pSearchNode result = nullptr;

		while(true){

			if(searchList.empty()){
				break;
			}

			//Step 1 : pop the first in priority queue, which has the smallest value
			auto first = searchList.front();
			searchList.leave();
			
			auto firstNode = first->front();

			if(expand(firstNode)){

				
				
				delete first;
			}else{
				//Expand fail, this node has been visited already
				//don't delete AStarSearchNode.
				delete first;
				continue;
			}

		}
		
		for(auto& ptr : nodeBuffer){
			delete ptr;
		}
		nodeBuffer.clear();

		expandList.clear();

		while(!searchList.empty()){
			auto ptr = searchList.front();
			searchList.leave();
			delete ptr;
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

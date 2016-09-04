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

		//To Calculate the distance between two points
		auto distanceFunc = [=](Point2F& p1, Point2F& p2)->float{
			float dx = abs(p1.x - p2.x);
			float dy = abs(p1.y - p2.y);

			return sqrt(dx*dx + dy*dy);
		};

		//Calculate the Heuristic Function : the distance between two points plus the potential value
		auto heuristicFunc = [=](Point2F& p1, Point2F& p2)->float{

			float distLevel = RigidController::getInstance().calculateDistanceLevel(p1);
			
			if(distLevel < 0.f){
				return -1.f;
			}
			distLevel = 100/(1+exp(3*distLevel-5));
			
			float distance = distanceFunc(p1,p2);

			float result = distLevel + distance;
			return result;
		};
			
		startNode->heuristic = heuristicFunc(startNode->position, goalNode->position);
		startNode->gValue = 0.f;

		auto initNode = new AStarSearchNode(*startNode);
		nodeBuffer.insert(initNode);

		std::set<Point2I> expandList;

		

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

		GMMPriorityQueue<float, pSearchNode> searchList;
		std::set<pSearchNode> searchNodeBuffer;

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
		searchNodeBuffer.insert(currentNode);

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

			//Reach Goal
			/*
			if(reachFunc(firstNode->position, goalNode->position)){
				result = first;
				break;
			}*/
			if(firstNode->coord == goalNode->coord){
				result = first;
				break;
			}


			for(int i=-1;i<=1;i++){
				for(int j=-1;j<=1;j++){
					if(!i && !j){
						continue;
					}

					Point2I nextCoord = firstNode->coord;
					nextCoord.x += j;
					nextCoord.y += i;

					auto iter = expandList.find(nextCoord);
					if(iter != expandList.end()){
						continue;
					}

					auto nextNode = new AStarSearchNode;			

					nextNode->coord = nextCoord;
					float gx = nextCoord.x * div_x;
					float gy = nextCoord.y * div_y;
					nextNode->gValue += sqrt(gx*gx + gy*gy);
					nextNode->position.x = startNode->position.x + nextCoord.x*div_x;
					nextNode->position.y = startNode->position.y + nextCoord.y*div_y;
					
					
					if( abs(nextNode->position.x) > 250 || abs(nextNode->position.y) > 200){
						delete nextNode;
						continue;
					}

					nextNode->heuristic = heuristicFunc(nextNode->position,goalNode->position);

					if(nextNode->heuristic < 0){
						delete nextNode;
						continue;
					}

					expandList.insert(nextCoord);

					nodeBuffer.insert(nextNode);
					pSearchNode nextSearchNode = new SearchNode(*first);
					searchNodeBuffer.insert(nextSearchNode);
						
					nextSearchNode->push_front(nextNode);

					float key = calculateKey(nextSearchNode);

					searchList.join(key,nextSearchNode);
				}
			}
				
			

		}
		
		//output result
		if(result != nullptr){

			pInternalData->idxSize = result->size();
			pInternalData->dataSize = result->size() * 2;
			pInternalData->dataList = new double[pInternalData->dataSize];


			size_t i = 0;
			for(auto& ptr : *result){
				pInternalData->dataList[i*2] = ptr->position.x;
				pInternalData->dataList[i*2+1] = ptr->position.y;
				i++;
			}
		}

		expandList.clear();

		for(auto& ptr : nodeBuffer){
			delete ptr;
		}

		for(auto& ptr : searchNodeBuffer){
			delete ptr;
		}

		nodeBuffer.clear();

		searchList.clear();


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

#include <ctime>
#include <cmath>

#include "ModuleBase.h"
#include "GMMHeap.h"
#include "RigidBody.h"

using namespace std;

AIAngularAStar::AIAngularAStar(const std::string& name):AIModuleBase(name){
	pInternalData = nullptr;
	hasInit = false;
}

AIAngularAStar::~AIAngularAStar(){

}

GlobalFlag AIAngularAStar::loadAIData(const pAIData pdata){
	if(this->hasInit){
		pInternalData = pdata;
		pInternalData->clear();
		return GlobalFlag::GlobalFlag_Success;
	}
	return GlobalFlag_Uninitialized;
}

GlobalFlag AIAngularAStar::processAIData(double dt){
	if(!(this->hasInit)){
		return GlobalFlag::GlobalFlag_Uninitialized;
	}
	float checkdist = RigidController::getInstance().calculateDistanceLevel(goalNode);
	if(checkdist < 0.f){

		cout<<"Target in obstacle : "<<goalNode.x<<','<<goalNode.y<<endl;
		pInternalData->idxSize = 1;
		return GlobalFlag_GeneralError;
	}

	float div_x =  ( goalNode.x - startNode.x ) / (float)(xlevel);
	float div_y =  ( goalNode.y - startNode.y ) / (float)(ylevel);
	
	float map_x;
	float map_y;
	float map_coe;
	if(abs(div_x) > abs(div_y)){
		map_y = M_PI;
		map_coe = M_PI/abs(div_y);
		map_x = abs(div_x)*map_coe;
	}else{
		map_coe = M_PI/abs(div_x);
		map_x = M_PI;
		map_y = abs(div_y)*map_coe;
	}

	int xmax = 220/abs(div_x);
	int ymax = 180/abs(div_y);

	Point2I checkGoal(xlevel,ylevel);


	std::set<Node> expandList;
	GMMPriorityQueue<float, pNode> searchQueue;
	std::set<pNode> nodeBuffer;
	//std::map<Node,float> hMap;

	auto distanceFunc = [=](Point2F& p1, Point2F& p2)->float{
		float dx = p1.x - p2.x;
		float dy = p1.y - p2.y;

		return sqrt(dx*dx + dy*dy);
	};

	auto generalizeVector = [=](const Point2F& p)->Point2F{	
		if(!p){
			return p;
		}		
		float r = sqrt(p.x * p.x + p.y * p.y);
		return Point2F(p.x/r,p.y/r);
	};

	auto arcBetweenVector = [=](const Point2F& _u, const Point2F& _v)->float{
		Point2F u(generalizeVector(_u));
		Point2F v(generalizeVector(_v));
		return GlobalVariables::Acos(u.x*v.x + u.y*v.y);
	};

	auto heuristicFunc = [=](const Node& n, const Point2F& goal)->float{
		float dy = goal.y - n.pos.y;
		float dx = goal.x - n.pos.x;

		return arcBetweenVector(Point2F(dx,dy),n.direction);
	};

	auto makeKey = [=](const Node& n, const Point2F& goal)->float{

		return n.actualDist + n.bendArc + heuristicFunc(n,goal);
	};


	pNode firstNode = new Node(startNode,initD,Point2I(0,0));
	firstNode->bendArc = 0;
	//firstNode->actualDist = 0;
	
	float distLevel = RigidController::getInstance().calculateDistanceLevel(startNode);
	distLevel = map_coe * 200.f/(1+exp(distLevel-7));
	firstNode->actualDist = distLevel;
	
	nodeBuffer.insert(firstNode);

	float key = makeKey(*firstNode,goalNode);

	searchQueue.join(key,firstNode);

	pNode result = nullptr;

	clock_t stime = clock();

	while(!searchQueue.empty()){
		
		auto M = searchQueue.front();
		searchQueue.leave();

		if(M->coord == checkGoal){
			result = M;
			break;
		}

		if(expandList.end() != expandList.find(*M)){
			continue;
		}
		expandList.insert(*M);

		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(!i&&!j){
					continue;
				}

				Point2I nextCoord(M->coord.x + j, M->coord.y + i);
				if(abs(nextCoord.x) > xmax || abs(nextCoord.y)>ymax){
					continue;
				}
				Point2F nextPosition(startNode.x + nextCoord.x*div_x,startNode.y + nextCoord.y*div_y);
				if(abs(nextPosition.x) > 125){
					continue;
                }else if(abs(nextPosition.y)>90){
                    continue;
                }else if(abs(nextPosition.x) > 110 && abs(nextPosition.y) > 15){
                    continue;
                }
				//Line_Of_Sight
				pNode P = M;
				if(M->parent !=nullptr){
					pNode tmpP = M->parent;
					if(RigidController::getInstance().checkLineOfSight(tmpP->pos,nextPosition)){
						P = tmpP;
					}
				}


				distLevel = RigidController::getInstance().calculateDistanceLevel(nextPosition);
				if(distLevel < 0.f){
					continue;
				}


				float dy = nextPosition.y - P->pos.y;
				float dx = nextPosition.x - P->pos.x;	
				Point2F v(dx,dy);

				Node tmpNode(nextPosition,v,nextCoord);
				tmpNode.bendArc = arcBetweenVector(P->direction,v);
				
				dx = map_x*(P->coord.x - tmpNode.coord.x);
				dy = map_y*(P->coord.y - tmpNode.coord.y);
				float dist = sqrt(dx*dx + dy*dy); //with LOS, use parent coord diff & map_x/map_y
				
				tmpNode.actualDist = P->actualDist + dist;
				tmpNode.parent = P;

				if(expandList.end() != expandList.find(tmpNode)){
					continue;
				}

				
				distLevel = map_coe * 200.f/(1+exp(distLevel-7));
				tmpNode.actualDist += distLevel;

				auto nextNode = new Node(tmpNode);
				float key = makeKey(*nextNode,goalNode);
				nodeBuffer.insert(nextNode);

				searchQueue.join(key,nextNode);

			}
		}

	}

	cout<<"Time spend of Angular A* with LOS :"<<clock() - stime<<"ms"<<endl;

	if(nullptr != result){
		cout<<"\t- Visited Node :\t"<<nodeBuffer.size()<<endl;
		cout<<"\t- Expand Node :\t"<<expandList.size()<<endl;

		size_t size = 0;

		auto ptr = result;
		while(ptr != nullptr){
			ptr = ptr->parent;
			size++;
		}
		cout<<"\t- Path length :\t"<<size<<endl;

		pInternalData->idxSize = size;
		pInternalData->dataSize = size*2;
		pInternalData->dataList = new double[size*2];

		ptr = result;

		for(size_t i=0;i<size;i++){
			pInternalData->dataList[i*2] = ptr->pos.x;
			pInternalData->dataList[i*2+1] = ptr->pos.y;
			ptr = ptr->parent;
		}
	}

	for(auto ptr : nodeBuffer){
		delete ptr;
	}

	return GlobalFlag::GlobalFlag_Success;
}

GlobalFlag AIAngularAStar::outputAIData(pAIData pdata){
	if(this->hasInit){

		return GlobalFlag::GlobalFlag_Success;
	}
	return GlobalFlag_Uninitialized;
}

GlobalFlag AIAngularAStar::releaseAIData(void){
	pInternalData = nullptr;
	return GlobalFlag::GlobalFlag_Success;
}

void AIAngularAStar::setProcessFunction(ProcessFunction func){

}


void AIAngularAStar::init(const Point2F& _startNode, const Point2F& _goalNode,const Point2F& initDirec,uint32_t _xlevel, uint32_t _ylevel){
	this->startNode = _startNode;
	this->goalNode = _goalNode;
	this->xlevel = max(static_cast<uint32_t>(1),_xlevel);
	this->ylevel = max(static_cast<uint32_t>(1),_ylevel);
	this->initD = initDirec;
	this->hasInit = true;
}

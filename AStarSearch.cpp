#include <ctime>
#include <cmath>

#include "ModuleBase.h"
#include "GMMHeap.h"
#include "RigidBody.h"

using namespace std;

AIAStarSearch::AIAStarSearch(const std::string& name,double marg):AIModuleBase(name){
	pInternalData = nullptr;
	hasInit = false;
	goalNode = nullptr;
	startNode = nullptr;
    margin = marg;
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

        bool magFlag = true;
        
		bool useLOS = dt>1?true:false;
		string losStr = useLOS?" Theta* ":" A* ";
		//clock_t c = clock();

		float checkdist = RigidController::getInstance().calculateDistanceLevel(goalNode->position);
		if(checkdist < 0.f){

			cout<<"Target in obstacle : "<<goalNode->position.x<<','<<goalNode->position.y<<endl;
			pInternalData->idxSize = 1;
			return GlobalFlag_GeneralError;
		}

		float div_x =  ( goalNode->position.x - startNode->position.x ) / (float)(xlevel)  ;
		float div_y =  ( goalNode->position.y - startNode->position.y ) / (float)(ylevel)  ;
        //float div_x = (goalNode->position.x - startNode->position.x);
        //float div_y = (goalNode->position.y - startNode->position.y);
        //div_x = div_x / fabs(div_x);
        //div_y = div_y / fabs(div_y);
        //div_x *= 1;
        //div_y *= 1;
		float div_z = sqrt(div_x*div_x + div_y*div_y);

		int xmax = 220/abs(div_x);
		int ymax = 180/abs(div_y);

		startNode->coord = Point2I(0,0);
		goalNode->coord = Point2I(xlevel,ylevel);


		std::set<Point2I> expandList;
		
		GMMPriorityQueue<float,pAStarNode> searchQueue;

		std::set<pAStarNode> nodeBuffer;

		auto distanceFunc = [=](Point2F& p1, Point2F& p2)->float{
			float dx = abs(p1.x - p2.x);
			float dy = abs(p1.y - p2.y);

			return sqrt(dx*dx + dy*dy);
		};
        auto magnitudeFunc = [=](float distLevel)->float{
            //float distLevel = RigidController::getInstance().calculateDistanceLevel(pt);
            if(distLevel < 1.f){
                distLevel = 1.f;
            }
            distLevel = 200.f/(1+exp(distLevel-7));
            return distLevel;
        };
		//Calculate the Heuristic Function : the distance between two points plus the potential value
		auto heuristicFunc = [=](Point2F& p1, Point2F& p2,float magLevel)->float{

			float distance = distanceFunc(p1,p2);
            if(magFlag){
                //float distLevel = magnitudeFunc(p1);
                float result = sqrt(distance*distance + magLevel*magLevel);
                return result;
            }else{
                float result = distance;// + distLevel;
                return result;
            }
        };

		std::map<Point2I,float> hMap;

        float rawDistLevel = RigidController::getInstance().calculateDistanceLevel(startNode->position);
		float magLevel = magnitudeFunc(rawDistLevel);
        //RigidController::getInstance().calculateDistanceLevel(startNode->position);
        //hMap.insert(make_pair(startNode->coord,distLevel));
		//distLevel = 200.f/(1+exp(distLevel-7));

		pAStarNode firstNode = new AStarNode(*startNode);
		firstNode->next = nullptr;
        if(magFlag){
            firstNode->gValue = magLevel;
        }else{
            firstNode->gValue = 0;
        }
        firstNode->magnitude = magLevel;
		firstNode->heuristic = heuristicFunc(firstNode->position,goalNode->position,magLevel);
		hMap.insert(make_pair(firstNode->coord,firstNode->heuristic));

		nodeBuffer.insert(firstNode);
		float key = firstNode->gValue + firstNode->heuristic;
		
		searchQueue.join(key,firstNode);

		pAStarNode result = nullptr;

		clock_t stime = clock();

		while(!searchQueue.empty()){
			auto currentNode = searchQueue.front();
			searchQueue.leave();

			if(currentNode->coord == goalNode->coord){
				result = currentNode;
				break;
			}

			//Expand
			if(expandList.end() != expandList.find(currentNode->coord)){
				continue;
			}
			
			expandList.insert(currentNode->coord);
			
            float curMag = 0;
            if(magFlag){
                curMag = currentNode->magnitude;
            }
			for(int i=-1;i<2;i++){
				for(int j=-1;j<2;j++){

					if(!i && !j){
						continue;
					}

					Point2I nextCoord;
					nextCoord.x = currentNode->coord.x +j;
					nextCoord.y = currentNode->coord.y + i;
					
					//ignore the waypoint out of range
					if((abs(nextCoord.x) > xmax)||abs(nextCoord.y) > ymax){
						continue;
					}

					//if the coord has been expanded, it means it belongs to a path that has been searched
					if(expandList.end() != expandList.find(nextCoord)){
						continue;
					}

					Point2F nextPosition;
					nextPosition.x = startNode->position.x + nextCoord.x*div_x;
					nextPosition.y = startNode->position.y + nextCoord.y*div_y;

                    
                    rawDistLevel = RigidController::getInstance().calculateDistanceLevel(nextPosition);
                    
                    if(rawDistLevel < margin){
                        //delete nextNode;
                        continue;
                    }
                    
                    
                    if(abs(nextPosition.x) > 125){
						continue;
                    }else if(abs(nextPosition.y)>90){
                        continue;
                    }else if(abs(nextPosition.x) > 110 && abs(nextPosition.y) > 15){
                        continue;
                    }

					pAStarNode P = currentNode;
					auto nextNode = new AStarNode;
					float nextG = 0.f;
					
					if(useLOS){
						if(currentNode->next != nullptr){
							pAStarNode tmpP = currentNode->next;
							if(RigidController::getInstance().checkLineOfSight(tmpP->position,nextPosition)){
								P = tmpP;
							}
						}
					
						float dx = P->position.x - nextPosition.x;
						float dy = P->position.y - nextPosition.y;
						nextG = sqrt(dy*dy + dx*dx);
					}else{
						if(!i){
							nextG = abs(div_y);
						}else if(!j){
							nextG = abs(div_x);
						}else{
							nextG = div_z;
						}
					}
                    
                    if(magFlag){
                        float nextMag = magnitudeFunc(nextPosition);
                        float difMag = nextMag - curMag;
                        nextG += difMag;
						/*
						if(difMag <= 0){
							nextG += difMag;
						}else{
							nextG = sqrt(nextG*nextG + difMag*difMag);
						}*/
						//nextG = sqrt(nextG*nextG + difMag*difMag);
                        /*
						difMag = abs(difMag);
						if(nextMag < curMag){
                            nextG = nextG-difMag;
                        }else{
							nextG = nextG+difMag;
						}*/
                        nextNode->magnitude = nextMag;
                    }
                    
                    nextNode->gValue = P->gValue + nextG;
                    nextNode->coord = nextCoord;
                    nextNode->position = nextPosition;
             

					/*else if(distLevel < 1.f){
                        distLevel = 1.f;
                    }*/
                    
                    //distLevel = 200.f/(1+exp(distLevel-7));
                    
                    //nextNode->gValue += distLevel;

                    /*
					auto iter = hMap.find(nextNode->coord);
					if(iter != hMap.end()){
						nextNode->heuristic = iter->second;
					}else{
						nextNode->heuristic = heuristicFunc(nextNode->position,goalNode->position);
						hMap.insert(make_pair(nextNode->coord,nextNode->heuristic));
					}*/
					auto iter = hMap.find(nextNode->coord);
					if(iter != hMap.end()){
						nextNode->heuristic = iter->second;
					}else{
						nextNode->heuristic = heuristicFunc(nextNode->position,goalNode->position,nextNode->magnitude);
						hMap.insert(make_pair(nextNode->coord,nextNode->heuristic));
					}
                    
                    if(nextNode->heuristic < 0){
                        delete nextNode;
                        continue;
                    }
                    
					nextNode->next = P;
					nodeBuffer.insert(nextNode);

					float key;

					if(nextNode->heuristic == FLT_MAX){
						key = FLT_MAX;
					}else{
						key = nextNode->gValue + nextNode->heuristic;
					}
					searchQueue.join(key,nextNode);
				}
			}
		}

		cout<<"Time spend of "<<losStr<<clock() - stime<<"ms"<<endl;
		if(nullptr != result){

			cout<<"\t- Visited Node :\t"<<nodeBuffer.size()<<endl;
			cout<<"\t- Expand Node :\t"<<expandList.size()<<endl;

			size_t size = 0;

			auto ptr = result;
			while(ptr != nullptr){
				ptr = ptr->next;
				size++;
			}
			
			pInternalData->idxSize = size;
			pInternalData->dataSize = size*2;
			pInternalData->dataList = new double[size*2];

			ptr = result;
			double pl = 0.f;
			for(size_t i=0;i<size;i++){
				pInternalData->dataList[i*2] = ptr->position.x;
				pInternalData->dataList[i*2+1] = ptr->position.y;
				ptr = ptr->next;
			}
            
			for(size_t i=1;i<size;i++){
				size_t k = i-1;
				double x1 = pInternalData->dataList[k*2];
				double y1 = pInternalData->dataList[i*2];
				double x2 = pInternalData->dataList[k*2+1];
				double y2 = pInternalData->dataList[i*2+1];

				double dx = x1-x2;
				double dy = y1-y2;
				double r = sqrt(dx*dx+dy*dy);
				pl += r;

			}

			cout<<"\t- Path length :\t"<<pl<<endl;

            //validate result and find where the issue it might be
            
            ptr = result;
            
            for(size_t i = 0; i < size; i++){
                float checkDist = RigidController::getInstance().calculateDistanceLevel(result->position);
                if(checkDist < 0){
                    cout<<"Validate Fail at waypoint "<<i<<" pos "<<result->position.x<<':'<<result->position.y<<endl;
                }
                result = result->next;
                
            }
            
			//cout<<"Path found : "<<size<<" steps"<<endl;

		}else{
			pInternalData->idxSize = 1;
			//cout<<"No Result Found"<<endl;

		}

		for(auto ptr : nodeBuffer){
			delete ptr;
		}

		/*
		std::set<pAStarSearchNode> nodeBuffer;

		//divide level

		float div_x = abs( ( goalNode->position.x - startNode->position.x ) / (float)(level) ) ;
		float div_y = abs( ( goalNode->position.y - startNode->position.y ) / (float)(level) ) ;
		float div_z = sqrt(div_x*div_x + div_y*div_y);

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
			if(distLevel < 1.f){
				distLevel = 1.f;
			}
			//distLevel = 5000/(1+exp(3*distLevel-5));
			distLevel = 10000.f/distLevel;

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
			
			//if(reachFunc(firstNode->position, goalNode->position)){
				//result = first;
				//break;
			//}
			if(firstNode->coord == goalNode->coord){
				result = first;
				break;
			}

			auto iter = expandList.find(firstNode->coord);
			if(iter != expandList.end()){
				continue;
			}

			expandList.insert(firstNode->coord);

			for(int i=-1;i<=1;i++){
				for(int j=-1;j<=1;j++){
					if(!i && !j){
						continue;
					}

					Point2I nextCoord = firstNode->coord;
					nextCoord.x += j;
					nextCoord.y += i;

					iter = expandList.find(nextCoord);
					if(iter != expandList.end()){
						continue;
					}

					auto nextNode = new AStarSearchNode;			

					nextNode->coord = nextCoord;

					if(!i){
						nextNode->gValue = firstNode->gValue + div_y;
					}else if(!j){
						nextNode->gValue = firstNode->gValue + div_x;
					}else{
						nextNode->gValue = firstNode->gValue + div_z;
					}

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

		*/

		//cout<<"Total Timespend of A* : "<<clock()-c<<"ms"<<endl;

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


void AIAStarSearch::init(pAStarSearchNode _startNode, pAStarSearchNode _goalNode,uint32_t _xlevel,uint32_t _ylevel){
	this->startNode = _startNode;
	this->goalNode = _goalNode;
	this->xlevel = max(static_cast<uint32_t>(1),_xlevel);
	this->ylevel = max(static_cast<uint32_t>(1),_ylevel);



	this->hasInit = true;
}

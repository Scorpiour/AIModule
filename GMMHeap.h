//
//  GMMHeap.h
//  159709
//
//  Created by Chen DONG on 22/08/16.
//  Copyright © 2016 Chen DONG. All rights reserved.
//

#ifndef GMMHeap_h
#define GMMHeap_h

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <exception>

template <typename K,typename V>
class GMMPriorityQueue{
public:
    GMMPriorityQueue(){
        
    }
    
    ~GMMPriorityQueue(){
        
    }
    
    size_t size(){
        return this->elements.size();
    }
    
    bool empty(){
        return this->elements.empty();
    }
    
    void join(const std::pair<K,V>& p){
        elements.push_back(p);
        size_t cur = elements.size()-1;
        while(cur != 0){
            size_t pIdx = (cur-1)/2;
            if(elements[pIdx].first > elements[cur].first){
                std::swap(elements[pIdx],elements[cur]);
                cur = pIdx;
            }else{
                break;
            }
        }
    }
    
    void join(const K& key,const V& value){
        elements.push_back(std::make_pair(key,value));
        size_t cur = elements.size()-1;
        while(cur != 0){
            size_t pIdx = (cur-1)/2;
            if(elements[pIdx].first > elements[cur].first){
                std::swap(elements[pIdx],elements[cur]);
                cur = pIdx;
            }else{
                break;
            }
        }
    }
    


    void leave(void){
        size_t tail = elements.size()-1;
        size_t head = 0;
        std::swap(elements[tail],elements[head]);
        elements.pop_back();
		if(elements.empty()){
			return;
		}
        tail--;
        
        size_t left;
        size_t right;
        size_t cur;
        
        while(true){
            left = head*2+1;
            right = head*2+2;
            
            if(left > tail){
                break;
            }else if(right > tail){
                cur = left;
            }else{
                if(elements[left].first < elements[right].first){
                    cur = left;
                }else{
                    cur = right;
                }
            }
            
            if(elements[head].first < elements[cur].first){
                break;
            }
            
            std::swap(elements[head],elements[cur]);
            head = cur;
        }
    }
    
    const V& front(){
        if(elements.empty()){
            throw std::exception();
        }
        return elements[0].second;
    }

	const K& frontKey(){
		if(elements.empty()){
			throw std::exception();
		}
		return elements[0].first;
	}

	void traversal(std::function<void(K,V)> callback){
		for(auto & p : elements){
			callback(p.first, p.second);
		}
	}

	void clear(){
		elements.clear();
	}
    
protected:
    std::vector<std::pair<K,V>> elements;
    
};



#endif /* GMMHeap_h */

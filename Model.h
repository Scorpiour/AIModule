#ifndef __INCLUDE_MODEL_H__
#define __INCLUDE_MODEL_H__

#include "Sprite.h"

#include <string>
#include <string>

class Model{
public:
	Model();
	virtual ~Model();


	const std::string& getModelName()const;

protected:
	std::string modelName;
	std::set<Sprite*> spriteList;
};


#endif

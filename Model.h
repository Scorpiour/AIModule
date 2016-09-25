#ifndef __INCLUDE_MODEL_H__
#define __INCLUDE_MODEL_H__

#include "Sprite.h"

#include <string>
#include <string>

class Model{
public:
	explicit Model(const std::string& _name);
	virtual ~Model();


	const std::string& getModelName()const;

	void addSprite(Sprite* _sp);

	void enable(bool _e);
	void visible(bool _v);

	void swithActive();

	bool getActive()const;
	void setActive(bool _a);
protected:
	std::string modelName;
	std::set<Sprite*> spriteList;
	bool active;
};


#endif

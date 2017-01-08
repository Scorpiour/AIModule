#ifndef __INCLUDE_MODEL_H__
#define __INCLUDE_MODEL_H__

#include "Sprite.h"

#include <string>
#include <string>

class Model{
public:
	explicit Model(const std::string& _name);
    explicit Model(const std::string& _name,const Point2F& robot,const Point2F& ball);
    explicit Model(const std::string& _name,Point2F && robot,Point2F && ball);
	virtual ~Model();


	const std::string& getModelName()const;

	void addSprite(Sprite* _sp);

	void enable(bool _e);
	void visible(bool _v);

	void swithActive();

	bool getActive()const;
	void setActive(bool _a);
    
    void setRobotPos(const Point2F& pos);
    void setBallPos(const Point2F& pos);
    
    void setRobotPos(Point2F && pos);
    void setBallPos(Point2F && pos);
    
    const Point2F& getRobotPos(void)const;
    const Point2F& getBallPos(void)const;
protected:
	std::string modelName;
	std::set<Sprite*> spriteList;
	bool active;
    Point2F ballPos;
    Point2F robotPos;
};


#endif

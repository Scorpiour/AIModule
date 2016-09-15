#ifndef __INCLUDE_RIGIDBODY_H__
#define __INCLUDE_RIGIDBODY_H__



#include <iostream>
#include <fstream>
#include <cmath>
#include <set>
#include <deque>
#include <vector>
#include <functional>
#include <algorithm>
#include "GlobalMacro.h"

typedef class RigidBody{
protected:
	bool visible;
	bool movable;
	bool enabled;

	float sx;
	float sy;
	int id;

	Point2F collisionForce;

	std::function<bool(RigidBody*,Point2F&)> forceFunc;
public:
	static const float speedlimit;
	float mass;
public:
	RigidBody();
	virtual ~RigidBody();
	virtual float getX()const=0;
	virtual float getY()const=0;
	virtual void setSX(float)=0;
	virtual void setSY(float)=0;
	virtual float getSX()const=0;
	virtual float getSY()const=0;
	virtual float getRadius()const=0;

	virtual void move(double)=0;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) = 0;
	virtual float calculateDistance(const Point2F& point, float rad) = 0;
	
	virtual void setForceFunc(std::function<bool(RigidBody*,Point2F&)> _forceFunc);
	virtual bool processForceFunc(RigidBody*,Point2F&);
	virtual void removeForceFunc();

	virtual void setCollisionForce(float fx,float fy);
	virtual void setID(int _id);
	virtual int getID()const;

	virtual void setVisible(bool isVisible);
	virtual bool isVisible()const;

	virtual void setMovable(bool isMovable);
	virtual bool isMovable()const;

	virtual void setEnable(bool enable);
	virtual bool isEnabled()const;
}*pRigidBody;


typedef class RigidController{
	friend class RigidBody;
protected:

	RigidController();
	~RigidController();

	void addToController(RigidBody* body);
	void removeFromController(RigidBody* body);
public:
	static RigidController controller;
	std::set<RigidBody*> rigids;
	static RigidController& getInstance();
public:
	void calculateCollisionForce(double dt);
	float calculateDistanceLevel(const Point2F& pt);

}*pRigidController;


typedef class VirtualAttractivePoint:public RigidBody{
public:

	VirtualAttractivePoint();
	virtual ~VirtualAttractivePoint();


	virtual float getX()const override;
	virtual float getY()const override;
	virtual void setSX(float) override;
	virtual void setSY(float) override;
	virtual float getSX()const override;
	virtual float getSY()const override;
	virtual float getRadius()const override;

	virtual void move(double) override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual float calculateDistance(const Point2F& point, float rad) override;

	virtual void setPosition(const Point2F& pos);

protected:
	Point2F position;
}*pVirtualAttractivePoint;

#endif
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

enum class RigidTypeID{
    RigidType_Ball          = 0,
    RigidType_Robot         = 1,
    RigidType_Keeper        = 2,
    
    RigidType_Unknown       = -1,
    RigidType_Obstacle      = -2,
    RigidType_VirtualObject = -4,
    
};

typedef class RigidBody{
protected:
	bool movable;

	float sx;
	float sy;
	RigidTypeID id;

	Point2F collisionForce;
	bool activeDistance;

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
	virtual bool calculateVirtualForce(RigidBody* dest, Point2F& result,double dt)=0;
	virtual float calculateDistance(const Point2F& point, float rad) = 0;
	
	virtual void setForceFunc(std::function<bool(RigidBody*,Point2F&)> _forceFunc);
	virtual bool processForceFunc(RigidBody*,Point2F&);
	virtual void removeForceFunc();

	virtual void setCollisionForce(float fx,float fy);
	virtual void setID(RigidTypeID _id);
	virtual RigidTypeID getID()const;
	virtual void activeDistanceCalculate(bool b);


	virtual void setMovable(bool isMovable);
	virtual bool isMovable()const;

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
	void calculateVirtualForce(RigidBody* dest, Point2F& result, double dt);
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
	virtual bool calculateVirtualForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual float calculateDistance(const Point2F& point, float rad) override;

	virtual void setPosition(const Point2F& pos);
	virtual const Point2F& getPosition()const;

protected:
	Point2F position;
}*pVirtualAttractivePoint;

#endif

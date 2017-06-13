#ifndef __INCLUDE_SPRITE_H__
#define __INCLUDE_SPRITE_H__

#include <set>
#include <ctime>

#include "Camera.h"
#include "RigidBody.h"
#include "ModuleBase.h"
#include "GlobalVariable.h"

class SpriteManager;

typedef class Sprite{
	friend class SpriteManager;
protected:

	pShaderProgram program;
	pCamera camera;
	GLuint vao;

	glm::vec3 colourValue;		//color RGB
	glm::vec3 colourIntensity;	//Ambient, Diffuse, Specular
	glm::vec3 position;
	glm::vec3 angles;
	
	GLfloat modelMatrix[16];
	GLfloat rotateMatrix[16];
	GLfloat scaleMatrix[16];
	double innerRadius;
    double outerRadius;
	bool isVisible;
	bool isEnable;
protected:
	AIModuleBase* activeModule;
	int activeModuleIndex;
	std::map<std::string,AIModuleBase*> modules;
public:
	Sprite();
	virtual ~Sprite();
	virtual void update(double dt) = 0;
	virtual void draw(double dt) = 0;

	virtual void setProgram(pShaderProgram _program);
	virtual void setVAO(GLuint _vao);
	virtual void setCamera(pCamera _camera);
	virtual void setColour(glm::vec3 _colour);
	virtual void setColourIntensity(glm::vec3 _intensity);
	
	virtual void setPosition(glm::vec3 _pos);
	virtual void setScale(glm::vec3 _scale);
	virtual void setAngle(glm::vec3 _angle);
	virtual void setRadius(double r);

	//DO NOT DELETE ANY AIMODULES, JUST NEW THEM AND USE DIRECTLY
	virtual void addModule(const std::string& name, AIModuleBase* pModule);
	virtual void removeModule(const std::string& name);
	virtual void activeAIModule(const std::string& name,int idx);

	virtual void visible(bool _v);
	virtual void enable(bool _e);

}*pSprite;

class SpriteManager{
protected:
	static SpriteManager manager;

	std::set<pSprite> spriteList;

	SpriteManager();
	virtual ~SpriteManager();
public:
	static SpriteManager* getInstance();
	
	//Add a sprite to the manager. The sprites will be added to the manager automatically when constructed
	void addToManager(pSprite p);

	//Remove a sprite from the manager, the removed sprite will be deleted permanently as well!
	void removeFromManager(pSprite & p);

	void updateAll(double dt);
	void drawAll(double dt);
};



typedef class GridView : public Sprite{
    friend class SpriteManager;
protected:
    virtual ~GridView(void);
public:
    explicit GridView(size_t _xlevel,size_t _ylevel,float _xdist,float _ydist);
    
    void update(double dt) override;
    void draw(double dt) override;
    
    void updateGrid(size_t _xlevel,size_t _ylevel,float _xdist,float _ydist);
protected:
    size_t xlevel;
    size_t ylevel;
    float xdist;
    float ydist;
}*pGridView;

enum TrailType{
	TrailType_Line,
	TrailType_Dot
};

typedef class TrailPoints : public Sprite{
	friend class SpriteManager;
protected:
	TrailType type;
	~TrailPoints(void);
public:
    TrailPoints(TrailType _t,size_t max_count);
    
    void update(double dt) override;
    void draw(double dt) override;
    
    void addPoint(const Point2F& pt);
	void removeFrontPoint();
	void removeBackPoint();
	const Point2F& getFrontPoint()const;
	const Point2F& getBackPoint()const;

	bool isNearGoal(double px, double py, double r);
	Point2F getNearestWaypointandCut(double px, double py);

	size_t size()const;
    void clear();
protected:
    size_t maxCount;
    std::deque<Point2F> points;
}*pTrailPoints;

class Robot : public Sprite,public RigidBody{
	friend class SpriteManager;
protected:
	float width;
	float length;

	bool thetaStarHint;

	AIData data;
    
    pTrailPoints pts;
	pTrailPoints path;

	pVirtualAttractivePoint pTargetPoint;

	Point2F virtualForce;

	float maxSpeed;

	bool inTouch;
	int touchCount;

	bool resetPath;
	bool appendPath;
	//for FSM
	//status list:
	/*
		- 0: init status
		- 1: not in kicking pos
		- 2: try kicking
	*/
    enum class AttackerStatus{
        Attacker_Init = 0,
        Attacker_PersueKickingPos = 1,
        Attacker_TryKicking = 2
        
    };
	AttackerStatus currentStatus;
	AttackerStatus prevStatus;
	

	
protected:
	virtual ~Robot();
public:
	Robot();
	
	virtual void update(double dt) override;
	virtual void draw(double dt) override;

	virtual float getX()const override;
	virtual float getY()const override;
	virtual void setSX(float) override;
	virtual void setSY(float) override;
	virtual float getSX()const override;
	virtual float getSY()const override;
    virtual float getOuterRadius()const override;
    virtual float getInnerRadius()const override;

	virtual void move(double) override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual bool calculateVirtualForce(RigidBody* dest, Point2F& result,double dt) override;
	virtual float calculateDistance(const Point2F& point, float rad) override;
	virtual void setScale(glm::vec3 _scale) override;
    virtual bool checkLOS(const Point2F& p1, const Point2F& p2)override;

    void resetTargetpoint(const Point2F& pt);
    void addTrailer(pTrailPoints _pts);
	void addPathview(pTrailPoints _path);
	void setTargetPoint(pVirtualAttractivePoint pTP);

	void clearAIData(const Point2F& defaultTargetPos);
	void switchThetaStar();
	void resetTouchCount();
};

class Ball : public Sprite,public RigidBody{
	friend class SpriteManager;
protected:
	float friction;
	AIData data;
	//float nextVelo;
	clock_t startTime;
protected:
	virtual ~Ball();
public:
	Ball();

	virtual void update(double dt)override;
	virtual void draw(double dt)override;

	virtual float getX()const override;
	virtual float getY()const override;
	virtual void setSX(float) override;
	virtual void setSY(float) override;
	virtual float getSX()const override;
	virtual float getSY()const override;
    virtual float getOuterRadius()const override;
    virtual float getInnerRadius()const override;
	virtual void setFriction(float);

	virtual void move(double) override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual bool calculateVirtualForce(RigidBody* dest, Point2F& result,double dt) override;
	virtual float calculateDistance(const Point2F& point, float rad) override;
	virtual bool checkLOS(const Point2F& p1, const Point2F& p2)override;
	//virtual void forceAccel(float velo);

	virtual Point2F getKickingPos(float gap);
	void resetTimer();
};

class Keeper : public Robot{
    friend class SpriteManager;
protected:
    virtual ~Keeper();
protected:
    enum class KeeperStatus{
        Keeper_Parking = 0,		//navigate the keeper to gate area
        Keeper_Waiting = 1,		//stay at gate area
        Keeper_Persuing = 2,	//persue the ball
        Keeper_Blocking = 3		//find a proper place to block the ball
    };
    
    enum BallPosStatus{
        BallPos_Far = 0x0,
        BallPos_Middle = 0x1,
        BallPos_Near = 0x2,
        BallPos_Inside = 0x4,
        
        BallPos_Dangerous =  BallPos_Near | BallPos_Middle
    };
    
    KeeperStatus currentStatus;
    
public:
    Keeper();
    
    virtual void move(double)override;
    virtual void setScale(glm::vec3 _scale)override;
    virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
    virtual bool calculateVirtualForce(RigidBody* dest, Point2F& result,double dt) override;
    virtual float calculateDistance(RigidBody* dest);
    virtual float calculateDistance(const Point2F& point,float rad) override;
};

class Obstacle : public Robot{
	friend class SpriteManager;
protected:
	virtual ~Obstacle();
public:
	Obstacle();
	
	virtual void move(double)override;
	virtual void setScale(glm::vec3 _scale)override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual bool calculateVirtualForce(RigidBody* dest, Point2F& result,double dt) override;
	virtual float calculateDistance(RigidBody* dest);
	virtual float calculateDistance(const Point2F& point,float rad) override;
	virtual bool checkLOS(const Point2F& p1, const Point2F& p2)override;
};

class Wall : public Sprite,public RigidBody{
	friend class SpriteManager;
protected:
	virtual ~Wall();
public:
	Wall();
	
	virtual void update(double dt) override;
	virtual void draw(double dt) override;

	virtual float getX()const override;
	virtual float getY()const override;
	virtual void setSX(float) override;
	virtual void setSY(float) override;
	virtual float getSX()const override;
	virtual float getSY()const override;
    virtual float getOuterRadius()const override;
    virtual float getInnerRadius()const override;

	virtual void move(double) override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual bool calculateVirtualForce(RigidBody* dest, Point2F& result,double dt) override;
	virtual float calculateDistance(const Point2F& point, float rad) override;
	virtual bool checkLOS(const Point2F& p1, const Point2F& p2)override;
};

#endif

#ifndef __INCLUDE_SPRITE_H__
#define __INCLUDE_SPRITE_H__

#include <set>

#include "Camera.h"
#include "RigidBody.h"
#include "ModuleBase.h"

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
	double radius;
protected:
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

}*pSprite;

class SpriteManager{
protected:
	static SpriteManager manager;

	std::set<pSprite> spriteList;

	SpriteManager();
	virtual ~SpriteManager();
public:
	static SpriteManager* getInstance();
	
	void addToManager(pSprite p);
	void removeFromManager(pSprite p);
};

typedef class TrailPoints : public Sprite{
	friend class SpriteManager;
protected:
	~TrailPoints(void);
public:
    TrailPoints(size_t max_count);
    
    void update(double dt) override;
    void draw(double dt) override;
    
    void addPoint(const Point2F& pt);
    
protected:
    size_t maxCount;
    std::deque<Point2F> points;
}*pTrailPoints;

class Robot : public Sprite,public RigidBody{
	friend class SpriteManager;
protected:
	float width;
	float length;

	AIData data;
    
    pTrailPoints pts;
	
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
	virtual float getRadius()const override;


	virtual void move(double) override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual void setDistanceHeuristic(float h);

	virtual void setScale(glm::vec3 _scale) override;
    
    void addTrailer(pTrailPoints _pts);
};

class Ball : public Sprite,public RigidBody{
	friend class SpriteManager;
protected:
	float friction;
	AIData data;
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
	virtual float getRadius()const override;
	virtual void setFriction(float);

	virtual void move(double) override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
};

class Obstacle : public Robot{
	friend class SpriteManager;
protected:
	virtual ~Obstacle();
public:
	Obstacle();
	
	virtual void move(double)override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual float calculateDistance(RigidBody* dest);
	virtual float calculateDistance(Point2F& point,float rad);

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
	virtual float getRadius()const override;

	virtual void move(double) override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
};

#endif
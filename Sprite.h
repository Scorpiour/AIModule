#ifndef __INCLUDE_SPRITE_H__
#define __INCLUDE_SPRITE_H__

#include "Camera.h"
#include "RigidBody.h"
#include "ModuleBase.h"

typedef class Sprite{

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
	Sprite(void);
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

typedef class TrailPoints : public Sprite{
public:
    TrailPoints(size_t max_count);
    ~TrailPoints(void);
    
    void update(double dt) override;
    void draw(double dt) override;
    
    void addPoint(const Point2F& pt);
    
protected:
    size_t maxCount;
    std::deque<Point2F> points;
}*pTrailPoints;

class Robot : public Sprite,public RigidBody{
protected:
	float width;
	float length;

	AIData data;
    
    pTrailPoints pts;
public:
	Robot();
	virtual ~Robot();

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
protected:
	float friction;
	AIData data;
public:
	Ball();
	virtual ~Ball();

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
public:
	Obstacle();
	virtual ~Obstacle();

	virtual void move(double)override;
	virtual bool calculateForce(RigidBody* dest,Point2F& result,double dt) override;
	virtual float calculateDistance(RigidBody* dest);
	virtual float calculateDistance(Point2F& point,float rad);

};

class Wall : public Sprite,public RigidBody{
protected:

public:
	Wall();
	virtual ~Wall();


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
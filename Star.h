#ifndef __INCLUDE_STAR_H__
#define __INCLUDE_STAR_H__


#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "GlobalMacro.h"

class Star{
protected:
	float rotationVelocity;
	float revolutionVelocity;
	float orbitRadius;
	float radius;
	glm::vec2 orbitCentre;
	glm::vec3 colourValue;		//color RGB
	glm::vec3 colourIntensity;	//Ambient, Diffuse, Specular

	glm::vec3 position;

	GLfloat modelMatrix[16];

	bool isHighlighted;

	GLuint ID;
protected:
	GLint modelMatrixLoc;
	GLint translateLoc;
	GLint starRadiusLoc;
	GLint starColorLoc;
	GLint starIntensityLoc;
	GLint highlightLoc;
public:
	explicit Star(GLuint program,GLuint id);
	virtual ~Star();
public:
	glm::vec3 getColor();
	glm::vec3 getIntensity();
	glm::vec2 getOrbitCentre();
	glm::vec3 getPosition();

	float getRotationVelo();
	float getOrbitRadius();
	float getRevolutionVelo();
	float getRadius();

	GLuint getID();
public:
	void setColour(glm::vec3 colour);
	void setIntensity(glm::vec3 intensity);
	void setRadius(float value);
	void setRotationVelo(float value);
	void setRevolutionVelo(float value);
	void setOrbitRadius(float value);
	void setOrbitCentre(glm::vec2 centre);
	void setHighlight(bool b);
public:
	void update(double dt);
	void draw(double dt);
};

#endif
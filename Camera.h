#ifndef __INCLUDE_CAMERA_H__
#define __INCLUDE_CAMERA_H__

#include "ShaderProgram.h"

typedef class Camera{
protected:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;

	GLfloat lookat[16];
	GLfloat project[16];
	GLfloat view[16];

	GLfloat invLookat[16];
	GLfloat invProject[16];
public:

	Camera(glm::vec3 _pos, glm::vec3 _tar, glm::vec3 _up);
	virtual ~Camera();

	void setTarget(glm::vec3 _tar);
	void setPosition(glm::vec3 _pos);
	void setUp(glm::vec3 _up);

	const glm::vec3 getTarget()const;
	const glm::vec3 getPosition()const;
	const glm::vec3 getUp()const;

	bool rayClip(Point2F& worldCoord,GLFWwindow* window);

	void update(double dt, double radius, double persp_angle,double ratio);
	void draw(pShaderProgram _program);
}*pCamera;

#endif
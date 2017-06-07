#ifndef __INCLUDE_GLOBAL_VARIABLE_H__
#define __INCLUDE_GLOBAL_VARIABLE_H__

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "SOIL.h"

#include "GlobalMacro.h"


class GlobalVariables{
public:
	GlobalVariables(){}
	~GlobalVariables(){}
	static std::string mainPath;

	static glm::vec4 Mat4MultiVec4(float A[], const glm::vec4& B);
	static void Mat4Multi(const float A[],const float B[],float out[]);
	static void Vec3Cross(const glm::vec3 A, const glm::vec3 B, glm::vec3 & out);
	static void Mat4Transpose(float A[]);
	static bool Mat4Inverse(float A[]);

	static void calculateLookat(glm::vec3 & pos, glm::vec3 & target, glm::vec3 & up, GLfloat lookatMat4[]);
	static void calculateProjection(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far,GLfloat projectMat4[]);
	static bool linesIntersect(const Point2F& p1, const Point2F& p2,const Point2F& p3, const Point2F& p4);
	static float distancePointToLine(const Point2F& pt, const Point2F& _p1, const Point2F& _p2);
	static float Atan2(float y,float x);
	static float Acos(float x);
	
	static Point2F rotatePointByArcPivot(const Point2F& pt, const Point2F& pivot, float arc);


	//This algirthm is come from Wikipedia, with slight modification for any-angle rectangle
	enum CSCode{
		CS_INSIDE = 0x0,
		CS_LEFT = 0x1,
		CS_RIGHT = 0x2,
		CS_BOTTOM = 0x4,
		CS_TOP = 0x8
	};
	static bool Cohen_Sutherland(Point2F p1, Point2F p2,const Point2F& rpmin, const Point2F& rpmax);
private:
	static int outcode(const Point2F& pt,const Point2F& rpmin, const Point2F& rpmax);
};


#endif

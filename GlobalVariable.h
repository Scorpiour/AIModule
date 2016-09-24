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
};


#endif
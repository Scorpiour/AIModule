#include "GlobalVariable.h"

using namespace std;


std::string GlobalVariables::mainPath;


glm::vec4 GlobalVariables::Mat4MultiVec4(float A[], const glm::vec4& B){
	glm::vec4 ret;

	for(int i=0;i<4;i++){
		float sum = 0;
		for(int j=0;j<4;j++){
			sum += A[i*4+j]*B[j];
		}
		ret[i] = sum;
	}
	return ret;
}


void GlobalVariables::Mat4Transpose(float A[]){
	for(int i = 0 ; i < 4; i++){
		for(int j = i; j < 4; j++){
			std::swap(A[i*4+j],A[j*4+i]);
		}
	}
}

bool GlobalVariables::Mat4Inverse(float A[]){
	float inv[16];
	float det;

	inv[0] = A[5]  * A[10] * A[15] - 
             A[5]  * A[11] * A[14] - 
             A[9]  * A[6]  * A[15] + 
             A[9]  * A[7]  * A[14] +
             A[13] * A[6]  * A[11] - 
             A[13] * A[7]  * A[10];

    inv[4] = -A[4]  * A[10] * A[15] + 
              A[4]  * A[11] * A[14] + 
              A[8]  * A[6]  * A[15] - 
              A[8]  * A[7]  * A[14] - 
              A[12] * A[6]  * A[11] + 
              A[12] * A[7]  * A[10];

    inv[8] = A[4]  * A[9] * A[15] - 
             A[4]  * A[11] * A[13] - 
             A[8]  * A[5] * A[15] + 
             A[8]  * A[7] * A[13] + 
             A[12] * A[5] * A[11] - 
             A[12] * A[7] * A[9];

    inv[12] = -A[4]  * A[9] * A[14] + 
               A[4]  * A[10] * A[13] +
               A[8]  * A[5] * A[14] - 
               A[8]  * A[6] * A[13] - 
               A[12] * A[5] * A[10] + 
               A[12] * A[6] * A[9];

    inv[1] = -A[1]  * A[10] * A[15] + 
              A[1]  * A[11] * A[14] + 
              A[9]  * A[2] * A[15] - 
              A[9]  * A[3] * A[14] - 
              A[13] * A[2] * A[11] + 
              A[13] * A[3] * A[10];

    inv[5] = A[0]  * A[10] * A[15] - 
             A[0]  * A[11] * A[14] - 
             A[8]  * A[2] * A[15] + 
             A[8]  * A[3] * A[14] + 
             A[12] * A[2] * A[11] - 
             A[12] * A[3] * A[10];

    inv[9] = -A[0]  * A[9] * A[15] + 
              A[0]  * A[11] * A[13] + 
              A[8]  * A[1] * A[15] - 
              A[8]  * A[3] * A[13] - 
              A[12] * A[1] * A[11] + 
              A[12] * A[3] * A[9];

    inv[13] = A[0]  * A[9] * A[14] - 
              A[0]  * A[10] * A[13] - 
              A[8]  * A[1] * A[14] + 
              A[8]  * A[2] * A[13] + 
              A[12] * A[1] * A[10] - 
              A[12] * A[2] * A[9];

    inv[2] = A[1]  * A[6] * A[15] - 
             A[1]  * A[7] * A[14] - 
             A[5]  * A[2] * A[15] + 
             A[5]  * A[3] * A[14] + 
             A[13] * A[2] * A[7] - 
             A[13] * A[3] * A[6];

    inv[6] = -A[0]  * A[6] * A[15] + 
              A[0]  * A[7] * A[14] + 
              A[4]  * A[2] * A[15] - 
              A[4]  * A[3] * A[14] - 
              A[12] * A[2] * A[7] + 
              A[12] * A[3] * A[6];

    inv[10] = A[0]  * A[5] * A[15] - 
              A[0]  * A[7] * A[13] - 
              A[4]  * A[1] * A[15] + 
              A[4]  * A[3] * A[13] + 
              A[12] * A[1] * A[7] - 
              A[12] * A[3] * A[5];

    inv[14] = -A[0]  * A[5] * A[14] + 
               A[0]  * A[6] * A[13] + 
               A[4]  * A[1] * A[14] - 
               A[4]  * A[2] * A[13] - 
               A[12] * A[1] * A[6] + 
               A[12] * A[2] * A[5];

    inv[3] = -A[1] * A[6] * A[11] + 
              A[1] * A[7] * A[10] + 
              A[5] * A[2] * A[11] - 
              A[5] * A[3] * A[10] - 
              A[9] * A[2] * A[7] + 
              A[9] * A[3] * A[6];

    inv[7] = A[0] * A[6] * A[11] - 
             A[0] * A[7] * A[10] - 
             A[4] * A[2] * A[11] + 
             A[4] * A[3] * A[10] + 
             A[8] * A[2] * A[7] - 
             A[8] * A[3] * A[6];

    inv[11] = -A[0] * A[5] * A[11] + 
               A[0] * A[7] * A[9] + 
               A[4] * A[1] * A[11] - 
               A[4] * A[3] * A[9] - 
               A[8] * A[1] * A[7] + 
               A[8] * A[3] * A[5];

    inv[15] = A[0] * A[5] * A[10] - 
              A[0] * A[6] * A[9] - 
              A[4] * A[1] * A[10] + 
              A[4] * A[2] * A[9] + 
              A[8] * A[1] * A[6] - 
              A[8] * A[2] * A[5];

    det = A[0] * inv[0] + A[1] * inv[4] + A[2] * inv[8] + A[3] * inv[12];
	
	if(det == 0){
		return false;
	}

	det = 1.f / det;

	for(int i=0;i<16;i++){
		A[i] = inv[i] * det;
	}

	return true;
}

void GlobalVariables::Mat4Multi(const float A[],const float B[],float out[]){
	float temp[16];
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			float sum = 0.0;
			for(int k=0;k<4;k++){
				sum += A[i*4+k]*B[k*4+j];
			}
			temp[i*4+j] = sum;
		}
	}
	for(int i=0;i<16;i++){
		out[i] = temp[i];
	}
}

void GlobalVariables::Vec3Cross(const glm::vec3 A, const glm::vec3 B, glm::vec3 & out){
	float temp[3];

	temp[0] = A[1]*B[2] - A[2]*B[1];
	temp[1] = A[2]*B[0] - A[0]*B[2];
	temp[2] = A[0]*B[1] - A[1]*B[0];

	out[0] = temp[0];
	out[1] = temp[1];
	out[2] = temp[2];
}


void GlobalVariables::calculateLookat(glm::vec3 & pos, glm::vec3 & target, glm::vec3 & up, GLfloat lookatMat4[]){
	GLfloat tempR[16];
	GLfloat tempP[16];
	
	for(int i = 0; i < 16; i++){
		tempR[i] = 0.f;
		tempP[i] = 0.f;
		lookatMat4[i] = 0.f;
	}
	
	up = glm::normalize(up);
	glm::vec3 direction = glm::normalize(pos - target);
	glm::vec3 camRight;
	glm::vec3 camUp;

	GlobalVariables::Vec3Cross(up, direction, camRight);
	camRight = glm::normalize(camRight);

	GlobalVariables::Vec3Cross(direction, camRight, camUp);
	camUp = glm::normalize(camUp);

	tempR[0] = camRight[0];
	tempR[1] = camRight[1];
	tempR[2] = camRight[2];

	tempR[4] = camUp[0];
	tempR[5] = camUp[1];
	tempR[6] = camUp[2];

	tempR[8] = direction[0];
	tempR[9] = direction[1];
	tempR[10] = direction[2];

	tempR[15] = 1.f;

	
	tempP[0] = 1.f;
	tempP[5] = 1.f;
	tempP[10] = 1.f;
	tempP[15] = 1.f;
	
	tempP[3] = -pos[0];
	tempP[7] = -pos[1];
	tempP[11] = -pos[2];

	GlobalVariables::Mat4Multi(tempR,tempP,lookatMat4);
	//Mat4Transpose(lookatMat4);
}

void GlobalVariables::calculateProjection(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far,GLfloat projectMat4[]){
	/*
		1/asp*f		0		0				0
		0			1/f		0				0
		0			0		-(n+f)/(f-n)	-1
		0			0		-(2*n*f)/(f-n)	0
	*/
	for(int i = 0 ; i < 16; i++){
		projectMat4[i] = 0.f;
	}

	GLfloat f = tan(fovy/2);

	projectMat4[0]	= 1.f / (aspect * f );
	projectMat4[5]	= 1.f / f;
	projectMat4[10]	= -(near + far)/(far - near);
	projectMat4[11] = -1.f;
	projectMat4[14] = -(2 * near * far)/(far - near);

}

float GlobalVariables::Atan2(float y,float x){
	if(x==0&&y==0){
		return 0;
	}
	return atan2(y,x);
}

float GlobalVariables::Acos(float x){
	if(x > 1){
		x = 1.f;
	}else if(x < -1){
		x = -1.f;
	}
	return acos(x);
}
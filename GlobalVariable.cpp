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

float GlobalVariables::distancePointToLine(const Point2F& pt, const Point2F& p1, const Point2F& p2){
	float a = (p2.y - p1.y);
	float b = (p1.x - p2.x);
	float c = p1.y*(p2.x-p1.x) + p1.x*(p1.y-p2.y);

	if(fabs(a)<1E-7&&fabs(b)<1E-7){
		return 0;
	}

	float denom = 1/sqrt(a*a+b*b);
		
	Point2F foot;
	foot.x = (b*b*pt.x - a*b*pt.y - a*c)*denom*denom;
	foot.y = (a*a*pt.y - a*b*pt.x - b*c)*denom*denom;

	if(foot.x < p1.x){
		float dx = p1.x - pt.x;
		float dy = p1.y - pt.y;
		return sqrt(dx*dx + dy*dy);
	}else if(foot.x > p2.x){
		float dx = p2.x - pt.x;
		float dy = p2.y - pt.y;
		return sqrt(dx*dx + dy*dy);
	}
	return abs(a*pt.x + b*pt.y + c)*denom;

}

Point2F GlobalVariables::rotatePointByArcPivot(const Point2F& pt, const Point2F& pivot, float arc){
	Point2F result(pt);

	if (arc == 0){
        return result;
	}
        
	float s = sin(arc);
    float c = cos(arc);

	result.x -= pivot.x;
    result.y -= pivot.y;


	float nx = (result.x * c) - (result.y * s);
	float ny = (result.x * s) + (result.y * c);

    result.x = nx + pivot.x;
	result.y = ny + pivot.y;

	return result;
}


bool GlobalVariables::linesIntersect(const Point2F& p1, const Point2F& p2,
						const Point2F& p3, const Point2F& p4)
{
    float a1, a2, b1, b2, c1, c2; 
    float r1, r2, r3, r4;    
    float denom, offset, num;   

    a1 = p2.y - p1.y;
    b1 = p1.x - p2.x;
    c1 = p2.x * p1.y - p1.x * p2.y;

    r3 = a1 * p3.x + b1 * p3.y + c1;
    r4 = a1 * p4.x + b1 * p4.y + c1;

    if ( fabs(r3) > 1E-7 &&  fabs(r4)>1E-7 &&  r3*r4> 0){
        return false;
	}

    a2 = p4.y - p3.y;
    b2 = p3.x - p4.x;
    c2 = p4.x * p3.y - p3.x * p4.y;

    r1 = a2 * p1.x + b2 * p1.y + c2;
    r2 = a2 * p2.x + b2 * p2.y + c2;

    if ( fabs(r1)>1E-7 &&  fabs(r2)>1E-7 &&  r1*r2 > 0){
        return false;
	}
	return true;

	/*
    denom = a1 * b2 - a2 * b1;
    if ( abs(denom) <1E-7 ){
        return true;  //Cointerlinear
	}
    offset = denom < 0 ? - denom / 2 : denom / 2;

    num = b1 * c2 - b2 * c1;
    *x = ( num < 0 ? num - offset : num + offset ) / denom;

    num = a2 * c1 - a1 * c2;
    *y = ( num < 0 ? num - offset : num + offset ) / denom;

    return true; //intersection*/
}

int GlobalVariables::outcode(const Point2F& pt,const Point2F& rpmin, const Point2F& rpmax){
	int code = CS_INSIDE;          // initialised as being inside of [[clip window]]

	if (pt.x < rpmin.x){           // to the left of clip window
		code |= CS_LEFT;
	}else if (pt.x > rpmax.x){      // to the right of clip window
		code |= CS_RIGHT;
	}if (pt.y < rpmin.y){           // below the clip window
		code |= CS_BOTTOM;
	}else if (pt.y > rpmax.y){      // above the clip window
		code |= CS_TOP;
	}
	return code;

}

bool GlobalVariables::Cohen_Sutherland(Point2F p1, Point2F p2,const Point2F& rpmin, const Point2F& rpmax){
	int outcode0 = GlobalVariables::outcode(p1,rpmin,rpmax);//ComputeOutCode(p1.x, p1.y);
	int outcode1 = GlobalVariables::outcode(p2,rpmin,rpmax);//ComputeOutCode(p2.x, p2.y);
	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		} else if (outcode0 & outcode1) { // Bitwise AND is not 0. (implies both end points are in the same region outside the window). Reject and get out of loop
			break;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			float x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			int outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas:
			//   slope = (p2.y - p1.y) / (p2.x - p1.x)
			//   x = p1.x + (1 / slope) * (ym - p1.y), where ym is rpmin.y or rpmax.y
			//   y = p1.y + slope * (xm - p1.x), where xm is rpmin.x or rpmax.x
			if (outcodeOut & CS_TOP) {           // point is above the clip rectangle
				x = p1.x + (p2.x - p1.x) * (rpmax.y - p1.y) / (p2.y - p1.y);
				y = rpmax.y;
			} else if (outcodeOut & CS_BOTTOM) { // point is below the clip rectangle
				x = p1.x + (p2.x - p1.x) * (rpmin.y - p1.y) / (p2.y - p1.y);
				y = rpmin.y;
			} else if (outcodeOut & CS_RIGHT) {  // point is to the right of clip rectangle
				y = p1.y + (p2.y - p1.y) * (rpmax.x - p1.x) / (p2.x - p1.x);
				x = rpmax.x;
			} else if (outcodeOut & CS_LEFT) {   // point is to the left of clip rectangle
				y = p1.y + (p2.y - p1.y) * (rpmin.x - p1.x) / (p2.x - p1.x);
				x = rpmin.x;
			}


			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				p1.x = x;
				p1.y = y;
				outcode0 = GlobalVariables::outcode(p1,rpmin,rpmax);
			} else {
				p2.x = x;
				p2.y = y;
				outcode1 = GlobalVariables::outcode(p2,rpmin,rpmax);
			}
		}
	}

	return accept;
}
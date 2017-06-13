#ifndef __INCLUDE_GLOBALMACRO_H__
#define __INCLUDE_GLOBALMACRO_H__

#ifndef __OUTREF__
#define __OUTREF__(type) type& 
#endif

#ifndef __CONST_INREF__
#define __CONST_INREF__(type) const type&
#endif

#ifndef DC
#define DC(x,y) dynamic_cast<x>(y)
#endif

#ifndef RC
#define RC(x,y) reinterpret_cast<x>(y)
#endif

#ifndef SAFE_COPY_LEN
#define SAFE_COPY_LEN(desc, src, len) \
    do { \
	    if(desc) { \
		    delete[] desc; \
            desc = nullptr; \
        } \
	    desc = new char[(len)+1]; \
	    memset(desc,0, (len)+1); \
	    strncpy(desc, src, len); \
    } while (0)
#endif

#ifndef SAFE_COPY_TYPE_LEN
#define SAFE_COPY_TYPE_LEN(type, desc, src, len) \
		do{ \
			if(desc){ \
				delete[] desc; \
				desc = nullptr; \
			} \
			desc = new type[sizeof(type)*len]; \
			memcpy(desc, src, sizeof(type)*len); \
		}while(0)
#endif

#ifndef SAFE_COPY
#define SAFE_COPY(desc, src)\
        do {\
			if(desc) { \
				delete[] desc; \
                desc = nullptr; \
            } \
			if(src != nullptr){ \
				size_t len = strlen(src);\
				desc = new char[len+1]; \
				memset(desc,0, len+1); \
				strcpy(desc, src);\
            }\
        } while (0)
#endif

#ifndef SAFE_DEL
#define SAFE_DEL(obj) \
		if(obj != nullptr){\
			delete obj; \
			obj = nullptr;\
		}
#endif

#ifndef SAFE_DELS
#define SAFE_DELS(obj)\
		if(obj != nullptr){\
			delete[] obj; \
			obj = nullptr;\
		}
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif

enum Position {LEFT_SIDE, RIGHT_SIDE};

#include <cmath>

#ifndef FLT_MAX
#define FLT_MAX 1e+37
#endif



typedef struct Point2F{
	float x;
	float y;

	Point2F(){
		x = 0.f;
		y = 0.f;
	}

	Point2F(const Point2F& pt){
		x = pt.x;
		y = pt.y;
	}
	Point2F(Point2F && pt){
		x = pt.x;
		y = pt.y;
	}

	Point2F(float a,float b){
		x = a;
		y = b;
	}
	/*
	operator bool(){
		if(fabs(x) < 1E-7 && fabs(y) < 1E-7){
			return false;
		}
		return true;
	}*/

	operator bool()const{
		if(fabs(x) < 1E-7 && fabs(y) < 1E-7){
			return false;
		}
		return true;
	}

	const Point2F& operator = (const Point2F& pt){
		if(this != &pt){
			x = pt.x;
			y = pt.y;
		}
		return *this;
	}

	bool operator < (const Point2F& pt)const{

		float dy = fabs(this->y - pt.y);
		float dx = fabs(this->x - pt.x);

		if(dy < 1E-7){
			if(dx < 1E-7){
				return false;
			}
			return this->x < pt.x;

		}else if(this->y < pt.y){
			return true;
		}
		return false;
	}

	bool operator == (const Point2F& pt)const{
		float dy = fabs(this->y - pt.y);
		float dx = fabs(this->x - pt.x);

		return (dx < 1E-7) && (dy < 1E-7);
	}

	bool operator != (const Point2F& pt)const{
		return !(*this == pt);
	}

}*pPoint2F;

typedef struct Point2I{
	int x;
	int y;

	Point2I(){
		x = 0;
		y = 0;
	}

	Point2I(const Point2I& pt){
		this->x = pt.x;
		this->y = pt.y;
	}
	
	Point2I(Point2I && pt){
		this->x = pt.x;
		this->y = pt.y;
	}
	Point2I(int a,int b){
		x = a;
		y = b;
	}

	const Point2I& operator = (const Point2I& pt){
		if(this != &pt){
			this->x = pt.x;
			this->y = pt.y;
		}
		return *this;
	}

	bool operator < (const Point2I& pt)const{
		if(this->y < pt.y){
			return true;
		}else if(this->y == pt.y){
			return this->x < pt.x;
		}
		return false;
	}

	bool operator == (const Point2I& pt)const{
		return (this->y == pt.y) && (this->x == pt.x) ;
	}
}*pPoint2I;

#ifdef _WIN32
#include <crtdbg.h>
#ifdef _DEBUG

#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

#endif
#endif
#endif
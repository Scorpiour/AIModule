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

typedef struct Point2F{
	float x;
	float y;

	operator bool(){
		if(abs(x) < 1E-7 && abs(y) < 1E-7){
			return false;
		}
		return true;
	}

}*pPoint2F;

#ifdef _WIN32
#include <crtdbg.h>
#ifdef _DEBUG

#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

#endif
#endif
#endif
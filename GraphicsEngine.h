#ifndef __INCLUDE_GRAPHICS_ENGINE_H__
#define __INCLUDE_GRAPHICS_ENGINE_H__


#include "Model.h"
#include "Camera.h"


class GraphicsEngine{
protected:	//flags
	bool keyWHold;
	bool keySHold;
	bool keyAHold;
	bool keyDHold;
	bool keyQHold;
	bool keyEHold;
	bool keyCHold;
	bool keyXHold;
	bool keyUPHold;
	bool keyDOWNHold;
    
    bool keyALTHold;    //Key Option on Mac
    
	bool manualFlag;
protected:	//General Members
	float speedcoe;// = 1.0f;
	bool showMode;// = false;
	bool pauseFlag;

	pShaderProgram sphere_program;
	pShaderProgram box_program;
	GLFWwindow* window;
	
	float ratio;
	float radius;
	float persp_angle;
	double pausetime;
	bool resetBall;

	Ball* pb;
	Robot* pr;

	std::string rootPath;
protected:	//Buffers & Shaders
	GLuint sphere_vao;
	GLuint box_vao;
	GLuint plate_vao;
	std::vector<GLuint> bufferList;
protected:	//Camara
	pCamera main_camera;
    glm::vec3 prevTarget;
    glm::vec3 prevPosition;
    float prev_angle;
    bool recordLock;
protected:	//Lamp
	glm::vec3 lightPosition;
protected:  //External Function
	std::function<void*(double,void*)> externalFunc;
protected:  //Models
	std::map<std::string,Model*> modelList;
protected:	//Constructor / Destructor
	static GraphicsEngine* instance;
	explicit GraphicsEngine(void);
	virtual ~GraphicsEngine(void);
protected:	//Action Control
	void handleAction(double time);
protected:	//Event Handlers
	static void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mod);
	static void mouseButtonCallback(GLFWwindow* window, int btn, int action, int mode);
	static void scrollCallback(GLFWwindow* window, double xoff, double yoff);
	static void resizeWindowCallback(GLFWwindow* window, int width, int height);
	static void resizeFramebufferCallback(GLFWwindow* window, int width, int height);
public:
	//These function must be called in order!
	bool init(void);
	bool loadProgram(void);
	bool prepareVAO(const std::string& dataPath);
	bool prepareVBO(const std::string& dataPath);
	bool prepareCamara(void);
	bool prepareSprites(void);
	bool setExternalFunction(std::function<void*(double,void*)> _ext);	//This function will be called BEFORE all the object updated, after handelAction at each loop
	bool mainLoop();
	bool clear(void);
	Model* getModel(const std::string& name);
	bool loadModel(const std::string& name, Model* pm);

	void inactiveAllModels();
    void activeModel(const std::string& name);
	void resetObject(const Point2F& robotPos, const Point2F& ballPos);
public:
	static GraphicsEngine* getInstance(void);
	static void releaseInstance(void);
};

#endif

#include <ctime>

#include "GraphicsEngine.h"
#include "GlobalMacro.h"

using namespace std;

//Access Entry / Contructor / Destructor

GraphicsEngine* GraphicsEngine::instance = nullptr;


GraphicsEngine* GraphicsEngine::getInstance(void){
	if(GraphicsEngine::instance == nullptr){
		GraphicsEngine::instance = new GraphicsEngine;
	}
	return GraphicsEngine::instance;
}

void GraphicsEngine::releaseInstance(void){
	if(GraphicsEngine::instance != nullptr){
		delete GraphicsEngine::instance;
		GraphicsEngine::instance = nullptr;
	}
}

GraphicsEngine::GraphicsEngine(){
	this->speedcoe = 8.f;
	this->showMode = false;
	this->ratio = 1.f;
	this->radius = 30.0f;
	this->persp_angle = 0;

	this->keyWHold = false;
	this->keySHold = false;
	this->keyAHold = false;
	this->keyDHold = false;
	this->keyQHold = false;
	this->keyEHold = false;
	this->keyCHold = false;
	this->keyXHold = false;

	this->manualFlag = true;
	this->pausetime = 0;
	this->pauseFlag = false;
	this->resetBall = false;

	this->lightPosition = glm::vec3(0,0,0);

	this->rootPath = GlobalVariables::mainPath;

	this->sphere_program = nullptr;
	this->box_program = nullptr;
	this->main_camera = nullptr;
	this->externalFunc = nullptr;
}

GraphicsEngine::~GraphicsEngine(){
	this->clear();
}


void GraphicsEngine::keyCallback(GLFWwindow* window,int key,int scancode,int action,int mod){

	auto graphics = GraphicsEngine::getInstance();

	if(action == GLFW_PRESS){	
		switch(key){
		case GLFW_KEY_0:
			{
				graphics->inactiveAllModels();
			}break;
		case GLFW_KEY_1:
			{
				graphics->inactiveAllModels();
				auto m = graphics->getModel("RegObs");
				m->swithActive();

			}break;

		case GLFW_KEY_2:
			{
				graphics->inactiveAllModels();
				auto m = graphics->getModel("Block");
				m->swithActive();

			}break;

		case GLFW_KEY_3:
			{
				graphics->inactiveAllModels();
				auto m = graphics->getModel("Wiggle");
				m->swithActive();

			}break;
		case GLFW_KEY_4:
			{
				graphics->inactiveAllModels();
				auto m = graphics->getModel("Trap");
				m->swithActive();
				
			}break;
		case GLFW_KEY_5:
			{
				graphics->inactiveAllModels();
				auto m = graphics->getModel("Cross");
				m->swithActive();
			}break;
		case GLFW_KEY_ESCAPE:
			{
				glfwSetWindowShouldClose(window,GL_TRUE);
			}break;

		case GLFW_KEY_R:
			{
				//graphics->inactiveAllModels();
				Point2F rp;
				rp.x = 50.f;
				rp.y = 50.f;

				Point2F bp;
				bp.x = -50.f;
				bp.y = -50.f;

				graphics->resetObject(rp,bp);

			}break;

		case GLFW_KEY_W:
			{
				graphics->keyWHold = true;
			}break;
		case GLFW_KEY_S:
			{
				graphics->keySHold = true;
			}break;
		case GLFW_KEY_A:
			{
				graphics->keyAHold = true;
			}break;
		case GLFW_KEY_D:
			{
				graphics->keyDHold = true;
			}break;
		case GLFW_KEY_Q:
			{
				graphics->keyQHold = true;
			}break;
		case GLFW_KEY_E:
			{
				graphics->keyEHold = true;
			}break;
		case GLFW_KEY_X:
			{
				graphics->keyXHold = true;
			}break;
		case GLFW_KEY_C:
			{
				graphics->keyCHold = true;
			}break;
		case GLFW_KEY_M:
			{
				graphics->manualFlag = !(graphics->manualFlag);
				if(graphics->manualFlag){
					cout<<"Set Manual Control Enabled."<<endl;
				}else{
					cout<<"Set Manual Control Disabled."<<endl;
					graphics->main_camera->setTarget(glm::vec3(0,0,0));
				}
				
			}break;
		case GLFW_KEY_L:
			{
				graphics->showMode = !(graphics->showMode);
			}break;
		case GLFW_KEY_UP:
			{
				graphics->keyUPHold = true;
			}break;
		case GLFW_KEY_DOWN:
			{
				graphics->keyDOWNHold = true;
			}break;
		case GLFW_KEY_SPACE:
			{
				graphics->pauseFlag = !(graphics->pauseFlag);
			}break;
		}
	}else if(action == GLFW_RELEASE){
		switch(key){
		case GLFW_KEY_W:
			{
				graphics->keyWHold = false;
			}break;
		case GLFW_KEY_S:
			{
				graphics->keySHold = false;
			}break;
		case GLFW_KEY_A:
			{
				graphics->keyAHold = false;
			}break;
		case GLFW_KEY_D:
			{
				graphics->keyDHold = false;
			}break;
		case GLFW_KEY_Q:
			{
				graphics->keyQHold = false;
			}break;
		case GLFW_KEY_E:
			{
				graphics->keyEHold = false;
			}break;
		case GLFW_KEY_X:
			{
				graphics->keyXHold = false;
			}break;
		case GLFW_KEY_C:
			{
				graphics->keyCHold = false;
			}break;
		case GLFW_KEY_UP:
			{
				graphics->keyUPHold = false;
			}break;
		case GLFW_KEY_DOWN:
			{
				graphics->keyDOWNHold = false;
			}break;
		}
	}
}

void GraphicsEngine::resizeWindowCallback(GLFWwindow* window, int width, int height){
	auto graphics = GraphicsEngine::getInstance();
	graphics->ratio = float(height)/float(width);

}

void GraphicsEngine::resizeFramebufferCallback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

void GraphicsEngine::mouseButtonCallback(GLFWwindow* window, int btn, int action, int mode){
	if(btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		GraphicsEngine::getInstance()->resetBall = true;
		GraphicsEngine::getInstance()->pr->resetTouchCount();
		GraphicsEngine::getInstance()->pb->resetTimer();
	}
}

void GraphicsEngine::scrollCallback(GLFWwindow* window, double xoff, double yoff){

}


//Public Methods

bool GraphicsEngine::init(void){
	if(!glfwInit()){
		cerr<<"Cannot initialize GLFW!"<<endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES,4);
	window = glfwCreateWindow(
			800,
			800,
			"AI Module",
			nullptr,
			nullptr);

	if(!window){
		cerr<<"Create GLFWwindow failed!"<<endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window,GraphicsEngine::keyCallback);
	glfwSetMouseButtonCallback(window,GraphicsEngine::mouseButtonCallback);
	glfwSetScrollCallback(window,GraphicsEngine::scrollCallback);
	glfwSetWindowSizeCallback(window,GraphicsEngine::resizeWindowCallback);
	glfwSetFramebufferSizeCallback(window,GraphicsEngine::resizeFramebufferCallback);

	glewExperimental = true;
		
	if(GLEW_OK != glewInit()){
		cerr<<"Cannot initialized GLEW!"<<endl;
		return false;
	}

	glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);


	glDepthFunc(GL_LESS);
	glDepthRange(-1,1);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);

	int width;
	int height;
	
	glfwGetWindowSize(this->window,&width,&height);
	ratio = float(height)/float(width);

	glfwGetFramebufferSize(this->window,&width,&height);
	glViewport(0,0,width,height);


	return true;
}

bool GraphicsEngine::prepareVAO(const std::string& dataPath){
	glGenVertexArrays(1,&(this->box_vao));
	glGenVertexArrays(1,&(this->sphere_vao));
	glGenVertexArrays(1,&(this->plate_vao));
	//glBindVertexArray(vertexarray);
	return true;
}

bool GraphicsEngine::prepareVBO(const std::string& dataPath){
	//sphere
	GLfloat sphere_vertices[] = 
		{
			-0.404509,	0,	0.25,	1,	1,0,0,	//p0
			0.404509,	0,	0.25,	1,	0,1,0,	//p1
			0.404509,	0,	-0.25,	1,	0,0,1,	//p2	
			-0.404509,	0,	-0.25,	1,	1,1,0,	//p3

			-0.25,	-0.404509,	0,	1,	0,1,1,	//p4
			0.25,	-0.404509,	0,	1,	1,0,1,	//p5
			0.25,	0.404509,	0,	1,	1,1,1,	//p6
			-0.25,	0.404509,	0,	1,	1,0,1,	//p7

			0,	-0.25,	-0.404509,	1,	0,1,1,	//p8
			0,	0.25,	-0.404509,	1,	1,1,0,	//p9
			0,	0.25,	0.404509,	1,	0,0,1,	//p10
			0,	-0.25,	0.404509,	1,	0,1,0	//p11
	};

	glBindVertexArray(this->sphere_vao);

	GLuint vertexbuffer;
	glGenBuffers(1,&vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_vertices),sphere_vertices,GL_STATIC_DRAW);
	bufferList.push_back(vertexbuffer);
	
	GLuint sphere_elements[] =
		{
			0,11,10,
			0,10,7,
			0,4,11,
			4,5,11,
			1,11,5,
			1,10,11,
			1,6,10,
			6,7,10,

			0,7,3,
			0,3,4,
			7,9,3,
			9,8,3,
			8,4,3,

			1,2,6,
			1,5,2,
			2,9,6,
			2,8,9,
			2,5,8,

			7,6,9,
			4,8,5
		};

	GLuint elementbuffer;
	glGenBuffers(1,&elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(sphere_elements),sphere_elements,GL_STATIC_DRAW);
	bufferList.push_back(elementbuffer);

	//Setting Shader Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			7*sizeof(GLfloat),
			0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			7*sizeof(GLfloat),
			(void*)(4*sizeof(GLfloat))
	);

	int maxVertices;
	glGetIntegerv(GL_MAX_PATCH_VERTICES,&maxVertices);
	glPatchParameteri(GL_MAX_PATCH_VERTICES,maxVertices);
	glBindVertexArray(0);



	//box
	glBindVertexArray(this->box_vao);
	GLfloat box_vertices[] = {
		-0.5f, -0.5f, 0.5f,		0,0,1,		0,0,
		0.5f,  -0.5f, 0.5f,		0,0,1,		0,0,
		0.5f,  -0.5f, -0.5f,	0,0,-1,		0,0,
		-0.5f, -0.5f, -0.5f,	0,0,-1,		0,0,

		-0.5f, 0.5f, 0.5f,		0,0,1,		0,0,
		0.5f,  0.5f, 0.5f,		0,0,1,		0,0,
		0.5f,  0.5f, -0.5f,		0,0,-1,		0,0,
		-0.5f, 0.5f, -0.5f,		0,0,-1,		0,0
	};

	glGenBuffers(1,&vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(box_vertices),box_vertices,GL_STATIC_DRAW);
	bufferList.push_back(vertexbuffer);

	GLuint box_elements[] = {
		1,0,4, 4,5,1,
		1,5,6, 6,2,1,
		1,0,3, 3,2,1,
		
		3,7,4, 4,0,3,
		3,2,6, 6,7,3,
		6,7,4, 4,5,6
	};
	
	glGenBuffers(1,&elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(box_elements),box_elements,GL_STATIC_DRAW);
	bufferList.push_back(elementbuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8*sizeof(GLfloat),
			0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			8*sizeof(GLfloat),
			(void*)(3*sizeof(GLfloat))
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			8*sizeof(GL_FLOAT),
			(void*)(6*sizeof(GLfloat))
	);


	glBindVertexArray(0);
	//plate
	glBindVertexArray(this->plate_vao);

	GLfloat plate_vertices[] = {
		-0.5f, -0.5f, 0.f,	0,0,1,	0,0,
		0.5f, -0.5f, 0.f,		0,0,1,	0,0,
		0.5f, 0.5f, 0.f,		0,0,1,	0,0,
		-0.5f, 0.5f, 0.f,		0,0,1,	0,0
	};

	glGenBuffers(1,&vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(plate_vertices),plate_vertices,GL_STATIC_DRAW);
	bufferList.push_back(vertexbuffer);

	GLuint plate_elements[] = {
		0,1,2,
		2,3,0
	};
	
	glGenBuffers(1,&elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(plate_elements),plate_elements,GL_STATIC_DRAW);
	bufferList.push_back(elementbuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8*sizeof(GLfloat),
			0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			8*sizeof(GLfloat),
			(void*)(3*sizeof(GLfloat))
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			8*sizeof(GL_FLOAT),
			(void*)(6*sizeof(GLfloat))
	);

	glBindVertexArray(0);
	return true;
}


bool GraphicsEngine::prepareSprites(void){

	auto pball = new Ball();
	pball->setID(0);
	pball->setProgram(this->sphere_program);
	pball->setVAO(this->sphere_vao);
	pball->setCamera(this->main_camera);
	pball->setColour(glm::vec3(0,1,1));
	pball->setColourIntensity(glm::vec3(0.3,0.6,0.9));
	pball->setPosition(glm::vec3(5,-0.2,5));
	pball->setRadius(0.3);
	pball->setSX(1.0);
	pball->setSY(1.0);

	this->pb = pball;
    
    auto pTrailer = new TrailPoints(30);
    pTrailer->setProgram(this->sphere_program);
    pTrailer->setVAO(this->sphere_vao);
    pTrailer->setCamera(this->main_camera);
    pTrailer->setColour(glm::vec3(0,1,0));
    pTrailer->setColourIntensity(glm::vec3(0.3,0.6,0.9));
    pTrailer->setRadius(0.1);
    pTrailer->setPosition(glm::vec3(0,-0.4,0));

	auto pPath = new TrailPoints(100);
	pPath->setProgram(this->sphere_program);
    pPath->setVAO(this->sphere_vao);
    pPath->setCamera(this->main_camera);
    pPath->setColour(glm::vec3(1,1,0));
    pPath->setColourIntensity(glm::vec3(0.3,0.6,0.9));
    pPath->setRadius(0.1);
    pPath->setPosition(glm::vec3(0,-0.4,0));

	auto pTargetPoint = new VirtualAttractivePoint;

	auto pRobot = new Robot();
	pRobot->setID(1);
	pRobot->setProgram(this->box_program);
	pRobot->setVAO(this->box_vao);
	pRobot->setCamera(this->main_camera);
	pRobot->setColour(glm::vec3(1,0,0));
	pRobot->setColourIntensity(glm::vec3(0.3,0.6,0.9));
	pRobot->setPosition(glm::vec3(-5.f,-0.15,-1.f));
	pRobot->setScale(glm::vec3(0.7, 0.7, 0.7));
	pRobot->setAngle(glm::vec3(0,M_PI/4,0));
	pRobot->setSX(0.0);
	pRobot->setSY(0.0);
	//pRobot->setMovable(false);
    pRobot->addTrailer(pTrailer);
	pRobot->addPathview(pPath);
	pRobot->setTargetPoint(pTargetPoint);

	std::string robot_module_name = "Robot AStar Search";
	auto pAStar = new AIAStarSearch(robot_module_name);
	pRobot->addModule(robot_module_name,pAStar);
	//pRobot->activeAIModule(robot_module_name,1);
	pRobot->activeAIModule("",2);
	this->pr = pRobot;

	//Walls
	//Top & Bottom Walls
	auto pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1.0, 0.0, 0.0));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(0,0,-9));
	pWall->setScale(glm::vec3(20.6,1,1));
	pWall->setAngle(glm::vec3(0,0,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float iy = pWall->getY();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(tx < 103 && tx > -103){
			if(ty < iy+r){
				result.y = 100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1.0, 0.0, 0.0));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(0,0,9));
	pWall->setScale(glm::vec3(20.6,1,1));
	pWall->setAngle(glm::vec3(0,0,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float iy = pWall->getY();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(tx < 103 && tx > -103){
			if(ty > iy-r){
				result.y = -100.f;
			}
		}
		return true;
	});

	//Left & Right Walls
	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1, 1, 0));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(11,0,5.15));
	pWall->setScale(glm::vec3(6.3,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/2,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float ix = 110;
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(ty < 83 && ty > 20){
			if(tx >= ix-r){
				result.x = -100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,1,0));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-11,0,5.15));
	pWall->setScale(glm::vec3(6.3,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/2,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float ix = -110;
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(ty < 83 && ty > 20){
			if(tx <= ix+r){
				result.x = +100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1, 1, 1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(11,0,-5.15));
	pWall->setScale(glm::vec3(6.3,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/2,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float ix = 110;
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(ty < -20 && ty > -83){
			if(tx >= ix-r){
				result.x = -100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,1,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-11,0,-5.15));
	pWall->setScale(glm::vec3(6.3,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/2,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float ix = -110;
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(ty < -20 && ty > -83){
			if(tx <= ix+r){
				result.x = +100.f;
			}
		}
		return true;
	});

	//Gates
	//Right Gate
	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,0,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(11.75,0,-2));
	pWall->setScale(glm::vec3(1.5,1,1));
	pWall->setAngle(glm::vec3(0,0,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float iy = pWall->getY();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(tx > 110 && tx < 125){
			if(ty < iy+r){
				result.y = 100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,0,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(11.75,0,2));
	pWall->setScale(glm::vec3(1.5,1,1));
	pWall->setAngle(glm::vec3(0,0,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float iy = pWall->getY();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(tx > 110 && tx < 125){
			if(ty > iy-r){
				result.y = -100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,0,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(12.5,0,0));
	pWall->setScale(glm::vec3(4,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/2,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float ix = 125;
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(ty <= 20 && ty >= -20){
			if(tx > ix-r){
				result.x = -100.f;
			}
		}
		return true;
	});


	//Left Gate
	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,0,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-11.75,0,-2));
	pWall->setScale(glm::vec3(1.5,1,1));
	pWall->setAngle(glm::vec3(0,0,0));
		pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float iy = pWall->getY();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(tx > -125 && tx < -110){
			if(ty < iy+r){
				result.y = 100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,0,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-11.75,0,2));
	pWall->setScale(glm::vec3(1.5,1,1));
	pWall->setAngle(glm::vec3(0,0,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float iy = pWall->getY();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(tx > -125 && tx < -110){
			if(ty > iy-r){
				result.y = -100.f;
			}
		}
		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(1,0,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-12.5,0,0));
	pWall->setScale(glm::vec3(4,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/2,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float ix = -125;
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;
		if(ty <= 20 && ty >= -20){
			if(tx < ix+r){
				result.x = +100.f;
			}
		}
		return true;
	});

	//Corners
	GLfloat length = 0.7*sqrt(2.0);

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(0.3,0.3,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-10.65,0,-8.65));
	pWall->setScale(glm::vec3(length,1,1));
	pWall->setAngle(glm::vec3(0,3*M_PI/4,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;

		if(ty <= -83 && ty >= -90 && tx <= -103 && tx >= -110){
			//y = -x - 193
			float ey = -tx-193;
			if(ty <= ey+r){
				result.x += 100.f;
				result.y += 100.f;
			}
		}

		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(0.3,0.3,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(10.65,0,8.65));
	pWall->setScale(glm::vec3(length,1,1));
	pWall->setAngle(glm::vec3(0,3*M_PI/4,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;

		if(ty <= 90 && ty >= 83 && tx <= 110 && tx >= 103){
			//y = -x + 193
			float ey = -tx+193;
			if(ty >= ey-r){
				result.x -= 100.f;
				result.y -= 100.f;
			}
		}

		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(0.3,0.3,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-10.65,0,8.65));
	pWall->setScale(glm::vec3(length,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/4,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;

		if(ty <= 90 && ty >= 83 && tx <= -103 && tx >= -110){
			//y = x - 193
			float ey = tx-193;
			if(ty <= ey+r){
				result.x += 100.f;
				result.y -= 100.f;
			}
		}

		return true;
	});

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(0.3,0.3,1));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(10.65,0,-8.65));
	pWall->setScale(glm::vec3(length,1,1));
	pWall->setAngle(glm::vec3(0,M_PI/4,0));
	pWall->setForceFunc([pWall](RigidBody* dest, Point2F& result)->bool{
		if(dest->getID() == -2){
			return true;
		}
		float ty = dest->getY();
		float tx = dest->getX();
		float r = dest->getRadius();

		result.x = 0.f;
		result.y = 0.f;

		if(ty <= -83 && ty >= -90 && tx <= 110 && tx >= 103){
			//y = x + 193
			float ey = tx+193;
			if(ty <= ey-r){
				result.x -= 100.f;
				result.y += 100.f;
			}
		}

		return true;
	});

	//Flooor
	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(0.8,0.8,0.8));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(0,-0.5,0));
	pWall->setScale(glm::vec3(22,18,1));
	pWall->setAngle(glm::vec3(M_PI/2,0,0));

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(0.8,0.8,0.8));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(11.75,-0.5,0));
	pWall->setScale(glm::vec3(1.5,4.0,1));
	pWall->setAngle(glm::vec3(M_PI/2,0,0));

	pWall = new Wall();
	pWall->setProgram(this->box_program);
	pWall->setVAO(this->plate_vao);
	pWall->setCamera(this->main_camera);
	pWall->setColour(glm::vec3(0.8,0.8,0.8));
	pWall->setColourIntensity(glm::vec3(0.5, 0.6, 0.9));
	pWall->setPosition(glm::vec3(-11.75,-0.5,0));
	pWall->setScale(glm::vec3(1.5,4.0,1));
	pWall->setAngle(glm::vec3(M_PI/2,0,0));

	//Obstacles;

	//10 Regular Obstacles
    string name = "RegObs";
	Model* m = new Model(name);
	this->modelList.insert(make_pair(name,m));

	srand(time(NULL));

	for(int i=0;i<10;i++){

		float x = rand()%17;
		x += -8;

		float y = rand()%13;
		y += -6;

		float p = rand()%12+1;

		auto pObs = new Obstacle();
		pObs->setProgram(this->box_program);
		pObs->setVAO(this->box_vao);
		pObs->setCamera(this->main_camera);
		pObs->setColour(glm::vec3(0.3,0.3,0.3));
		pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
		pObs->setPosition(glm::vec3(x,-0.15,y));
		pObs->setScale(glm::vec3(0.7,0.7,0.7));
		pObs->setAngle(glm::vec3(0,2*M_PI/p,0));
		//pObs->setMovable(false);
		m->addSprite(pObs);
	}


	//Corners
	name = "Block";
	m = new Model(name);
	this->modelList.insert(make_pair(name,m));

	auto pObs = new Obstacle();
	pObs->setProgram(this->box_program);
	pObs->setVAO(this->box_vao);
	pObs->setCamera(this->main_camera);
	pObs->setColour(glm::vec3(0.3,0.3,0.3));
	pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
	pObs->setPosition(glm::vec3(1,-0.15,-4));
	pObs->setScale(glm::vec3(8.0,0.7,2.0));
	pObs->setAngle(glm::vec3(0,M_PI/6,0));
	pObs->setMovable(false);
	m->addSprite(pObs);

	pObs = new Obstacle();
	pObs->setProgram(this->box_program);
	pObs->setVAO(this->box_vao);
	pObs->setCamera(this->main_camera);
	pObs->setColour(glm::vec3(0.3,0.3,0.3));
	pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
	pObs->setPosition(glm::vec3(1,-0.15,0));
	pObs->setScale(glm::vec3(12.0,0.7,2.0));
	pObs->setAngle(glm::vec3(0,M_PI*2/3,0));
	pObs->setMovable(false);
	m->addSprite(pObs);

	pObs = new Obstacle();
	pObs->setProgram(this->box_program);
	pObs->setVAO(this->box_vao);
	pObs->setCamera(this->main_camera);
	pObs->setColour(glm::vec3(0.3,0.3,0.3));
	pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
	pObs->setPosition(glm::vec3(-3,-0.15,4));
	pObs->setScale(glm::vec3(8.0,0.7,2.0));
	pObs->setAngle(glm::vec3(0,M_PI/6,0));
	pObs->setMovable(false);
	m->addSprite(pObs);
	m->setActive(false);

	//Wiggle
	name = "Wiggle";
	m = new Model(name);
	this->modelList.insert(make_pair(name,m));

	for(int i=0;i<5;i++){
		float y = (float(i%2)-0.5)*9;
		float x = -8 + 4*i;

		pObs = new Obstacle();
		pObs->setProgram(this->box_program);
		pObs->setVAO(this->box_vao);
		pObs->setCamera(this->main_camera);
		pObs->setColour(glm::vec3(0.3,0.3,0.3));
		pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
		pObs->setPosition(glm::vec3(x,-0.15,y));
		pObs->setScale(glm::vec3(1.5,0.7,9.0));
		pObs->setAngle(glm::vec3(0,0,0));
		pObs->setMovable(false);
		m->addSprite(pObs);

	}
	m->setActive(false);

	//Trap
	name = "Trap";
	m = new Model(name);
	this->modelList.insert(make_pair(name,m));

	float gap = 2.f;

	for(int i=-1;i<2;i++){
		for(int j=-1;j<2;j++){

			if(!i && !j){
				continue;
			}

			float y = i*gap;
			float x = j*gap;

			auto pObs = new Obstacle();
			pObs->setProgram(this->box_program);
			pObs->setVAO(this->box_vao);
			pObs->setCamera(this->main_camera);
			pObs->setColour(glm::vec3(0.3,0.3,0.3));
			pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
			pObs->setPosition(glm::vec3(x,-0.15,y));
			pObs->setScale(glm::vec3(0.7,0.7,0.7));
			pObs->setAngle(glm::vec3(0,0,0));
			//pObs->setMovable(false);
			m->addSprite(pObs);
		}
	}
	m->setActive(false);

	//Cross
	name = "Cross";
	m = new Model(name);
	this->modelList.insert(make_pair(name,m));

	pObs = new Obstacle();
	pObs->setProgram(this->box_program);
	pObs->setVAO(this->box_vao);
	pObs->setCamera(this->main_camera);
	pObs->setColour(glm::vec3(0.3,0.3,0.3));
	pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
	pObs->setPosition(glm::vec3(0,-0.15,0));
	pObs->setScale(glm::vec3(12.0,0.7,2.0));
	pObs->setAngle(glm::vec3(0,M_PI/2,0));
	pObs->setMovable(false);
	m->addSprite(pObs);

	pObs = new Obstacle();
	pObs->setProgram(this->box_program);
	pObs->setVAO(this->box_vao);
	pObs->setCamera(this->main_camera);
	pObs->setColour(glm::vec3(0.3,0.3,0.3));
	pObs->setColourIntensity(glm::vec3(0.5,0.6,0.9));
	pObs->setPosition(glm::vec3(0,-0.15,0));
	pObs->setScale(glm::vec3(12.0,0.7,2.0));
	pObs->setAngle(glm::vec3(0,0,0));
	pObs->setMovable(false);
	m->addSprite(pObs);

	m->setActive(false);
	return true;
}

bool GraphicsEngine::prepareCamara(void){
	//look at the origin point always

	main_camera = new Camera(glm::vec3(0,30,30), glm::vec3(0,0,0), glm::vec3(0,1,0));

	return true;
}

bool GraphicsEngine::clear(void){

	glBindVertexArray(this->sphere_vao);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(this->box_vao);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);

	glBindVertexArray(this->plate_vao);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);

	SAFE_DEL(this->sphere_program);
	SAFE_DEL(this->box_program);
	SAFE_DEL(this->main_camera);
	

	for(auto& bf : this->bufferList){
		glDeleteBuffers(1,&bf);

	}

	glDeleteVertexArrays(1,&(this->sphere_vao));
	glDeleteVertexArrays(1,&(this->box_vao));
	glDeleteVertexArrays(1,&(this->plate_vao));

	for(auto& p : this->modelList){
		delete p.second;
	}
	modelList.clear();

	/*
	for(auto& sp : this->spriteList){
		delete sp;
	}*/

	return true;
}

Model* GraphicsEngine::getModel(const std::string& name){
	auto iter = this->modelList.find(name);
	if(iter!=this->modelList.end()){
		return iter->second;
	}
	return nullptr;
}

bool GraphicsEngine::loadModel(const std::string& name, Model* pm){
	auto iter = this->modelList.find(name);
	if(iter == this->modelList.end()){
		this->modelList.insert(make_pair(name,pm));
		return true;
	}
	return false;
}

void GraphicsEngine::inactiveAllModels(){
	for(auto& pr : this->modelList){
		(pr.second)->setActive(false);
	}
}

void GraphicsEngine::resetObject(const Point2F& robotPos, const Point2F& ballPos){

	this->pr->setPosition(glm::vec3(robotPos.x / 10.f, -0.15, robotPos.y / 10.f));
	this->pr->setSX(0);
	this->pr->setSY(0);
	this->pr->resetTouchCount();

	this->pb->setPosition(glm::vec3(ballPos.x / 10.f, -0.2, ballPos.y / 10.f));
	this->pb->setSX(0);
	this->pb->setSY(0);
	this->pb->resetTimer();
}

bool GraphicsEngine::loadProgram(void){

	std::map<int,string> shaders;

	shaders.insert(make_pair(GL_FRAGMENT_SHADER,"sphere.fsd"));
	shaders.insert(make_pair(GL_TESS_CONTROL_SHADER,"sphere.tcsd"));
	shaders.insert(make_pair(GL_TESS_EVALUATION_SHADER,"sphere.tesd"));
	shaders.insert(make_pair(GL_VERTEX_SHADER,"sphere.vsd"));

	this->sphere_program = new ShaderProgram;

	if(!sphere_program->load(shaders)){
		cout<<"Load Sphere Program Failed!"<<endl;
		return false;
	}

	shaders.clear();

	shaders.insert(make_pair(GL_FRAGMENT_SHADER,"box.fsd"));
	shaders.insert(make_pair(GL_VERTEX_SHADER,"box.vsd"));

	this->box_program = new ShaderProgram;

	if(!box_program->load(shaders)){
		cout<<"Load Box Program Failed!"<<endl;
		return false;
	}

	return true;
}

bool GraphicsEngine::setExternalFunction(std::function<void*(double,void*)> _ext){
	this->externalFunc = _ext;
	return true;
}

bool GraphicsEngine::mainLoop(){

	double dt;
	static double prevtime;
	static double time;
	
	auto sl = SpriteManager::getInstance();

	prevtime = time;
	time = glfwGetTime();

	dt = time - prevtime;

	this->handleAction(dt);
	
	glClearColor(0.15f,0.15f,0.15f,1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	if(this->externalFunc != nullptr){
		this->externalFunc(dt,nullptr);
	}

	if(resetBall){
		Point2F worldPos;
		this->main_camera->rayClip(worldPos,window);
		
		this->pb->setPosition(glm::vec3(worldPos.x, -0.2, worldPos.y));
		this->pb->setSX(0);
		this->pb->setSY(0);
		
		this->pr->setSX(0.f);
		this->pr->setSY(0.f);
		this->pr->clearAIData();



		resetBall = false;
	}
	RigidController::getInstance().calculateCollisionForce(dt);

	this->main_camera->update(dt,this->radius, this->persp_angle,this->ratio);

	if(showMode){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	
	sl->updateAll(dt);
	sl->drawAll(dt);
	
	glBindVertexArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();

	if(glfwWindowShouldClose(window)){
		return false;
	}

	glUseProgram(0);
	return true;
}

//Action Handle
void GraphicsEngine::handleAction(double time){
	
	if(pauseFlag){
		glfwSetTime(pausetime);
	}else{
		this->pausetime = glfwGetTime();
	}

	
	if(!this->manualFlag){
		return;
	}

	glm::vec3 nextTarget = this->main_camera->getTarget();
	glm::vec3 nextPosition = this->main_camera->getPosition();

	double cost = cos(this->persp_angle);
	double sint = sin(this->persp_angle);
	double cospt = cos(this->persp_angle + M_PI/2);
	double sinpt = sin(this->persp_angle + M_PI/2);

	if(keyWHold){
			nextTarget.z -= speedcoe * time * cost;
			nextPosition.z -= speedcoe * time * cost;
			nextTarget.x -= speedcoe * time * sint;
			nextPosition.x -= speedcoe * time * sint;
	}

	if(keySHold){
			nextTarget.z += speedcoe * time * cost;
			nextPosition.z += speedcoe * time * cost;
			nextTarget.x += speedcoe * time * sint;
			nextPosition.x += speedcoe * time * sint;
	}

	if(keyAHold){
			nextTarget.z -= speedcoe * time * cospt;;
			nextPosition.z -= speedcoe * time * cospt;
			nextTarget.x -= speedcoe * time * sinpt;
			nextPosition.x -= speedcoe * time * sinpt;
	}

	if(keyDHold){
			nextTarget.z += speedcoe * time * cospt;;
			nextPosition.z += speedcoe * time * cospt;
			nextTarget.x += speedcoe * time * sinpt;
			nextPosition.x += speedcoe * time * sinpt;
	}

	if(keyXHold){
		nextPosition.y -= speedcoe * time;
	}

	if(keyCHold){
		nextPosition.y += speedcoe * time;
	}

	if(keyEHold){
		persp_angle -= M_PI/6 * speedcoe * time;
	}

	if(keyQHold){
		persp_angle += M_PI/6 * speedcoe * time;
	}

	if(keyUPHold){
		nextTarget.y += speedcoe * time;
	}

	if(keyDOWNHold){
		nextTarget.y -= speedcoe * time;
	}

	this->main_camera->setTarget(nextTarget);
	this->main_camera->setPosition(nextPosition);

}
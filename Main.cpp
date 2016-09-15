#include "Main.h"

using namespace std;

int main(int argc, char** argv){
#ifdef _WIN32
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
#endif

	GlobalVariables::mainPath = argv[0];

	std::replace(GlobalVariables::mainPath.begin(),GlobalVariables::mainPath.end(),'\\','/');

	std::string::size_type off = GlobalVariables::mainPath.rfind('/');
	if(off != std::string::npos){
		GlobalVariables::mainPath = GlobalVariables::mainPath.substr(0,off+1);
	}

	auto gph = GraphicsEngine::getInstance();


	do{

		if(!gph->init()){
			cout<<"Graphics Engine Initialize Failed!"<<endl;
			break;
		}

		if(!gph->loadProgram()){
			cout<<"Load Shader Program Failed!"<<endl;
			break;
		}

		if(!gph->prepareVAO("")){
			cout<<"Prepare VAO Failed!"<<endl;
			break;
		}

		if(!gph->prepareVBO("")){
			cout<<"Prepare VBO Failed!"<<endl;
			break;
		}

		if(!gph->prepareCamara()){
			cout<<"Prepare Camera Failed!"<<endl;
			break;
		}

		if(!gph->prepareSprites()){
			cout<<"Prepare Sprites Failed!"<<endl;
			break;
		}

		while(gph->mainLoop()){
		
		}

	}while(false);

	GraphicsEngine::releaseInstance();

	cout<<"Program Ends!"<<endl;

	return 0;
}
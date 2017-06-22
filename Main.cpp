#include "Main.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

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

	time_t cur = time(nullptr);
	stringstream ss;
	ss<<"ErrLog_"<< std::put_time(std::localtime(&cur), "%y_%m_%d") <<".txt";
	ofstream fout;
	string filename = ss.str();
	
	fout.open(filename,ios::app);

	//redirect cerr to file
	if(fout){
		cerr.rdbuf(fout.rdbuf());
	}

	auto gph = GraphicsEngine::getInstance();





	do{

		if(!gph->init()){
			cerr<<"Graphics Engine Initialize Failed!"<<endl;
			break;
		}

		if(!gph->loadProgram()){
			cerr<<"Load Shader Program Failed!"<<endl;
			break;
		}

		if(!gph->prepareVAO("")){
			cerr<<"Prepare VAO Failed!"<<endl;
			break;
		}

		if(!gph->prepareVBO("")){
			cerr<<"Prepare VBO Failed!"<<endl;
			break;
		}

		if(!gph->prepareCamara()){
			cerr<<"Prepare Camera Failed!"<<endl;
			break;
		}

		if(!gph->prepareSprites()){
			cerr<<"Prepare Sprites Failed!"<<endl;
			break;
		}




		while(gph->mainLoop()){
		
		}

	}while(false);

	GraphicsEngine::releaseInstance();

	fout.close();
	
	cout<<"Program Ends!"<<endl;

	return 0;
}
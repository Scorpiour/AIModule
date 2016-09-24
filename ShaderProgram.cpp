#include "ShaderProgram.h"

using namespace std;


ShaderProgram::ShaderProgram(){

}

ShaderProgram::~ShaderProgram(){
	for(auto shdr : this->shaderList){
		glDetachShader(program, shdr);
	}

	for(auto shdr : this->shaderList){	
		glDeleteShader(shdr);
	}
		
	glDeleteProgram(program);
}

//Protected Methods
GLuint ShaderProgram::loadShader(const std::string& path,GLenum shader_type,GLuint program){
	
	GLuint shaderID = glCreateShader(shader_type);

	//load shader data
	ifstream fin(path.c_str(),ios::in);
	
	if(!fin.is_open()){
		cerr<<"Cannot open shader file : "<<path.c_str()<<endl;
		return 0;
	}
	fin.seekg(0,ios::end);
	size_t size = fin.tellg();
	fin.seekg(0,ios::beg);

	char* buffer = new char[size+1];
	memset(buffer,0,size+1);
	fin.read(buffer,size);
	fin.close();

	GLint result = GL_FALSE;
	int infosize;

	glShaderSource(shaderID,1,&buffer,nullptr);
	glCompileShader(shaderID);
	
	delete[] buffer;

	glGetShaderiv(shaderID,GL_COMPILE_STATUS,&result);
	glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&infosize);

	if(infosize > 0){
		char* errorbuffer = new char[infosize+1];
		glGetShaderInfoLog(shaderID,infosize,nullptr,errorbuffer);
		cerr<<errorbuffer<<endl;
		delete[] errorbuffer;
	}

	glAttachShader(program,shaderID);

	return shaderID;
}

void ShaderProgram::use(){
	glUseProgram(program);
}

void ShaderProgram::unuse(){
	glUseProgram(0);
}

bool ShaderProgram::load(const std::map<int,string>& shaderTable){
	//Compile & Link Shaders
	program = glCreateProgram();
	/*
	GLuint fragShader = loadShader("sphere.fsd",GL_FRAGMENT_SHADER,sphere_program);
	GLuint tcs = loadShader("sphere.tcsd",GL_TESS_CONTROL_SHADER,sphere_program);
	GLuint tes = loadShader("sphere.tesd",GL_TESS_EVALUATION_SHADER,sphere_program);
	GLuint vertShader = loadShader("sphere.vsd",GL_VERTEX_SHADER,sphere_program);
	*/
	for(auto& p : shaderTable){
		GLuint shdr = loadShader(p.second.c_str() , p.first, program);
		this->shaderList.push_back(shdr);
	}
	glLinkProgram(program);

	GLint result = 0;
	int infosize = 0;
	glGetProgramiv(program,GL_LINK_STATUS,&result);
	glGetProgramiv(program,GL_INFO_LOG_LENGTH,&infosize);
	if(infosize > 0){
		char* errorbuffer = new char[infosize+1];
		memset(errorbuffer,0,infosize+1);
		glGetProgramInfoLog(program,infosize,nullptr,errorbuffer);
		cerr<<errorbuffer<<endl;
		delete[] errorbuffer;
		return false;
	}
	
	/*
	viewMatrixLoc = glGetUniformLocation(sphere_program,"ViewMatrix");
	divideOuterLevelLoc = glGetUniformLocation(sphere_program,"divideOuterLevel");
	divideInnerLevelLoc = glGetUniformLocation(sphere_program,"divideInnerLevel");
	lightPosLoc = glGetUniformLocation(sphere_program,"lightPos");
	worldWLoc = glGetUniformLocation(sphere_program,"worldW");
	*/

	/*
	box_program = glCreateProgram();
	fragShader = loadShader("box.fsd",GL_FRAGMENT_SHADER,box_program);
	vertShader = loadShader("box.vsd",GL_VERTEX_SHADER,box_program);
	glLinkProgram(box_program);
	
	result = 0;
	infosize = 0;
	glGetProgramiv(box_program,GL_LINK_STATUS,&result);
	glGetProgramiv(box_program,GL_INFO_LOG_LENGTH,&infosize);
	if(infosize > 0){
		char* errorbuffer = new char[infosize+1];
		memset(errorbuffer,0,infosize+1);
		glGetProgramInfoLog(box_program,infosize,nullptr,errorbuffer);
		cerr<<errorbuffer<<endl;
		delete[] errorbuffer;
		return false;
	}

	shaderList.push_back(fragShader);
	shaderList.push_back(vertShader);*/
    
    return true;
}

GLint ShaderProgram::getLoc(const std::string& locName){
	return glGetUniformLocation(this->program,locName.c_str());
}

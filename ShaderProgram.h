#ifndef __INCLUDE_SHADER_PROGRAM_H__
#define __INCLUDE_SHADER_PROGRAM_H__

#include "GlobalVariable.h"

typedef class ShaderProgram{
protected:
	GLuint program;
	std::vector<GLuint> shaderList;
	GLuint loadShader(const std::string& path,GLenum shader_type,GLuint program);
public:

	ShaderProgram();
	~ShaderProgram();

	bool load(const std::map<int,std::string>& shaderTable);
	void use();
	void unuse();

	GLint getLoc(const std::string& locName);
}*pShaderProgram;


#endif
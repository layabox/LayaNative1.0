/*
 Copyright 2009 Johannes Vuorinen
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at 
 
 http://www.apache.org/licenses/LICENSE-2.0 
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <cstring>
#include <cstdlib>
#include "Shader.h"
#include "OpenGLESUtil.h"
#include "OpenGLESConfig.h"
#include "ShaderSource.h"
#include "ShaderFile.h"

using namespace OpenGLES::OpenGLES2;

Shader::Shader(GLenum type, std::vector<ShaderSource *> &sources) : type(type), sources(sources)
{

}

Shader::~Shader()
{
}

GLuint Shader::compile() 
{
	id = glCreateShader(type);

	const char* typeString = type == GL_FRAGMENT_SHADER ? "Fragment shader" : "Vertex shader";

	if (id == 0 && typeString)
	{
		LOG_MESSAGE(__FILE__, __LINE__, OpenGLESString("ERROR: Could not create ") + typeString);
		return 0;
	}

	if (!readShaderSource()) 
	{
		LOG_MESSAGE(__FILE__, __LINE__, OpenGLESString("ERROR: Could not read ") + typeString + OpenGLESString(" source."));
		return 0;
	}

	glCompileShader(id);

	GLint compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);

	if (!compiled || OpenGLESConfig::DEBUGEX)
	{
		GLint infoLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 1)
		{
			char *infoLog = (char *)malloc(sizeof(char) * infoLength);

			glGetShaderInfoLog(id, infoLength, NULL, infoLog);

			if (compiled) 
			{
				LOG_MESSAGE(__FILE__, __LINE__, OpenGLESString("WARNING: Compiled ") + typeString + " with warnings:\n" + infoLog);
			} 
			else 
			{
				LOG_MESSAGE(__FILE__, __LINE__, OpenGLESString("ERROR: Compiling ") + typeString + " failed:\n" + infoLog);
			}
			free(infoLog);
		}

		if (compiled != 0)
		{
			LOG_MESSAGE(OpenGLESString("Compiled ") + typeString + " successfully.");
		}
		else
		{
			glDeleteShader(id);
			return 0;
		}
	}

	return id;
}

char *convertStringToChar(const std::string &str)
{
	char *retPtr(new char[str.length() + 1]);

	strcpy(retPtr, str.c_str());

	return retPtr;
}

bool Shader::readShaderSource()
{
	char **shaderSources = (char **)malloc(sizeof(char *) * sources.size());

	if (shaderSources == NULL)
	{
		LOG_MESSAGE(__FILE__, __LINE__, "ERROR: Cannot allocate memory.");
		return false;
	}

	for (size_t i = 0; i < sources.size(); i++) 
	{
		shaderSources[i] = convertStringToChar(sources[i]->getSource());
	}

	glShaderSource(id, (GLsizei)sources.size(), (const char **)shaderSources, NULL);

	for (size_t i = 0; i < sources.size(); i++)
	{
		free(shaderSources[i]);
	}

	free(shaderSources);

	return true;
}

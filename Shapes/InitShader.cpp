
#include "Angel.h"

namespace Angel {

	unsigned long getFileLength(std::ifstream& file)
	{
		if (!file.good()) return 0;

		unsigned long pos = file.tellg();
		file.seekg(0, std::ios::end);
		unsigned long len = file.tellg();
		file.seekg(std::ios::beg);

		return len;
	}

	static char* readShaderSource1(const char* filename)
	{
		std::ifstream myfile;
		myfile.open(filename, std::ios::in);
		if (!myfile) return NULL;

		unsigned long len = getFileLength(myfile);

		if (len == 0) return NULL;   // "Empty File" 

		char* ShaderSource = new char[len + 1];
		if (ShaderSource == 0) return NULL;   // can't reserve memory

		ShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
								// it is important to 0-terminate the real length later, len is just max possible value...
		unsigned int i = 0;
		while (myfile.good())
		{
			ShaderSource[i] = myfile.get();       // get character from file.
			if (!myfile.eof())
				i++;
		}

		ShaderSource[i] = 0;  // 0 terminate it.

		myfile.close();

		return ShaderSource;
	}

	// Create a GLSL program object from vertex and fragment shader files
	GLuint
		InitShader(const char* vShaderFile, const char* fShaderFile)
	{
		struct Shader {
			const char*  filename;
			GLenum       type;
			GLchar*      source;
		}  shaders[2] = {
		{ vShaderFile, GL_VERTEX_SHADER, NULL },
		{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
		};

		GLuint program = glCreateProgram();


		for (int i = 0; i < 2; ++i) {
			Shader& s = shaders[i];
			s.source = readShaderSource1(s.filename);
			//s.source = readShaderSource( s.filename );
			if (shaders[i].source == NULL) {
				std::cerr << "Failed to read " << s.filename << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cerr << s.source << std::endl;

			GLuint shader = glCreateShader(s.type);

			glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
			glCompileShader(shader);

			GLint  compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				std::cerr << s.filename << " failed to compile:" << std::endl;
				GLint  logSize;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
				char* logMsg = new char[logSize];
				glGetShaderInfoLog(shader, logSize, NULL, logMsg);
				std::cerr << logMsg << std::endl;
				delete[] logMsg;
				exit(EXIT_FAILURE);
			}

			delete[] s.source;

			glAttachShader(program, shader);
		}

		/* link  and error check */
		glLinkProgram(program);

		GLint  linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked) {
			std::cerr << "Shader program failed to link" << std::endl;
			GLint  logSize;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetProgramInfoLog(program, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete[] logMsg;

			exit(EXIT_FAILURE);
		}

		/* use program object */
		glUseProgram(program);

		return program;
	}

}  // Close namespace Angel block

#include "ComputeShader.h"

ComputeShader::ComputeShader() = default;

ComputeShader::ComputeShader(const string &path)
{
	ImportShader(path);
}

ComputeShader::operator GLuint() const
{
	return ProgramID;
}

bool ComputeShader::ImportShader(const std::string &path)
{
	GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);

	string ComputeShaderCode;

	ifstream ComputeShaderStream(path);

	if (ComputeShaderStream.is_open())
	{
		stringstream sstr;
		sstr << ComputeShaderStream.rdbuf();
		ComputeShaderCode = sstr.str();
		ComputeShaderStream.close();
	}
	else
	{
		cout << "Impossible to open \"" << path << "\" Are you in the right directory?" << endl;
		getchar();
		return false;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Compute Shader
	cout << "Compiling shader: \"" << path << "\"\n";
	char const *ComputeSourcePointer = ComputeShaderCode.c_str();
	glShaderSource(ComputeShaderID, 1, &ComputeSourcePointer, NULL);
	glCompileShader(ComputeShaderID);

	// Check Compute Shader
	glGetShaderiv(ComputeShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ComputeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> ComputeShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(ComputeShaderID, InfoLogLength, NULL, &ComputeShaderErrorMessage[0]);
		printf("%s\n", &ComputeShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, ComputeShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, ComputeShaderID);

	glDeleteShader(ComputeShaderID);

	return true;
}

bool ComputeShader::BindShader()
{
	try
	{
		glUseProgram(ProgramID);
	}
	catch (int e)
	{
		cout << "Error enabling Shader. Maybe not loaded yet. Error code: " << e << "\n";
		return false;
	}

	return true;
}

template <class U>
void ComputeShader::Copy(U &&other)
{
	if (&other != this)
	{
		Release();

		GlStorage::Copy(other.id);
		ProgramID = other.ProgramID;
	}
}

ComputeShader::ComputeShader(ComputeShader &&other)
{
	Copy(other);
}

ComputeShader::ComputeShader(const ComputeShader &other)
{
	Copy(other);
}

ComputeShader &ComputeShader::operator=(ComputeShader &&other)
{
	Copy(other);
	return *this;
}

ComputeShader &ComputeShader::operator=(const ComputeShader &other)
{
	Copy(other);
	return *this;
}

bool ComputeShader::Release()
{
	if (GlStorage::Release())
	{
		glDeleteProgram(ProgramID);
		return true;
	}
	return false;
}

ComputeShader::~ComputeShader()
{
	Release();
}
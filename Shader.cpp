#include "Shader.h"

Shader::Shader() = default;

Shader::Shader(const string &vertexPath, const string &fragmentPath)
{
    ImportShader(vertexPath, fragmentPath);
}

Shader::operator GLuint() const
{
    return ProgramID;
}

bool Shader::ImportShader(const string &vertexPath, const string &fragmentPath)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    string VertexShaderCode;
    ifstream VertexShaderStream(vertexPath);

    if (VertexShaderStream.is_open())
    {
        stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else
    {
        cout << "Impossible to open \"" << vertexPath << "\" Are you in the right directory?" << endl;
        getchar();
        return false;
    }

    // Read the Fragment Shader code from the file
    string FragmentShaderCode;
    ifstream FragmentShaderStream(fragmentPath);

    if (FragmentShaderStream.is_open())
    {
        stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }
    else
    {
        cout << "Impossible to open \"" << fragmentPath << "\" Are you in the right directory?" << endl;
        getchar();
        return false;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    cout << "Compiling shader: \"" << vertexPath << "\"\n";
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    cout << "Compiling shader: \"" << fragmentPath << "\"\n";
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
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

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return true;
}

bool Shader::BindShader()
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
void Shader::Copy(U &&other)
{
    if (&other != this)
    {
        Release();

        GlStorage::Copy(other.id);
        ProgramID = other.ProgramID;
    }
}

Shader::Shader(Shader &&other)
{
    Copy(other);
}

Shader::Shader(const Shader &other)
{
    Copy(other);
}

Shader &Shader::operator=(Shader &&other)
{
    Copy(other);
    return *this;
}

Shader &Shader::operator=(const Shader &other)
{
    Copy(other);
    return *this;
}

bool Shader::Release()
{
    if (GlStorage::Release())
    {
        glDeleteProgram(ProgramID);
        return true;
    }
    return false;
}

Shader::~Shader()
{
    Release();
}
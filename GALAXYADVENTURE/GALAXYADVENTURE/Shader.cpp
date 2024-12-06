#include "Shader.h"


char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

void make_vertexShaders(GLuint& vertexShader, string shaderName)
{
	GLchar* vertexSource;
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
	vertexSource = filetobuf(shaderName.c_str());
	//--- ���ؽ����̴���ü�����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴��ڵ带���̴���ü���ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ����̴��������ϱ�
	glCompileShader(vertexShader);
	//--- ������������ε����������: ����üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{

		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}

//--- �����׸�Ʈ ���̴� ��ü �����
void make_fragmentShaders(GLuint& fragmentShader, string shaderName)
{
	GLchar* fragmentSource;
	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentSource = filetobuf(shaderName.c_str()); // �����׼��̴� �о����
	//--- �����׸�Ʈ���̴���ü�����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴��ڵ带���̴���ü���ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ���̴�������
	glCompileShader(fragmentShader);
	//--- ������������ε����������: �����Ͽ���üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return;
	}
}

//--- ���̴� ���α׷� ����� ���̴� ��ü ��ũ�ϱ�
GLvoid make_shaderProgram(GLuint& shaderProgramID, GLuint& vertexShader, GLuint& fragmentShader)
{
	//-- shader Program
	shaderProgramID = glCreateProgram();
	//--- ���ؽ����̴������
   //--- �����׸�Ʈ���̴������
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴������ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);

}

void CreateTriBuffer(GLuint& VAO, GLuint VBO[2], GLfloat triShape[9], GLfloat colors[9])
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(2, VBO);
	//--- 1��° VBO��Ȱ��ȭ�Ͽ����ε��ϰ�, ���ؽ��Ӽ�(��ǥ��)������
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//--- ���� diamond �������ؽ������Ͱ������ۿ������Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	//--- ��ǥ����attribute �ε���0��������Ѵ�: ���ؽ���3*float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ���
	glEnableVertexAttribArray(0);

	//--- 2��° VBO��Ȱ��ȭ�Ͽ����ε��ϰ�, ���ؽ��Ӽ�(����)������
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//--- ���� colors���� ���ؽ������������Ѵ�.
	//--- colors �迭�� ������: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	//--- ������attribute �ε���1��������Ѵ�: ���ؽ���3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��밡���ϰ���.
	glEnableVertexAttribArray(1);
}

// �ε������� ����
void CreateRectBuffer(GLfloat vPositionList[], GLuint& VAO, GLuint& EBO, GLuint VBO[2], GLfloat colors[12])
{
	unsigned int index[] = {
	0,1,3,
	1,2,3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vPositionList, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);		//--- GL_ELEMENT_ARRAY_BUFFER �����������ι��ε�
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//--- 2��° VBO��Ȱ��ȭ�Ͽ����ε��ϰ�, ���ؽ��Ӽ�(����)������
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//--- ���� colors���� ���ؽ������������Ѵ�.
	//--- colors �迭�� ������: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	//--- ������attribute �ε���1��������Ѵ�: ���ؽ���3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��밡���ϰ���.
	glEnableVertexAttribArray(1);
}

void CreateIndexBuffer(GLuint& VAO, GLuint VBO[2], GLuint& EBO, GLfloat point[12], GLfloat colors[12], int shape, unsigned int index[])
{
	float size = 0;
	float indexsize = 0;
	if (shape == 1)
	{
		indexsize = 1 * sizeof(unsigned int);
		size = 3 * sizeof(GLfloat);
	}
	else if (shape == 2)
	{
		indexsize = 2 * sizeof(unsigned int);
		size = 6 * sizeof(GLfloat);
	}
	else if (shape == 3)
	{
		indexsize = 3 * sizeof(unsigned int);
		size = 9 * sizeof(GLfloat);
	}
	else if (shape == 4)
	{
		indexsize = 6 * sizeof(unsigned int);
		size = 12 * sizeof(GLfloat);
	}
	else if (shape == 5)
	{
		indexsize = 9 * sizeof(unsigned int);
		size = 15 * sizeof(GLfloat);
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, size, point, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);		//--- GL_ELEMENT_ARRAY_BUFFER �����������ι��ε�
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//--- 2��° VBO��Ȱ��ȭ�Ͽ����ε��ϰ�, ���ؽ��Ӽ�(����)������
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//--- ���� colors���� ���ؽ������������Ѵ�.
	//--- colors �迭�� ������: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, size, colors, GL_STATIC_DRAW);
	//--- ������attribute �ε���1��������Ѵ�: ���ؽ���3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��밡���ϰ���.
	glEnableVertexAttribArray(1);
}

void CreateModel(GLuint& VAO, GLuint VBO[2], GLuint& EBO, Model model, GLfloat color[], size_t sizeColor)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Model::Vertex), model.vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)offsetof(Model::Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)offsetof(Model::Vertex, normal));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.faces.size() * sizeof(Model::Face), model.faces.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

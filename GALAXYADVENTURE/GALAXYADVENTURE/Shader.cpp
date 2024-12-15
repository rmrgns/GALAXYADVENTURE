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
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf(shaderName.c_str());
	//--- 버텍스세이더객체만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더코드를세이더객체에넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스세이더컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이제대로되지않은경우: 에러체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{

		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

//--- 프래그먼트 세이더 객체 만들기
void make_fragmentShaders(GLuint& fragmentShader, string shaderName)
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf(shaderName.c_str()); // 프래그세이더 읽어오기
	//--- 프래그먼트세이더객체만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더코드를세이더객체에넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트세이더컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이제대로되지않은경우: 컴파일에러체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

//--- 세이더 프로그램 만들고 세이더 객체 링크하기
GLvoid make_shaderProgram(GLuint& shaderProgramID, GLuint& vertexShader, GLuint& fragmentShader)
{
	//-- shader Program
	shaderProgramID = glCreateProgram();
	//--- 버텍스세이더만들기
   //--- 프래그먼트세이더만들기
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	//glUseProgram(shaderProgramID);

}

void CreateTriBuffer(GLuint& VAO, GLuint VBO[2], GLfloat triShape[9], GLfloat colors[9])
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(2, VBO);
	//--- 1번째 VBO를활성화하여바인드하고, 버텍스속성(좌표값)을저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//--- 변수 diamond 에서버텍스데이터값을버퍼에복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	//--- 좌표값을attribute 인덱스0번에명시한다: 버텍스당3*float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를활성화하여바인드하고, 버텍스속성(색상)을저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//--- 변수 colors에서 버텍스색상을복사한다.
	//--- colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	//--- 색상값을attribute 인덱스1번에명시한다: 버텍스당3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용가능하게함.
	glEnableVertexAttribArray(1);
}

// 인덱스버퍼 생성
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);		//--- GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를활성화하여바인드하고, 버텍스속성(색상)을저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//--- 변수 colors에서 버텍스색상을복사한다.
	//--- colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	//--- 색상값을attribute 인덱스1번에명시한다: 버텍스당3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용가능하게함.
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);		//--- GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를활성화하여바인드하고, 버텍스속성(색상)을저장
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//--- 변수 colors에서 버텍스색상을복사한다.
	//--- colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, size, colors, GL_STATIC_DRAW);
	//--- 색상값을attribute 인덱스1번에명시한다: 버텍스당3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용가능하게함.
	glEnableVertexAttribArray(1);
}

void CreateModel(GLuint& VAO, GLuint VBO[2], GLuint& EBO, Model model)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// VBO에 정점 데이터 업로드
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Model::Vertex), model.vertices.data(), GL_STATIC_DRAW);

	// Position attribute 설정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)offsetof(Model::Vertex, position));
	glEnableVertexAttribArray(0);

	// Normal attribute 설정
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)offsetof(Model::Vertex, normal));
	glEnableVertexAttribArray(1);

	// Texture coordinate attribute 설정
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)offsetof(Model::Vertex, texCoord));
	glEnableVertexAttribArray(2);


	
	// EBO에 인덱스 데이터 업로드
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.faces.size() * sizeof(Model::Face), model.faces.data(), GL_STATIC_DRAW);

	glBindVertexArray(0); // VAO 언바인딩
}

void CreateModel2(GLuint& VAO, GLuint VBO[2], GLuint& EBO, Model2 model)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// VBO에 정점 데이터 업로드
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Model2::Vertex), model.vertices.data(), GL_STATIC_DRAW);

	// Position attribute 설정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model2::Vertex), (void*)offsetof(Model2::Vertex, position));
	glEnableVertexAttribArray(0);

	// Normal attribute 설정
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model2::Vertex), (void*)offsetof(Model2::Vertex, normal));
	glEnableVertexAttribArray(1);



	// EBO에 인덱스 데이터 업로드
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.faces.size() * sizeof(Model2::Face), model.faces.data(), GL_STATIC_DRAW);

	glBindVertexArray(0); // VAO 언바인딩
}

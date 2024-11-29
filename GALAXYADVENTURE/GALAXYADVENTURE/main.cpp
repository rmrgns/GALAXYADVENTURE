#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <random>
#include <cmath>
#include "game.h"
#include "Shape.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60

GLuint vao, vbo[2], ebo, axesVAO, axesVBO;
void drawAxes();
void convertXY(int x, int y, float& fx, float& fy);
void UpdateBuffer();
void InitBuffer();
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKey(int key, int x, int y);

GLvoid Timer(int value);

static const int index = 1;
float bGCr = 1.0, bGCg = 1.0, bGCb = 1.0;
GLuint shaderPID;

void drawAxes()
{
	glm::vec3 axesVertices[] = {
		glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 시작점 (빨간색)
		glm::vec3(1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 끝점
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // y축 시작점 (녹색)
		glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  // y축 끝점
		glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), // z축 시작점 (파랑)
		glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),  // z축 끝점
	};
	glGenVertexArrays(1, &axesVAO);
	glGenBuffers(1, &axesVBO);

	glBindVertexArray(axesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);

	// 위치 속성
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 색상 속성
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Galaxy Adventure");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	Game_Start();
	glEnable(GL_DEPTH_TEST);

	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutTimerFunc(1000 / FPS, Timer, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform"); //뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 10.0f); //직교 투형
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); //원근 투형
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(axesVAO);
	glm::mat4 axesTransform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(axesTransform));
	glDrawArrays(GL_LINES, 0, 6);

	UpdateBuffer();

	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	player.DrawPlayer();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void convertXY(int x, int y, float& fx, float& fy)
{
	int w = WINDOW_WIDTH;
	int h = WINDOW_HEIGHT;
	fx = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	fy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	Game_Keyboard(key, x, y);

	if (key == 'q')
		glutLeaveMainLoop();

	glutPostRedisplay();
}

GLvoid SpecialKey(int key, int x, int y)
{

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float fx = 0.0, fy = 0.0;
	convertXY(x, y, fx, fy);

	Game_Mouse(button, state, fx, fy);
}

GLvoid Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, Timer, 1);
}



/*
=========동작이 의도한대로 되지 않을때 확인할 것=========

1. 버퍼 크기를 할당했는가
2. 버퍼와 인덱스의 간격을 적정하게 주었는가
3. 접근하려는 버퍼의 인덱스가 적절한가

=========자주 실수하는 부분이니까 반드시 확인=========
*/



void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기


	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//--- 변수 colors에서 버텍스 색상을 복사한다.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

	drawAxes();
}

void UpdateBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(player.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(player.shapecolor[0]));

}
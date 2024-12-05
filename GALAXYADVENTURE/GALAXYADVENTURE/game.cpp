#include "game.h"

Game game;

GLvoid Game::drawScene()
{
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(game.shaderProgramID);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(game.shaderProgramID, "viewTransform"); //뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 10.0f); //직교 투형
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); //원근 투형
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));
	unsigned int projectionLocation = glGetUniformLocation(game.shaderProgramID, "projectionTransform"); //투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(axesVAO);
	glm::mat4 axesTransform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(game.shaderProgramID, "modelTransform");

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(axesTransform));
	glDrawArrays(GL_LINES, 0, 6);

	game.Update();

	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	game.player.DrawPlayer();
	//GLfloat color[] = {
	//   0.0f, 1.0f, 0.0f,//1
	//   0.0f, 1.0f, 1.f,//2
	//   1.f, 1.0f, 0.f,//3

	//   1.0f, 0.0f, 1.f,//4
	//   0.0f, 0.f, 1.f,//5
	//   1.f, 0.f, 0.f,//6

	//   0.f, 1.0f, 0.5f,//7
	//	0.5f, 1.f, 1.f,//8

	//};
	//GLuint VAO, VBO[2], EBO;
	//CreateModel(VAO, VBO, EBO, game.star.getModel(), color, sizeof(color));
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

GLvoid Game::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutLeaveMainLoop();

	//w, a, s, d로 비행기 조작
	game.player.Control(key, KEY_DOWN);

	glutPostRedisplay();
}

GLvoid Game::KeyboardUp(unsigned char key, int x, int y)
{
	game.player.Control(key, KEY_UP);

	return GLvoid();
}

GLvoid Game::Mouse(int button, int state, int x, int y)
{
	float fx = 0.0, fy = 0.0;
	game.convertXY(x, y, fx, fy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		game.holdmouse = true;
		game.mouseX = x;
		game.mouseY = y;
		game.prevmouseX = x;
		game.prevmouseY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		game.holdmouse = false;
		game.player.Tilt(0, 0);
	}
}

GLvoid Game::Motion(int x, int y)
{
	game.mouseX = x;
	game.mouseY = y;
}

GLvoid Game::timerFunction(int n)
{
	//마우스를 누르고 움직이면 해당 방향으로 기체 회전
	//비행기는 자동으로 -z 방향으로 이동
	//입력받은 조작이 있으면 해당 방향으로 x, y 이동
	//비행기 이동에 맞춰서 카메라 위치, 보는 방향 이동
	if (game.holdmouse)
	{
		game.player.Tilt(game.mouseX - game.prevmouseX, game.mouseY - game.prevmouseY);
	}
	game.player.Move_by_Time();
	cameraPos += game.player.GetMoveValue();
	cameraDirection += game.player.GetMoveValue();

	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timerFunction, 1);
}

void Game::convertXY(int x, int y, float& fx, float& fy)
{
	int w = WINDOW_WIDTH;
	int h = WINDOW_HEIGHT;
	fx = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	fy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

void Game::utilityFunctions()
{
	glutKeyboardFunc(Keyboard);			// 키보드 입력
	glutKeyboardUpFunc(KeyboardUp);			// 키보드 입력
	//glutSpecialFunc(SpecialKeyboard);	// 키보드 특수 키 입력
	glutMouseFunc(Mouse);				// 마우스 입력
	glutMotionFunc(Motion);				// 마우스 움직임
	glutTimerFunc(16, timerFunction, 0);	// 타이머
}

void Game::Init()
{
	player = Player();
	//star = Star();
	InitBuffer();
}

void Game::Update()
{
	UpdateBuffer();
}

void Game::InitBuffer()
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

void Game::UpdateBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(player.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(player.shapecolor[0]));

}

void Game::drawAxes()
{
	glm::vec3 axesVertices[] = {
		   glm::vec3(-1000.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 시작점 (빨간색)
		   glm::vec3(1000.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x축 끝점
		   glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // y축 시작점 (녹색)
		   glm::vec3(0.0f,  1000.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  // y축 끝점
		   glm::vec3(0.0f, 0.0f, -1000.0f), glm::vec3(0.0f, 0.0f, 1.0f), // z축 시작점 (파랑)
		   glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 1.0f),  // z축 끝점
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
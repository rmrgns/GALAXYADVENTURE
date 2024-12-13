#include "game.h"

Game game;

GLvoid Game::drawScene()
{
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(game.shaderProgramID);

	game.cameraSet();
	game.projectionSet();

	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 10.0f); //���� ����
	//glBindVertexArray(axesVAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	game.player.DrawPlayer(game.getShaderProgramID(), game.transformLoc);
	//GLfloat color[] = {
	//   0.0f, 1.0f, 0.0f,//1
	//   0.0f, 1.0f, 1.f,//2
	//   1.f, 1.0f, 0.f,//3

	//   1.0f, 0.0f, 1.f,//4
	//   0.0f, 0.f, 1.f,//5
	//   1.f, 0.f, 0.f,//6

	//   0.f, 1.0f, 0.5f,//7
	//	0.5f, 1.f, 1.f,//8


	/*CreateModel(VAO, VBO, EBO, game.star.front().getModel(), color, sizeof(color));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
	game.light();
	glm::mat4 StarMatrix(1.f);
	glm::vec3 objectColor(0.8f, 0.3f, 0.3f);
	for (const auto& s : game.star)
	{
		s.Draw(game.getShaderProgramID(), game.transformLoc);
	}


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

	float fixedDeltaTime = 1.0f / FPS; // 60FPS ����
	game.Update(fixedDeltaTime);

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
	transformLoc = glGetUniformLocation(shaderProgramID, "modelTransform");
	player = Player();
	//star = Star();

	for (int i{}; i < STAR_COUNT; i++)
	{
		star.emplace_back(Star());
	}

	InitBuffer();
	glutTimerFunc(1000 / FPS, timerFunction, 1);
}

void Game::Update(float time)
{
	UpdateBuffer();
	for (auto& s : star)
	{
		s.Update();
	}

	//마우스를 누르고 움직이면 해당 방향으로 기체 회전
	//비행기는 자동으로 -z 방향으로 이동
	//입력받은 조작이 있으면 해당 방향으로 x, y 이동
	//비행기 이동에 맞춰서 카메라 위치, 보는 방향 이동
	if (game.holdmouse)
	{
		game.player.Tilt(game.mouseX - game.prevmouseX, game.mouseY - game.prevmouseY);
		game.prevmouseX = game.mouseX;
		game.prevmouseY = game.mouseY;
	}
	game.player.Move_by_Time();
	cameraPos = game.player.translation + glm::vec3(5.0f * glm::sin(game.player.angle.y), 3.0f, 5.0f * glm::cos(game.player.angle.y));
	cameraDirection = game.player.translation;

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

}

void Game::UpdateBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(player.shapecoord[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_POINTS * 3 * sizeof(GLfloat), glm::value_ptr(player.shapecolor[0]));

}

void Game::cameraSet()
{
	glm::mat4 cameraMatrix(1.0f);
	glm::mat4 cameraTranslate(1.f);
	glm::mat4 cameraRotate(1.f);


	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	cameraTranslate = glm::translate(cameraTranslate, camTranslate);
	cameraRotate = glm::rotate(cameraRotate, glm::radians(camRotate.y), glm::vec3(0.f, 1.f, 0.f));
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	cameraMatrix = view * cameraRotate;

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Game::projectionSet()
{
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	//glm::mat4 projection2 = glm::mat4(1.0f);
	if (projType == 1)
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	else
	{
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));
	}


	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void Game::light()
{
	glUniform3fv(glGetUniformLocation(shaderProgramID, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(shaderProgramID, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(shaderProgramID, "viewPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 5.0f)));
}


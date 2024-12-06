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
	glm::mat4 axesTransform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(game.shaderProgramID, "modelTransform");

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(axesTransform));
	glDrawArrays(GL_LINES, 0, 6);

	game.Update();

	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	game.player.DrawPlayer();
	GLfloat color[] = {
	   0.0f, 1.0f, 0.0f,//1
	   0.0f, 1.0f, 1.f,//2
	   1.f, 1.0f, 0.f,//3

	   1.0f, 0.0f, 1.f,//4
	   0.0f, 0.f, 1.f,//5
	   1.f, 0.f, 0.f,//6

	   0.f, 1.0f, 0.5f,//7
		0.5f, 1.f, 1.f,//8

	};
	GLuint VAO, VBO[2], EBO;
	/*CreateModel(VAO, VBO, EBO, game.star.front().getModel(), color, sizeof(color));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

	glm::mat4 StarMatrix(1.f);

	for (const auto& s : game.star)
	{
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(StarMatrix));
		CreateModel(VAO, VBO, EBO, s.getModel(), color, sizeof(color));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 240, GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();
}

GLvoid Game::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutLeaveMainLoop();

	glutPostRedisplay();
}

GLvoid Game::Mouse(int button, int state, int x, int y)
{
	float fx = 0.0, fy = 0.0;
	game.convertXY(x, y, fx, fy);
}

GLvoid Game::Motion(int x, int y)
{
    
}

GLvoid Game::timerFunction(int n)
{
	float fixedDeltaTime = 1.0f / 60.0f; // 60FPS ����
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
    glutKeyboardFunc(Keyboard);			// Ű���� �Է�
    //glutSpecialFunc(SpecialKeyboard);	// Ű���� Ư�� Ű �Է�
    glutMouseFunc(Mouse);				// ���콺 �Է�
    glutMotionFunc(Motion);				// ���콺 ������
    glutTimerFunc(16, timerFunction, 0);	// Ÿ�̸�
}

void Game::Init()
{
	player = Player();
	//star = Star();
	for (int i{}; i < STAR_COUNT; i++)
	{
		star.emplace_back(Star());
	}
	
	InitBuffer();
}

void Game::Update()
{
	UpdateBuffer();
	for (auto& s : star)
	{
		s.Update();
	}
}

void Game::InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�


	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
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
		   glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x�� ������ (������)
		   glm::vec3(1.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // x�� ����
		   glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // y�� ������ (���)
		   glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  // y�� ����
		   glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), // z�� ������ (�Ķ�)
		   glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),  // z�� ����
	};
	glGenVertexArrays(1, &axesVAO);
	glGenBuffers(1, &axesVBO);

	glBindVertexArray(axesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);

	// ��ġ �Ӽ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ���� �Ӽ�
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Game::cameraSet()
{
	glm::mat4 cameraMatrix(1.0f);
	glm::mat4 cameraTranslate(1.f);
	glm::mat4 cameraRotate(1.f);


	view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
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

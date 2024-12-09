#include "game.h"

Game game;

GLvoid Game::drawScene()
{
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(game.shaderProgramID);

	game.cameraSet();
	game.projectionSet();

	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 10.0f); //Á÷±³ ÅõÇü
	//glBindVertexArray(axesVAO);

	glm::mat4 axesTransform = glm::mat4(1.0f);
	

	glUniformMatrix4fv(game.transformLoc, 1, GL_FALSE, glm::value_ptr(axesTransform));
	glDrawArrays(GL_LINES, 0, 6);

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

	//w, a, s, dë¡ ë¹íê¸° ì¡°ì
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

	float fixedDeltaTime = 1.0f / FPS; // 60FPS ±âÁØ
	game.Update(fixedDeltaTime);
	//ë§ì°ì¤ë¥¼ ëë¥´ê³  ìì§ì´ë©´ í´ë¹ ë°©í¥ì¼ë¡ ê¸°ì²´ íì 
	//ë¹íê¸°ë ìëì¼ë¡ -z ë°©í¥ì¼ë¡ ì´ë
	//ìë ¥ë°ì ì¡°ìì´ ìì¼ë©´ í´ë¹ ë°©í¥ì¼ë¡ x, y ì´ë
	//ë¹íê¸° ì´ëì ë§ì¶°ì ì¹´ë©ë¼ ìì¹, ë³´ë ë°©í¥ ì´ë
	if (game.holdmouse)
	{
		game.player.Tilt(game.mouseX - game.prevmouseX, game.mouseY - game.prevmouseY);
	}
	game.player.Move_by_Time();
	cameraPos = game.player.translation + glm::vec3(5.0f * glm::sin(game.player.angle.y), 3.0f, 5.0f * glm::cos(game.player.angle.y));
	cameraDirection = game.player.translation;

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
	glutKeyboardFunc(Keyboard);			// í¤ë³´ë ìë ¥
	glutKeyboardUpFunc(KeyboardUp);			// í¤ë³´ë ìë ¥
	//glutSpecialFunc(SpecialKeyboard);	// í¤ë³´ë í¹ì í¤ ìë ¥
	glutMouseFunc(Mouse);				// ë§ì°ì¤ ìë ¥
	glutMotionFunc(Motion);				// ë§ì°ì¤ ìì§ì
	glutTimerFunc(16, timerFunction, 0);	// íì´ë¨¸
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
}

void Game::InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO ë¥¼ ì§ì íê³  í ë¹íê¸°
	glBindVertexArray(vao); //--- VAOë¥¼ ë°ì¸ëíê¸°


	glGenBuffers(2, vbo); //--- 2ê°ì VBOë¥¼ ì§ì íê³  í ë¹íê¸°
	//--- 1ë²ì§¸ VBOë¥¼ íì±ííì¬ ë°ì¸ëíê³ , ë²íì¤ ìì± (ì¢íê°)ì ì ì¥
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ë³ì diamond ìì ë²íì¤ ë°ì´í° ê°ì ë²í¼ì ë³µì¬íë¤.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- ì¢íê°ì attribute ì¸ë±ì¤ 0ë²ì ëªìíë¤: ë²íì¤ ë¹ 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute ì¸ë±ì¤ 0ë²ì ì¬ì©ê°ë¥íê² í¨
	glEnableVertexAttribArray(0);

	//--- 2ë²ì§¸ VBOë¥¼ íì±í íì¬ ë°ì¸ë íê³ , ë²íì¤ ìì± (ìì)ì ì ì¥
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//--- ë³ì colorsìì ë²íì¤ ììì ë³µì¬íë¤.
	glBufferData(GL_ARRAY_BUFFER, index * MAX_POINTS * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	//--- ììê°ì attribute ì¸ë±ì¤ 1ë²ì ëªìíë¤: ë²íì¤ ë¹ 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute ì¸ë±ì¤ 1ë²ì ì¬ì© ê°ë¥íê² í¨.
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
		   glm::vec3(-1000.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // xì¶ ììì  (ë¹¨ê°ì)
		   glm::vec3(1000.0f,  0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // xì¶ ëì 
		   glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // yì¶ ììì  (ë¹ì)
		   glm::vec3(0.0f,  1000.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  // yì¶ ëì 
		   glm::vec3(0.0f, 0.0f, -1000.0f), glm::vec3(0.0f, 0.0f, 1.0f), // zì¶ ììì  (íë)
		   glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 1.0f),  // zì¶ ëì 
	};
	glGenVertexArrays(1, &axesVAO);
	glGenBuffers(1, &axesVBO);

	glBindVertexArray(axesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);

	// ìì¹ ìì±
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ìì ìì±
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

void Game::light()
{
	glUniform3fv(glGetUniformLocation(shaderProgramID, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(shaderProgramID, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(shaderProgramID, "viewPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 5.0f)));
}


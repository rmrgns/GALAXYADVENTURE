#include "game.h"
#include "texture.h"

Game game;

GLvoid Game::drawScene()
{
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(game.shaderProgramID);

	game.cameraSet(game.shaderProgramID);
	game.projectionSet(game.shaderProgramID);
	game.light(game.shaderProgramID);
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	game.player.DrawPlayer(game.getShaderProgramID(), game.transformLoc);
	game.light(game.shaderProgramID);

	glBindVertexArray(0);

	

	// Star, Meteor Rendering
	glUseProgram(game.getShaderProgramStar());
	game.cameraSet(game.shaderProgramStar);
	game.projectionSet(game.shaderProgramStar);
	game.light(game.shaderProgramStar);

	for (auto& s : game.star)
	{
		s.Draw(game.transformStarLoc);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, game.meteortexture);
	glUniform1i(glGetUniformLocation(game.shaderProgramStar, "texture1"), 0);
	for (const auto& m : game.meteor)
	{	
		m.Draw(game.shaderProgramStar, game.transformStarLoc);
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
	transformStarLoc = glGetUniformLocation(shaderProgramStar, "modelTransform");
	player = Player();

	startexture[0] = loadTexture("OBJ/sun.jpg");
	startexture[1] = loadTexture("OBJ/Neptune.png");
	startexture[2] = loadTexture("OBJ/Mars.png");
	startexture[3] = loadTexture("OBJ/Uranus.png");
	meteortexture = loadTexture("OBJ/Rock6.jpg");

	for (int i{}; i < STAR_COUNT; i++)
	{
		int num = RandomTexture();
		star.emplace_back(Star(shaderProgramStar, startexture[num]));
	}

	for (int i{}; i < METEOR_COUNT; i++)
	{
		meteor.emplace_back(Meteor());
	}

	glutTimerFunc(1000 / FPS, timerFunction, 1);
}

void Game::Update(float time)
{

	for (auto& s : star)
	{
		if (!game.player.crash)
		{
			if (CheckColide_BS(game.player.translation, s.GetPos(), game.player.radius, s.radius))
				game.player.Explosion();
		}

		s.Update(time);
	}

	for (auto& m : meteor)
	{
		if (!game.player.crash)
		{
			if (CheckColide_BS(game.player.translation, m.GetPos(), game.player.radius, m.radius))
				game.player.Explosion();
		}

		m.Update(time);
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
	cameraPos = game.player.translation + glm::vec3(5.0f * glm::sin(game.player.angle.y), 
		3.0f, 5.0f * glm::cos(game.player.angle.y));
	cameraDirection = game.player.translation;
}

void Game::cameraSet(GLuint ID)
{
	glm::mat4 cameraMatrix(1.0f);
	glm::mat4 cameraTranslate(1.f);
	glm::mat4 cameraRotate(1.f);


	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	cameraTranslate = glm::translate(cameraTranslate, camTranslate);
	cameraRotate = glm::rotate(cameraRotate, glm::radians(camRotate.y), glm::vec3(0.f, 1.f, 0.f));
	unsigned int viewLocation = glGetUniformLocation(ID, "viewTransform");

	cameraMatrix = view * cameraRotate;

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Game::projectionSet(GLuint ID)
{
	unsigned int projectionLocation = glGetUniformLocation(ID, "projectionTransform");
	//glm::mat4 projection2 = glm::mat4(1.0f);
	if (projType == 1)
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	else
	{
		projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));
	}


	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void Game::light(GLuint ID)
{
	glUniform3fv(glGetUniformLocation(ID, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(ID, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(ID, "viewPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 5.0f)));
}
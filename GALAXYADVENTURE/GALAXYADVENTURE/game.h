#pragma once
#include "common.h"
#include "player.h"
#include "star.h"
#include "meteor.h"

#define STAR_COUNT 100
#define METEOR_COUNT 300

static const int index = 1;

static float bGCr = 0.0, bGCg = 0.0, bGCb = 0.0; // 배경색
static GLuint vao, vbo[2], ebo; // 일반 vao
static GLuint vaoStar, vboStar[2], eboStar; // Star vao
static GLuint vaoMeteor, vboMeteor[2], eboMeteor; // Meteor vao

static glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 5.0f); //카메라 위치
static glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f); //카메라 바라보는 방향
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //카메라 위쪽 방향

static glm::vec3 lightPos(1.5f, 1.5f, 0.0f);	// 메인 조명 위치
static glm::vec3 lightColor(1.0f, 1.0f, 1.0f);	// 메인 조명 색

class Game
{
private:
	GLuint shaderProgramID = 0; //--- 세이더 프로그램 이름
	GLuint shaderProgramStar = 0; //--- Star 세이더 프로그램 이름
	GLuint transformLoc = 0;
	GLuint transformStarLoc = 0;
	Player player; // player객체
	vector<Star> star;	// star object
	GLuint startexture[TEX_COUNT];	// star texture
	vector<Meteor> meteor;	// meteor object
	GLuint meteortexture;	// meteor texture

private:
	// camera
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 camTranslate = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 camRotate = glm::vec3(0.f, 0.f, 0.f);

private:
	// projection
	int projType = 0;
	glm::mat4 projection = glm::mat4(1.0f);

private:
	// mouse control
	int prevmouseX, prevmouseY;
	int mouseX, mouseY;
	bool holdmouse;

public:
	static GLvoid drawScene();
	static GLvoid Keyboard(unsigned char key, int x, int y);
	static GLvoid KeyboardUp(unsigned char key, int x, int y);
	//static GLvoid SpecialKeyboard(int key, int x, int y);
	static GLvoid Mouse(int button, int state, int x, int y);
	static GLvoid Motion(int x, int y);
	static GLvoid timerFunction(int n);
	void convertXY(int x, int y, float& fx, float& fy);
	void utilityFunctions();
public:
	Game()
	{
		prevmouseX = 0;
		prevmouseY = 0;
		mouseX = 0;
		mouseY = 0;
		holdmouse = false;
	}
	~Game()
	{

	}
	void Init();
	void Update(float time);

	void cameraSet(GLuint ID);
	void projectionSet(GLuint ID);

	void light(GLuint ID);

	// method
	//Player getPlayer() { return player; }
	void setShaderProgramID(GLuint ID) { shaderProgramID = ID; };
	GLuint getShaderProgramID() const { return shaderProgramID; };

	void setShaderProgramStar(GLuint ID) { shaderProgramStar = ID; };
	GLuint getShaderProgramStar() const { return shaderProgramStar; };
};
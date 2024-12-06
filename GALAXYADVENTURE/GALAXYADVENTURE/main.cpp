#include "game.h"

GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

GLvoid Reshape(int w, int h);

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

	glEnable(GL_DEPTH_TEST);

	extern Game game;

	//--- 세이더 읽어와서 세이더 프로그램 만들기
	string vertexshadername = "Shader/vertex.glsl";
	string fragmentshadername = "Shader/fragment.glsl";
	make_vertexShaders(vertexShader, vertexshadername);
	make_fragmentShaders(fragmentShader, fragmentshadername);
	make_shaderProgram(shaderProgramID, vertexShader, fragmentShader);
	game.setShaderProgramID(shaderProgramID);
	game.Init();

	glutDisplayFunc(game.drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	game.utilityFunctions();
	glutMainLoop();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

/*
=========동작이 의도한대로 되지 않을때 확인할 것=========

1. 버퍼 크기를 할당했는가
2. 버퍼와 인덱스의 간격을 적정하게 주었는가
3. 접근하려는 버퍼의 인덱스가 적절한가

=========자주 실수하는 부분이니까 반드시 확인=========
*/

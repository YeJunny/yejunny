#include "GameManager.h"

using namespace std;

void InputKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void InputMouse(int button, int state, int x, int y);
void ErrorCallBack(int errorCode, const char* errorDescription);

GameManager* gameManager;

int main(int argc, char* argv[])
{
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const char WIN_NAME[] = "Sokoban!";

	gameManager = new GameManager(WIDTH, HEIGHT, WIN_NAME);

	gameManager->SetErrorCallBackGLFW(ErrorCallBack);

	gameManager->Init(InputKeyCallback);

	while (gameManager->IsQuit())
	{
		gameManager->Update();
		gameManager->Render();
	}

	delete gameManager;
	return 0;
}

void InputKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void InputMouse(int button, int state, int x, int y)
{
	
}

void ErrorCallBack(int errorCode, const char* errorDescription)
{
	fputs(errorDescription, stderr);
}
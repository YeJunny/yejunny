#pragma once

#include <iostream>
#include <fstream>
#include "glew-2.1.0/include/GL/glew.h"
#include "glfw-3.3.2.bin.WIN64/include/GLFW/glfw3.h"
#include "FreeImage/Dist/x64/FreeImage.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "Map.h"
#include "Player.h"

class GLManager
{
public:
	enum EPos
	{
		BackgroundOnly,
		Tile
	};

	enum ETexture
	{
		Player,
		Box,
		Background,
		Target,
		Wall,
		Title,
		Select,
		Load,
		Menu,
		Clear,
	};

public:
	GLManager(const GLManager& manager) = delete;
	GLManager(int width, int height, const char winName[]);
	
	~GLManager();

	bool GetKeyDown(int key);

	GLint CreateTexture(const char* textureFileName);
	void ChangeViewPort(int width, int height) const;
	void SetRenderFunction(void(__cdecl* Func)()) const;
	void SetKeyboardFunction() const;
	void SetMouseFunction() const;
	void SetErrorCallBackGLFW(GLFWerrorfun func);

public:
	void Init(GLFWkeyfun keyFun);
	void Input();
	void Update();
	bool IsQuit() const;
	void RenderOnlyBackground(ETexture texture);
	void RenderPlay(Map::EObject* mapData, int width, int height, Player::EState playerState);

public:
	bool GetIsRenderMovingPlayer() const { return mbIsRenderMovingPlayer; }

	GLFWwindow* GetWindow() const { return mWindow; }
	double GetDeltaTime() const { return mDeltaTime; }

private:
	void ErrorHandling(const char message[]);
	void CalculateTime();
	void CompileShader(const char vertexShaderFileName[], const char fragmentShaderFileName[]);
	

private:
	GLFWwindow* mWindow;

	GLuint* mPosBufferObject;
	GLuint mVertexBufferObject;
	GLuint mTextureBufObjID;
	GLuint mShaderID;
	GLuint* mTextureIDs;

	glm::mat4 mModelMat;

	float* mTileVertexPos;
	float* mBackgroundVertexPos;

	double mDeltaTime;
	bool mbIsRenderMovingPlayer;

	const int SHADER_BUF = 1024 * 10;
};
#include "GLManager.h"
#include "glm-0.9.9.8/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"
#include <cstddef>
#include <assert.h>

using namespace std;

GLManager::GLManager(int width, int height, const char winName[])
	: mbIsRenderMovingPlayer(false)
{
	// GLFW

	if (!glfwInit())
	{
		ErrorHandling("glfwInit() error");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	mWindow = glfwCreateWindow(
		width,
		height,
		winName,
		NULL, NULL
	);
	if (mWindow == NULL)
	{
		ErrorHandling("glfwCreateWindow() error");
	}

	glfwMakeContextCurrent(mWindow);
	glClear(GL_COLOR_BUFFER_BIT);

	glewExperimental = GL_TRUE;

	GLenum errorCode = glewInit();
	if (errorCode != GLEW_OK)
	{
		ErrorHandling("glewInit() error!");
	}

	if (!GLEW_VERSION_3_3)
	{
		ErrorHandling("OpenGL 3.3 API is not Supported!");
	}

	fputs("OpenGL Version : ", stdout);
	cout << glGetString(GL_VERSION) << '\n';
	fputs("GLSL Version : ", stdout);
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	fputs("Vendor : ", stdout);
	cout << glGetString(GL_VENDOR) << '\n';
	fputs("Renderer : ", stdout);
	cout << glGetString(GL_RENDERER) << '\n';

	glfwSwapInterval(1);
}

GLManager::~GLManager()
{
	glUseProgram(0);
	glBindVertexArray(0);

	delete[] mTileVertexPos;
	delete[] mBackgroundVertexPos;
	delete[] mTextureIDs;

	glDeleteProgram(mShaderID);
	glDeleteBuffers(2, mPosBufferObject);
	delete[] mPosBufferObject;
	glDeleteVertexArrays(1, &mVertexBufferObject);

	glfwTerminate();
}

void GLManager::Input()
{
}

void GLManager::Update()
{
	CalculateTime();
}

void GLManager::RenderOnlyBackground(ETexture texture)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	GLint posAttribute = glGetAttribLocation(mShaderID, "posAttribute");
	if (posAttribute == -1)
	{
		_ASSERT(false);
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, mPosBufferObject[EPos::BackgroundOnly]);
	glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttribute);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureIDs[texture]);

	glUseProgram(mShaderID);
	glBindVertexArray(mVertexBufferObject);
	glm::mat4 indentityMat = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(indentityMat));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void GLManager::RenderPlay(Map::EObject* mapData, int width, int height, Player::EState playerState)
{
	enum EDirection
	{
		Left,
		Right,
		Up,
		Down
	};
	enum ECOORD
	{
		X,
		Y
	};

	const float DELAY_TIME = 50.0f;
	const float DIFF_TILE = 0.14f;
	const float TILE_SPEED = 0.02f;

	static float dir[2] = { 0, 0 };
	static float acc[2] = { 0, 0 };
	static int playerIndexTo[2] = { 0, 0 };
	static int playerIndexFrom[2] = { 0, 0 };
	static glm::mat4 transPlayerMat = glm::mat4(1.0f);


	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	GLint posAttribute = glGetAttribLocation(mShaderID, "posAttribute");
	if (posAttribute == -1)
	{
		_ASSERT(false);
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, mPosBufferObject[EPos::Tile]);
	glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttribute);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(mShaderID);
	glBindVertexArray(mVertexBufferObject);


	bool bIsInitMovingPlayer = false;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int index = y * width + x;
			Map::EObject object = mapData[index];

			glm::mat4 transMat = glm::mat4(1.0f);
			transMat = glm::translate(transMat, glm::vec3(x * DIFF_TILE - 0.5f, -y * DIFF_TILE + 0.5f, 0.0f));
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Background]);
			glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(transMat));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			switch (object)
			{
			case Map::EObject::Background:
			case Map::EObject::Other:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Background]);

				break;
			}
			case Map::EObject::Wall:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Wall]);

				break;
			}
			case Map::EObject::Box:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Box]);

				break;
			}
			case Map::EObject::Filled:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Target]);
				glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(transMat));
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Box]);

				break;
			}
			case Map::EObject::Target:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Target]);

				break;
			}
			case Map::EObject::PlayerInPlace:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Player]);

				break;
			}
			case Map::EObject::PlayerFrom:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Background]);

				if (mbIsRenderMovingPlayer)
				{
					break;
				}

				playerIndexFrom[ECOORD::X] = x;
				playerIndexFrom[ECOORD::Y] = y;

				break;
			}
			case Map::EObject::PlayerTo:
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Background]);

				if (mbIsRenderMovingPlayer)
				{
					break;
				}

				playerIndexTo[ECOORD::X] = x;
				playerIndexTo[ECOORD::Y] = y;

				bIsInitMovingPlayer = true;

				break;
			}
			}

			glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(transMat));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}


	if (bIsInitMovingPlayer)
	{
		if (playerIndexTo[ECOORD::X] > playerIndexFrom[ECOORD::X])
		{
			dir[ECOORD::X] = 1.0f;
		}
		else if (playerIndexTo[ECOORD::X] < playerIndexFrom[ECOORD::X])
		{
			dir[ECOORD::X] = -1.0f;
		}
		else if (playerIndexTo[ECOORD::Y] > playerIndexFrom[ECOORD::Y])
		{
			dir[ECOORD::Y] = -1.0f;
		}
		else if (playerIndexTo[ECOORD::Y] < playerIndexFrom[ECOORD::Y])
		{
			dir[ECOORD::Y] = 1.0f;
		}

		transPlayerMat = glm::mat4(1.0f);
		transPlayerMat = glm::translate(transPlayerMat, glm::vec3(playerIndexFrom[ECOORD::X] * DIFF_TILE - 0.5f, -playerIndexFrom[ECOORD::Y] * DIFF_TILE + 0.5f, 0.0f));

		bIsInitMovingPlayer = false;
		mbIsRenderMovingPlayer = true;
	}


	if (mbIsRenderMovingPlayer)
	{
		float movingSpeed = TILE_SPEED;// *mDeltaTime;

		float diffX = dir[ECOORD::X] * movingSpeed;
		float diffY = dir[ECOORD::Y] * movingSpeed;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Player]);
		transPlayerMat = glm::translate(transPlayerMat, glm::vec3(diffX, diffY, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(transPlayerMat));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		acc[ECOORD::X] += diffX;
		acc[ECOORD::Y] += diffY;

		if (abs(acc[ECOORD::X]) >= DIFF_TILE || abs(acc[ECOORD::Y]) >= DIFF_TILE)
		{
			dir[ECOORD::X] = 0;
			dir[ECOORD::Y] = 0;
			acc[ECOORD::X] = 0;
			acc[ECOORD::Y] = 0;
			transPlayerMat = glm::mat4(1.0f);
			mbIsRenderMovingPlayer = false;
		}
	}


	// Render Menu Or Clear
	if (playerState == Player::EState::Menu)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mPosBufferObject[EPos::BackgroundOnly]);
		glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttribute);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Menu]);

		glm::mat4 indentityMat = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(indentityMat));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else if (playerState == Player::EState::Clear)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mPosBufferObject[EPos::Tile]);
		glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttribute);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureIDs[ETexture::Clear]);
		
		glm::mat4 scaleMat = glm::mat4(1.0f);
		scaleMat = glm::scale(scaleMat, glm::vec3(14.0f, 7.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(scaleMat));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

bool GLManager::IsQuit() const
{
	return !glfwWindowShouldClose(mWindow);
}

void GLManager::ChangeViewPort(int width, int height) const
{
}

void GLManager::SetRenderFunction(void(__cdecl* Func)()) const
{
}

void GLManager::SetErrorCallBackGLFW(GLFWerrorfun func)
{
	glfwSetErrorCallback(func);
}

void GLManager::Init(GLFWkeyfun keyFun)
{
	glfwSetKeyCallback(mWindow, keyFun);
	//glEnable(GL_DEPTH_TEST);

	CompileShader("Shader\\VertexShader.glsl", "Shader\\FragmentShader.glsl");

	float backgroundVertexPos[] =
	{
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
	};

	float tileVertexPos[] =
	{
		-0.07f, 0.07f, 0.0f,
		0.07f, 0.07f, 0.0f,
		0.07f, -0.07f, 0.0f,
		-0.07f, -0.07f, 0.0f,
	};

	float texture[] =
	{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
	};

	GLuint elements[] =
	{
		0, 1, 2,
		2, 3, 0,
	};

	// Pos Buffer
	mPosBufferObject = new GLuint[2];
	glGenBuffers(1, &mPosBufferObject[EPos::BackgroundOnly]);
	glBindBuffer(GL_ARRAY_BUFFER, mPosBufferObject[EPos::BackgroundOnly]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertexPos), backgroundVertexPos, GL_STATIC_DRAW);
	glGenBuffers(1, &mPosBufferObject[EPos::Tile]);
	glBindBuffer(GL_ARRAY_BUFFER, mPosBufferObject[EPos::Tile]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tileVertexPos), tileVertexPos, GL_STATIC_DRAW);

	// Texture Buffer
	glGenBuffers(1, &mTextureBufObjID);
	glBindBuffer(GL_ARRAY_BUFFER, mTextureBufObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture), texture, GL_STATIC_DRAW);

	// Vertex Array
	glGenVertexArrays(1, &mVertexBufferObject);
	glBindVertexArray(mVertexBufferObject);

	// Element Buffer
	GLuint elementBufferObject;
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLint posAttribute = glGetAttribLocation(mShaderID, "posAttribute");
	if (posAttribute == -1)
	{
		_ASSERT(false);
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, mPosBufferObject[EPos::BackgroundOnly]);
	glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttribute);

	GLint textureAttribute = glGetAttribLocation(mShaderID, "textureAttribute");
	if (textureAttribute == -1)
	{
		_ASSERT(false);
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, mTextureBufObjID);
	glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureAttribute);

	// Matrix
	//mModelMat = glm::mat4(1.0f);
	//mModelMat = glm::translate(mModelMat, glm::vec3(0.1f, 0.1f, 0.0f));
	//glUniformMatrix4fv(glGetUniformLocation(mShaderID, "model"), 1, GL_FALSE, glm::value_ptr(mModelMat));


	glBindVertexArray(0);

	glBindVertexArray(mVertexBufferObject);

	// Texutre
	mTextureIDs = new GLuint[10];
	mTextureIDs[ETexture::Wall] = CreateTexture("GameData\\Image\\Wall.png");
	mTextureIDs[ETexture::Background] = CreateTexture("GameData\\Image\\Background.png");
	mTextureIDs[ETexture::Load] = CreateTexture("GameData\\Image\\Load.png");
	mTextureIDs[ETexture::Title] = CreateTexture("GameData\\Image\\Title.png");
	mTextureIDs[ETexture::Box] = CreateTexture("GameData\\Image\\Box.png");
	mTextureIDs[ETexture::Clear] = CreateTexture("GameData\\Image\\Clear.png");
	mTextureIDs[ETexture::Menu] = CreateTexture("GameData\\Image\\Menu.png");
	mTextureIDs[ETexture::Player] = CreateTexture("GameData\\Image\\Player.png");
	mTextureIDs[ETexture::Select] = CreateTexture("GameData\\Image\\Select.png");
	mTextureIDs[ETexture::Target] = CreateTexture("GameData\\Image\\Target.png");

	glUniform1i(glGetUniformLocation(mShaderID, "tex"), 0);
}

bool GLManager::GetKeyDown(int key)
{
	const float DELAY_TIME = 50.0f;
	static int keyStateMemory = GLFW_RELEASE;
	static float accTime = DELAY_TIME;

	bool bIsKeyDown;

	int state = glfwGetKey(mWindow, key);
	if (accTime >= DELAY_TIME && state == GLFW_PRESS && keyStateMemory == GLFW_RELEASE)
	{
		accTime = 0;
		bIsKeyDown = true;
	}
	else
	{
		accTime += mDeltaTime;
		bIsKeyDown = false;
	}

	keyStateMemory = state;

	return bIsKeyDown;
}

void GLManager::ErrorHandling(const char message[])
{
	fputs(message, stderr);
	glfwTerminate();
	exit(EXIT_FAILURE);
}

void GLManager::CalculateTime()
{
	static double lastTime = glfwGetTime();
	static int frameCount = 0;

	double currentTime = glfwGetTime();
	++frameCount;

	mDeltaTime = currentTime - lastTime;
	if (mDeltaTime >= 1.0)
	{
		printf("%f ms / frame ... %d fps \n",
			1000.0 / static_cast<double>(frameCount),
			frameCount);
		frameCount = 0;
		lastTime = currentTime;
	}
}

void GLManager::CompileShader(const char vertexShaderFileName[], const char fragmentShaderFileName[])
{
	GLchar* vertexBuffer = new GLchar[SHADER_BUF];
	GLchar* fragmentBuffer = new GLchar[SHADER_BUF];
	
	ifstream fin;

	fin.open(vertexShaderFileName, ios::binary);
	fin.seekg(0, ifstream::end);
	int fileSize = fin.tellg();
	fin.seekg(0, ifstream::beg);
	fin.read(vertexBuffer, fileSize);
	vertexBuffer[fileSize] = 0;
	fin.close();

	fin.open(fragmentShaderFileName, ios::binary);
	fin.seekg(0, ifstream::end);
	fileSize = fin.tellg();
	fin.seekg(0, ifstream::beg);
	fin.read(fragmentBuffer, fileSize);
	fragmentBuffer[fileSize] = 0;
	fin.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexBuffer, NULL);
	glCompileShader(vertexShader);

	GLint result;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar errorLog[512];
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), NULL, errorLog);
		fputs("Error : Fail to Compile Vertex Shader\n", stderr);
		glDeleteShader(vertexShader);
		_ASSERT(false);
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentBuffer, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar errorLog[512];
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), NULL, errorLog);
		fputs("Error : Fail to Compile Vertex Shader\n", stderr);
		glDeleteShader(vertexShader);
		_ASSERT(false);
		return;
	}

	mShaderID = glCreateProgram();
	glAttachShader(mShaderID, vertexShader);
	glAttachShader(mShaderID, fragmentShader);
	glLinkProgram(mShaderID);

	glGetProgramiv(mShaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		GLchar errorLog[512];
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), NULL, errorLog);
		fputs("Error : Fail to Compile Vertex Shader\n", stderr);
		glDeleteShader(vertexShader);
		_ASSERT(false);
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	delete[] vertexBuffer;
	delete[] fragmentBuffer;
}

GLint GLManager::CreateTexture(const char* textureFileName)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(textureFileName, 0);
	if (format == -1)
	{
		ErrorHandling("Could not find image - Aborting.");
	}
	else if (format == FIF_UNKNOWN)
	{
		fputs("Couldn't determine file format - attempting to get from file extension...", stderr);
		format = FreeImage_GetFIFFromFilename(textureFileName);
		if (!FreeImage_FIFSupportsReading(format))
		{
			ErrorHandling("Detected image format cannot be read!");
		}
	}

	FIBITMAP* bitMap = FreeImage_Load(format, textureFileName);

	int bitsPerPixel = FreeImage_GetBPP(bitMap);

	FIBITMAP* bitMap32;
	if (bitsPerPixel == 32)
	{
		fprintf(stdout, "Source Image has %d bits per Pixel. Skip Conversion.\n", bitsPerPixel);
		bitMap32 = bitMap;
	}
	else
	{
		fprintf(stdout, "Source Image has %d bits per Pixel. Convert to 32 bit color.\n", bitsPerPixel);
		bitMap32 = FreeImage_ConvertTo32Bits(bitMap);
	}

	int imageWidth = FreeImage_GetWidth(bitMap32);
	int imageHeight = FreeImage_GetHeight(bitMap32);

	fprintf(stdout, "Image : %s is Size : %d x %d.\n", textureFileName, imageWidth, imageHeight);

	GLubyte* textureData = FreeImage_GetBits(bitMap32);

	GLuint tempTextureID;
	glGenTextures(1, &tempTextureID);
	glBindTexture(GL_TEXTURE_2D, tempTextureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		imageWidth,
		imageHeight,
		0,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		textureData
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum glError = glGetError();
	if (glError)
	{
		puts("There was an Error Loading the Texture.");

		switch (glError)
		{
		case GL_INVALID_ENUM:
		{
			puts("Invalid enum.");
			break;
		}
		case GL_INVALID_VALUE:
		{
			puts("Invalid value.");
			break;
		}
		case GL_INVALID_OPERATION:
		{
			puts("Invalid operation.");
			break;
		}
		default:
		{
			puts("Unrecongnised GLenum.");
			break;
		}
		}

		puts("See https ://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details.");
	}

	FreeImage_Unload(bitMap32);

	if (bitsPerPixel != 32)
	{
		FreeImage_Unload(bitMap);
	}

	return tempTextureID;
}
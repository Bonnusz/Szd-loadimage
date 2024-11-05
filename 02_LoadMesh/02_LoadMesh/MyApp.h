#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// mesh
#include "ObjParser_OGL3.h"

#include <imgui/imgui.h>

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void MainWindow();
	void Window1();
	void Window2();
	void Verify(const char strIn[], SDL_Surface* imageSurface, GLuint imageTexture);
	void Verify1(const char strIn[]);
	void Verify2(const char strIn[]);
	void Verify3(const char strIn[]);

	void plotLineLow(int x0,int y0,int x1,int y1, SDL_Surface* imageSurface);
	void plotLineHigh(int x0, int y0, int x1, int y1, SDL_Surface* imageSurface);
	void plotLine(int x0, int y0, int x1, int y1, SDL_Surface* imageSurface);

	void plotLineSSIM(int x0, int y0, int x1, int y1);

	Uint32 GetColor(SDL_Surface* surface,int x,int y);

	SDL_Surface* loadImageToMemory(const char* filePath);
	GLuint textureFromSurface(SDL_Surface* surface);
	void PutPixel32_nolock(SDL_Surface* surface, int x, int y, Uint32 color);
	void PutPixel32(SDL_Surface* surface, int x, int y, Uint32 color);
	void Zoom();


	Uint8 greyscale(Uint32 pixel, SDL_PixelFormat* format);
	float SSIM(std::vector<std::vector<Uint8>> window1, std::vector<std::vector<Uint8>> window2, int size);
	SDL_Surface* SSIMSurface(SDL_Surface* img1, SDL_Surface* img2, int windowSize);


	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:


	// shaderekhez szükséges változók
	GLuint m_programID; // shaderek programja

	// transzformációs mátrixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// Uniformok helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint	m_loc_worldIT;
	GLuint	m_loc_world;
	GLuint	m_loc_texture;
	GLuint	m_loc_eye;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object erõforrás azonosító
	GLuint m_vboID; // vertex buffer object erõforrás azonosító
	GLuint m_ibID;  // index buffer object erõforrás azonosító
	GLuint m_waterTextureID; // fájlból betöltött textúra azonosítója
	GLuint m_samplerID; // sampler object azonosító

	struct Vertex
	{
		glm::vec3 p; // pozíció
		glm::vec3 c; // szín
		glm::vec2 t; // textúra koordináták
	};

	// mesh adatok
	Mesh* m_mesh;

	glm::vec3 toDesc(float fi, float theta);
	float m_fi = M_PI / 2.0;
	float m_theta = M_PI / 2.0;

	glm::vec3 m_eye = glm::vec3(0, 5, 20);
	glm::vec3 m_at = m_eye + toDesc(m_fi, m_theta);
	glm::vec3 m_up = glm::vec3(0, 1, 0);
	glm::vec3 m_forward = glm::vec3(m_at - m_eye);

	bool hide = false;

	float t = 1;

	enum Windows {
		WINDOW_MAIN,
		WINDOW1,
		WINDOW2
	};
	Windows currentWindow = WINDOW_MAIN;
	ImGuiWindowFlags window_flags;
	char str1[128];
	char str2[128];
	char str3[128];
	bool upd;
	ImVec2 u0;
	ImVec2 u1;
	int zoomW;
	int zoomH;
	float zoomTimes;
	int smallW;
	int smallH;
	int bigW;
	int bigH;
	char outstr1[128];
	char outstr2[128];
	int ssimSize;
	const float C1 = 0.5356f, C2 = 0.7105f;
	bool smallChange;

	SDL_Surface* imageSurface1;
	GLuint imageTexture1;

	SDL_Surface* imageSurface2;
	GLuint imageTexture2;
	SDL_Surface* imageSurface3;
	GLuint imageTexture3;
	SDL_Surface* ssimSurface;
	GLuint ssimTexture;
	SDL_Surface* ssimSurface2;
	GLuint ssimTexture2;
};


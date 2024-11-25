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


class Image {
public:
	Image(void);
	~Image(void);

	void plotLine(int x0, int y0, int x1, int y1);

	void PutPixel32(int x, int y, Uint32 color);

	Uint32 GetColor(int x, int y);
	void textureFromSurface();

	void Load(char* s);
	void loadImage(char* s);

	SDL_Surface* getSurface() { return surface; }
	void setSurface(SDL_Surface* surface) {this->surface = surface;}
	GLuint getTexture() { return texture; }

private:
	SDL_Surface* surface;
	GLuint texture;

	void plotLineLow(int x0, int y0, int x1, int y1);
	void plotLineHigh(int x0, int y0, int x1, int y1);

	void PutPixel32_nolock(int x, int y, Uint32 color);

};



class CMyApp
{
public:


	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();
	void CursorPos(float offset);
	void Resize(int, int);

	void Window1(/*Image im1, Image imZoom*/);
	void ZoomMethod(/*Image im1, Image imZoom*/ );

	void Window2AfterColumn();
	bool Verify(char strIn[],char strInv[],int noErr);
	void plotLineSSIM(int x, int y, float slope);
	SDL_Surface* SSIMSurface(Image img1, Image img2, int windowSize);
	Uint8 greyscale(Uint32 pixel, SDL_PixelFormat* format);
	struct colorsStruckt {Uint8 grey1, grey2, red1, red2, green1, green2, blue1, blue2, alpha1, alpha2;};
	float SSIM(std::vector<std::vector<colorsStruckt>> window, int size, int currCol);

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);

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
	float t = 1;

	enum Windows {
		WINDOW1,
		WINDOW2
	};

	Image im1;
	Image im2;
	Image imZoom;
	Image imSSIM1;
	Image imSSIM2;

	ImGuiWindowFlags window_flags;
	Windows currentWindow;
	bool currentError[5];
	char str1[128];
	char str1verified[128];
	char str2[128];
	char str2verified[128];
	char outstr1[128];
	char outstr2[128];
	bool upd;
	bool updSSIM;

	int zoomW;
	int zoomH;
	float zoomTimes;
	int smallW;
	int smallH;
	bool smallChange;
	int bigW;
	int bigH;

	int ssimColor;
	float slope;
	int ssimSize;
	const float C1 = 0.5356f, C2 = 0.7105f;
	float ssimOsszeg;

	

};



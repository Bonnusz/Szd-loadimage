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
	/*Image(void);
	~Image(void);*/

	SDL_Surface* getSurface() { return surface; }
	void setSurface(SDL_Surface* surface) { this->surface = surface; }
	//void blintSurface(SDL_Surface* copy) { SDL_BlitSurface(copy, NULL, surface, NULL); }
	GLuint getTexture() { return texture; }
	void textureFromSurface();

	void drawImage();
	void editableDrawImage();

	//void Load(char* s);
	//void loadImage(char* s);

protected:
	SDL_Surface* surface;
	GLuint texture;

};

class Image0 : public Image {

};

class Image0FromFile : public Image0 {
public:
	void Load(char* s);
};

class Image1 : public Image {
public:
	Image1(void);
	Image1(Image im);

protected:
	Image imIn;
};

class Image1Magnify : public Image1 {
public:
	Image1Magnify(void);
	Image1Magnify(Image im);

	void editableDrawImage();

protected:
	void MagnifyMethod();

	int zoomW;
	int zoomH;
	float zoomTimes;
	int smallW;
	int smallH;
	bool smallChange;
	int bigW;
	int bigH;
	bool upd;
};

class Image2 : public Image {
public:
	Image2(void);
	Image2(Image im1,Image im2);

protected:
	Image imIn1, imIn2;
};

class Image2SSIM : public Image2 {
public:
	Image2SSIM(void);
	Image2SSIM(Image im1, Image im2);

	void SSIMSurface();
	void editableDrawImage();

protected:
	struct colorsStruckt { Uint8 grey1, grey2, red1, red2, green1, green2, blue1, blue2, alpha1, alpha2; };
	float SSIMmethod(std::vector<std::vector<colorsStruckt>> window, int currCol);

	int ssimColor;
	int ssimSize;
	float C1 = 0.01f, C2 = 0.03f; //const?
	float ssimOsszeg;

};

class Image2Merge : public Image2 {
public:
	Image2Merge(void);
	Image2Merge(Image im1, Image im2);

	void plotLineMerge(int x, int y);
	void editableDrawImage();

protected:
	float slope;
	bool upd;
};

class ImageModify {
public:
	static void plotLine(int x0, int y0, int x1, int y1, SDL_Surface* sur);
	static void PutPixel32(int x, int y, Uint32 color, SDL_Surface* sur);
	static Uint32 GetColor(int x, int y, SDL_Surface* sur);

private:
	static void plotLineLow(int x0, int y0, int x1, int y1, SDL_Surface* sur);
	static void plotLineHigh(int x0, int y0, int x1, int y1, SDL_Surface* sur);
	static void PutPixel32_nolock(int x, int y, Uint32 color, SDL_Surface* sur);
};

class RegularModify {
public:
	static void CursorPos(float offset);
	static Uint8 greyscale(Uint32 pixel, SDL_PixelFormat* format);

	//add the save here

	//static void Resize(int, int);
};


class CMyApp
{
public:

	CMyApp(void);
	~CMyApp(void);

	//basic
	bool Init();
	void Clean();
	void Update();
	void Render();

	void Resize(int, int);

	//static??
	bool Verify(char* filePath, char* filePathv, int noErr);

	//potencialremove
	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	/*
	*/

	std::vector<Image> imageVec;
	std::vector<bool> boolVec; //not needed
	std::vector<int> selectedImageVec;

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

	enum ImageEnum {
		SEMMIENUM,

		LOADENUM,
		
		MAGNIFYENUM,
		SAVEENUM,

		SSIMENUM,
		MERGEENUM
	};
	ImageEnum currentImageEnum;

	ImGuiWindowFlags window_flags;

	bool currentError[5];

	char stradd[128];
	char straddverified[128];
	char outstr[128];
	
	Image1Magnify im1mag;
	Image2SSIM im2ssim;
	Image2Merge im2merge;
};



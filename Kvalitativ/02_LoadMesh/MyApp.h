#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui/imgui.h>


class Image {
public:
	/*~Image(void);*/

	SDL_Surface* getSurface() { return surface; }
	void setSurface(SDL_Surface* surface) { this->surface = surface; }
	GLuint getTexture() { return texture; }
	void textureFromSurface();

	void drawImage();
	void drawImage(int size, bool selected);
	virtual void editableDrawImage();

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

	void editableDrawImage() override;

protected:
	void MagnifyMethod();

	int zoomW;
	int zoomH;
	float zoomTimes;
	int smallX; 
	int smallY;
	int bigX;
	int bigY;
	bool smallChange;
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
	void editableDrawImage() override;

protected:
	struct colorsStruckt { Uint8 grey1, grey2, red1, red2, green1, green2, blue1, blue2, alpha1, alpha2; };
	float SSIMmethod(std::vector<std::vector<colorsStruckt>> window/*, std::vector<std::vector<colorsStruckt>> window2*/, int currCol);

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
	void editableDrawImage() override;

protected:
	float slope;
	bool upd;
};

class SurfaceModify {
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
	static void ShowHelpMarker(const char* desc);
	static Uint8 greyScale(Uint32 pixel, SDL_PixelFormat* format);
	static Uint32 heatmapColor(float value);
	static bool Verify(char* filePath, char* filePathv);

	//add the save here

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


protected:

	std::vector<Image> imageVec;
	std::vector<int> selectedImageVec;

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

	bool currentErrors[2]; 
	/*	0 - betoltes: hibas eleresi utvonal
	*	1 - mentes: hibas eleresi utvonal
	*/

	char stradd[128];
	char straddverified[128];
	char outstr[128];
	
	Image1Magnify im1mag;
	Image2SSIM im2ssim;
	Image2Merge im2merge;
};



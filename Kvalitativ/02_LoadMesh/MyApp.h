#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui/imgui.h>
#include <functional>


class Image {
public:
	Image() : surface(nullptr), texture(0) {}
	~Image(void);

	SDL_Surface* getSurface() { return surface; }
	void setSurface(SDL_Surface* surface);
	GLuint getTexture() { return texture; }
	void textureFromSurface();

	void drawImage();
	void drawImage(int size, bool selected);
	virtual void editableDrawImage();

protected:
	SDL_Surface* surface = nullptr;
	GLuint texture;

};


class Operations{};

class Image0 : public Operations{

};

class Image0FromFile : public Image0 {
public:
	Image0FromFile(void);
	static Image Load(char* s);
	enum loadTypeEnum { PICTURE, FOLDER };

	int getLoadType() { return loadType; }
	void setLoadType(int t) { loadType = t; }

protected:
	int loadType;
};

class Image1 : public Operations {
public:
	Image1(void);
	//Image1(Image im);

	void setImage(Image im); //one

	Image imOut;
protected:
};

class Image1Magnify : public Image1 {
public:
	Image1Magnify(void);
	//Image1Magnify(Image im);

	int getZoomW() { return zoomW; }
	void setZoomW(int w) { zoomW = w; }
	int getZoomH() { return zoomH; }
	void setZoomH(int h) { zoomH = h; }
	float getZoomTimes() { return zoomTimes; }
	void setZoomTimes(float t) { zoomTimes = t; }
	bool getSmallChange() { return smallChange; }
	void setSmallChange(bool b) { smallChange = b; }

	void editableDrawImage(Image im);
	void Reset();
	void MagnifyMethod(Image im);
protected:

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

class Image2 : public Operations {
public:
	Image2(void);
	//Image2(Image im1,Image im2);

	void setImage(Image im);

	Image imOut;
protected:
};

class Image2SSIM : public Image2 {
public:
	Image2SSIM(void);
	//Image2SSIM(Image im1, Image im2);

	void SSIMSurface(Image im1,Image im2);
	void editableDrawImage();
	void Reset();

	int getSsimColor() { return ssimColor; }
	void setSsimColor(int c) { ssimColor = c; }
	int getSsimSize() { return ssimSize; }
	void setSsimSize(int s) { ssimSize = s; }
	float getSsimOsszeg() { return ssimOsszeg; }

protected:
	enum colorResult { Red, Green, Blue, Alpha, Grey, NumberOfTypes};
	std::vector<float> SSIMmethod(std::vector<std::vector<Uint32>> window1, std::vector<std::vector<Uint32>> window2, Image im1, Image im2);

	int ssimColor;
	int ssimSize;
	float C1 = 0.01f, C2 = 0.03f; //const?
	float ssimOsszeg;

};

class Image2Merge : public Image2 {
public:
	Image2Merge(void);
	//Image2Merge(Image im1, Image im2);

	void plotLineMerge(int x, int y, Image im1, Image im2);
	void editableDrawImage(Image im1, Image im2);
	void Reset();

	float getSlope() { return slope; }
	void setSlope(float s) { slope = s; }
	int getUx() { return ux; }
	int getUy() { return uy; }

protected:
	float slope;
	bool upd;
	int ux, uy;
};

class Folder : public Image {
public:
	Folder(void);
	bool Load(char* s);
	void Append(Image app);
	void Append(Folder app);
	void createIconImageFromImages();

	enum operationTypeEnum { oteImage1Magnify, oteImage2SSIM };
	struct storedOperation { Image1Magnify i1m; Image2SSIM i2s; operationTypeEnum ote = oteImage1Magnify; std::vector<int> affectedElements; };
	std::vector<storedOperation> storedOperationsVector;

	std::vector<Image> images;
};

class Image1Save : public Image1 {
public:
	void SaveFolder(Folder f, char* cstr, int j);
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

	void Back();
	void SetBasicUI();
	void PushStyleColorGreenButton();

	ImFont* arial; //delete?
	//ImFont* notosans; //delete?
	enum imageOrFolder { iofImage, iofFolder };
	struct ImageFolder { Image im; Folder f; imageOrFolder iof = iofImage; };
	std::vector<ImageFolder> imfVec;
	std::vector<int> selectedImFVec;


	std::vector<Image> imageVec;
	std::vector<int> selectedImageVec;

	std::vector<Folder> folderVec;
	std::vector<int> selectedFolderVec;

	enum ColorEnum {
		TEXT_LIGHT,
		TEXT_DARK,
		TEXT_HINT,
		CHILD_BG,
		POPUP_BG,
		POPUP_BORDER,
		SCROLL_GRAB,
		SCROLL_GRAB_HOVERED,
		SCROLL_GRAB_ACTIVE,
		SCROLL_BG,
		HEADER,
		HEADER_HOVERED,
		HEADER_ACTIVE,
		INPUT_BG,
		BUTTON_BLUE,
		BUTTON_BLUE_HOVERED,
		BUTTON_BLUE_ACTIVE,
		BUTTON_GREEN,
		BUTTON_GREEN_HOVERED,
		BUTTON_GREEN_ACTIVE,
		BUTTON_RED,
		BUTTON_RED_HOVERED,
		BUTTON_RED_ACTIVE,
		BUTTON_GREY,
		BUTTON_GREY_HOVERED,
		BUTTON_GREY_ACTIVE,

		NumberOfTypes
	};

	ImVec4 Colors[ColorEnum::NumberOfTypes];

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
	
	Image0FromFile im0load;
	Image1Save im1sav;
	Image1Magnify im1mag;
	Image2SSIM im2ssim;
	Image2Merge im2merge;
	//Folder imFold;
};

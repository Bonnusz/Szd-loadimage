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

	SDL_Surface* getSurface() const { return surface; }
	void setSurface(SDL_Surface* surface);
	GLuint getTexture() const { return texture; }
	void textureFromSurface();
	void freeSurface();

	void drawImage() const;
	void drawImage(int size, bool selected) const;

protected:
	SDL_Surface* surface = nullptr;
	GLuint texture;

};

class Folder{ 
public:
	Folder(void);
	
	void Append(Image app);
	void Append(Folder app);

	void createIconImageFromImages();
	Image getIcon() const { return icon; }

	int getIconN() const { return iconN; }
	void setIconN(int i) { iconN = i; }
	std::vector<Image> getImages()  const { return images; }
	void pushImages(Image im) { images.push_back(im); }

protected:
	Image icon;
	std::vector<Image> images;
	int iconN;
};

enum ImageOrFolderEnum { iofImage, iofFolder, iofEmpty };
struct ImageFolder { Image im; Folder f; ImageOrFolderEnum iof = iofEmpty; };

class Operations{};

class Image0 : public Operations{

};

class Image0FromFile : public Image0 {
public:
	Image0FromFile(void);
	ImageFolder Load(char* s, bool popup);
	enum LoadTypeEnum { PICTURE, FOLDER };

	int getLoadType() const { return loadType; }
	void setLoadType(int t) { loadType = t; }

protected:
	int loadType;
};

class Image0StaticNoise : public Image0 {
public:
	Image0StaticNoise(void);

	void StaticMethod();
	void Reset();

	std::vector<ImVec4> color;
	std::vector<ImVec4> backup_color;
	ImVec4 saved_palette[32];

	Image getImOut() const { return imOut; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	void setWidth(int w) { width = w; }
	void setHeight(int h) { height = h; }

protected:
	Image imOut;
	int width;
	int height;

};

class Image1 : public Operations {
public:
	Image1(void);

	void setImage(Image im); 
	Image getImOut() const { return imOut; }

protected:
	Image imOut;
};

class Image1Magnify : public Image1 {
public:
	Image1Magnify(void);

	int getZoomW() const { return zoomW; }
	void setZoomW(int w) { zoomW = w; }
	int getZoomH() const { return zoomH; }
	void setZoomH(int h) { zoomH = h; }
	float getZoomTimes() const { return zoomTimes; }
	void setZoomTimes(float t) { zoomTimes = t; }
	bool getSmallChange() const { return smallChange; }
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
	bool updMove;
	bool updResize;
};

class Image1Blur : public Image1 {
public:
	Image1Blur(void);

	void Reset();
	void BlurMethod(Image im);

	enum BlurTypeEnum {blurBox,blurGauss};

	int getBlurSize() const { return blurSize; }
	void setBlurSize(int s) { blurSize = s; }
	BlurTypeEnum getBlurType() const { return blurType; }
	void setBlurType(BlurTypeEnum e) { blurType = e; }

protected:
	BlurTypeEnum blurType;
	int blurSize;
};

class Image1Color : public Image1 {
public:
	Image1Color(void);

	void Reset();
	void ColorMethod(Image im);

	enum ImageColorType {Null,GreyScale,Red,Green,Blue,Inverted};

	void setImcType(ImageColorType i) { imctype = i; }
	ImageColorType getImcType() const { return imctype; }

protected:
	ImageColorType imctype;
};

class Image1Save : public Image1 {
public:
	bool SaveFolder(Folder f, std::string path, int n);
	bool Save(Image im, char* cstr);
};

class Image2 : public Operations {
public:
	Image2(void);

	void setImage(Image im);
	Image getImOut() const { return imOut; }

protected:
	Image imOut;
};

class Image2SSIM : public Image2 {
public:
	Image2SSIM(void);

	void SSIMMethod(Image im1,Image im2);

	void Reset();

	int getSsimColor() const { return ssimColor; }
	void setSsimColor(int c) { ssimColor = c; }
	int getSsimSize() const { return ssimSize; }
	void setSsimSize(int s) { ssimSize = s; }
	float getSsimOsszeg() const { return ssimOsszeg; }

protected:
	enum colorResult { Red, Green, Blue, Alpha, Grey, NumberOfTypes};

	int ssimColor;
	int ssimSize;
	float C1 = 0.01f, C2 = 0.03f; 
	float ssimOsszeg;

};

class Image2Merge : public Image2 {
public:
	Image2Merge(void);

	void MergeMethod(Image im1, Image im2); 
	void editableDrawImage(Image im1, Image im2);
	void Reset();

	float getSlope() const { return slope; }
	void setSlope(float s) { slope = s; }
	int getUx() const { return ux; }
	int getUy() const { return uy; }


protected:
	float slope;
	bool upd;
	int ux, uy;
};

class SurfaceModify {
public:
	static void plotLine(int x0, int y0, int x1, int y1, SDL_Surface* sur);
	static void PutPixel(int x, int y, Uint32 color, SDL_Surface* sur);
	static Uint32 GetColor(int x, int y, SDL_Surface* sur);

private:
	static void plotLineMixed(int x0, int y0, int x1, int y1, SDL_Surface* sur);
	static void plotLineLow(int x0, int y0, int x1, int y1, SDL_Surface* sur);
	static void plotLineHigh(int x0, int y0, int x1, int y1, SDL_Surface* sur);
	static void PutPixel_nolock(int x, int y, Uint32 color, SDL_Surface* sur);
};

class RegularModify {
public:
	static void CursorPos(float offset);
	static void ShowHelpMarker(const char* desc);
	static Uint8 greyScale(Uint32 pixel, SDL_PixelFormat* format);
	static Uint32 heatmapColor(float value);
	static bool Verify(char* filePath);

};

class CMyApp
{
public:

	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();
	void Render();

	void Resize(int, int);

protected:
	void TestMethod();

	void Back();
	void SetBasicUI_5Col_2Var() const;
	void PushStyleColorGreenButton_3Col() const;

	ImFont* arial;
	Image arrow_left, arrow_right,arrow_left_unavailable,arrow_right_unavailable;

	std::vector<ImageFolder> imfVec;
	std::vector<int> selectedImFVec;

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
		COLORPICKER_FRAMEBG,

		NumberOfTypes
	};

	ImVec4 Colors[ColorEnum::NumberOfTypes];

	enum MenuEnum {
		OPERATIONSENUM,

		STATICNOISEENUM,
		LOADENUM,
		
		MAGNIFYENUM,
		BLURENUM,
		COLORENUM,
		SAVEENUM,

		SSIMENUM,
		MERGEENUM
	};

	MenuEnum currentMenuEnum;

	ImGuiWindowFlags window_flags;

	char stradd[128];
	char outstr[128];
	
	Image0FromFile im0load;
	Image0StaticNoise im0stat;
	Image1Magnify im1mag;
	Image1Blur im1blur;
	Image1Color im1col;
	Image1Save im1sav;
	Image2SSIM im2ssim;
	Image2Merge im2merge;

};

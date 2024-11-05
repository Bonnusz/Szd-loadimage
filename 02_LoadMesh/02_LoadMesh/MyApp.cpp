#define STB_IMAGE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#define IM_ALLOC(_SIZE)                     ImGui::MemAlloc(_SIZE)
#define IM_FREE(_PTR)                       ImGui::MemFree(_PTR)

#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <imgui/imgui.h>
#include "stb_image.h"

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	m_waterTextureID = 0;
	m_samplerID = 0;
	m_mesh = 0;

	window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar;
	u0 = ImVec2(0, 0);
	u1 = ImVec2(1, 1);
	upd = true;
	strcpy(str1, "C:/Users/User/Pictures/ac.jpg");
	strcpy(str2, "C:/Users/User/Pictures/ac.jpg");
	strcpy(str3, "C:/Users/User/Pictures/ac.jpg");

	imageSurface1 = loadImageToMemory("C:/Users/User/Pictures/ac.jpg");
	imageTexture1 = textureFromSurface(imageSurface1);

	imageSurface2 = loadImageToMemory("C:/Users/User/Pictures/ac2.jpg");
	imageTexture2 = textureFromSurface(imageSurface2);
	imageSurface3 = loadImageToMemory("C:/Users/User/Pictures/ac.jpg");
	imageTexture3 = textureFromSurface(imageSurface3);
	ssimSize = 1;

	ssimSurface = SSIMSurface(imageSurface2, imageSurface3, ssimSize);
	ssimTexture = textureFromSurface(ssimSurface);
	ssimSurface2 = loadImageToMemory("C:/Users/User/Pictures/ac.jpg");
	plotLineSSIM(100,0,100,ssimSurface2->h);
	ssimTexture2 = textureFromSurface(ssimSurface2);

	zoomW = 100;
	zoomH = 50;
	zoomTimes = 2.f;

	smallW = 0;
	smallH = 0;
	smallChange = true;
	bigW = 0;
	bigH = 0;

	strcpy(outstr1, "C:/Users/User/Pictures/save.png");
	strcpy(outstr2, "C:/Users/User/Pictures/ssim.png");
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{
		//          x,  y, z               nx,ny,nz			 s, t
		{glm::vec3(-10, 0, -10), glm::vec3(0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3(-10, 0,  10), glm::vec3(0, 1, 0), glm::vec2(0, 1)},
		{glm::vec3(10, 0, -10), glm::vec3(0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3(10, 0,  10), glm::vec3(0, 1, 0), glm::vec2(1, 1)},
	};

	// indexpuffer adatai
	GLushort indices[] =
	{
		// 1. háromszög
		0,1,2,
		// 2. háromszög
		2,1,3,
	};

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);

	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		sizeof(vert),		// ennyi bájt nagyságban
		vert,	// errõl a rendszermemóriabeli címrõl olvasva
		GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat


	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	);

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	// textúrakoordináták bekapcsolása a 2-es azonosítójú attribútom csatornán
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(glm::vec3)));

	// index puffer létrehozása
	glGenBuffers(1, &m_ibID);
	// a VAO észreveszi, hogy bind-olunk egy index puffert és feljegyzi, hogy melyik volt ez!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés elõtt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonosítója, amibõl egy változóhoz szeretnénk hozzárendelést csinálni
		0,				// a VAO-beli azonosító index
		"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation(m_programID, 1, "vs_in_norm");
	glBindAttribLocation(m_programID, 2, "vs_in_tex0");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy(aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	//
	// egyéb inicializálás
	//

	// vetítési mátrix létrehozása
	m_matProj = glm::perspective(45.0f, 640 / 480.0f, 1.0f, 1000.0f);

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");

	m_loc_world = glGetUniformLocation(m_programID, "world");
	m_loc_worldIT = glGetUniformLocation(m_programID, "WorldIT");

	m_loc_texture = glGetUniformLocation(m_programID, "texture");
	m_loc_eye = glGetUniformLocation(m_programID, "eye_Pos");

	//
	// egyéb erõforrások betöltése
	//

	// textúra betöltése
	m_waterTextureID = TextureFromFile("texture.bmp");
	//m_waterTextureID = TextureFromFile("Large_cloud_over_Mexican_land.jpg");
	m_samplerID = genSampler();

	// mesh betoltese
	m_mesh = ObjParser::parse("Suzanne.obj");
	m_mesh->initBuffers();

	return true;
}

void CMyApp::Clean()
{
	delete m_mesh;
	glDeleteTextures(1, &m_waterTextureID);

	glDeleteSamplers(1, &m_samplerID);

	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram(m_programID);
}

void CMyApp::Update()
{
	// nézeti transzformáció beállítása
	m_matView = glm::lookAt(m_eye,	// honnan nézzük a színteret
		m_at,	// a színtér melyik pontját nézzük
		m_up);	// felfelé mutató irány a világban
}

float f(float t) {
	//return (t - 1) * (t - 1);
	return t * t;
}

float df(float t) {
	return 2 * t;
}

Uint8 CMyApp::greyscale(Uint32 pixel, SDL_PixelFormat* format) {
	Uint8 r, g, b;
	SDL_GetRGB(pixel, format, &r, &g, &b);
	return 0.299f * r + 0.587f * g + 0.114f * b;
}

float CMyApp::SSIM(std::vector<std::vector<Uint8>> window1, std::vector<std::vector<Uint8>> window2, int size) {

	float mean1 = 0.f;
	float mean2 = 0.f;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			mean1 += window1[i][j];
			mean2 += window2[i][j];
		}
	}
	mean1 /= size;
	mean2 /= size;

	float var1 = 0.f;
	float var2 = 0.f;
	float covar = 0.f;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			var1 += (window1[i][j] - mean1) * (window1[i][j] - mean1);
			var2 += (window2[i][j] - mean2) * (window2[i][j] - mean2);
			covar += (window1[i][j] - mean1) * (window2[i][j] - mean2);
		}
	}
	var1 /= size;
	var2 /= size;
	covar /= size;

	return ((2 * mean1 * mean2 + C1) * (2 * covar + C2)) / ((mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2));
}

SDL_Surface* CMyApp::SSIMSurface(SDL_Surface* img1, SDL_Surface* img2, int windowSize) {

	SDL_Surface* ssimSurface = SDL_CreateRGBSurface(0, img1->w, img1->h, img1->format->BitsPerPixel, 0, 0, 0, 0);

	//SDL_LockSurface(ssimSurface);

	std::vector<std::vector<Uint8>> grey1(windowSize, std::vector<Uint8>(windowSize));
	std::vector<std::vector<Uint8>> grey2(windowSize, std::vector<Uint8>(windowSize));

	for (int x = 0; x < ssimSurface->w; x += windowSize) {
		for (int y = 0; y < ssimSurface->h; y += windowSize) {

			for (int i = 0; i < windowSize; i++) {
				for (int j = 0; j < windowSize; j++) {
					
					grey1[i][j] = greyscale(GetColor(img1,x+i,y+j),img1->format);
					grey2[i][j] = greyscale(GetColor(img2,x+i,y+j),img2->format);
				}
			}

			float ssimValue = SSIM(grey1, grey2,windowSize);
			Uint8 rgb = (Uint8)((ssimValue + 1.0) * 255 / 2);

			for (int i = 0; i < windowSize; i++) {
				for (int j = 0; j < windowSize; j++) {
					if (x + i < ssimSurface->w && y + j < ssimSurface->h) {

						PutPixel32(ssimSurface, x+i, y+j, SDL_MapRGB(ssimSurface->format, rgb, rgb, rgb));
					}
				}
			}
		}
	}

	//SDL_UnlockSurface(ssimSurface); seperable filter matlab 

	return ssimSurface;
}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3f(glGetUniformLocation(m_programID, "eye_pos"), m_eye.x, m_eye.y, m_eye.z);

	ImGuiIO io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::GetStyle().WindowRounding = 0.0f;

	switch (currentWindow) {
		case WINDOW_MAIN:
		MainWindow(); 
		break;
	case WINDOW1:
		Window1();
		break;
	case WINDOW2:
		Window2();
		break;
	}
}

void CMyApp::MainWindow() {
	ImGui::Begin("Main",0,window_flags);
	if (ImGui::Button("Window1")) {
		currentWindow = WINDOW1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Window2")) {
		currentWindow = WINDOW2;
	}
	ImGui::End();
}

Uint32 CMyApp::GetColor(SDL_Surface* surface, int x, int y) {
	return *(Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
}

void CMyApp::Window1() {
	ImGui::Begin("First",0,window_flags);
	ImGui::Text("A kep eleresi utvonala");
	ImGui::InputText(" ", str1, IM_ARRAYSIZE(str1));
	if (ImGui::Button("Verify ")) {
		//Verify(str1,imageSurface1,imageTexture1);
		Verify1(str1);
	}

	ImGuiIO& io = ImGui::GetIO();
	ImTextureID my_tex_id = (ImTextureID)(intptr_t)imageTexture1;
	
	//ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	
	ImGui::Image((void*)(intptr_t)imageTexture1, ImVec2(imageSurface1->w, imageSurface1->h));
	int a = 0;
	if (ImGui::IsItemHovered())
	{
		if (!upd) {
			Zoom();
			float focus_sz = 32.0f;
			float focus_x = io.MousePos.x - pos.x - focus_sz * 0.5f; if (focus_x < 0.0f) focus_x = 0.0f; else if (focus_x > imageSurface1->w - focus_sz) focus_x = imageSurface1->w - focus_sz;
			float focus_y = io.MousePos.y - pos.y - focus_sz * 0.5f; if (focus_y < 0.0f) focus_y = 0.0f; else if (focus_y > imageSurface1->h - focus_sz) focus_y = imageSurface1->h - focus_sz;
			ImVec2 uv0 = ImVec2((focus_x) / imageSurface1->w, (focus_y) / imageSurface1->h);
			ImVec2 uv1 = ImVec2((focus_x + focus_sz) / imageSurface1->w, (focus_y + focus_sz) / imageSurface1->h);
				u0 = uv0; //not needed
				u1 = uv1; //not needed
			if (smallChange) {
				smallW = focus_x;
				smallH = focus_y;
			}
			else {
				bigW = imageSurface1->w - focus_x;
				bigH = imageSurface1->h - focus_y;
			}
			std::cout << focus_x <<std::endl;
		}
	}
	if (ImGui::IsItemClicked()) {
		upd = !upd;
	}
	//ImGui::Image(my_tex_id, ImVec2(128, 128), u0, u1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

	if (ImGui::Button("SmallChange")) {
		smallChange = true;
	}
	if (ImGui::Button("BigChange")) {
		smallChange = false;
	}
	ImGui::InputFloat("ZoomTimes",&zoomTimes,0.1f,1.f);
	ImGui::InputInt("ZoomWidth",&zoomW);
	ImGui::InputInt("ZoomHeight", &zoomH);
	if (ImGui::Button("Zoom")) {
		Zoom();
	}

	ImGui::InputText("SavePath1", outstr1, IM_ARRAYSIZE(outstr1));
	if (ImGui::Button("Save1")) {
		IMG_SavePNG(imageSurface1, outstr1);
	}
		
	if (ImGui::Button("Back")) {
		currentWindow = WINDOW_MAIN;
	}
	ImGui::End();
}

void CMyApp::Zoom() {
	Uint32 red = (255 << 24) | (0 << 16) | (0 << 8) | 255;
	Verify1(str1);

	for (int i = zoomW * zoomTimes; i > 0; i--) {
		for (int j = zoomH * zoomTimes; j > 0; j--) {
			if (i == 1 || i == std::floor(zoomW * zoomTimes) || j == 1 || j == std::floor(zoomH * zoomTimes)) {
				PutPixel32(imageSurface1, imageSurface1->w - i - bigW - 1, imageSurface1->h - j - bigH - 1, red);
			}
			else {
				//PutPixel32(imageSurface1, imageSurface1->w-i-bigW-1 , imageSurface1->h-j-bigH-1, *(Uint32*)((Uint8*)imageSurface1->pixels + int((zoomH*zoomTimes-j)/zoomTimes + smallH) * imageSurface1->pitch + int((zoomW*zoomTimes-i)/zoomTimes + smallW) * imageSurface1->format->BytesPerPixel));
				PutPixel32(imageSurface1, imageSurface1->w - i - bigW - 1, imageSurface1->h - j - bigH - 1, GetColor(imageSurface1, int((zoomW * zoomTimes - i) / zoomTimes + smallW), int((zoomH * zoomTimes - j) / zoomTimes + smallH)));
			}
		}
	}
	for (int i = 0 + smallW; i < zoomW + smallW; i++) {
		for (int j = 0 + smallH; j < zoomH + smallH; j++) {
			if (i == 0 + smallW || j == 0 + smallH || i == zoomW + smallW - 1 || j == zoomH + smallH - 1) {
				PutPixel32(imageSurface1, i, j, red);
			}
		}
	}
	ImVec2 small1 = ImVec2(smallW, zoomH + smallH);
	ImVec2 small2 = ImVec2(zoomW + smallW, smallH);
	ImVec2 big1 = ImVec2(imageSurface1->w - bigW - zoomW * zoomTimes, imageSurface1->h - bigH);
	ImVec2 big2 = ImVec2(imageSurface1->w - bigW, imageSurface1->h - bigH - zoomH * zoomTimes);

	int tempSmall1y = small1.y, tempBig1y = big1.y;

	if (small1.x > big1.x && small1.y < big1.y || small1.x < big1.x && small1.y > big1.y) {
		small1.y = small2.y;
		big1.y = big2.y;
	}

	if (small2.x > big2.x && small2.y < big2.y || small2.x < big2.x && small2.y > big2.y) {
		small2.y = tempSmall1y;
		big2.y = tempBig1y;
	}

	int x0 = small1.x, x1 = big1.x, y0 = small1.y, y1 = big1.y;
	plotLine(x0, y0, x1, y1,imageSurface1);
	x0 = small2.x, x1 = big2.x, y0 = small2.y, y1 = big2.y;
	plotLine(x0, y0, x1, y1,imageSurface1);


	imageTexture1 = textureFromSurface(imageSurface1);
}

void CMyApp::plotLineLow(int x0, int y0, int x1, int y1,SDL_Surface * imageSurface){
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int D = (2 * dy) - dx;
	int y = y0;

	for (int x = x0; x < x1 ; x++){
		PutPixel32(imageSurface, x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255);

		if (D > 0){
			y = y + yi;
			D = D + (2 * (dy - dx));
		}
		else{
			D = D + 2 * dy;
		}
	}
}

void CMyApp::plotLineHigh(int x0,int y0,int x1,int y1,SDL_Surface* imageSurface){
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}

	int D = (2 * dx) - dy;
	int x = x0;

	for (int y=y0;y<y1;y++){
		PutPixel32(imageSurface, x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255);

		if (D > 0) {
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else {
			D = D + 2 * dx;
		}
	}
}

void CMyApp::plotLine(int x0, int y0, int x1, int y1,SDL_Surface* imageSurface) {
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			plotLineLow(x1, y1, x0, y0,imageSurface);
		}
		else {
			plotLineLow(x0, y0, x1, y1,imageSurface);
		}
	}
	else {
		if (y0 > y1) {
			plotLineHigh(x1, y1, x0, y0,imageSurface);
		}
		else {
			plotLineHigh(x0, y0, x1, y1,imageSurface);
		}
	}
}

void CMyApp::Window2() {
	ImGui::Begin("Second",0,window_flags);

	ImGui::Text("A kep eleresi utvonala");
	ImGui::InputText("  ", str2, IM_ARRAYSIZE(str2));
	if (ImGui::Button("Verify  ")) {
		//Verify(str2,imageSurface2,imageTexture2);
		Verify2(str2);
	}
	ImGui::Image((void*)(intptr_t)imageTexture2, ImVec2(imageSurface2->w, imageSurface2->h));

	ImGui::Text("A kep eleresi utvonala");
	ImGui::InputText("   ", str3, IM_ARRAYSIZE(str3));
	if (ImGui::Button("Verify   ")) {
		//Verify(str3,imageSurface3,imageTexture3);
		Verify3(str3);
	}
	ImGui::Image((void*)(intptr_t)imageTexture3, ImVec2(imageSurface3->w, imageSurface3->h));

	ImGui::InputInt("Size", &ssimSize);

	if (ImGui::Button("SSIM")) {
		ssimSurface = SSIMSurface(imageSurface2, imageSurface3, ssimSize);
		ssimTexture = textureFromSurface(ssimSurface);

		plotLineSSIM(100, 0, 120, ssimSurface2->h);
		ssimTexture2 = textureFromSurface(ssimSurface2);
	}

	ImGui::Image((void*)(intptr_t)ssimTexture2, ImVec2(ssimSurface2->w, ssimSurface2->h));


	ImGui::InputText("SavePath2", outstr2, IM_ARRAYSIZE(outstr2));
	if (ImGui::Button("Save2")) {
		IMG_SavePNG(ssimSurface2, outstr2);
	}


	if (ImGui::Button("Back")) {
		currentWindow = WINDOW_MAIN;
	}
	ImGui::End();
}

void CMyApp::plotLineSSIM(int x0, int y0, int x1, int y1) {
	
	if (x0 == x1) {
		for (int i = 0; i < ssimSurface2->w; i++) {
			for (int j = 0; j < ssimSurface2->h; j++) {
				if (i>x0) {
					PutPixel32(ssimSurface2, i, j, GetColor(ssimSurface, i, j));
				}
				else {
					PutPixel32(ssimSurface2, i, j, GetColor(imageSurface2, i, j));
				}
			}
		}
	}
	else{
		float slope = (y0 - y1) / (x0 - x1);

		for (int i = 0; i < ssimSurface2->w; i++) {
			for (int j = 0; j < ssimSurface2->h; j++) {
				if (j < slope * (i - x0) + y0) {
					PutPixel32(ssimSurface2, i, j, GetColor(ssimSurface, i, j));
				}
				else {
					PutPixel32(ssimSurface2, i, j, GetColor(imageSurface2, i, j));
				}
			}
		}
	}

	plotLine(x0, y0, x1, y1, ssimSurface2);
}

void CMyApp::PutPixel32_nolock(SDL_Surface* surface, int x, int y, Uint32 color)
{
	Uint8* pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + x * surface ->format->BytesPerPixel;
	*((Uint32*)pixel) = color;
}

void CMyApp::PutPixel32(SDL_Surface* surface, int x, int y, Uint32 color)
{
	if (SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);
	PutPixel32_nolock(surface, x, y, color);
	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
}


void CMyApp::Verify(const char strIn[], SDL_Surface* imageSurface, GLuint imageTexture) {

	imageSurface = loadImageToMemory(strIn);
	imageTexture = textureFromSurface(imageSurface);

}

void CMyApp::Verify1(const char strIn[]) {

	//SDL_FreeSurface(imageSurface1);
	imageSurface1 = loadImageToMemory(strIn);
	imageTexture1 = textureFromSurface(imageSurface1);
}

void CMyApp::Verify2(const char strIn[]) {

	imageSurface2 = loadImageToMemory(strIn);
	imageTexture2 = textureFromSurface(imageSurface2);

}

void CMyApp::Verify3(const char strIn[]) {

	imageSurface3 = loadImageToMemory(strIn);
	imageTexture3 = textureFromSurface(imageSurface3);

}


SDL_Surface* CMyApp::loadImageToMemory(const char* filePath) {
	SDL_Surface* imageSurface = IMG_Load(filePath);
	if (!imageSurface) {
		imageSurface = IMG_Load("texture.bmp");
	}
	if (!imageSurface) {
		printf("Failed to load image: %s\n", IMG_GetError());
		return nullptr;
	}

	return imageSurface;
}

GLuint CMyApp::textureFromSurface(SDL_Surface* surface) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLenum format = GL_RGBA;
	if (surface->format->BytesPerPixel == 3) {
		format = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}


glm::vec3 CMyApp::toDesc(float fi, float theta) {
	return glm::vec3(sin(fi) * cos(theta), cos(fi), -sin(fi) * sin(theta));
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(45.0f,		// 90 fokos nyilasszog
		_w / (float)_h,	// ablakmereteknek megfelelo nezeti arany
		0.01f,			// kozeli vagosik
		100.0f);		// tavoli vagosik
}
#define STB_IMAGE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

//#define IM_ALLOC(_SIZE)                     ImGui::MemAlloc(_SIZE)
//#define IM_FREE(_PTR)                       ImGui::MemFree(_PTR)

#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <imgui/imgui.h>
#include "stb_image.h"

float f(float t) {
	//return (t - 1) * (t - 1);
	return t * t;
}

float df(float t) {
	return 2 * t;
}

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	m_waterTextureID = 0;
	m_samplerID = 0;
	m_mesh = 0;

	window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar;
	upd = false;
	updSSIM = false;
	ssimColor = 0;

	strcpy(str1, "C:/Users/User/Pictures/ac2.jpg");
	strcpy(str1verified, str1);
	strcpy(str2, "C:/Users/User/Pictures/ac.jpg");
	strcpy(str2verified, str2);
	strcpy(outstr1, "C:/Users/User/Pictures/save.png");
	strcpy(outstr2, "C:/Users/User/Pictures/ssim.png");

	Verify(str1, str1verified,0);
	Verify(str2, str2verified,1);

	im1.Load(str1verified);
	im1.textureFromSurface();
	imZoom.Load(str1verified);
	imZoom.textureFromSurface();
	im2.Load(str2verified);
	im2.textureFromSurface();

	ssimSize = 1;

	imSSIM1.setSurface(SSIMSurface(im1, im2, ssimSize));
	imSSIM1.textureFromSurface();
	imSSIM2.Load(str1verified);	
	plotLineSSIM(imSSIM2.getSurface()->w / 2, imSSIM2.getSurface()->h / 2, 1.3f);
	imSSIM2.textureFromSurface();

	zoomW = 100;
	zoomH = 50;
	zoomTimes = 2.f;
	smallW = 0;
	smallH = 0;
	smallChange = true;
	bigW = 0;
	bigH = 0;
	slope = 1.3f;
	currentWindow = WINDOW1;

	for (int i = 0;i< sizeof(currentError);i++) {
		currentError[i] = false;
	}
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

Uint8 CMyApp::greyscale(Uint32 pixel, SDL_PixelFormat* format) {
	Uint8 r, g, b;
	SDL_GetRGB(pixel, format, &r, &g, &b);
	return 0.299f * r + 0.587f * g + 0.114f * b;
}

float CMyApp::SSIM(std::vector<std::vector<colorsStruckt>> window, int size, int currCol) {

	float mean1 = 0.f;
	float mean2 = 0.f;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			switch (currCol)
			{
			case 0:
				mean1 += window[i][j].grey1;
				mean2 += window[i][j].grey2;
				break;
			case 1:
				mean1 += window[i][j].red1;
				mean2 += window[i][j].red2;
				break;
			case 2:
				mean1 += window[i][j].green1;
				mean2 += window[i][j].green2;
				break;
			case 3:
				mean1 += window[i][j].blue1;
				mean2 += window[i][j].blue2;
				break;
			}


		}
	}
	mean1 /= size;
	mean2 /= size;

	float var1 = 0.f;
	float var2 = 0.f;
	float covar = 0.f;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			switch (currCol)
			{
			case 0:
				var1 += (window[i][j].grey1 - mean1) * (window[i][j].grey1 - mean1);
				var2 += (window[i][j].grey2 - mean2) * (window[i][j].grey2 - mean2);
				covar += (window[i][j].grey1 - mean1) * (window[i][j].grey2 - mean2);
				break;
			case 1:
				var1 += (window[i][j].red1 - mean1) * (window[i][j].red1 - mean1);
				var2 += (window[i][j].red2 - mean2) * (window[i][j].red2 - mean2);
				covar += (window[i][j].red1 - mean1) * (window[i][j].red2 - mean2);
				break;
			case 2:
				var1 += (window[i][j].green1 - mean1) * (window[i][j].green1 - mean1);
				var2 += (window[i][j].green2 - mean2) * (window[i][j].green2 - mean2);
				covar += (window[i][j].green1 - mean1) * (window[i][j].green2 - mean2);
				break;
			case 3:
				var1 += (window[i][j].blue1 - mean1) * (window[i][j].blue1 - mean1);
				var2 += (window[i][j].blue2 - mean2) * (window[i][j].blue2 - mean2);
				covar += (window[i][j].blue1 - mean1) * (window[i][j].blue2 - mean2);
				break;

			}
		}
	}
	var1 /= size;
	var2 /= size;
	covar /= size;

	return ((2 * mean1 * mean2 + C1) * (2 * covar + C2)) / ((mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2));
}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3f(glGetUniformLocation(m_programID, "eye_pos"), m_eye.x, m_eye.y, m_eye.z);

	ImGuiIO io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Begin("Kvalitativ osszehasonlitas", 0, window_flags); 
	ImFont* imFo = ImGui::GetFont();

	ImGui::NewLine();
	if (currentWindow == WINDOW1) {	CursorPos(fmax( (fmax(im1.getSurface()->w, 300) + 20) / 2 - 235 , 20));	}
	else { CursorPos(1 * (fmax(im1.getSurface()->w, 300) + fmax(im2.getSurface()->w, 300) + 20) / 2 - 235);	}
	imFo->Scale = 2.f;
	ImGui::PushFont(imFo);
	ImGui::Text("Kepek kvalitativ osszehasonlitasa");
	imFo->Scale = 1.f;
	ImGui::PopFont();
	ImGui::NewLine();

	//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 1.0f, 1.0f)); !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);

	if (currentWindow == WINDOW1) {	CursorPos(1 * (fmax(im1.getSurface()->w, 300) + 20) / 4 - 150 / 2);	}
	else {	CursorPos(1 * (fmax(im1.getSurface()->w, 300) + fmax(im2.getSurface()->w, 300) + 20) / 3 - 150/2);	}
	if (ImGui::Button("Kep nagyitasa", ImVec2(150,50))) {
		upd = false;
		updSSIM = false;
		if (currentWindow == WINDOW2)currentWindow = WINDOW1;
	}
	ImGui::SameLine();

	if (currentWindow == WINDOW1) { CursorPos(3 * (fmax(im1.getSurface()->w, 300) + 20) / 4 - 150 / 2); }
	else {	CursorPos(2 * (fmax(im1.getSurface()->w, 300) + fmax(im2.getSurface()->w, 300) + 20) / 3 - 150/2);	}
	if (ImGui::Button("Ket kep SSIM-e", ImVec2(150, 50))) {
		upd = false;
		updSSIM = false;
		if (currentWindow == WINDOW1)currentWindow = WINDOW2;
	}

	imFo->Scale = 1.f;
	ImGui::PopFont();

	//ImGui::PopStyleColor();
		
	ImGui::NewLine();
	ImGui::NewLine();

	ImGui::Text("Az elso kep eleresi utvonala"); 
	if (currentWindow == WINDOW2) {
		ImGui::SameLine(); 
		CursorPos(fmax(im1.getSurface()->w, 300) + 20);
		ImGui::Text("A masodik kep eleresi utvonala");
	}

	ImGui::PushItemWidth(300);
	ImGui::InputText("##Str1", str1, IM_ARRAYSIZE(str1)); 
	if (currentWindow == WINDOW2) { 
		ImGui::SameLine(); 
		CursorPos(fmax(im1.getSurface()->w, 300) + 20);
		ImGui::InputText("##Str2", str2, IM_ARRAYSIZE(str2)); 
	}
	ImGui::PopItemWidth();

	if (currentError[0]) {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("Hibas eleresi utvonal");
		ImGui::PopStyleColor();
	}
	else ImGui::NewLine();
	if (currentWindow == WINDOW2) {
		ImGui::SameLine();
		CursorPos(fmax(im1.getSurface()->w, 300) + 20);
		if (currentError[1]) {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("Hibas eleresi utvonal");
			ImGui::PopStyleColor();
		}
		else ImGui::NewLine();
	}

	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);
	if (ImGui::Button("Betoltes##str1", ImVec2(150,50))) {
		upd = false;
		if (Verify(str1, str1verified,0)) {
			im1.loadImage(str1verified);
			imZoom.loadImage(str1verified);
			if (zoomW > fmin(im1.getSurface()->w / zoomTimes - 1, im1.getSurface()->w - 1)) { zoomW = fmin(im1.getSurface()->w / zoomTimes - 1, im1.getSurface()->w - 1);} // bigW = im1.getSurface()->w - zoomW * zoomTimes;
			if (zoomH > fmin(im1.getSurface()->h / zoomTimes - 1, im1.getSurface()->h - 1)) { zoomH = fmin(im1.getSurface()->h / zoomTimes - 1, im1.getSurface()->h - 1);} // bigH = im1.getSurface()->h - zoomH * zoomTimes;
		}
	}
	if (currentWindow == WINDOW2){
		ImGui::SameLine();
		CursorPos(fmax(im1.getSurface()->w, 300) + 20 );
		if (ImGui::Button("Betoltes##str2", ImVec2(150, 50))) {
			updSSIM = false;
			if (Verify(str2, str2verified, 1)) {
				im2.loadImage(str2verified);
			}
		}
	}
	imFo->Scale = 1.f;
	ImGui::PopFont();

	ImGui::NewLine();

	if (currentWindow == WINDOW1) {
		Window1();
	}
	else ImGui::Image((void*)(intptr_t)im1.getTexture(), ImVec2(im1.getSurface()->w, im1.getSurface()->h));
	if (currentWindow == WINDOW2) {
		ImGui::SameLine();
		CursorPos(fmax(im1.getSurface()->w, 300) + 20);
		ImGui::Image((void*)(intptr_t)im2.getTexture(), ImVec2(im2.getSurface()->w, im2.getSurface()->h));
	}
		
	if (currentWindow == WINDOW2) {
		Window2AfterColumn();
	}

	ImGui::NewLine();
	ImGui::Text("A kep mentesi utvonala:");

	if (currentWindow == WINDOW1) {
		ImGui::PushItemWidth(300);
		ImGui::InputText("##SavePath1", outstr1, IM_ARRAYSIZE(outstr1));
		ImGui::PopItemWidth();
		if (currentError[2]) {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("Hibas mentesi utvonal");
			ImGui::PopStyleColor();
		}

		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		if (ImGui::Button("Mentes##1",ImVec2(150,50))) {
			upd = false;
			struct stat sb;
			if (stat(outstr1, &sb) != 0) {
				currentError[2] = true;
			}
			else {
				IMG_SavePNG(imZoom.getSurface(), outstr1);
				currentError[2] = false;
			}
		}
		imFo->Scale = 1.f;
		ImGui::PopFont();
	}
	else {
		ImGui::PushItemWidth(300);
		ImGui::InputText("##SavePath2", outstr2, IM_ARRAYSIZE(outstr2));
		ImGui::PopItemWidth();
		if (currentError[3]) {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("Hibas mentesi utvonal");
			ImGui::PopStyleColor();
		}

		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		if (ImGui::Button("Mentes##2", ImVec2(150, 50))) {
			updSSIM = false;
			struct stat sb;
			if (stat(outstr2, &sb) != 0) {
				currentError[3] = true;
			}
			else {
				IMG_SavePNG(imSSIM2.getSurface(), outstr2);
				currentError[3] = false;
			}
		}
		imFo->Scale = 1.f;
		ImGui::PopFont();
	}

	ImGui::End();
}

void CMyApp::CursorPos(float offset) {
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
}

void CMyApp::Window1() {	
	ImFont* imFo = ImGui::GetFont();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::Image((void*)(intptr_t)imZoom.getTexture(), ImVec2(imZoom.getSurface()->w, imZoom.getSurface()->h));
		
	if (ImGui::IsItemHovered())
	{
		if (upd) {
			ZoomMethod();
			float focus_sz_x = 0.f;
			float focus_sz_y = 0.f;
			if (smallChange) {
				focus_sz_x = zoomW;
				focus_sz_y = zoomH;
			}
			else {
				focus_sz_x = zoomW * zoomTimes * -1;
				focus_sz_y = zoomH * zoomTimes * -1;
			}
			float focus_x = io.MousePos.x - pos.x - focus_sz_x * 0.5f; 
			if (smallChange && focus_x < 0.0f ) focus_x = 0.0f;
			else if (smallChange && focus_x > im1.getSurface()->w - focus_sz_x) focus_x = im1.getSurface()->w - focus_sz_x;
			else if (!smallChange && focus_x < zoomW * zoomTimes+1) focus_x = zoomW * zoomTimes+1;
			else if (!smallChange && focus_x > im1.getSurface()->w) focus_x = im1.getSurface()->w;
			float focus_y = io.MousePos.y - pos.y - focus_sz_y * 0.5f; 
			if (smallChange && focus_y < 0.0f) focus_y = 0.0f;
			else if (smallChange && focus_y > im1.getSurface()->h - focus_sz_y) focus_y = im1.getSurface()->h - focus_sz_y;
			else if (!smallChange && focus_y < zoomH * zoomTimes + 1) focus_y = zoomH * zoomTimes + 1;
			else if (!smallChange && focus_y > im1.getSurface()->h) focus_y = im1.getSurface()->h;
			ImVec2 uv0 = ImVec2((focus_x) / im1.getSurface()->w, (focus_y) / im1.getSurface()->h);
			ImVec2 uv1 = ImVec2((focus_x + focus_sz_x) / im1.getSurface()->w, (focus_y + focus_sz_y) / im1.getSurface()->h);
			if (smallChange) {
				smallW = focus_x;
				smallH = focus_y;
			}
			else {
				bigW = im1.getSurface()->w - focus_x;
				bigH = im1.getSurface()->h - focus_y;
			}
		}
	}
	if (ImGui::IsItemClicked()) {
		upd = !upd;
	}

	ImGui::NewLine();

	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);
	CursorPos(1 * fmax(im1.getSurface()->w, 300) / 4 - 70);
	if (ImGui::Button("KisTeglalap",ImVec2(140,40))) {
		upd = false;
		smallChange = true;
	}
	ImGui::SameLine();
	CursorPos(3 * fmax(im1.getSurface()->w, 300) / 4 - 70);
	if (ImGui::Button("Nagyteglalap", ImVec2(140, 40))) {
		upd = false;
		smallChange = false;
	}
	imFo->Scale = 1.f;
	ImGui::PopFont();

	ImGui::NewLine();

	ImGui::Text("Nagyitando terulet szelessege:");
	ImGui::SameLine();
	CursorPos(230);
	ImGui::PushItemWidth(fmax(im1.getSurface()->w,300) - 230);
	ImGui::InputInt("##ZoomWidth",&zoomW,0);
	ImGui::PopItemWidth();
	if (zoomW < 1) {zoomW = 1;} 
	if (zoomW > fmin(im1.getSurface()->w / zoomTimes - 1, im1.getSurface()->w - 1)) { zoomW = fmin(im1.getSurface()->w / zoomTimes - 1, im1.getSurface()->w - 1);  } //bigW = im1.getSurface()->w - zoomW * zoomTimes;
	ImGui::Text("Nagyitando terulet magassaga:");
	ImGui::SameLine();
	CursorPos(230);
	ImGui::PushItemWidth(fmax(im1.getSurface()->w, 300) - 230);
	ImGui::InputInt("##ZoomHeight", &zoomH,0);
	ImGui::PopItemWidth();

	ImGui::Text("Nagyitas merteke:");
	ImGui::SameLine();
	CursorPos(150);
	ImGui::PushItemWidth(fmax(im1.getSurface()->w, 300) - 150);
	ImGui::SliderFloat("##ZoomTimes", &zoomTimes, 0.1f, 10.0f, "%.4f");
	ImGui::PopItemWidth();

	if (zoomH < 1) {zoomH = 1;}
	if (zoomH > fmin(im1.getSurface()->h / zoomTimes - 1, im1.getSurface()->h - 1)) { zoomH = fmin(im1.getSurface()->h / zoomTimes - 1, im1.getSurface()->h - 1);  } //bigH = im1.getSurface()->h - zoomH * zoomTimes;
}

void CMyApp::ZoomMethod() {
	Uint32 red = (255 << 24) | (0 << 16) | (0 << 8) | 255;
	
	for (int i = 0; i < imZoom.getSurface()->w; i++) {
		for (int j = 0; j < imZoom.getSurface()->h; j++) {
			imZoom.PutPixel32(i, j, im1.GetColor(i, j));
		}
	}

	for (int i = zoomW * zoomTimes; i > 0; i--) {
		for (int j = zoomH * zoomTimes; j > 0; j--) {
			if (i == 1 || i == std::floor(zoomW * zoomTimes) || j == 1 || j == std::floor(zoomH * zoomTimes)) {
				imZoom.PutPixel32(imZoom.getSurface()->w - i - bigW - 1, imZoom.getSurface()->h - j - bigH - 1, red);
			}
			else {
				imZoom.PutPixel32(imZoom.getSurface()->w - i - bigW - 1, imZoom.getSurface()->h - j - bigH - 1, imZoom.GetColor(int((zoomW * zoomTimes - i) / zoomTimes + smallW), int((zoomH * zoomTimes - j) / zoomTimes + smallH)));
			}
		}
	}
	for (int i = 0 + smallW; i < zoomW + smallW; i++) {
		for (int j = 0 + smallH; j < zoomH + smallH; j++) {
			if (i == 0 + smallW || j == 0 + smallH || i == zoomW + smallW - 1 || j == zoomH + smallH - 1) {
				imZoom.PutPixel32(i, j, red);
			}
		}
	}
	ImVec2 small1 = ImVec2(smallW, zoomH + smallH);
	ImVec2 small2 = ImVec2(zoomW + smallW, smallH);
	ImVec2 big1 = ImVec2(imZoom.getSurface()->w - bigW - zoomW * zoomTimes, imZoom.getSurface()->h - bigH);
	ImVec2 big2 = ImVec2(imZoom.getSurface()->w - bigW, imZoom.getSurface()->h - bigH - zoomH * zoomTimes);

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
	imZoom.plotLine(x0, y0, x1, y1);
	x0 = small2.x, x1 = big2.x, y0 = small2.y, y1 = big2.y;
	imZoom.plotLine(x0, y0, x1, y1);

	imZoom.textureFromSurface();
}

void CMyApp::Window2AfterColumn() {
	ImFont* imFo = ImGui::GetFont();

	ImGui::NewLine();

	ImGui::Text("Az atmenet meredeksege:");
	ImGui::SameLine();
	CursorPos(200);
	ImGui::PushItemWidth(200);
	ImGui::SliderFloat("##slope", &slope, 1.0f, 10.0f, "%.4f", 3.0f);
	ImGui::PopItemWidth();

	ImGui::Text("Az SSIM szine: "); ImGui::SameLine();
	ImGui::RadioButton("Fekete-Feher", &ssimColor, 0); ImGui::SameLine();
	ImGui::RadioButton("Piros", &ssimColor, 1); ImGui::SameLine();
	ImGui::RadioButton("Zold", &ssimColor, 2);  ImGui::SameLine();
	ImGui::RadioButton("Kek", &ssimColor, 3);  ImGui::SameLine();
	ImGui::RadioButton("Osszes szin", &ssimColor, 4);

	ImGui::Text("Az SSIM szeletek merete:");
	ImGui::SameLine();
	CursorPos(200);
	ImGui::PushItemWidth(100);
	ImGui::InputInt("##Size", &ssimSize,0);
	ImGui::PopItemWidth();
	if (ssimSize < 1) {
		ssimSize = 1;
	}
	if (ssimSize > fmin(im1.getSurface()->w / 2, im1.getSurface()->h / 2)) {
		ssimSize = fmin(im1.getSurface()->w / 2, im1.getSurface()->h / 2);
	}

	ImGui::NewLine();

	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);
	if (ImGui::Button("SSIM",ImVec2(150,50))) {
		updSSIM = false;

		if (im1.getSurface()->h == im2.getSurface()->h && im1.getSurface()->w == im2.getSurface()->w) {
			imSSIM1.setSurface(SSIMSurface(im1, im2, ssimSize));
			imSSIM1.textureFromSurface();
			imSSIM2.Load(str1verified);
			plotLineSSIM(imSSIM2.getSurface()->w / 2, imSSIM2.getSurface()->h / 2, slope);
			imSSIM2.textureFromSurface();
			currentError[4] = false;
		}
		else currentError[4] = true;
	}
	imFo->Scale = 1.f;
	ImGui::PopFont();

	if (currentError[4]) {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("A kepek meretenek meg kell egyeznie");
		ImGui::PopStyleColor();
	}

	ImGui::NewLine();

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImGui::Image((void*)(intptr_t)imSSIM2.getTexture(), ImVec2(imSSIM2.getSurface()->w, imSSIM2.getSurface()->h));
	if (ImGui::IsItemHovered())
	{
		if (updSSIM) {
			plotLineSSIM(io.MousePos.x - pos.x, io.MousePos.y - pos.y, slope);
			imSSIM2.textureFromSurface();
		}
	}
	if (ImGui::IsItemClicked()) {
		updSSIM = !updSSIM;
	}

	ImGui::Text("Osszesitett SSIM ertek: %f", ssimOsszeg);

}

SDL_Surface* CMyApp::SSIMSurface(Image img1, Image img2, int windowSize) {

	SDL_Surface* ssimSurface = SDL_CreateRGBSurface(0, img1.getSurface()->w, img1.getSurface()->h, img1.getSurface()->format->BitsPerPixel, 0, 0, 0, 0);
	Image seged;
	seged.setSurface(ssimSurface);

	//SDL_LockSurface(ssimSurface);

	std::vector<std::vector<colorsStruckt>> colorVect(windowSize, std::vector<colorsStruckt>(windowSize));

	//std::vector<std::vector<Uint8>> grey1(windowSize, std::vector<Uint8>(windowSize));
	//std::vector<std::vector<Uint8>> grey2(windowSize, std::vector<Uint8>(windowSize));

	//std::vector<std::vector<Uint8>> red1(windowSize, std::vector<Uint8>(windowSize));
	//std::vector<std::vector<Uint8>> red2(windowSize, std::vector<Uint8>(windowSize));

	//std::vector<std::vector<Uint8>> green1(windowSize, std::vector<Uint8>(windowSize));
	//std::vector<std::vector<Uint8>> green2(windowSize, std::vector<Uint8>(windowSize));

	//std::vector<std::vector<Uint8>> blue1(windowSize, std::vector<Uint8>(windowSize));
	//std::vector<std::vector<Uint8>> blue2(windowSize, std::vector<Uint8>(windowSize));

	ssimOsszeg = 0.f;
	int num = 0;

	for (int x = 0; x < seged.getSurface()->w; x += windowSize) {
		for (int y = 0; y < seged.getSurface()->h; y += windowSize) {

			for (int i = 0; i < windowSize; i++) {
				for (int j = 0; j < windowSize; j++) {

					Uint8 r, g, b, a;
					Uint8 r2, g2, b2, a2;
					SDL_GetRGBA(img1.GetColor(x + i, y + j), img1.getSurface()->format, &r, &g, &b,&a);
					SDL_GetRGBA(img2.GetColor(x + i, y + j), img2.getSurface()->format, &r2, &g2, &b2,&a2);

					switch (ssimColor)
					{
					case 0:
						colorVect[i][j].grey1 = greyscale(img1.GetColor(x + i, y + j), img1.getSurface()->format);
						colorVect[i][j].grey2 = greyscale(img2.GetColor(x + i, y + j), img2.getSurface()->format);
						break;
					case 1:
						colorVect[i][j].red1 = r;
						colorVect[i][j].red2 = r2;
						break;
					case 2:
						colorVect[i][j].green1 = g;
						colorVect[i][j].green2 = g2;
						break;
					case 3:
						colorVect[i][j].blue1 = b;
						colorVect[i][j].blue2 = b2;
						break;
					case 4:
						colorVect[i][j].red1 = r;
						colorVect[i][j].red2 = r2;
						colorVect[i][j].green1 = g;
						colorVect[i][j].green2 = g2;
						colorVect[i][j].blue1 = b;
						colorVect[i][j].blue2 = b2;
						break;
					}

					colorVect[i][j].alpha1 = a;
					colorVect[i][j].alpha2 = a2;
					
				}
			}
			float ssimGrey = 0.f;
			float ssimRed = 0.f;
			float ssimGreen = 0.f;
			float ssimBlue = 0.f;
			Uint8 ymc,rU,gU,bU;

			switch (ssimColor)
			{
			case 0:
				ssimGrey = SSIM(colorVect, windowSize,0);
				ssimOsszeg += ssimGrey;
				num++;
				ymc = (Uint8)((ssimGrey + 1.0) * 255 / 2);
				break; 
			case 1:
				ssimRed = SSIM(colorVect, windowSize,1);
				ssimOsszeg += ssimRed;
				num++;
				ymc = (Uint8)((ssimRed + 1.0) * 255 / 2);
				break;
			case 2:
				ssimGreen = SSIM(colorVect, windowSize,2);
				ssimOsszeg += ssimGreen;
				num++;
				ymc = (Uint8)((ssimGreen + 1.0) * 255 / 2);
				break;
			case 3:
				ssimBlue = SSIM(colorVect, windowSize,3);
				ssimOsszeg += ssimBlue;
				num++;
				ymc = (Uint8)((ssimBlue + 1.0) * 255 / 2);
				break;
			case 4:
				ssimRed = SSIM(colorVect, windowSize,1);
				ssimOsszeg += ssimRed;
				num++;
				ssimGreen = SSIM(colorVect, windowSize,2);
				ssimOsszeg += ssimGreen;
				num++;
				ssimBlue = SSIM(colorVect, windowSize,3);
				ssimOsszeg += ssimBlue;
				num++;
				rU = (Uint8)((ssimRed + 1.0) * 255 / 2);
				gU = (Uint8)((ssimGreen + 1.0) * 255 / 2);
				bU = (Uint8)((ssimBlue + 1.0) * 255 / 2);
				break;
			}
			//red 0 0 255   green 0 255 0   blue 255 0 0     
			//r = -y -m	, g = -y -c , b = -m -c
			//Uint8 ymc = (Uint8)((ssimValue + 1.0) * 255 / 2); 

			for (int i = 0; i < windowSize; i++) {
				for (int j = 0; j < windowSize; j++) {
					if (x + i < seged.getSurface()->w && y + j < seged.getSurface()->h) {

						switch (ssimColor)
						{
						case 0:
							seged.PutPixel32(x + i, y + j, SDL_MapRGBA(seged.getSurface()->format, ymc, ymc, ymc, colorVect[i][j].alpha1));
							break;
						case 1:
							seged.PutPixel32(x + i, y + j, SDL_MapRGBA(seged.getSurface()->format, ymc, ymc, 255, colorVect[i][j].alpha1));
							break;
						case 2:
							seged.PutPixel32(x + i, y + j, SDL_MapRGBA(seged.getSurface()->format, ymc, 255, ymc, colorVect[i][j].alpha1));
							break;
						case 3:
							seged.PutPixel32(x + i, y + j, SDL_MapRGBA(seged.getSurface()->format, 255, ymc, ymc, colorVect[i][j].alpha1));
							break;
						case 4:
							seged.PutPixel32(x + i, y + j, SDL_MapRGBA(seged.getSurface()->format, bU, gU, rU, colorVect[i][j].alpha1));
							break;
						}
						
					}
				}
			}
		}
	}

	ssimOsszeg =  ssimOsszeg / num;

	//SDL_UnlockSurface(ssimSurface); seperable filter matlab 

	return ssimSurface;
}

void CMyApp::plotLineSSIM(int x, int y, float slope) {

	//ImVec2 first, second;

	for (int i = 0; i < imSSIM2.getSurface()->w; i++) {
		for (int j = 0; j < imSSIM2.getSurface()->h; j++) {
			if (j < slope * (i - x) + y) {
				imSSIM2.PutPixel32(i, j, imSSIM1.GetColor(i, j));
			}
			else {
				imSSIM2.PutPixel32(i, j, im1.GetColor(i, j));
			}

			/*if ((j == 0 || i == 0) && j == (int)(slope * (i - x) + y)) {
				first.x = i;
				first.y = j;
			}
			if ((j == ssimSurface2->h - 1 || i == ssimSurface2->w - 1) && j  == (int)(slope * (i - x) + y)) {
				second.x = i;
				second.y = j;
			}*/
		}
	}

	//plotLine(first.x,first.y,second.x,second.y , ssimSurface2);
}

bool CMyApp::Verify(char* filePath , char* filePathv, int noErr) {

	SDL_Surface* imageSurface = IMG_Load(filePath);
	if (!imageSurface) {
		imageSurface = IMG_Load("texture.bmp");
		strcpy(filePathv, "texture.bmp");
		currentError[noErr] = true;
	}
	else {
		strcpy(filePathv, filePath);
		currentError[noErr] = false;
	}
	if (!imageSurface) {
		printf("Failed to load image (also backup image got deleted): %s\n", IMG_GetError());
		return false;
	}
	SDL_FreeSurface(imageSurface);
	return true;
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


Image::Image(void)
{
}

Image::~Image(void)
{
}

void Image::plotLine(int x0, int y0, int x1, int y1) {
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			plotLineLow(x1, y1, x0, y0);
		}
		else {
			plotLineLow(x0, y0, x1, y1);
		}
	}
	else {
		if (y0 > y1) {
			plotLineHigh(x1, y1, x0, y0);
		}
		else {
			plotLineHigh(x0, y0, x1, y1);
		}
	}
}

void Image::plotLineLow(int x0, int y0, int x1, int y1){
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
		PutPixel32(x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255);

		if (D > 0){
			y = y + yi;
			D = D + (2 * (dy - dx));
		}
		else{
			D = D + 2 * dy;
		}
	}
}

void Image::plotLineHigh(int x0,int y0,int x1,int y1){
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
		PutPixel32(x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255);

		if (D > 0) {
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else {
			D = D + 2 * dx;
		}
	}
}

void Image::PutPixel32(int x, int y, Uint32 color)
{
	if (SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);
	PutPixel32_nolock(x, y, color);
	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
}

void Image::PutPixel32_nolock(int x, int y, Uint32 color)
{
	Uint8* pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + x * surface ->format->BytesPerPixel;
	*((Uint32*)pixel) = color;
}

Uint32 Image::GetColor(int x, int y) {
	return *(Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
}

void Image::textureFromSurface() {
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
	texture = textureID;
}

void Image::Load(char* s) {
	surface = IMG_Load(s);
}

void Image::loadImage(char* s) {
	SDL_FreeSurface(surface);
	Load(s);
	textureFromSurface();
}
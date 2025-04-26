#define STB_IMAGE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <imgui/imgui.h>
#include "stb_image.h"

CMyApp::CMyApp(void)
{
	window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar;

	for (int i = 0;i< sizeof(currentErrors);i++) {
		currentErrors[i] = false;
	}

	strcpy(stradd, "C:/Users/User/Pictures/ac2.jpg");
	strcpy(straddverified, stradd);
	RegularModify::Verify(stradd, straddverified);

	currentImageEnum = SEMMIENUM;
	strcpy(outstr, "C:/Users/User/Pictures/save.png");

}

CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

	
	//
	// egyéb inicializálás
	//

	return true;
}

void CMyApp::Clean()
{

}

void CMyApp::Update()
{

}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGuiIO io = ImGui::GetIO();

	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Begin("Kvalitativ osszehasonlitas", 0, window_flags); 

		/*
		ImFont* arial = io.Fonts->AddFontFromFileTTF("ARIAL.ttf", 13.0f);
		io.Fonts->Build();

		ImGui::PushFont(arial);
		ImGui::Text("Teszt");
		ImGui::PopFont();
		*/

	ImFont* imFo = ImGui::GetFont();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::NewLine();
	imFo->Scale = 2.f;	ImGui::PushFont(imFo);
	//RegularModify::CursorPos(io.DisplaySize.x / 2);
	//ImGui::Text("Test");
	RegularModify::CursorPos(std::max((int)(io.DisplaySize.x / 2 - 240),10));
	ImGui::Text("Kepek kvalitativ osszehasonlitasa");
	imFo->Scale = 1.f;	ImGui::PopFont();
	ImGui::NewLine(); ImGui::NewLine();


	int maxHeightVecIndex = 0;
	for (int i = 0; i < imageVec.size(); i++) {
		if (imageVec[i].getSurface()->h > imageVec[maxHeightVecIndex].getSurface()->h) {
			maxHeightVecIndex = i;
		}
	}

	if (currentImageEnum == SEMMIENUM || currentImageEnum == LOADENUM)	{
		imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
		ImGui::Text("Kepek:");
		imFo->Scale = 1.f;	ImGui::PopFont();
		RegularModify::ShowHelpMarker("A betoltott kepek jelennek meg itt. Kattints rajuk, hogy kivalaszd oket.");
		ImGui::NewLine();

		if(imageVec.size()>0) {

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
			
			ImGui::BeginChild("scrolling", ImVec2(0, 320), false , ImGuiWindowFlags_HorizontalScrollbar);
			for (int i = 0; i < imageVec.size(); i++) {
				imageVec[i].drawImage(300, std::find(selectedImageVec.begin(),selectedImageVec.end(),i)!=selectedImageVec.end() );
				if (ImGui::IsItemClicked() && currentImageEnum == SEMMIENUM)
				{
					if (std::find(selectedImageVec.begin(), selectedImageVec.end(), i) == selectedImageVec.end()) {
						selectedImageVec.push_back(i);
					}
					else {
						selectedImageVec.erase(std::remove(selectedImageVec.begin(), selectedImageVec.end(), i), selectedImageVec.end());
					}
				}
				ImGui::SameLine();

			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2);
		}
		else {
			ImGui::Text("Nincsen betoltott kep.");
			ImGui::NewLine();
		}
		if (selectedImageVec.size() == 0) {
			ImGui::NewLine(); ImGui::NewLine();
		}
		
		
	}

	if(currentImageEnum == SEMMIENUM){
		for (int i = 0; i < selectedImageVec.size(); i++) { //edit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			imageVec[selectedImageVec[i]].drawImage();
			ImGui::SameLine();
		}
		if (selectedImageVec.size() > 0) {
			ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine();
		}

		imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
		ImGui::Text("Muveletek:");
		imFo->Scale = 1.f;	ImGui::PopFont();
		RegularModify::ShowHelpMarker("A kivalasztott kepekkel vegrehajthato muveletek.");
		ImGui::NewLine();

		switch (selectedImageVec.size())
		{
		case 0:
			ImGui::Columns(1, "mycolumns0selected", false);
			imFo->Scale = 1.3f;
			ImGui::PushFont(imFo);

			if (ImGui::Button("Betoltes", ImVec2(150, 50))) {
				currentImageEnum = LOADENUM;
			}
			ImGui::NextColumn();
		
			ImGui::Columns(1);
			imFo->Scale = 1.f;
			ImGui::PopFont();
			break;
		case 1:
			ImGui::Columns(2, "mycolumns1selected", false);
			imFo->Scale = 1.3f;
			ImGui::PushFont(imFo);

			if (ImGui::Button("Nagyitas", ImVec2(150, 50))) {
				currentImageEnum = MAGNIFYENUM;
				im1mag = Image1Magnify(imageVec[selectedImageVec[0]]);
			}
			ImGui::NextColumn();

			if (ImGui::Button("Mentes", ImVec2(150, 50))) {
				currentImageEnum = SAVEENUM;
			}
			ImGui::NextColumn();

			ImGui::Columns(1);
			imFo->Scale = 1.f;
			ImGui::PopFont();
			break;
		case 2:
			if (imageVec[selectedImageVec[0]].getSurface()->w == imageVec[selectedImageVec[1]].getSurface()->w && imageVec[selectedImageVec[0]].getSurface()->h == imageVec[selectedImageVec[1]].getSurface()->h) {
				ImGui::Columns(2, "mycolumns2selected", false);
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);

				if (ImGui::Button("SSIM", ImVec2(150, 50))) {
					currentImageEnum = SSIMENUM;
					im2ssim = Image2SSIM(imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);
					im2ssim.SSIMSurface();
				}
				ImGui::NextColumn();

				if (ImGui::Button("Merge", ImVec2(150, 50))) {
					currentImageEnum = MERGEENUM;
					im2merge = Image2Merge(imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);
				}
				ImGui::NextColumn();

				ImGui::Columns(1);
				imFo->Scale = 1.f;
				ImGui::PopFont();
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
				ImGui::Text("Nincsen muvelet kulonbozo meretu kepekre");
				ImGui::PopStyleColor();
			}
			break;
		default:
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("Nincsen muvelet ennyi darab kepre");
			ImGui::PopStyleColor();
			break;
		}
	}

	switch (currentImageEnum)
	{

	case CMyApp::LOADENUM:

		imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
		ImGui::Text("Kep betoltese");
		imFo->Scale = 1.f;	ImGui::PopFont();
		ImGui::NewLine();
		ImGui::Text("A betoltendo kep eleresi utvonala:");
		ImGui::PushItemWidth(300);
		ImGui::InputText("##StrAdd", stradd, IM_ARRAYSIZE(stradd));
		ImGui::PopItemWidth();

		if (currentErrors[0]) {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("Hibas eleresi utvonal");
			ImGui::PopStyleColor();
		}
		else ImGui::NewLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 1.0f, 0.7f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 1.0f, 0.7f, 0.45f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 1.0f, 0.7f, 0.5f));

		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		if (ImGui::Button("Betoltes##stradd", ImVec2(150, 50))) {
			if (RegularModify::Verify(stradd, straddverified)) {
				Image0FromFile imadd;
				imadd.Load(straddverified);
				imadd.textureFromSurface();
				imageVec.push_back(imadd);
				currentErrors[0] = false;
			}
			else {
				currentErrors[0] = true;
			}
		}
		imFo->Scale = 1.f;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		break;

	case CMyApp::MAGNIFYENUM:

		im1mag.editableDrawImage();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 1.0f, 0.7f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 1.0f, 0.7f, 0.45f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 1.0f, 0.7f, 0.5f));

		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		if (ImGui::Button("Betoltes##stradd", ImVec2(150, 50))) {
			imageVec.push_back(im1mag);

			ImGui::OpenPopup("Betoltes##Pop");
		}
		imFo->Scale = 1.f;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		break;

	case CMyApp::SAVEENUM:

		imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
		ImGui::Text("Kep mentese");
		imFo->Scale = 1.f;	ImGui::PopFont();
		ImGui::NewLine();

		ImGui::Image((void*)(intptr_t)imageVec[selectedImageVec[0]].getTexture(), ImVec2(imageVec[selectedImageVec[0]].getSurface()->w, imageVec[selectedImageVec[0]].getSurface()->h));
		ImGui::NewLine();

		ImGui::Text("A kep mentesi utvonala:");
		ImGui::PushItemWidth(std::max(imageVec[selectedImageVec[0]].getSurface()->w, 300));
		ImGui::InputText("##SavePath", outstr, IM_ARRAYSIZE(outstr));
		ImGui::PopItemWidth();
		if (currentErrors[1]) {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("Hibas mentesi utvonal");
			ImGui::PopStyleColor();
		}
		ImGui::NewLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 1.0f, 0.7f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 1.0f, 0.7f, 0.45f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 1.0f, 0.7f, 0.5f));

		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		if (ImGui::Button("Mentes", ImVec2(150, 50))) {
			struct stat sb;
			if (stat(outstr, &sb) != 0) {
				currentErrors[1] = true;
			}
			else {
				IMG_SavePNG(imageVec[selectedImageVec[0]].getSurface(), outstr);
				currentImageEnum = SEMMIENUM;
				currentErrors[1] = false;

				ImGui::OpenPopup("Mentes##Pop");
			}
		}
		imFo->Scale = 1.f;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		break;

	case CMyApp::SSIMENUM:

		im2ssim.editableDrawImage();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 1.0f, 0.7f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 1.0f, 0.7f, 0.45f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 1.0f, 0.7f, 0.5f));

		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		if (ImGui::Button("Betoltes##stradd", ImVec2(150, 50))) {
			imageVec.push_back(im2ssim);

			ImGui::OpenPopup("Betoltes##Pop");
		}
		imFo->Scale = 1.f;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		break;

	case CMyApp::MERGEENUM:

		im2merge.editableDrawImage();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 1.0f, 0.7f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 1.0f, 0.7f, 0.45f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 1.0f, 0.7f, 0.5f));

		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		if (ImGui::Button("Betoltes##stradd", ImVec2(150, 50))) {
			imageVec.push_back(im2merge);

			ImGui::OpenPopup("Betoltes##Pop");
		}
		imFo->Scale = 1.f;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		
		break;

	default:
		break;
	}


	if (currentImageEnum != SEMMIENUM) {

		ImGui::SameLine();
		if (currentImageEnum != LOADENUM) {
			RegularModify::CursorPos(std::max(170, imageVec[selectedImageVec[0]].getSurface()->w-150));
		}
		else {
			RegularModify::CursorPos(170);
		}
		imFo->Scale = 1.3f;
		ImGui::PushFont(imFo);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.8f, 0.8f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.8f, 0.8f, 0.45f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.8f, 0.8f, 0.5f));
		if (ImGui::Button("Vissza", ImVec2(150, 50))) {
			currentImageEnum = SEMMIENUM;
		}
		ImGui::PopStyleColor(3);
		imFo->Scale = 1.f;
		ImGui::PopFont();
	}

	ImGui::NewLine();


	//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
	//ImGui::PopStyleColor();

	if (ImGui::BeginPopupModal("Betoltes##Pop", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Kep sikeresen betoltve");
		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Mentes##Pop", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Kep sikeresen elmentve");
		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::End();
}

void CMyApp::Resize(int _w, int _h) //??????????????????????????????????????????????????
{
	glViewport(0, 0, _w, _h);

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

void Image::drawImage() {
	ImGui::Image((void*)(intptr_t)texture, ImVec2(surface->w, surface->h));
}

void Image::drawImage(int size, bool selected) {
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));
	ImGui::ImageButton((void*)(intptr_t)texture, ImVec2(size, size), ImVec2(0, 0), ImVec2(1,1), selected ? 5 : 0, ImColor(0, 0, 0, 255));
	ImGui::PopStyleColor(2);
}

void Image::editableDrawImage() {
	ImGui::Image((void*)(intptr_t)texture, ImVec2(surface->w, surface->h));
}

void Image0FromFile::Load(char* s) {
	surface = IMG_Load(s);
}

Image1::Image1(void) {}

Image1::Image1(Image im) {
	SDL_Surface* source = im.getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);

	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}

	Image imseged;
	imseged.setSurface(destination);
	imseged.textureFromSurface();

	imIn = imseged;
}

Image1Magnify::Image1Magnify(void){}

Image1Magnify::Image1Magnify(Image im) : Image1(im) {
	SDL_Surface* source = imIn.getSurface();  //put this in image1
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0, source->w, source->h, source->format->BitsPerPixel, source->format->format);
	SDL_BlitSurface(source, nullptr, destination, nullptr);

	surface = destination;
	textureFromSurface();

	zoomW = 100;
	zoomH = 50;
	zoomTimes = 2.f;
	smallX = 0;
	smallY = 0;
	smallChange = true;
	bigX = 0;
	bigY = 0;
	upd = false;
}

void Image1Magnify::editableDrawImage() {

	ImFont* imFo = ImGui::GetFont();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
	ImGui::Text("Kep nagyitasa");
	imFo->Scale = 1.f;	ImGui::PopFont();
	ImGui::NewLine();

	ImGui::Image((void*)(intptr_t)texture, ImVec2(surface->w, surface->h));
	if (ImGui::IsItemHovered())
	{
		if (upd) {

			MagnifyMethod();

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
			if (smallChange && focus_x < 0.0f) focus_x = 0.0f;
			else if (smallChange && focus_x > surface->w - focus_sz_x) focus_x = surface->w - focus_sz_x;
			else if (!smallChange && focus_x < zoomW * zoomTimes + 1) focus_x = zoomW * zoomTimes + 1;
			else if (!smallChange && focus_x > surface->w) focus_x = surface->w;
			float focus_y = io.MousePos.y - pos.y - (smallChange ? focus_sz_y : 0); // change here for cursor.y relative to the rectangles
			if (smallChange && focus_y < 0.0f) focus_y = 0.0f;
			else if (smallChange && focus_y > surface->h - focus_sz_y) focus_y = surface->h - focus_sz_y;
			else if (!smallChange && focus_y < zoomH * zoomTimes + 1) focus_y = zoomH * zoomTimes + 1;
			else if (!smallChange && focus_y > surface->h) focus_y = surface->h;
			ImVec2 uv0 = ImVec2((focus_x) / surface->w, (focus_y) / surface->h);
			ImVec2 uv1 = ImVec2((focus_x + focus_sz_x) / surface->w, (focus_y + focus_sz_y) / surface->h);
			if (smallChange) {
				smallX = focus_x;
				smallY = focus_y;
			}
			else {
				bigX = surface->w - focus_x;
				bigY = surface->h - focus_y;
			}
		}
	}
	else {
		upd = false;
	}
	if (ImGui::IsItemClicked()) {
		upd = !upd;
	}

	ImGui::NewLine();

	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);
	RegularModify::CursorPos(1 * fmax(surface->w, 300) / 4 - 70);

	if (smallChange) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 1.f, 0.2f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 1.f, 0.2f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 1.f, 0.2f, 0.7f));
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.75f));
	}
	if (ImGui::Button("KisTeglalap", ImVec2(140, 40))) {
		smallChange = true;
	}
	ImGui::PopStyleColor(3);
	imFo->Scale = 1.f;
	ImGui::PopFont();
	RegularModify::ShowHelpMarker("Ezt kivalasztva a nagyitando terulet helyet tudod majd mozgatni.");
	
	ImGui::SameLine();
	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);
	RegularModify::CursorPos(3 * fmax(surface->w, 300) / 4 - 70);
	if (!smallChange) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 1.f, 0.2f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 1.f, 0.2f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 1.f, 0.2f, 0.7f));
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.75f));
	}
	if (ImGui::Button("NagyTeglalap", ImVec2(140, 40))) {
		smallChange = false;
	}
	ImGui::PopStyleColor(3);
	imFo->Scale = 1.f;
	ImGui::PopFont();

	RegularModify::ShowHelpMarker("Ezt kivalasztva a felnagyitott terulet kivetitesenek a helyet tudod majd mozgatni.");


	ImGui::NewLine();

	ImGui::Text("Nagyitando terulet szelessege:"); //still errors
	ImGui::SameLine();
	RegularModify::CursorPos(230);
	ImGui::PushItemWidth(fmax(surface->w, 300) - 230);
	ImGui::InputInt("##ZoomWidth", &zoomW, 0);
	ImGui::PopItemWidth();
	if (zoomW < 1) { zoomW = 1; }
	if (zoomW > fmin(surface->w / zoomTimes - 1,surface->w - 1)) { zoomW = fmin(surface->w / zoomTimes - 1, surface->w - 1); } //bigX = im1.getSurface()->w - zoomW * zoomTimes;
	ImGui::Text("Nagyitando terulet magassaga:");
	ImGui::SameLine();
	RegularModify::CursorPos(230);
	ImGui::PushItemWidth(fmax(surface->w, 300) - 230);
	ImGui::InputInt("##ZoomHeight", &zoomH, 0);
	ImGui::PopItemWidth();

	ImGui::Text("Nagyitas merteke:");
	ImGui::SameLine();
	RegularModify::CursorPos(150);
	ImGui::PushItemWidth(fmax(surface->w, 300) - 150);
	ImGui::SliderFloat("##ZoomTimes", &zoomTimes, 0.1f, 10.0f, "%.4f");
	ImGui::PopItemWidth();

	ImGui::NewLine();
}

void Image1Magnify::MagnifyMethod() {
	Uint32 red = (255 << 24) | (0 << 16) | (0 << 8) | 255;

	for (int i = 0; i < surface->w; i++) {
		for (int j = 0; j < surface->h; j++) {
			SurfaceModify::PutPixel32(i, j, SurfaceModify::GetColor(i, j, imIn.getSurface()), surface);
		}
	}

	for (int i = zoomW * zoomTimes; i > 0; i--) {
		for (int j = zoomH * zoomTimes; j > 0; j--) {
			if (i == 1 || i == std::floor(zoomW * zoomTimes) || j == 1 || j == std::floor(zoomH * zoomTimes)) {
				SurfaceModify::PutPixel32(surface->w - i - bigX - 1, surface->h - j - bigY - 1, red, surface);
			}
			else {
				SurfaceModify::PutPixel32(surface->w - i - bigX - 1, surface->h - j - bigY - 1, SurfaceModify::GetColor(int((zoomW * zoomTimes - i) / zoomTimes + smallX), int((zoomH * zoomTimes - j) / zoomTimes + smallY), surface), surface);
			}
		}
	}
	for (int i = 0 + smallX; i < zoomW + smallX; i++) {
		for (int j = 0 + smallY; j < zoomH + smallY; j++) {
			if (i == 0 + smallX || j == 0 + smallY || i == zoomW + smallX - 1 || j == zoomH + smallY - 1) {
				SurfaceModify::PutPixel32(i, j, red, surface);
			}
		}
	}
	ImVec2 small1 = ImVec2(smallX, zoomH + smallY);
	ImVec2 small2 = ImVec2(zoomW + smallX, smallY);
	ImVec2 big1 = ImVec2(surface->w - bigX - zoomW * zoomTimes, surface->h - bigY);
	ImVec2 big2 = ImVec2(surface->w - bigX, surface->h - bigY - zoomH * zoomTimes);

	if (!(small1.y < big1.y && small2.y > big2.y && small1.x > big1.x && small2.x < big2.x)) {
		if (small1.y < big1.y && small2.y > big2.y) {
			if (small1.x < big1.x) {
				small2.x = small1.x;
				big2.x = big1.x;
			}
			else {
				small1.x = small2.x;
				big1.x = big2.x;
			}
		}
		else{
			int tempSmall1y = small1.y, tempBig1y = big1.y;

			if (small1.x > big1.x && small1.y < big1.y || small1.x < big1.x && small1.y > big1.y) {
				small1.y = small2.y;
				big1.y = big2.y;
			}
			if (small2.x > big2.x && small2.y < big2.y || small2.x < big2.x && small2.y > big2.y) {
				small2.y = tempSmall1y;
				big2.y = tempBig1y;
			}
		}

		SurfaceModify::plotLine(small1.x, small1.y, big1.x, big1.y, surface);
		SurfaceModify::plotLine(small2.x, small2.y, big2.x, big2.y, surface);
	}

	textureFromSurface();
}

Image2::Image2(void) {}

Image2::Image2(Image im1, Image im2) {
	SDL_Surface* source1 = im1.getSurface();
	SDL_Surface* destination1 = SDL_CreateRGBSurfaceWithFormat(0,
		source1->w, source1->h, source1->format->BitsPerPixel, source1->format->format);

	if (destination1 != nullptr) {
		SDL_BlitSurface(source1, nullptr, destination1, nullptr);
	}

	Image imseged1;
	imseged1.setSurface(destination1);
	imseged1.textureFromSurface();

	imIn1 = imseged1;

	SDL_Surface* source2 = im2.getSurface();
	SDL_Surface* destination2 = SDL_CreateRGBSurfaceWithFormat(0,
		source2->w, source2->h, source2->format->BitsPerPixel, source2->format->format);

	if (destination2 != nullptr) {
		SDL_BlitSurface(source2, nullptr, destination2, nullptr);
	}

	Image imseged2;
	imseged2.setSurface(destination2);
	imseged2.textureFromSurface();

	imIn2 = imseged2;
}

Image2SSIM::Image2SSIM(void){}

Image2SSIM::Image2SSIM(Image im1, Image im2) : Image2(im1,im2) {
	SDL_Surface* source = imIn1.getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);

	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}

	surface = destination;
	textureFromSurface();

	ssimSize = 1;
	ssimColor = 0;
	ssimOsszeg = 0;
}

void Image2SSIM::editableDrawImage() {

	ImFont* imFo = ImGui::GetFont();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
	ImGui::Text("Ket kep SSIM-e");
	imFo->Scale = 1.f;	ImGui::PopFont();
	ImGui::NewLine();

	ImGui::Image((void*)(intptr_t)texture, ImVec2(surface->w, surface->h));
	ImGui::NewLine();

	ImGui::Text("Osszesitett SSIM ertek: %f", ssimOsszeg);
	RegularModify::ShowHelpMarker("A kepeken vegrehajtott SSIM algoritmusok eredmenyenek az atlaga.");

	ImGui::Text("Az SSIM szine: "); ImGui::SameLine();
	ImGui::RadioButton("Fekete-Feher", &ssimColor, 0); ImGui::SameLine();
	ImGui::RadioButton("Kek", &ssimColor, 1); ImGui::SameLine();
	ImGui::RadioButton("Zold", &ssimColor, 2);  ImGui::SameLine();
	ImGui::RadioButton("Piros", &ssimColor, 3);  ImGui::SameLine();
	ImGui::RadioButton("Osszes szin", &ssimColor, 4);	ImGui::SameLine();
	ImGui::RadioButton("Heatmap", &ssimColor, 5);
	RegularModify::ShowHelpMarker("Itt tudod kivalasztani az SSIM kep szinet/stilusat.");

	ImGui::Text("Az SSIM szeletek merete:");
	ImGui::SameLine();
	RegularModify::CursorPos(200);
	ImGui::PushItemWidth(100);
	ImGui::InputInt("##Size", &ssimSize, 0);
	RegularModify::ShowHelpMarker("Az SSIM algoritmus hanyszor hanyas szeletekben szamoljon.");

	ImGui::NewLine();
	ImGui::PopItemWidth();
	if (ssimSize < 1) { //lookinto
		ssimSize = 1;
	}
	if (ssimSize > fmin(surface->w / 2, surface->h / 2)) {
		ssimSize = fmin(surface->w / 2, surface->h / 2);
	}

	RegularModify::CursorPos((surface->w / 2) - (150/2) );
	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);
	if (ImGui::Button("Ujrageneralas", ImVec2(150, 50))) {

		SSIMSurface();
	}
	imFo->Scale = 1.f;
	ImGui::PopFont();
	

	ImGui::NewLine(); ImGui::NewLine();

}

void Image2SSIM::SSIMSurface() {

	//SDL_LockSurface(ssimSurface);

	std::vector<std::vector<colorsStruckt>> colorVect(ssimSize);
	for (int i = 0; i < ssimSize; i++) {
		colorVect[i].resize(ssimSize);
	}

	/*std::vector<std::vector<colorsStruckt*>> colorVect2(ssimSize);
	for (int i = 0; i < ssimSize; i++) {
		colorVect[i].resize(ssimSize);
	}*/

	//std::vector<std::vector<colorsStruckt*>> colorVect2(ssimSize, std::vector<colorsStruckt*>(ssimSize)); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

	for (int x = 0; x < surface->w; x += ssimSize) {
		for (int y = 0; y < surface->h; y += ssimSize) {

			for (int i = 0; i < ssimSize; i++) { //check
				for (int j = 0; j < ssimSize; j++) {

					Uint8 r, g, b, a;
					Uint8 r2, g2, b2, a2;
					SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, imIn1.getSurface()), imIn1.getSurface()->format, &r, &g, &b, &a);
					SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, imIn2.getSurface()), imIn2.getSurface()->format, &r2, &g2, &b2, &a2);

					/*switch (ssimColor)
					{
					case 0:*/
						colorVect[i][j].grey1 = RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, imIn1.getSurface()), imIn1.getSurface()->format);
						colorVect[i][j].grey2 = RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, imIn2.getSurface()), imIn2.getSurface()->format);
						/*break;
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
					case 4:*/
						colorVect[i][j].red1 = r;
						colorVect[i][j].red2 = r2;
						colorVect[i][j].green1 = g;
						colorVect[i][j].green2 = g2;
						colorVect[i][j].blue1 = b;
						colorVect[i][j].blue2 = b2;
						/*break;
					}*/

					colorVect[i][j].alpha1 = a;
					colorVect[i][j].alpha2 = a2;

				}
			}
			float ssimGrey = 0.f;
			float ssimRed = 0.f;
			float ssimGreen = 0.f;
			float ssimBlue = 0.f;
			Uint32 putColor;
			Uint8 seged;

			switch (ssimColor)
			{
			case 0:
				ssimGrey = SSIMmethod(colorVect, 0);
				ssimOsszeg += ssimGrey;
				num++;
				seged = (Uint8)((ssimGrey) * 255);
				putColor = (seged << 24) + (seged << 16) + (seged << 8);
				break;
			case 1:
				ssimRed = SSIMmethod(colorVect, 1);
				ssimOsszeg += ssimRed;
				num++;
				seged = (Uint8)((ssimRed) * 255);
				putColor = (seged << 24) + (seged << 16) + (255 << 8); //fix
				break;
			case 2:
				ssimGreen = SSIMmethod(colorVect, 2);
				ssimOsszeg += ssimGreen;
				num++;
				seged = (Uint8)((ssimGreen) * 255);
				putColor = (seged << 24) + (255 << 16) + (seged << 8);
				break;
			case 3:
				ssimBlue = SSIMmethod(colorVect, 3);
				ssimOsszeg += ssimBlue;
				num++;
				seged = (Uint8)((ssimBlue) * 255);
				putColor = (255 << 24) + (seged << 16) + (seged << 8);
				break;
			case 4:
				ssimRed = SSIMmethod(colorVect, 1);
				ssimOsszeg += ssimRed;
				num++;
				ssimGreen = SSIMmethod(colorVect, 2);
				ssimOsszeg += ssimGreen;
				num++;
				ssimBlue = SSIMmethod(colorVect, 3);
				ssimOsszeg += ssimBlue;
				num++;
				putColor = ((Uint8)((ssimRed) * 255) << 24) + ((Uint8)((ssimGreen) * 255) << 16) + ((Uint8)((ssimBlue) * 255) << 8);
				break;
			case 5:
				ssimGrey = SSIMmethod(colorVect, 0);
				ssimOsszeg += ssimGrey;
				num++;
				putColor = RegularModify::heatmapColor(ssimGrey);
				break;
			}
			//red 0 0 255   green 0 255 0   blue 255 0 0     
			//r = -y -m	, g = -y -c , b = -m -c 

			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize; j++) {
					if (x + i < surface->w && y + j < surface->h) {

						SurfaceModify::PutPixel32(x + i, y + j, SDL_MapRGBA(surface->format, (putColor >> 24) & 0xFF, (putColor >> 16) & 0xFF, (putColor >> 8) & 0xFF, 255 - (colorVect[i][j].alpha1 - colorVect[i][j].alpha2) ), surface);
					}
				}
			}
		}
	}

	ssimOsszeg = ssimOsszeg / num;

	textureFromSurface();

}

float Image2SSIM::SSIMmethod(std::vector<std::vector<colorsStruckt>> window, /*std::vector<std::vector<colorsStruckt>> window2,*/ int currCol) { 

	float mean1 = 0.f;
	float mean2 = 0.f;
	for (int i = 0; i < ssimSize; i++) {
		for (int j = 0; j < ssimSize; j++) {
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
	mean1 /= ssimSize;
	mean2 /= ssimSize;

	float var1 = 0.f;
	float var2 = 0.f;
	float covar = 0.f;

	for (int i = 0; i < ssimSize; i++) {
		for (int j = 0; j < ssimSize; j++) {
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
	var1 /= ssimSize;
	var2 /= ssimSize;
	covar /= ssimSize;

	return ((2 * mean1 * mean2 + C1) * (2 * covar + C2)) / ((mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2));
}

Image2Merge::Image2Merge(void){}

Image2Merge::Image2Merge(Image im1, Image im2) : Image2(im1, im2) {
	SDL_Surface* source = imIn1.getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);

	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}

	surface = destination;
	textureFromSurface();

	slope = 1.3f;
	upd = false;
}

void Image2Merge::editableDrawImage() {

	ImFont* imFo = ImGui::GetFont();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
	ImGui::Text("Ket kep osszeolvasztasa");
	imFo->Scale = 1.f;	ImGui::PopFont();
	ImGui::NewLine();

	ImGui::Image((void*)(intptr_t)texture, ImVec2(surface->w, surface->h));
	if (ImGui::IsItemHovered())
	{
		if (upd) {
			plotLineMerge(io.MousePos.x - pos.x, io.MousePos.y - pos.y);
			textureFromSurface();
		}
	}
	else {
		upd = false;
	}
	if (ImGui::IsItemClicked()) {
		upd = !upd;
	}
	ImGui::NewLine();

	ImGui::Text("Az atmenet meredeksege:");
	ImGui::SameLine();
	RegularModify::CursorPos(200);
	ImGui::PushItemWidth(std::max(surface->w,300)-200 );
	ImGui::SliderFloat("##slope", &slope, 1.0f, 10.0f, "%.4f", 3.0f);
	ImGui::PopItemWidth();
	ImGui::NewLine();

}

void Image2Merge::plotLineMerge(int x, int y) {
	for (int i = 0; i < imIn1.getSurface()->w; i++) {
		for (int j = 0; j < imIn1.getSurface()->h; j++) {
			if (j < slope * (i - x) + y) {
				SurfaceModify::PutPixel32(i, j, SurfaceModify::GetColor(i, j, imIn1.getSurface()), surface);
			}
			else {
				SurfaceModify::PutPixel32(i, j, SurfaceModify::GetColor(i, j, imIn2.getSurface()), surface);
			}
		}
	}
}


void SurfaceModify::plotLine(int x0, int y0, int x1, int y1, SDL_Surface* sur) {
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			plotLineLow(x1, y1, x0, y0, sur);
		}
		else {
			plotLineLow(x0, y0, x1, y1, sur);
		}
	}
	else {
		if (y0 > y1) {
			plotLineHigh(x1, y1, x0, y0, sur);
		}
		else {
			plotLineHigh(x0, y0, x1, y1, sur);
		}
	}
}

void SurfaceModify::plotLineLow(int x0, int y0, int x1, int y1, SDL_Surface* sur){
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
		PutPixel32(x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);

		if (D > 0){
			y = y + yi;
			D = D + (2 * (dy - dx));
		}
		else{
			D = D + 2 * dy;
		}
	}
}

void SurfaceModify::plotLineHigh(int x0,int y0,int x1,int y1, SDL_Surface* sur){
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
		PutPixel32(x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);

		if (D > 0) {
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else {
			D = D + 2 * dx;
		}
	}
}

void SurfaceModify::PutPixel32(int x, int y, Uint32 color, SDL_Surface* sur)
{
	if (SDL_MUSTLOCK(sur))
		SDL_LockSurface(sur);
	PutPixel32_nolock(x, y, color,sur);
	if (SDL_MUSTLOCK(sur))
		SDL_UnlockSurface(sur);
}

void SurfaceModify::PutPixel32_nolock(int x, int y, Uint32 color, SDL_Surface* sur)
{
	Uint8* pixel = (Uint8*)sur->pixels;
	pixel += (y * sur->pitch) + x * sur->format->BytesPerPixel;
	*((Uint32*)pixel) = color;
}

Uint32 SurfaceModify::GetColor(int x, int y, SDL_Surface* sur) {
	return *(Uint32*)((Uint8*)sur->pixels + y * sur->pitch + x * sur->format->BytesPerPixel);
}


void RegularModify::CursorPos(float offset) {
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
}

void RegularModify::ShowHelpMarker(const char* desc)
{
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

Uint8 RegularModify::greyScale(Uint32 pixel, SDL_PixelFormat* format) {
	Uint8 r, g, b;
	SDL_GetRGB(pixel, format, &r, &g, &b);
	return 0.299f * r + 0.587f * g + 0.114f * b;
}

Uint32 RegularModify::heatmapColor(float value) {

	int r, g, b;

	if (value < 0.5f) {
		// red (255,0,0) -> yellow (255,255,0)
		r = 255;
		g = static_cast<int>(255 * (value / 0.5f));  // 0-0.5  1.
		b = 0;
	}
	else {
		// yellow (255,255,0) -> blue (0,0,255)
		r = g = static_cast<int>(255 * (1.0f - (value / 0.5f)));
		//g = static_cast<int>(255 * (1.0f - (value / 1.0f)));
		b = static_cast<int>(255 * (value / 0.5f));
	}

	return (r << 24) + (g << 16) + (b << 8);
}

bool RegularModify::Verify(char* filePath, char* filePathv) {

	SDL_Surface* imageSurface = IMG_Load(filePath);
	if (!imageSurface) {
		/*imageSurface = IMG_Load("texture.bmp");
		strcpy(filePathv, "texture.bmp");*/
		return false;
	}
	else {
		strcpy(filePathv, filePath);
	}
	/*if (!imageSurface) {
		printf("Failed to load image (also backup image got deleted): %s\n", IMG_GetError());
		return false;
	}*/
	SDL_FreeSurface(imageSurface);
	return true;
}
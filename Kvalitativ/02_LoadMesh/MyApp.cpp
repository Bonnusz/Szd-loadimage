#define STB_IMAGE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))  

#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <imgui/imgui.h>
#include "stb_image.h"
#include <random>

#include <filesystem>

#include <windows.h>
//#include "CommonFileDialogApp.h"

CMyApp::CMyApp(void)
{
	window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar;

	strcpy(stradd, "C:/Users/User/Pictures/ac2.jpg");
	RegularModify::Verify(stradd);

	currentImageEnum = OPERATIONSENUM;
	strcpy(outstr, "C:/Users/User/Pictures/save.png");


	ImGuiIO& io = ImGui::GetIO();

	ImVector<ImWchar> ranges;
	ImFontAtlas::GlyphRangesBuilder builder;
	builder.AddText(u8"áÁéÉíÍóÓöÖõÕúÚüÜûÛ");
	//builder.AddChar('é');
	builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
	builder.BuildRanges(&ranges);                          // Build the final result (ordered ranges with all the unique characters submitted)
	float dpiScale = 3.0f;  //rewrite
	arial = io.Fonts->AddFontFromFileTTF("ARIAL.ttf", 12.0f * dpiScale, NULL, ranges.Data);
	//notosans = io.Fonts->AddFontFromFileTTF("NOTOSANS.ttf", 12.0f * dpiScale, NULL, ranges.Data);
	io.FontGlobalScale = 1.0f / dpiScale;
	io.Fonts->Build();
}

CMyApp::~CMyApp(void)
{
}


bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	//glClearColor(85.f/255.f, 128.f/255.f, 187.f/255.f, 1.0f);

	//glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	//glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	//glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok 

	Colors[ColorEnum::TEXT_LIGHT] = ImVec4(183 / 255.f, 201 / 255.f, 226 / 255.f, 1.f);
	Colors[ColorEnum::TEXT_DARK] = ImVec4(16 / 255.f, 26 / 255.f, 40 / 255.f, 1.f);
	Colors[ColorEnum::TEXT_HINT] = ImVec4(39 / 255.f, 63 / 255.f, 96 / 255.f, 1.f);
	Colors[ColorEnum::CHILD_BG] = ImVec4(183 / 255.f, 201 / 255.f, 226 / 255.f, 1.f);
	Colors[ColorEnum::POPUP_BG] = ImVec4(169 / 255.f, 191 / 255.f, 220 / 255.f, 1.f);
	Colors[ColorEnum::POPUP_BORDER] = ImVec4(39 / 255.f, 63 / 255.f, 96 / 255.f, 1.f);
	Colors[ColorEnum::SCROLL_GRAB] = ImVec4(44 / 255.f, 72 / 255.f, 110 / 255.f, 1.f);
	Colors[ColorEnum::SCROLL_GRAB_HOVERED] = ImVec4(50 / 255.f, 81 / 255.f, 124 / 255.f, 1.f);
	Colors[ColorEnum::SCROLL_GRAB_ACTIVE] = ImVec4(55 / 255.f, 90 / 255.f, 138 / 255.f, 1.f);
	Colors[ColorEnum::SCROLL_BG] = ImVec4(27 / 255.f, 22 / 255.f, 68 / 255.f, 1.f);
	Colors[ColorEnum::HEADER] = ImVec4(183 / 255.f, 201 / 255.f, 226 / 255.f, 1.f);
	Colors[ColorEnum::HEADER_HOVERED] = ImVec4(211 / 255.f, 222 / 255.f, 237 / 255.f, 1.f);
	Colors[ColorEnum::HEADER_ACTIVE] = ImVec4(239 / 255.f, 243 / 255.f, 249 / 255.f, 1.f);
	Colors[ColorEnum::INPUT_BG] = ImVec4(127 / 255.f, 159 / 255.f, 203 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_BLUE] = ImVec4(55 / 255.f, 90 / 255.0f, 138 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_BLUE_HOVERED] = ImVec4(61 / 255.f, 99 / 255.0f, 152 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_BLUE_ACTIVE] = ImVec4(67 / 255.f, 108 / 255.0f, 166 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_GREEN] = ImVec4(50 / 255.f, 200 / 255.f, 50 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_GREEN_HOVERED] = ImVec4(63 / 255.f, 207 / 255.f, 63 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_GREEN_ACTIVE] = ImVec4(78 / 255.f, 211 / 255.f, 78 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_RED] = ImVec4(200 / 255.f, 50 / 255.f, 50 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_RED_HOVERED] = ImVec4(207 / 255.f, 63 / 255.f, 63 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_RED_ACTIVE] = ImVec4(211 / 255.f, 78 / 255.f, 78 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_GREY] = ImVec4(128 / 255.f, 128 / 255.f, 128 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_GREY_HOVERED] = ImVec4(141 / 255.f, 141 / 255.f, 141 / 255.f, 1.f);
	Colors[ColorEnum::BUTTON_GREY_ACTIVE] = ImVec4(160 / 255.f, 160 / 255.f, 160 / 255.f, 1.f);

	//TestMethod();

	return true;
}

void CMyApp::Clean()
{
	//delete[] stradd;
	//delete[] straddverified;
	//delete[] outstr;
}

/*void CMyApp::Update()
{
}*/

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/);

	ImGuiIO io = ImGui::GetIO();

	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Begin("Qualitative comparison", 0, window_flags); 

	ImFont* imFo = ImGui::GetFont();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::NewLine();
	imFo->Scale = 2.f;	ImGui::PushFont(imFo);
	ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
	RegularModify::CursorPos(max((int)(io.DisplaySize.x / 2 - 240),10));
	ImGui::Text("Qualitative comparison of images");
	imFo->Scale = 1.f;	ImGui::PopFont(); ImGui::PopStyleColor();
	ImGui::NewLine(); ImGui::NewLine();

	SetBasicUI();
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, Colors[ColorEnum::SCROLL_BG]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, Colors[ColorEnum::SCROLL_GRAB]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, Colors[ColorEnum::SCROLL_GRAB_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, Colors[ColorEnum::SCROLL_GRAB_ACTIVE]);

	bool isFolder = false;
	for (int i = 0; i < imfVec.size(); i++) {
		if (imfVec[i].iof == iofFolder) {
			isFolder = true;
		}
	}

	ImGui::BeginChild("Pictures", ImVec2(0, imfVec.size() == 0 ? 100 : isFolder? 450 : 395), false); {

		imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
		ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Images: ");
		imFo->Scale = 1.f;	ImGui::PopFont();
		RegularModify::ShowHelpMarker("The loaded images and folders will appear here. Click on them to select them."); ImGui::NewLine();

		if (imfVec.size() > 0) {


			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));

			ImGui::BeginChild("scrolling", ImVec2(0, isFolder? 385 : 330), false, ImGuiWindowFlags_HorizontalScrollbar); {
				//RegularModify::CursorPos(20);
				for (int i = 0; i < imfVec.size(); i++) {

					switch (imfVec[i].iof) {
						case iofImage: {
							RegularModify::CursorPos(20 + 315 * i);
							imfVec[i].im.drawImage(300, std::find(selectedImFVec.begin(), selectedImFVec.end(), i) != selectedImFVec.end());
							break;
						}
						case iofFolder: {
							RegularModify::CursorPos(20 + 315 * i);
							imfVec[i].f.drawImage(300, std::find(selectedImFVec.begin(), selectedImFVec.end(), i) != selectedImFVec.end());
							break;
						}
						default: {
							break;
						}
					}

					if (ImGui::IsItemClicked() && (currentImageEnum == OPERATIONSENUM || currentImageEnum == LOADENUM))
					{
						if (std::find(selectedImFVec.begin(), selectedImFVec.end(), i) == selectedImFVec.end()) {
							selectedImFVec.push_back(i);
						}
						else {
							selectedImFVec.erase(std::remove(selectedImFVec.begin(), selectedImFVec.end(), i), selectedImFVec.end());
						}
					}
					ImGui::SameLine();

				}
				RegularModify::CursorPos(ImGui::GetCursorPosX() + 20);

				if (isFolder) {
					ImGui::NewLine();
					ImGui::NewLine();
				}

				//RegularModify::CursorPos(20); 
				for (int i = 0; i < imfVec.size(); i++) {

					switch (imfVec[i].iof) {
						case iofImage: {
							//RegularModify::CursorPos(ImGui::GetCursorPosX() + 305); 
							break;
						}
						case iofFolder: {
							RegularModify::CursorPos(20 + 315 * i);
							if (ImGui::Button(("<-##" + std::to_string(i)).c_str(), ImVec2(147, 50))) {
								if (imfVec[i].f.iconN != 0) {
									imfVec[i].f.iconN--;
									imfVec[i].f.createIconImageFromImages();
								}
							}
							ImGui::SameLine();
							//RegularModify::CursorPos(ImGui::GetCursorPosX() + 20);
							if (ImGui::Button(("->##"+ std::to_string(i)).c_str(), ImVec2(147, 50))) {
								if (imfVec[i].f.iconN != imfVec[i].f.images.size()-1) {
									imfVec[i].f.iconN++;
									imfVec[i].f.createIconImageFromImages();
								}
							}
							//ImGui::SameLine();
							//imfVec[i].f.drawImage(300, std::find(selectedImFVec.begin(), selectedImFVec.end(), i) != selectedImFVec.end());
							break;
						}
						default: {
							break;
						}
					}
					ImGui::SameLine();

				}
				//RegularModify::CursorPos(ImGui::GetCursorPosX() + 20);
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2);
		}
		else {
			RegularModify::CursorPos(20); ImGui::Text("No loaded image.");
			ImGui::NewLine();
		}
	}
	ImGui::EndChild();

	ImGui::PopStyleVar(2); ImGui::PopStyleColor(9);

	ImGui::NewLine(); ImGui::NewLine();

	switch (currentImageEnum)
	{
	case CMyApp::OPERATIONSENUM: //--------------------------------------------------------------------------------------------------
	{
		if (selectedImFVec.size() > 0) {

			ImGui::PushStyleColor(ImGuiCol_Header, Colors[ColorEnum::HEADER]);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Colors[ColorEnum::HEADER_HOVERED]);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, Colors[ColorEnum::HEADER_ACTIVE]);
			ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_DARK]);

			if (ImGui::CollapsingHeader("Selected images in full size", ImGuiTreeNodeFlags_DefaultOpen)) {

				RegularModify::CursorPos(20);
				for (int i = 0; i < selectedImFVec.size(); i++) {

					switch (imfVec[selectedImFVec[i]].iof) {
					case iofImage: {
						imfVec[selectedImFVec[i]].im.drawImage();
						break;
					}
					case iofFolder: {
						imfVec[selectedImFVec[i]].f.drawImage();
						break;
					}
					default: {
						break;
					}
					}

					ImGui::SameLine();
				}
				ImGui::NewLine(); ImGui::NewLine();
			}
			ImGui::PopStyleColor(4);

			ImGui::NewLine(); ImGui::NewLine();
		}



		SetBasicUI();
		ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_BLUE]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_BLUE_HOVERED]);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_BLUE_ACTIVE]);

		ImGui::BeginChild("Operations", ImVec2(0, 140), false); {

			imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
			ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Operations:");
			imFo->Scale = 1.f;	ImGui::PopFont();
			RegularModify::ShowHelpMarker("The operations that can be done with the selected images will appear here.");

			ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);

			ImGui::NewLine();

			int rowNo = 0;
			if (selectedImFVec.size() == 0) { rowNo = 2; }
			else if (selectedImFVec.size() == 1) { rowNo = 4; }
			else if (selectedImFVec.size() == 2) { rowNo = 6; }
			else { rowNo = 4; }

			ImGui::Columns(rowNo, "mycolumnsselected", false);
			imFo->Scale = 1.3f;
			ImGui::PushFont(imFo);

			if (selectedImFVec.size() == 0) {

				RegularModify::CursorPos(20);
				if (ImGui::Button("Load", ImVec2(150, 50))) {
					currentImageEnum = LOADENUM;
				}
				ImGui::NextColumn();
				if (ImGui::Button("Static", ImVec2(150, 50))) {

					im0stat.Reset();

					im0stat.StaticMethod();

					currentImageEnum = STATICNOISEENUM;
				}
				ImGui::NextColumn();
			}

			if (selectedImFVec.size() >= 1) {

				RegularModify::CursorPos(20);
				if (ImGui::Button("Magnify", ImVec2(150, 50))) {
					currentImageEnum = MAGNIFYENUM;

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1mag.setImage(imfVec[selectedImFVec[0]].im);
						im1mag.Reset();
						im1mag.MagnifyMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1mag.setImage(imfVec[selectedImFVec[0]].f.images[0]);
						im1mag.Reset();
						im1mag.MagnifyMethod(imfVec[selectedImFVec[0]].f.images[0]);
					}
				}
				
				ImGui::NextColumn();
				if (ImGui::Button("Blur", ImVec2(150, 50))) {
					currentImageEnum = BLURENUM;

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1blur.setImage(imfVec[selectedImFVec[0]].im);
						im1blur.Reset();
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1blur.setImage(imfVec[selectedImFVec[0]].f.images[0]);
						im1blur.Reset();
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].f.images[0]);
					}
				}

				ImGui::NextColumn();
				if (ImGui::Button("Color", ImVec2(150, 50))) {
					currentImageEnum = COLORENUM;

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1col.setImage(imfVec[selectedImFVec[0]].im);
						im1col.Reset();
						im1col.ColorMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1col.setImage(imfVec[selectedImFVec[0]].f.images[0]);
						im1col.Reset();
						im1col.ColorMethod(imfVec[selectedImFVec[0]].f.images[0]);
					}
				}

				ImGui::NextColumn();

				if (ImGui::Button("Save", ImVec2(150, 50))) {
					currentImageEnum = SAVEENUM;
				}
				ImGui::NextColumn();

			}

			if (selectedImFVec.size() == 2) {

				if ((imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofImage && imfVec[selectedImFVec[0]].im.getSurface()->w == imfVec[selectedImFVec[1]].im.getSurface()->w && imfVec[selectedImFVec[0]].im.getSurface()->h == imfVec[selectedImFVec[1]].im.getSurface()->h) ||
					(imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofFolder && imfVec[selectedImFVec[0]].im.getSurface()->w == imfVec[selectedImFVec[1]].f.images[0].getSurface()->w && imfVec[selectedImFVec[0]].im.getSurface()->h == imfVec[selectedImFVec[1]].f.images[0].getSurface()->h) ||
					(imfVec[selectedImFVec[0]].iof == iofFolder && imfVec[selectedImFVec[1]].iof == iofImage && imfVec[selectedImFVec[0]].f.images[0].getSurface()->w == imfVec[selectedImFVec[1]].im.getSurface()->w && imfVec[selectedImFVec[0]].f.images[0].getSurface()->h == imfVec[selectedImFVec[1]].im.getSurface()->h) ||
					(imfVec[selectedImFVec[0]].iof == iofFolder && imfVec[selectedImFVec[1]].iof == iofFolder && imfVec[selectedImFVec[0]].f.images[0].getSurface()->w == imfVec[selectedImFVec[1]].f.images[0].getSurface()->w && imfVec[selectedImFVec[0]].f.images[0].getSurface()->h == imfVec[selectedImFVec[1]].f.images[0].getSurface()->h)) {


					if (ImGui::Button("SSIM", ImVec2(150, 50))) {
						currentImageEnum = SSIMENUM;
						if (imfVec[selectedImFVec[0]].iof == iofImage) {
							im2ssim.setImage(imfVec[selectedImFVec[0]].im);
							im2ssim.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.images[0]);
							}
						}
						else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							im2ssim.setImage(imfVec[selectedImFVec[0]].f.images[0]);
							im2ssim.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].f.images[0]);
							}
						}
					}
					ImGui::NextColumn();

					if (ImGui::Button("Merge", ImVec2(150, 50))) {
						currentImageEnum = MERGEENUM;
						if (imfVec[selectedImFVec[0]].iof == iofImage) {
							im2merge.setImage(imfVec[selectedImFVec[0]].im);
							im2merge.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.images[0]);
							}
						}
						else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							im2merge.setImage(imfVec[selectedImFVec[0]].f.images[0]);
							im2merge.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].f.images[0]);
							}
						}
					}
					ImGui::NextColumn();
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREY]);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREY_HOVERED]);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_RED_ACTIVE]);

					ImGui::Button("SSIM", ImVec2(150, 50));
					ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_DARK]);
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(450.0f);
						ImGui::TextUnformatted("The SSIM operation requires the images to be the same size.");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
					ImGui::PopStyleColor();

					ImGui::NextColumn();

					ImGui::Button("Merge", ImVec2(150, 50));
					ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_DARK]);
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(450.0f);
						ImGui::TextUnformatted("The Merge operation requires the images to be the same size.");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
					ImGui::PopStyleColor();

					ImGui::PopStyleColor(3);

					ImGui::NextColumn();
				}
			}

			ImGui::Columns(1);
			imFo->Scale = 1.f;
			ImGui::PopFont();

			ImGui::PopStyleColor();
		}
			ImGui::EndChild();
			ImGui::PopStyleVar(2); ImGui::PopStyleColor(8);

			break;
		}

		case CMyApp::LOADENUM: //-------------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Load", ImVec2(400, 65), false); {
				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Load from file");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			ImGui::BeginChild("Values_Load", ImVec2(400, 110), false); {

				ImGui::NewLine();
				ImGui::NewLine();
				char* segedLoadRadioNames[] = { "Image","Folder" };
				int segedLoadType = im0load.getLoadType();
				for (int i = 0; i < sizeof(segedLoadRadioNames) / sizeof(char*); i++) {
					ImGui::SameLine();
					if (ImGui::RadioButton(segedLoadRadioNames[i], &segedLoadType, i)) {
						im0load.setLoadType(segedLoadType);
					}
				}
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text(im0load.getLoadType() == im0load.PICTURE ? "The image's path:" : im0load.getLoadType() == im0load.FOLDER ? "The folder's path:" : "");
				ImGui::PushItemWidth(360);
				RegularModify::CursorPos(20); ImGui::InputText("##StrAdd", stradd, IM_ARRAYSIZE(stradd));
				ImGui::PopItemWidth();

				ImGui::NewLine();
			}

			ImGui::EndChild();
			ImGui::NewLine();
			ImGui::BeginChild("Buttons_Load", ImVec2(400, 80), false); {
				//--------button

				ImGui::NewLine();
				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo); RegularModify::CursorPos(20);
				if (ImGui::Button("Load##Load", ImVec2(150, 50))) {

					ImageFolder loadImFolder = im0load.Load(stradd);
					if (loadImFolder.iof != iofEmpty) {
						imfVec.push_back(loadImFolder);
						//StoredOperaionsClass s;
						//storedOperationsVector.push_back(s);
					}
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(400 - 170);
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();


				if (ImGui::BeginPopupModal("Load##InvalidPath", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Invalid path.");
					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Load##InvalidSize", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("The image's size must be bigger than 10x10 pixels.");
					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Load##FolderInvalidSize", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Some of the images might have not been loaded, as the image's size must be bigger than 10x10 pixels.");
					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Load##EmptyFolder", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("The folder has no images inside.");
					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::STATICNOISEENUM: { //----------------------------------------------------------------------------------------------
			//RegularModify::ShowHelpMarker("Right-click on the individual color widget to show options.");
			
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_StaticNoise", ImVec2(max(im0stat.staticnoise.getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Static Noise");
				imFo->Scale = 1.f;	ImGui::PopFont();
			
			}

			ImGui::EndChild();

			ImGui::NewLine();
			ImGui::Image((void*)(intptr_t)im0stat.staticnoise.getTexture(), ImVec2(im0stat.staticnoise.getSurface()->w, im0stat.staticnoise.getSurface()->h));
			ImGui::NewLine();

			ImGui::BeginChild("Values_StaticNoise", ImVec2(max(im0stat.staticnoise.getSurface()->w, 400), (215 + im0stat.color.size() * 76)), false); {

				ImGui::NewLine();

				ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_BLUE]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_BLUE_HOVERED]);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_BLUE_ACTIVE]);
				ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				RegularModify::CursorPos((max(im0stat.staticnoise.getSurface()->w, 400) / 2.0f) - (150.0f / 2.0f));
				if (ImGui::Button("Regenerate##StaticNoise", ImVec2(150, 50))) {
					im0stat.StaticMethod();
				}
				ImGui::PopStyleColor(4);
				imFo->Scale = 1.f; ImGui::PopFont();
				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Width:");
				ImGui::SameLine();
				RegularModify::CursorPos(270);
				ImGui::PushItemWidth(fmax(im0stat.staticnoise.getSurface()->w, 400) - 290);
				int segedim0statwidth = im0stat.width;
				if (ImGui::InputInt("##staticnoideWidth", &segedim0statwidth, 0)) {
					if (segedim0statwidth > 0) {
						im0stat.width = segedim0statwidth;
					}
				}
				ImGui::PopItemWidth();

				RegularModify::CursorPos(20); ImGui::Text("Height:");
				ImGui::SameLine();
				RegularModify::CursorPos(270);
				ImGui::PushItemWidth(fmax(im0stat.staticnoise.getSurface()->w, 400) - 290);
				int segedim0statheight = im0stat.height;
				if (ImGui::InputInt("##staticnoideHeight", &segedim0statheight, 0)) {
					if (segedim0statheight > 0) {
						im0stat.height = segedim0statheight;
					}
				}
				ImGui::PopItemWidth();

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Colors:");
				int misc_flags = ImGuiColorEditFlags_AlphaPreviewHalf /* | ImGuiColorEditFlags_AlphaPreview */;

				for (int i = 0; i < im0stat.color.size(); i++) {

					RegularModify::CursorPos(40);
					ImGui::Text("Color %i", i + 1);
					RegularModify::CursorPos(40); bool open_popup = ImGui::ColorButton(("MyColor##" + std::to_string(i)).c_str(), im0stat.color[i], misc_flags, ImVec2(40, 40));
					ImGui::SameLine();
					open_popup |= ImGui::Button(("Palette##" + std::to_string(i)).c_str(), ImVec2(60, 30));
					if (open_popup)
					{
						ImGui::OpenPopup(("mypicker" + std::to_string(i)).c_str());
						im0stat.backup_color[i] = im0stat.color[i];
					}
					if (im0stat.color.size() > 1) {
						ImGui::SameLine();
						if (ImGui::Button(("Delete##" + std::to_string(i)).c_str(), ImVec2(60, 30))) {
							im0stat.color.erase(im0stat.color.begin() + i);
							im0stat.backup_color.erase(im0stat.backup_color.begin() + i);
						}
					}

					//darkbg
					ImGui::PushStyleColor(ImGuiCol_PopupBg, Colors[ColorEnum::TEXT_DARK]);
					ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::TEXT_DARK]);
					ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
					if (ImGui::BeginPopup(("mypicker" + std::to_string(i)).c_str()))
					{
						ImGui::Text("COLOR PICKER");
						ImGui::Separator();
						ImGui::ColorPicker4("##picker", (float*)&im0stat.color[i], misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
						ImGui::SameLine();
						ImGui::BeginGroup();
						ImGui::Text("Current");
						ImGui::ColorButton("##current", im0stat.color[i], ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
						ImGui::Text("Previous");
						if (ImGui::ColorButton("##previous", im0stat.backup_color[i], ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
							im0stat.color[i] = im0stat.backup_color[i];
						ImGui::Separator();
						ImGui::Text("Palette");
						for (int n = 0; n < IM_ARRAYSIZE(im0stat.saved_palette); n++)
						{
							ImGui::PushID(n);
							if ((n % 8) != 0)
								ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
							if (ImGui::ColorButton("##palette", im0stat.saved_palette[n], ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)))
								im0stat.color[i] = ImVec4(im0stat.saved_palette[n].x, im0stat.saved_palette[n].y, im0stat.saved_palette[n].z, im0stat.color[i].w); // Preserve alpha!
							ImGui::PopID();
						}
						ImGui::EndGroup();
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor(3);
					ImGui::NewLine();

				}
				RegularModify::CursorPos(20);
				if (ImGui::Button("Add new", ImVec2(60, 30))) {
					im0stat.color.push_back(ImColor(255, 255, 255, 255));
					im0stat.backup_color.push_back(ImColor(255, 255, 255, 255));
				}

				ImGui::NewLine();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			ImGui::BeginChild("Buttons_StaticNoise", ImVec2(max(im0stat.staticnoise.getSurface()->w, 400), 80)); {

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##StaticNoise", ImVec2(150, 50))) {
					SDL_Surface* source = im0stat.staticnoise.getSurface();
					SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
						source->w, source->h, source->format->BitsPerPixel, source->format->format);
					if (destination != nullptr) {
						SDL_BlitSurface(source, nullptr, destination, nullptr);
					}
					Image imseged;
					imseged.setSurface(destination);
					imseged.textureFromSurface();

					ImageFolder imfseged;
					imfseged.iof = iofImage;
					imfseged.im = imseged;

					imfVec.push_back(imfseged);

					//StoredOperaionsClass s;
					//storedOperationsVector.push_back(s);
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im0stat.staticnoise.getSurface()->w - 170));
				Back();
				imFo->Scale = 1.f;
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::MAGNIFYENUM: //------------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Magnify", ImVec2(max(im1mag.imOut.getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Magnify");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();
			ImGui::NewLine();

			im1mag.editableDrawImage(imfVec[selectedImFVec[0]].iof==iofImage ? imfVec[selectedImFVec[0]].im : imfVec[selectedImFVec[0]].f.images[0]);

			ImGui::NewLine();

			ImGui::BeginChild("Values_Magnify", ImVec2(max(im1mag.imOut.getSurface()->w, 400), 180), false); {

				/*imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(1 * fmax(im1mag.imOut.getSurface()->w, 400) / 4 - 70);

				if (im1mag.getSmallChange()) {
					ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREEN]);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREEN_HOVERED]);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_GREEN_ACTIVE]);
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREY]);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREY_HOVERED]);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_GREY_ACTIVE]);
				}
				if (ImGui::Button("Small Rectangle", ImVec2(140, 40))) {
					im1mag.setSmallChange(true);
				}
				ImGui::PopStyleColor(3);
				imFo->Scale = 1.f;
				ImGui::PopFont();
				RegularModify::ShowHelpMarker("When selected, you can adjust the position of the area that will be magnified.");

				ImGui::SameLine();
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				RegularModify::CursorPos(3 * fmax(im1mag.imOut.getSurface()->w, 400) / 4 - 70);
				if (!im1mag.getSmallChange()) {
					ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREEN]);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREEN_HOVERED]);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_GREEN_ACTIVE]);
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREY]);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREY_HOVERED]);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_GREY_ACTIVE]);
				}
				if (ImGui::Button("Big Rectangle", ImVec2(140, 40))) {
					im1mag.setSmallChange(false);
				}
				ImGui::PopStyleColor(3);
				imFo->Scale = 1.f;
				ImGui::PopFont();

				RegularModify::ShowHelpMarker("When selected, you can adjust the position of the magnified area.");
				*/

				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Width:");
				ImGui::SameLine();
				RegularModify::CursorPos(270);
				ImGui::PushItemWidth(fmax(im1mag.imOut.getSurface()->w, 400) - 290);
				int segedZoomW = im1mag.getZoomW();
				if (ImGui::InputInt("##ZoomWidth", &segedZoomW, 0)) {
					if (segedZoomW < 1) {
						segedZoomW = 1;
					}
					if (segedZoomW > im1mag.imOut.getSurface()->w / (im1mag.getZoomTimes() + 1)) {
						segedZoomW = im1mag.imOut.getSurface()->w / (im1mag.getZoomTimes() + 1);
					}

					im1mag.setZoomW(segedZoomW);
				}
				ImGui::PopItemWidth();
				RegularModify::CursorPos(20); ImGui::Text("Heiht:");
				ImGui::SameLine();
				RegularModify::CursorPos(270);
				ImGui::PushItemWidth(fmax(im1mag.imOut.getSurface()->w, 400) - 290);
				int segedZoomH = im1mag.getZoomH();
				if (ImGui::InputInt("##ZoomHeight", &segedZoomH, 0)) {
					if (segedZoomH < 1) {
						segedZoomH = 1;
					}
					if (segedZoomH > im1mag.imOut.getSurface()->h / (im1mag.getZoomTimes() + 1)) {
						segedZoomH = im1mag.imOut.getSurface()->h / (im1mag.getZoomTimes() + 1);
					}

					im1mag.setZoomH(segedZoomH);
				}
				ImGui::PopItemWidth();

				RegularModify::CursorPos(20); ImGui::Text("Magnification level:");
				ImGui::SameLine();
				RegularModify::CursorPos(200);
				ImGui::PushItemWidth(fmax(im1mag.imOut.getSurface()->w, 400) - 220);
				float segedZoomTimes = im1mag.getZoomTimes();
				if (ImGui::SliderFloat("##ZoomTimes", &segedZoomTimes, 1.0f, 10.0f, "%.4f")) {
					im1mag.setZoomTimes(segedZoomTimes);
					if (segedZoomW > im1mag.imOut.getSurface()->w / (im1mag.getZoomTimes() + 1)) {
						segedZoomW = im1mag.imOut.getSurface()->w / (im1mag.getZoomTimes() + 1);
						im1mag.setZoomW(segedZoomW);
					}
					if (segedZoomH > im1mag.imOut.getSurface()->h / (im1mag.getZoomTimes() + 1)) {
						segedZoomH = im1mag.imOut.getSurface()->h / (im1mag.getZoomTimes() + 1);
						im1mag.setZoomH(segedZoomH);
					}

				}
				RegularModify::CursorPos(200);
				if (ImGui::InputFloat("##ZoomTimes2", &segedZoomTimes, 0)) {
					if(segedZoomTimes >= 1 && segedZoomTimes <= 10 ){
						im1mag.setZoomTimes(segedZoomTimes);
						if (segedZoomW > im1mag.imOut.getSurface()->w / (im1mag.getZoomTimes() + 1)) {
							segedZoomW = im1mag.imOut.getSurface()->w / (im1mag.getZoomTimes() + 1);
							im1mag.setZoomW(segedZoomW);
						}
						if (segedZoomH > im1mag.imOut.getSurface()->h / (im1mag.getZoomTimes() + 1)) {
							segedZoomH = im1mag.imOut.getSurface()->h / (im1mag.getZoomTimes() + 1);
							im1mag.setZoomH(segedZoomH);
						}
					}
				}

				ImGui::PopItemWidth();
			}
			ImGui::EndChild();
			ImGui::NewLine();

			//--------button

			ImGui::BeginChild("Buttons_Magnify", ImVec2(max(im1mag.imOut.getSurface()->w, 400), 80)); {
				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##Magnify", ImVec2(150, 50))) {

					//image
					if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {

						SDL_Surface* source = im1mag.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();

						ImageFolder imfseged;
						imfseged.iof = iofImage;
						imfseged.im = imseged;

						imfVec.push_back(imfseged);

						//StoredOperaionsClass s;
						//storedOperationsVector.push_back(s);

						/*//+ exit on load*/

					}

					//folder
					else {
						Folder segedFolder;
						//StoredOperaionsClass segedStoredOperationsClass;
						//int offset = 0;
						for (int i = 0; i < selectedImFVec.size(); i++) {
							if (imfVec[selectedImFVec[i]].iof == iofImage) {
								segedFolder.Append(imfVec[selectedImFVec[i]].im);
								//offset++;
							}
							else if (imfVec[selectedImFVec[i]].iof == iofFolder) {
								segedFolder.Append(imfVec[selectedImFVec[i]].f);
								//segedStoredOperationsClass += storedOperationsVector[i];

								/*for (int j = 0; j < storedOperationsVector[i].storedOperationsElement.size(); j++) {
									segedStoredOperationsClass.storedOperationsElement.push_back(storedOperationsVector[i].storedOperationsElement[j]);

									for (int k = 0; k < storedOperationsVector[i].storedOperationsElement[j].affectedElements.size(); k++) {
										segedStoredOperationsClass.storedOperationsElement[j].affectedElements[k] += offset;
									}
								}
								offset += imfVec[selectedImFVec[i]].f.images.size();*/
							}
						}

						Folder savedFolder;

						for (int i = 0; i < segedFolder.images.size(); i++) {

							if (segedFolder.images[i].getSurface()->w >= segedFolder.images[0].getSurface()->w &&
								segedFolder.images[i].getSurface()->h >= segedFolder.images[0].getSurface()->h) {

								im1mag.setImage(segedFolder.images[i]);
								im1mag.MagnifyMethod(segedFolder.images[i]);

								SDL_Surface* source = im1mag.imOut.getSurface();
								SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
									source->w, source->h, source->format->BitsPerPixel, source->format->format);
								if (destination != nullptr) {
									SDL_BlitSurface(source, nullptr, destination, nullptr);
								}

								Image imseged;
								imseged.setSurface(destination);
								imseged.textureFromSurface();

								savedFolder.images.push_back(imseged);
							}
						}

						savedFolder.createIconImageFromImages();

						/*SDL_Surface* source = im1mag.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();
						segedFolder.images[0] = imseged;
						segedFolder.createIconImageFromImages();

						StoredOperaionsClass::storedOperation segedOp;
						segedOp.ote = StoredOperaionsClass::oteImage1Magnify;
						segedOp.i1m = im1mag;
						for (int i = 0; i < segedFolder.images.size(); i++) {
							segedOp.affectedElements.push_back(i);
						}
						segedStoredOperationsClass.storedOperationsElement.push_back(segedOp);
						storedOperationsVector.push_back(segedStoredOperationsClass);*/

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						imfseged.f = savedFolder;//segedFolder;

						imfVec.push_back(imfseged);

						im1mag.setImage(segedFolder.images[0]);
						im1mag.MagnifyMethod(segedFolder.images[0]);
					}
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im1mag.imOut.getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::BLURENUM: {//---------------------------------------------------------------------------------------------------
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Blur", ImVec2(max(im1blur.imOut.getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Blur");
				imFo->Scale = 1.f;	ImGui::PopFont();

			}

			ImGui::EndChild();

			ImGui::NewLine();
			ImGui::Image((void*)(intptr_t)im1blur.imOut.getTexture(), ImVec2(im1blur.imOut.getSurface()->w, im1blur.imOut.getSurface()->h));
			ImGui::NewLine();

			ImGui::BeginChild("Values_Blur", ImVec2(max(im1blur.imOut.getSurface()->w, 400), 200), false); {

				ImGui::NewLine();


				ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_BLUE]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_BLUE_HOVERED]);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_BLUE_ACTIVE]);
				ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				RegularModify::CursorPos((max(im1blur.imOut.getSurface()->w, 400) / 2.0f) - (150.0f / 2.0f));

				if (ImGui::Button("Regenerate##Blur", ImVec2(150, 50))) {

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].f.images[0]);
					}
				}

				ImGui::PopStyleColor(4);
				imFo->Scale = 1.f; ImGui::PopFont();
				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Blur's type: "); ImGui::NewLine();
				RegularModify::CursorPos(20);

				char* segedBlurRadioNames[] = { "Box","Gaussian" };
				int segedBlurType = im1blur.blurType;
				for (int i = 0; i < sizeof(segedBlurRadioNames) / sizeof(char*); i++) {
					ImGui::SameLine();
					if (ImGui::RadioButton(segedBlurRadioNames[i], &segedBlurType, i)) {
						im1blur.blurType = segedBlurType;
						if (im1blur.blurSize > 9 && im1blur.blurType == 1) {
							im1blur.blurSize = 9;
						}
						if (im1blur.blurType == 1 && im1blur.blurSize % 2 == 0) {
							im1blur.blurSize--;
						}
					}
				}

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("The size of the Blur slices:");
				ImGui::SameLine();
				RegularModify::CursorPos(220);
				ImGui::PushItemWidth(fmax(im1blur.imOut.getSurface()->w, 400) - 240);

				int segedBlurSize = im1blur.blurSize;
				if (ImGui::InputInt("##BlurSize", &segedBlurSize, 0)) {
					if (segedBlurSize < 1) {
						segedBlurSize = 1;
					}
					if (segedBlurSize > fmin(im1blur.imOut.getSurface()->w / 2, im1blur.imOut.getSurface()->h / 2)) {
						segedBlurSize = fmin(im1blur.imOut.getSurface()->w / 2, im1blur.imOut.getSurface()->h / 2);
					}

					if (segedBlurSize > 9 && im1blur.blurType == 1) {
						segedBlurSize = 9;
					}
					if (segedBlurSize % 2 == 0 && im1blur.blurType == 1) {
						segedBlurSize--;
					}
					im1blur.blurSize = segedBlurSize;
				}
				//RegularModify::ShowHelpMarker("What times what should be the size of the slices that the SSIM algorithm uses");

				ImGui::NewLine();
			}

			ImGui::EndChild();
			ImGui::NewLine();

			//--------button

			ImGui::BeginChild("Buttons_Blur", ImVec2(max(im1blur.imOut.getSurface()->w, 400), 80)); {

				ImGui::NewLine();

				RegularModify::CursorPos(20);

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);

				PushStyleColorGreenButton();

				if (ImGui::Button("Load##Blur", ImVec2(150, 50))) {

					//image
					if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {

						SDL_Surface* source = im1blur.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();

						ImageFolder imfseged;
						imfseged.iof = iofImage;
						imfseged.im = imseged;

						imfVec.push_back(imfseged);

						//StoredOperaionsClass s;
						//storedOperationsVector.push_back(s);

					}

					//folder
					else {
						Folder segedFolder;
						//StoredOperaionsClass segedStoredOperationsClass;
						//int offset = 0;
						for (int i = 0; i < selectedImFVec.size(); i++) {
							if (imfVec[selectedImFVec[i]].iof == iofImage) {
								segedFolder.Append(imfVec[selectedImFVec[i]].im);
								//offset++;
							}
							else if (imfVec[selectedImFVec[i]].iof == iofFolder) {
								segedFolder.Append(imfVec[selectedImFVec[i]].f);
								//segedStoredOperationsClass += storedOperationsVector[i];

								/*for (int j = 0; j < storedOperationsVector[i].storedOperationsElement.size(); j++) {
									segedStoredOperationsClass.storedOperationsElement.push_back(storedOperationsVector[i].storedOperationsElement[j]);

									for (int k = 0; k < storedOperationsVector[i].storedOperationsElement[j].affectedElements.size(); k++) {
										segedStoredOperationsClass.storedOperationsElement[j].affectedElements[k] += offset;
									}
								}
								offset += imfVec[selectedImFVec[i]].f.images.size();*/
							}
						}

						Folder savedFolder;

						for (int i = 0; i < segedFolder.images.size(); i++) {

							im1blur.setImage(segedFolder.images[i]);
							im1blur.BlurMethod(segedFolder.images[i]);

							SDL_Surface* source = im1blur.imOut.getSurface();
							SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
								source->w, source->h, source->format->BitsPerPixel, source->format->format);
							if (destination != nullptr) {
								SDL_BlitSurface(source, nullptr, destination, nullptr);
							}

							Image imseged;
							imseged.setSurface(destination);
							imseged.textureFromSurface();

							savedFolder.images.push_back(imseged);

						}

						savedFolder.createIconImageFromImages();


						/*SDL_Surface* source = im1blur.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();
						segedFolder.images[0] = imseged;
						segedFolder.createIconImageFromImages();

						StoredOperaionsClass::storedOperation segedOp;
						segedOp.ote = StoredOperaionsClass::oteImage1Blur;
						segedOp.i1b = im1blur;
						for (int i = 0; i < segedFolder.images.size(); i++) {
							segedOp.affectedElements.push_back(i);
						}
						segedStoredOperationsClass.storedOperationsElement.push_back(segedOp);
						storedOperationsVector.push_back(segedStoredOperationsClass);*/

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						imfseged.f = savedFolder;

						imfVec.push_back(imfseged);

						im1blur.setImage(segedFolder.images[0]);
						im1blur.BlurMethod(segedFolder.images[0]);
					}
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im1blur.imOut.getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::COLORENUM: {//---------------------------------------------------------------------------------------------------
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Color", ImVec2(max(im1col.imOut.getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Modify image's color");
				imFo->Scale = 1.f;	ImGui::PopFont();

			}

			ImGui::EndChild();

			ImGui::NewLine();
			ImGui::Image((void*)(intptr_t)im1col.imOut.getTexture(), ImVec2(im1col.imOut.getSurface()->w, im1col.imOut.getSurface()->h));
			ImGui::NewLine();

			ImGui::BeginChild("Color_Values", ImVec2(max(im1col.imOut.getSurface()->w, 400), 75), false); {

				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Color modificatin type: "); ImGui::NewLine();
				RegularModify::CursorPos(20);

				char* segedColorRadioNames[] = { "Null","GreyScale","Red","Green","Blue","Invert"};
				int segedColorType = im1col.imctype;
				for (int i = 0; i < sizeof(segedColorRadioNames) / sizeof(char*); i++) {
					ImGui::SameLine();
					if (ImGui::RadioButton(segedColorRadioNames[i], &segedColorType, i)) {
						im1col.imctype = segedColorType;
						if (imfVec[selectedImFVec[0]].iof == iofImage) {
							im1col.ColorMethod(imfVec[selectedImFVec[0]].im);
						}
						else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							im1col.ColorMethod(imfVec[selectedImFVec[0]].f.images[0]);
						}
					}
				}

			}
			ImGui::EndChild();
			ImGui::NewLine();

			//--------button

			ImGui::BeginChild("Buttons_Color", ImVec2(max(im1col.imOut.getSurface()->w, 400), 80)); {

				ImGui::NewLine();

				RegularModify::CursorPos(20);

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);

				PushStyleColorGreenButton();

				if (ImGui::Button("Load##Color", ImVec2(150, 50))) {

					//image
					if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {

						SDL_Surface* source = im1col.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();

						ImageFolder imfseged;
						imfseged.iof = iofImage;
						imfseged.im = imseged;

						imfVec.push_back(imfseged);

						//StoredOperaionsClass s;
						//storedOperationsVector.push_back(s);

					}

					//folder
					else {
						Folder segedFolder;
						//StoredOperaionsClass segedStoredOperationsClass;
						//int offset = 0;
						for (int i = 0; i < selectedImFVec.size(); i++) {
							if (imfVec[selectedImFVec[i]].iof == iofImage) {
								segedFolder.Append(imfVec[selectedImFVec[i]].im);
								//offset++;
							}
							else if (imfVec[selectedImFVec[i]].iof == iofFolder) {
								segedFolder.Append(imfVec[selectedImFVec[i]].f);
								//segedStoredOperationsClass += storedOperationsVector[i];

								/*for (int j = 0; j < storedOperationsVector[i].storedOperationsElement.size(); j++) {
									segedStoredOperationsClass.storedOperationsElement.push_back(storedOperationsVector[i].storedOperationsElement[j]);

									for (int k = 0; k < storedOperationsVector[i].storedOperationsElement[j].affectedElements.size(); k++) {
										segedStoredOperationsClass.storedOperationsElement[j].affectedElements[k] += offset;
									}
								}
								offset += imfVec[selectedImFVec[i]].f.images.size();*/
							}
						}

						Folder savedFolder;

						for (int i = 0; i < segedFolder.images.size(); i++) {

								im1col.setImage(segedFolder.images[i]);
								im1col.ColorMethod(segedFolder.images[i]);

								SDL_Surface* source = im1col.imOut.getSurface();
								SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
									source->w, source->h, source->format->BitsPerPixel, source->format->format);
								if (destination != nullptr) {
									SDL_BlitSurface(source, nullptr, destination, nullptr);
								}

								Image imseged;
								imseged.setSurface(destination);
								imseged.textureFromSurface();

								savedFolder.images.push_back(imseged);
						}

						savedFolder.createIconImageFromImages();


						/*SDL_Surface* source = im1col.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();
						segedFolder.images[0] = imseged;
						segedFolder.createIconImageFromImages();

						StoredOperaionsClass::storedOperation segedOp;
						segedOp.ote = StoredOperaionsClass::oteImage1Color;
						segedOp.i1c = im1col;
						for (int i = 0; i < segedFolder.images.size(); i++) {
							segedOp.affectedElements.push_back(i);
						}
						segedStoredOperationsClass.storedOperationsElement.push_back(segedOp);
						storedOperationsVector.push_back(segedStoredOperationsClass);*/

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						imfseged.f = savedFolder;

						imfVec.push_back(imfseged);

						im1col.setImage(segedFolder.images[0]);
						im1col.ColorMethod(segedFolder.images[0]);
					}					
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im1col.imOut.getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::SAVEENUM: //-----------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Save", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Save");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::NewLine();
			if (imfVec[selectedImFVec[0]].iof == iofImage) {
				imfVec[selectedImFVec[0]].im.drawImage();
				//ImGui::Image((void*)(intptr_t)imfVec[selectedImFVec[0]].im.getTexture(), ImVec2(imfVec[selectedImFVec[0]].im.getSurface()->w, imfVec[selectedImFVec[0]].im.getSurface()->h));
			}
			else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
				imfVec[selectedImFVec[0]].f.images[0].drawImage();
				//ImGui::Image((void*)(intptr_t)imfVec[selectedImFVec[0]].f.images[0].getTexture(), ImVec2(imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, imfVec[selectedImFVec[0]].f.images[0].getSurface()->h));
			}
			ImGui::NewLine();

			ImGui::BeginChild("Values_Save", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, 400), 75), false); {

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Path:");
				ImGui::PushItemWidth(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w - 40 : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w - 40, 360));
				RegularModify::CursorPos(20); ImGui::InputText("##SavePath", outstr, IM_ARRAYSIZE(outstr));
				ImGui::PopItemWidth();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			ImGui::BeginChild("Buttons_Save", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, 400), 80)); {

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Save", ImVec2(150, 50))) {

					struct stat sb;
					//available path (there isn't a file with this name already)
					if (stat(outstr, &sb) != 0) {
						if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {
							if (im1sav.Save(imfVec[selectedImFVec[0]].im,outstr)) { 
								currentImageEnum = OPERATIONSENUM;
							}
							else {
								ImGui::OpenPopup("Save##Error");
							}
						}
						else {
							ImGui::OpenPopup("Save##InvalidPath");
						}
					}
					//folder path  !!!!!!!!!!!!!!!!!!!!!!!!!!check if name isnt already taken
					else if (sb.st_mode & S_IFDIR) {
						if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {
							ImGui::OpenPopup("Save##InvalidPath");
						}
						else {
							int n = 1;
							std::string path = outstr;

							for (int i = 0; i < selectedImFVec.size(); i++) {
								if (imfVec[selectedImFVec[i]].iof == iofImage) {

									std::string seged = path + "/image" + std::to_string(n) + ".png";
									char* cstr = new char[seged.size() + 1];
									std::strcpy(cstr, seged.c_str());

									im1sav.Save(imfVec[selectedImFVec[i]].im, cstr); //if

									delete[] cstr;
									n++;
								}
								else if (imfVec[selectedImFVec[i]].iof == iofFolder) {

									im1sav.SaveFolder(imfVec[selectedImFVec[i]].f, path,n); //if

								}
							}
							currentImageEnum = OPERATIONSENUM;
						}
					}
					//already existing image path
					else {
						ImGui::OpenPopup("Save##ImageOverritePath");
					}
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w - 170 : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f; ImGui::PopFont();
				ImGui::PopStyleColor(3);

				if (ImGui::BeginPopupModal("Save##InvalidPath", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Invalid path.");
					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Save##ImageOverritePath", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("A file already exist with this name.");
					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopupModal("Save##Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("An error occured while saving.");
					if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::SSIMENUM: //-------------------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);
			ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_BLUE]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_BLUE_HOVERED]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_BLUE_ACTIVE]);

			ImGui::BeginChild("Name_SSIM", ImVec2(max(im2ssim.imOut.getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("SSIM");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();
			ImGui::NewLine();

			im2ssim.editableDrawImage(); //?

			ImGui::NewLine();

			ImGui::BeginChild("Values_SSIM", ImVec2(max(im2ssim.imOut.getSurface()->w, 400), 210), false); {

				ImGui::NewLine();
				float segedSsimOsszeg = im2ssim.getSsimOsszeg();
				RegularModify::CursorPos(20); ImGui::Text("Overall SSIM value: %f", segedSsimOsszeg);
				RegularModify::ShowHelpMarker("The average value of the SSIM algorithm on the two image.");

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("SSIM's color/style: "); ImGui::NewLine();
				RegularModify::CursorPos(20);

				char* segedSsimRadioNames[] = { "GreyScale","Blue","Green","Red","All color","Heatmap" };
				int segedSsimColor = im2ssim.getSsimColor();
				for (int i = 0; i < sizeof(segedSsimRadioNames) / sizeof(char*); i++) {
					ImGui::SameLine();
					if (ImGui::RadioButton(segedSsimRadioNames[i], &segedSsimColor, i)) {
						im2ssim.setSsimColor(segedSsimColor);
					}
				}

				//RegularModify::ShowHelpMarker("Here you can select the color/style of the SSIM.");

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("The size of the SSIM slices:");
				ImGui::SameLine();
				RegularModify::CursorPos(max(im2ssim.imOut.getSurface()->w, 400) - 120);
				ImGui::PushItemWidth(80);
				int segedSsimSize = im2ssim.getSsimSize();
				if (ImGui::InputInt("##Size", &segedSsimSize, 0)) {
					if (segedSsimSize < 1) {
						segedSsimSize = 1;
					}
					if (segedSsimSize > fmin(im2ssim.imOut.getSurface()->w / 2, im2ssim.imOut.getSurface()->h / 2)) {
						segedSsimSize = fmin(im2ssim.imOut.getSurface()->w / 2, im2ssim.imOut.getSurface()->h / 2);
					}

					im2ssim.setSsimSize(segedSsimSize);
				}
				RegularModify::ShowHelpMarker("What should be the size of the slices that the SSIM algorithm uses");

				ImGui::NewLine();
				ImGui::PopItemWidth();

				ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
				RegularModify::CursorPos((max(im2ssim.imOut.getSurface()->w, 400) / 2.0f) - (150.0f / 2.0f));
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Regenerate##SSIM", ImVec2(150, 50))) {
					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						if (imfVec[selectedImFVec[1]].iof == iofImage) {
							im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].im);
						}
						else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
							im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.images[0]);
						}
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						if (imfVec[selectedImFVec[1]].iof == iofImage) {
							im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].im);
						}
						else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
							im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].f.images[0]);
						}
					}
				}
				imFo->Scale = 1.f;
				ImGui::PopStyleColor();
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			//--------button

			ImGui::BeginChild("Buttons_SSIM", ImVec2(max(im2ssim.imOut.getSurface()->w, 400), 80)); {
				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);

				if (ImGui::Button("Load##SSIM", ImVec2(150, 50))) {

					//images
					if (imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofImage) {

						SDL_Surface* source = im2ssim.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();

						ImageFolder imfseged;
						imfseged.iof = iofImage;
						imfseged.im = imseged;

						imfVec.push_back(imfseged);

						//StoredOperaionsClass s;
						//storedOperationsVector.push_back(s);

					}

					//folder (+image/folder)
					else {

						Folder segedFolder;
						//StoredOperaionsClass segedStoredOperationsClass;

						if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							segedFolder.Append(imfVec[selectedImFVec[0]].f);
							//segedStoredOperationsClass = storedOperationsVector[selectedImFVec[0]];
						}
						else if (imfVec[selectedImFVec[0]].iof == iofImage) {
							segedFolder.Append(imfVec[selectedImFVec[1]].f);
							//segedStoredOperationsClass = storedOperationsVector[selectedImFVec[1]];
						}

						/*SDL_Surface* source = im2ssim.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();
						segedFolder.images[0] = imseged;
						segedFolder.createIconImageFromImages();*/

						Folder savedFolder;

						//folder + image
						if ((imfVec[selectedImFVec[0]].iof == iofImage) || (imfVec[selectedImFVec[1]].iof == iofImage)) {
							Image* imPointer;
							Folder* fPointer;
							if (imfVec[selectedImFVec[0]].iof == iofImage) {
								imPointer = &imfVec[selectedImFVec[0]].im;
								fPointer = &imfVec[selectedImFVec[1]].f;
							}
							else {
								imPointer = &imfVec[selectedImFVec[1]].im;
								fPointer = &imfVec[selectedImFVec[0]].f;
							}

							for (int i = 0; i < fPointer->images.size(); i++) {
								if (fPointer->images[i].getSurface()->w == imPointer->getSurface()->w && fPointer->images[i].getSurface()->h == imPointer->getSurface()->h) {

									im2ssim.setImage(*imPointer);
									im2ssim.SSIMSurface(*imPointer, fPointer->images[i]);
									
									SDL_Surface* source = im2ssim.imOut.getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.images.push_back(imseged);
								}
							}

							im2ssim.setImage(segedFolder.images[0]);
							if (imfVec[selectedImFVec[0]].iof == iofImage) {
								im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.images[0]);
							}
							else {
								im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].im);
							}
						}

						//folders
						else {

							for (int i = 0; i < imfVec[selectedImFVec[0]].f.images.size() <= imfVec[selectedImFVec[1]].f.images.size() ? imfVec[selectedImFVec[0]].f.images.size() : imfVec[selectedImFVec[1]].f.images.size(); i++) {
								if (imfVec[selectedImFVec[0]].f.images[i].getSurface()->w == imfVec[selectedImFVec[1]].f.images[i].getSurface()->w && imfVec[selectedImFVec[0]].f.images[i].getSurface()->h == imfVec[selectedImFVec[1]].f.images[i].getSurface()->h) {

									im2ssim.setImage(imfVec[selectedImFVec[0]].f.images[i]);
									im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].f.images[i], imfVec[selectedImFVec[1]].f.images[i]);

									SDL_Surface* source = im2ssim.imOut.getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.images.push_back(imseged);
								}
							}

							im2ssim.setImage(segedFolder.images[0]);
							im2ssim.SSIMSurface(imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].f.images[0]);
						}

						/*StoredOperaionsClass::storedOperation segedOp;
						segedOp.ote = StoredOperaionsClass::oteImage2SSIM;
						segedOp.i2s = im2ssim;
						for (int i = 0; i < segedFolder.images.size(); i++) {
							segedOp.affectedElements.push_back(i);
						}
						segedStoredOperationsClass.storedOperationsElement.push_back(segedOp);
						storedOperationsVector.push_back(segedStoredOperationsClass);*/

						ImageFolder imfseged; 
						imfseged.iof == iofFolder;
						imfseged.f = segedFolder;

						imfVec.push_back(imfseged);

					}
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im2ssim.imOut.getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
				ImGui::PopStyleColor(3);
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(9);

			break;
		}

		case CMyApp::MERGEENUM: //-----------------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);
			
			ImGui::BeginChild("Name_Merge", ImVec2(max(im2merge.imOut.getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Merge");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			Image* im1;
			Image* im2;
			if (imfVec[selectedImFVec[0]].iof == iofImage) {
				if (imfVec[selectedImFVec[1]].iof == iofImage) {
					im1 = &imfVec[selectedImFVec[0]].im;
					im2 = &imfVec[selectedImFVec[1]].im;
				}
				else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
					im1 = &imfVec[selectedImFVec[0]].im;
					im2 = &imfVec[selectedImFVec[1]].f.images[0];
				}
			}
			else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
				if (imfVec[selectedImFVec[1]].iof == iofImage) {
					im1 = &imfVec[selectedImFVec[0]].f.images[0];
					im2 = &imfVec[selectedImFVec[1]].im;
				}
				else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
					im1 = &imfVec[selectedImFVec[0]].f.images[0];
					im2 = &imfVec[selectedImFVec[1]].f.images[0];
				}
			}

			im2merge.editableDrawImage(*im1, *im2);

			ImGui::NewLine();

			ImGui::BeginChild("Values_Merge", ImVec2(max(im2merge.imOut.getSurface()->w, 400), 70), false); {

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("The slope of the transition:");
				ImGui::SameLine();
				RegularModify::CursorPos(200);
				ImGui::PushItemWidth(max(im2merge.imOut.getSurface()->w, 400) - 220);
				float segedSlope = im2merge.getSlope();
				if (ImGui::SliderFloat("##slope", &segedSlope, 0.0f, 360.0f, "%.4f")) {
					im2merge.setSlope(segedSlope);
					im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), *im1, *im2);
				}
				RegularModify::CursorPos(200);
				if (ImGui::InputFloat("##slope2", &segedSlope, 0)) {
					if (segedSlope < 0.0f) {
						segedSlope = 0.0f;
					}
					if (segedSlope > 360.0f) {
						segedSlope = 360.0f;
					}
					im2merge.setSlope(segedSlope);
					im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), *im1, *im2);
				}
				ImGui::PopItemWidth();
			}
			ImGui::EndChild();
			ImGui::NewLine();

			//--------button

			ImGui::BeginChild("Buttons_Merge", ImVec2(max(im2merge.imOut.getSurface()->w, 400), 80)); {
				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##Merge", ImVec2(150, 50))) {

					//images
					if (imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofImage) {

						SDL_Surface* source = im2merge.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();

						ImageFolder imfseged;
						imfseged.iof = iofImage;
						imfseged.im = imseged;

						imfVec.push_back(imfseged);

						//StoredOperaionsClass s;
						//storedOperationsVector.push_back(s);

					}

					//folder (+image/folder)
					else {

						Folder segedFolder;
						//StoredOperaionsClass segedStoredOperationsClass;

						if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							segedFolder.Append(imfVec[selectedImFVec[0]].f);
							//segedStoredOperationsClass = storedOperationsVector[selectedImFVec[0]];
						}
						else if (imfVec[selectedImFVec[0]].iof == iofImage) {
							segedFolder.Append(imfVec[selectedImFVec[1]].f);
							//segedStoredOperationsClass = storedOperationsVector[selectedImFVec[1]];
						}

						/*SDL_Surface* source = im2ssim.imOut.getSurface();
						SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
							source->w, source->h, source->format->BitsPerPixel, source->format->format);
						if (destination != nullptr) {
							SDL_BlitSurface(source, nullptr, destination, nullptr);
						}
						Image imseged;
						imseged.setSurface(destination);
						imseged.textureFromSurface();
						segedFolder.images[0] = imseged;
						segedFolder.createIconImageFromImages();*/

						Folder savedFolder;

						//folder + image
						if ((imfVec[selectedImFVec[0]].iof == iofImage) || (imfVec[selectedImFVec[1]].iof == iofImage)) {
							Image* imPointer;
							Folder* fPointer;
							if (imfVec[selectedImFVec[0]].iof == iofImage) {
								imPointer = &imfVec[selectedImFVec[0]].im;
								fPointer = &imfVec[selectedImFVec[1]].f;
							}
							else {
								imPointer = &imfVec[selectedImFVec[1]].im;
								fPointer = &imfVec[selectedImFVec[0]].f;
							}

							for (int i = 0; i < fPointer->images.size(); i++) {
								if (fPointer->images[i].getSurface()->w == imPointer->getSurface()->w && fPointer->images[i].getSurface()->h == imPointer->getSurface()->h) {

									im2merge.setImage(*imPointer);
									im2merge.plotLineMerge(im2merge.getUx(),im2merge.getUy(), *imPointer, fPointer->images[i]);

									SDL_Surface* source = im2merge.imOut.getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.images.push_back(imseged);
								}
							}

							im2merge.setImage(segedFolder.images[0]);
							if (imfVec[selectedImFVec[0]].iof == iofImage) {
								im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.images[0]);
							}
							else {
								im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].im);
							}
						}

						//folders
						else {

							for (int i = 0; i < imfVec[selectedImFVec[0]].f.images.size() <= imfVec[selectedImFVec[1]].f.images.size() ? imfVec[selectedImFVec[0]].f.images.size() : imfVec[selectedImFVec[1]].f.images.size(); i++) {
								if (imfVec[selectedImFVec[0]].f.images[i].getSurface()->w == imfVec[selectedImFVec[1]].f.images[i].getSurface()->w && imfVec[selectedImFVec[0]].f.images[i].getSurface()->h == imfVec[selectedImFVec[1]].f.images[i].getSurface()->h) {

									im2merge.setImage(imfVec[selectedImFVec[0]].f.images[i]);
									im2merge.plotLineMerge(im2merge.getUx(),im2merge.getUy(), imfVec[selectedImFVec[0]].f.images[i], imfVec[selectedImFVec[1]].f.images[i]);

									SDL_Surface* source = im2merge.imOut.getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.images.push_back(imseged);
								}
							}
							im2merge.setImage(segedFolder.images[0]);
							im2merge.plotLineMerge(im2merge.getUx(),im2merge.getUy(), imfVec[selectedImFVec[0]].f.images[0], imfVec[selectedImFVec[1]].f.images[0]);
						}

						/*StoredOperaionsClass::storedOperation segedOp;
						segedOp.ote = StoredOperaionsClass::oteImage2SSIM;
						segedOp.i2s = im2ssim;
						for (int i = 0; i < segedFolder.images.size(); i++) {
							segedOp.affectedElements.push_back(i);
						}
						segedStoredOperationsClass.storedOperationsElement.push_back(segedOp);
						storedOperationsVector.push_back(segedStoredOperationsClass);*/

						ImageFolder imfseged;
						imfseged.iof == iofFolder;
						imfseged.f = segedFolder;

						imfVec.push_back(imfseged);

					}
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im2merge.imOut.getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
				ImGui::PopStyleColor(3);
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		default:
		{
			Back();
			break;
		}
	}

	ImGui::NewLine();

	/*if (ImGui::BeginPopupModal("Load##Pop", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("The image has been loaded.");
		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Betoltes##Pop", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("The image has been loaded.");
		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Mentes##Pop", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("The image has been saved.");
		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}*/

	ImGui::End();
}

void CMyApp::Back() {
	ImFont* imFo = ImGui::GetFont();
	imFo->Scale = 1.3f;
	ImGui::PushFont(imFo);
	ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_RED]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_RED_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_RED_ACTIVE]);
	if (ImGui::Button("Back", ImVec2(150, 50))) {
		currentImageEnum = OPERATIONSENUM;
	}
	ImGui::PopStyleColor(3);
	imFo->Scale = 1.f;
	ImGui::PopFont();
}


void CMyApp::Resize(int _w, int _h) //??????????????????????????????????????????????????
{
	glViewport(0, 0, _w, _h);

}

void CMyApp::SetBasicUI() {

	ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors[ColorEnum::CHILD_BG]);
	ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_DARK]);
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, Colors[ColorEnum::TEXT_HINT]);
	ImGui::PushStyleColor(ImGuiCol_Border, Colors[ColorEnum::POPUP_BORDER]);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, Colors[ColorEnum::POPUP_BG]);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);
}

void CMyApp::PushStyleColorGreenButton() {

	ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREEN]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREEN_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_GREEN_ACTIVE]);
}

void CMyApp::TestMethod() {

	//1.1
	char* white = "White.jpg";

	im0load.setLoadType(Image0FromFile::PICTURE);
	ImageFolder whiteTest = im0load.Load(white);

	if (whiteTest.im.getSurface() != nullptr) {
		std::cout << "1.1 Success" << std::endl;
	}

	//1.2
	char* folder = "TestImages";
	bool success = true;

	im0load.setLoadType(Image0FromFile::FOLDER);
	ImageFolder folderTest = im0load.Load(folder);

	if (folderTest.f.images.size() == 0) {
		success == false;
	}
	for (Image i : folderTest.f.images) {
		if (i.getSurface() == nullptr) {
			success = false;
		}
	}

	if (success) {
		std::cout << "1.2 Success" << std::endl;
	}
	success = true;
	
	//1.3
	char* incorrect = "IncorrectPath.jpg";

	im0load.setLoadType(Image0FromFile::PICTURE);
	ImageFolder incorrectTest = im0load.Load(incorrect);

	if (incorrectTest.iof == iofEmpty) {
		std::cout << "1.3 Success" << std::endl;
	}

	//2
	ImColor white2 = ImColor(255, 255, 255, 255);
	ImColor black2 = ImColor(0, 0, 0, 255);

	im0stat.StaticMethod();
	if (im0stat.staticnoise.getSurface()->w != im0stat.width || im0stat.staticnoise.getSurface()->h != im0stat.height) {
		success = false;
	}

	for (int i = 0; i < im0stat.width; i++) {
		for (int j = 0; j < im0stat.height; j++) {
			if (SurfaceModify::GetColor(i, j, im0stat.staticnoise.getSurface()) != white2 && SurfaceModify::GetColor(i, j, im0stat.staticnoise.getSurface()) != black2  )   {
				success = false;
			}
		}
	}

	Image staticBlackWhite2 = im0stat.staticnoise;

	if (success) {
		std::cout << "2 Success" << std::endl;
	}
	success = true;

	//3
	ImColor red3 = ImColor(255, 0, 0, 255);

	im1mag.setImage(whiteTest.im);
	im1mag.MagnifyMethod(whiteTest.im);

	if (SurfaceModify::GetColor(0, 0, im1mag.imOut.getSurface()) == red3) {
		std::cout << "3 Success" << std::endl;
	}

	//4.1
	im1blur.blurSize = 5;
	im1blur.blurType = 0;
	im1blur.setImage(staticBlackWhite2);
	im1blur.BlurMethod(staticBlackWhite2);
	Uint8 r4, g4, b4;

	for (int i = 0; i < im1blur.imOut.getSurface()->w ; i++) {
		for (int j = 0; j < im1blur.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i,j, im1blur.imOut.getSurface()), im1blur.imOut.getSurface()->format, &r4, &g4, &b4);
			if (r4!=b4 || b4!=g4) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "4.1 Success" << std::endl;
	}
	success = true;

	//4.2
	//im1blur.blurSize = 5;
	im1blur.blurType = 1;
	//im1blur.setImage(staticBlackWhite2);
	im1blur.BlurMethod(staticBlackWhite2);
	//Uint8 r4, g4, b4;

	for (int i = 0; i < im1blur.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im1blur.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im1blur.imOut.getSurface()), im1blur.imOut.getSurface()->format, &r4, &g4, &b4);
			if (r4 != b4 || b4 != g4) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "4.2 Success" << std::endl;
	}
	success = true;

	//5.1
	im1col.imctype = im1col.ImageColorType::GreyScale;
	im1col.setImage(staticBlackWhite2);
	im1col.ColorMethod(staticBlackWhite2);

	for (int i = 0; i < im1col.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im1col.imOut.getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) != SurfaceModify::GetColor(i, j, staticBlackWhite2.getSurface())) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.1 Success" << std::endl;
	}
	success = true;

	//5.2
	im0stat.width = whiteTest.im.getSurface()->w;
	im0stat.height = whiteTest.im.getSurface()->h;

	im0stat.StaticMethod();
	Image staticBlackWhite5 = im0stat.staticnoise;

	im0stat.color[0] =  ImColor(255, 0, 0, 255);
	im0stat.StaticMethod();
	Image staticRedWhite5 = im0stat.staticnoise;

	//im1col.imctype = im1col.ImageColorType::GreyScale;
	im1col.setImage(staticRedWhite5);
	im1col.ColorMethod(staticRedWhite5);

	for (int i = 0; i < im1col.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im1col.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()), im1col.imOut.getSurface()->format, &r4, &g4, &b4);
			if (r4 != b4 || b4 != g4) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.2 Success" << std::endl;
	}
	success = true;

	//5.3.a
	ImColor red5 = ImColor(255, 0, 0, 255);
	im1col.imctype = im1col.ImageColorType::Red;
	im1col.setImage(whiteTest.im);
	im1col.ColorMethod(whiteTest.im);

	//std::cout << SurfaceModify::GetColor(5, 5, im1col.imOut.getSurface()) << "  " << red5 << std::endl;

	for (int i = 0; i < im1col.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im1col.imOut.getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) != red5) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.3.a Success" << std::endl;
	}
	success = true;

	//5.3.b
	ImColor green5 = ImColor(0, 255, 0, 255);
	im1col.imctype = im1col.ImageColorType::Green;
	//im1col.setImage(whiteTest.im);
	im1col.ColorMethod(whiteTest.im);
		
	//std::cout << SurfaceModify::GetColor(5, 5, im1col.imOut.getSurface()) << "  " << green5 << std::endl;

	for (int i = 0; i < im1col.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im1col.imOut.getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) != green5) {
				success = false;
				//std::cout << SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) << std::endl;
			}
		}
	}



	if (success) {
		std::cout << "5.3.b Success" << std::endl;
	}
	success = true;

	//5.3.c
	ImColor blue5 = ImColor(0, 0, 255, 255);
	im1col.imctype = im1col.Blue;
	//im1col.setImage(whiteTest.im);
	im1col.ColorMethod(whiteTest.im);

	//std::cout << SurfaceModify::GetColor(5, 5, im1col.imOut.getSurface()) << "  " << blue5 << std::endl;

	for (int i = 0; i < im1col.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im1col.imOut.getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) != blue5) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.3.c Success" << std::endl;
	}
	success = true;

	//5.4
	im1col.imctype = im1col.Inverted;
	im1col.setImage(staticBlackWhite2);
	im1col.ColorMethod(staticBlackWhite2);

	for (int i = 0; i < im1col.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im1col.imOut.getSurface()->h; j++) {

			if (!((SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) == white2 && SurfaceModify::GetColor(i, j, staticBlackWhite2.getSurface()) == black2) || (SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) == black2 && SurfaceModify::GetColor(i, j, staticBlackWhite2.getSurface()) == white2)) ||
				(SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) != white2 && SurfaceModify::GetColor(i, j, im1col.imOut.getSurface()) != black2)) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.4 Success" << std::endl;
	}
	success = true;

	//6.1
	im2ssim.setImage(staticRedWhite5);

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(0);

	im2ssim.SSIMSurface(staticRedWhite5, whiteTest.im);

	for (int i = 0; i < im2ssim.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()), im2ssim.imOut.getSurface()->format, &r4, &g4, &b4);
			if (r4 != b4 || b4 != g4 || (SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface())==white2 && SurfaceModify::GetColor(i, j, staticRedWhite5.getSurface())!=white2 )) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "6.1 Success" << std::endl;
	}
	success = true;

	//6.2.a
	//im2ssim.setImage(staticBlackWhite5);

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(1);

	im2ssim.SSIMSurface(staticBlackWhite5, whiteTest.im);

	for (int i = 0; i < im2ssim.imOut.getSurface()->w; i++) {
		//std::cout << SurfaceModify::GetColor(i, 1, staticBlackWhite5.getSurface()) << "  " << red5 << std::endl;
		for (int j = 0; j < im2ssim.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()), im2ssim.imOut.getSurface()->format, &r4, &g4, &b4);
			if (//(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white2 && SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()) != white2) ||
				(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white2 && (r4!=255))) {
				success = false;
				//std::cout << SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()) << std::endl;
			}
		}
	}

	if (success) {
		std::cout << "6.2.a Success" << std::endl;
	}
	success = true;

	//6.2.b
    //im2ssim.setImage(staticRedWhite5);

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(2);

	im2ssim.SSIMSurface(staticBlackWhite5, whiteTest.im);

	for (int i = 0; i < im2ssim.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()), im2ssim.imOut.getSurface()->format, &r4, &g4, &b4);
			if (//(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white2 && SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()) != white2) ||
				(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white2 && (g4 != 255))) {
				success = false;
				//std::cout << SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()) << std::endl;
			}
		}
	}

	if (success) {
		std::cout << "6.2.b Success" << std::endl;
	}
	success = true;

	//6.2.c
   //im2ssim.setImage(staticRedWhite5);

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(3);

	im2ssim.SSIMSurface(staticBlackWhite5, whiteTest.im);

	for (int i = 0; i < im2ssim.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()), im2ssim.imOut.getSurface()->format, &r4, &g4, &b4);
			if (//(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white2 && SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()) != white2) ||
				(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white2 && (b4 != 255))) {
				success = false;
				//std::cout << SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()) << std::endl;
			}
		}
	}

	if (success) {
		std::cout << "6.2.c Success" << std::endl;
	}
	success = true;

	//6.3
   //im2ssim.setImage(staticRedWhite5);

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(4);

	im2ssim.SSIMSurface(staticRedWhite5, whiteTest.im);

	for (int i = 0; i < im2ssim.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.imOut.getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()) != SurfaceModify::GetColor(i, j, staticRedWhite5.getSurface())) {
				success = false;
			}
			
		}
	}

	if (success) {
		std::cout << "6.3 Success" << std::endl;
	}
	success = true;

	//6.4
	//im2ssim.setImage(staticRedWhite5);

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(5);

	im2ssim.SSIMSurface(staticBlackWhite5, whiteTest.im);

	for (int i = 0; i < im2ssim.imOut.getSurface()->w; i++) {
		
		for (int j = 0; j < im2ssim.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()), im2ssim.imOut.getSurface()->format, &r4, &g4, &b4);
			if ((SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white2 && b4 - (g4 + r4) < 245) ||
				(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == black2 && r4 - (g4 + b4) < 245)) {
				success = false;
			}

		}
	}

	if (success) {
		std::cout << "6.4 Success" << std::endl;
	}
	success = true;

	//6.5
	//im2ssim.setImage(staticRedWhite5);

	im2ssim.setSsimSize(5);
	im2ssim.setSsimColor(0);

	im2ssim.SSIMSurface(staticBlackWhite5, whiteTest.im);

	for (int i = 0; i < im2ssim.imOut.getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.imOut.getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.imOut.getSurface()), im2ssim.imOut.getSurface()->format, &r4, &g4, &b4);
			if (r4 != b4 || b4 != g4) {
				success = false;
			}

		}
	}

	if (success) {
		std::cout << "6.5 Success" << std::endl;
	}
	success = true;

	//7.1
	char* blackchar = "Black.jpg";
	im0load.setLoadType(Image0FromFile::PICTURE);
	ImageFolder blackTest = im0load.Load(blackchar);

	im2merge.setImage(whiteTest.im);
	im2merge.setSlope(0);
	im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), blackTest.im, whiteTest.im);

	if (SurfaceModify::GetColor(0, 0, im2merge.imOut.getSurface()) != black2 || SurfaceModify::GetColor(im2merge.imOut.getSurface()->w-1, im2merge.imOut.getSurface()->h-1, im2merge.imOut.getSurface()) != white2) {
		success = false;
	}

	if (success) {
		std::cout << "7.1 Success" << std::endl;
	}
	success = true;

	//7.2
	//im2merge.setImage(whiteTest.im);
	im2merge.setSlope(180);
	im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), blackTest.im, whiteTest.im);

	if (SurfaceModify::GetColor(0, 0, im2merge.imOut.getSurface()) != white2 || SurfaceModify::GetColor(im2merge.imOut.getSurface()->w-1, im2merge.imOut.getSurface()->h-1, im2merge.imOut.getSurface()) != black2) {
		success = false;
	}

	if (success) {
		std::cout << "7.2 Success" << std::endl;
	}
	success = true;

	//8.1

	if (im1sav.Save(whiteTest.im, "SaveTest/save.png")) {
		std::cout << "8.1 Success" << std::endl;
	}

	//8.2

	if (im1sav.Save(whiteTest.im, "SaveTest") == false) {
		std::cout << "8.2 Success" << std::endl;
	}

	//8.3

	if (im1sav.Save(whiteTest.im, "NotValidPath/save.png") == false) {
		std::cout << "8.3 Success" << std::endl;
	}

	//8.4

	int n = 1;
	if (im1sav.SaveFolder(folderTest.f, "SaveTest",n)) {
		std::cout << "8.4 Success" << std::endl;
	}

	//8.5

	if (im1sav.SaveFolder(folderTest.f, "SaveTest/save2.png",n) == false) {
		std::cout << "8.5 Success" << std::endl;
	}

	//8.6

	if (im1sav.SaveFolder(folderTest.f, "NotValidPath", n) == false) {
		std::cout << "8.6 Success" << std::endl;
	}

} 

//but not only black-white

Image::~Image(void) {
	/*
	if (surface != nullptr) {
		SDL_FreeSurface(surface);
		surface = nullptr;
	}
	if (texture != 0) {
		glDeleteTextures(1, &texture);
		texture = 0;
	}
	printf("d");*/
}

void Image::setSurface(SDL_Surface* surface) {
	if (this->surface != nullptr) {
		//SDL_FreeSurface(this->surface);
		//this->surface = nullptr;
		printf("s");
	}
	this->surface = surface;
}

void Image::textureFromSurface() {
	if (texture!=0) {
		glDeleteTextures(1, &texture); 
		printf("t");
	}

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

	glDeleteTextures(1, &format);
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

Folder::Folder(void){
	iconN = 0;
}

/*bool Folder::Load(char* s) {
	Image sizeVerify = Image0FromFile::Load(s);
	if (sizeVerify.getSurface()!=NULL && sizeVerify.getSurface()->h > 10 && sizeVerify.getSurface()->w > 10) {
		images.push_back(sizeVerify);
		return true;
	}
	else {
		return false;
	}
}*/

void Folder::Append(Image app) {
	images.push_back(app);
}

void Folder::Append(Folder app) {
	for (Image im : app.images) {
		images.push_back(im);
	}
}

void Folder::createIconImageFromImages() {
	SDL_Surface* source = images[iconN].getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);
	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}
	setSurface(destination);

	int border = (getSurface()->w < 40 || getSurface()->h < 40) ? 1 : 20;
	for (int i = 0; i < getSurface()->w; i++) {
		for (int j = 0; j < getSurface()->h; j++) {
			if (i < border || j < border || getSurface()->w - (i + 1) < border || getSurface()->h - (j + 1) < border) {
				SurfaceModify::PutPixel32(i, j, ((255 << 24) | (0 << 16) | (255 << 8) | 255), getSurface());
			}
		}
	}
	textureFromSurface();
}

Image0FromFile::Image0FromFile(void) {
	loadType = loadTypeEnum::PICTURE;
}

ImageFolder Image0FromFile::Load(char* s) {
	ImageFolder imfo;
	//-----------

	switch (loadType)
	{
	case Image0FromFile::loadTypeEnum::PICTURE: {

		if (RegularModify::Verify(s)) {
			Image sizeVerify;
			sizeVerify.setSurface(IMG_Load(s));
			if (sizeVerify.getSurface() != nullptr && sizeVerify.getSurface()->h > 10 && sizeVerify.getSurface()->w > 10) {
				sizeVerify.textureFromSurface();
				imfo.im = sizeVerify;
				imfo.iof = iofImage;
			}
			else {
				ImGui::OpenPopup("Load##InvalidSize");
			}

		}
		else {
			//ImGui::OpenPopup("Load##InvalidPath");  nake popups toggleable!!!!!!!!!
		}
		break;
	}

	case Image0FromFile::loadTypeEnum::FOLDER: {

		Folder fold;
		std::string path = s;
		path += "/*";
		std::string path2 = s;

		WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFileA(path.c_str(), &ffd);

		if (hFind == INVALID_HANDLE_VALUE) {
			ImGui::OpenPopup("Load##InvalidPath");
			break;
		}

		while (FindNextFileA(hFind, &ffd) != 0)
		{
			std::string seged = path2 + "/" + (std::string)ffd.cFileName;
			char* cstr = new char[seged.size() + 1];
			std::strcpy(cstr, seged.c_str());

			if (RegularModify::Verify(cstr)) {

				Image sizeVerify;
				sizeVerify.setSurface(IMG_Load(cstr));
				if (sizeVerify.getSurface() != nullptr && sizeVerify.getSurface()->h > 10 && sizeVerify.getSurface()->w > 10) {
					fold.images.push_back(sizeVerify);
				}
				else {
					ImGui::OpenPopup("Load##FolderInvalidSize");
				}
			}
			delete[] cstr;
		}
		FindClose(hFind);

		if (fold.images.size() == 0) {
			ImGui::OpenPopup("Load##EmptyFolder");
		}
		else {

			for (int i = 0; i < fold.images.size();i++) {
				fold.images[i].textureFromSurface();
			}

			fold.createIconImageFromImages();

			imfo.f = fold;
			imfo.iof = iofFolder;
		}

		break;
	}
	default:
		break;
	}

	return imfo;
}

Image0StaticNoise::Image0StaticNoise(void) {

	width = 200;
	height = 200;

	for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);

	color.push_back(ImColor(255, 255, 255, 255));
	backup_color.push_back(ImColor(255, 255, 255, 255));
	color.push_back(ImColor(0, 0, 0, 255));
	backup_color.push_back(ImColor(0, 0, 0, 255));
}

void Image0StaticNoise::Reset() {
	width = 200;
	height = 200;
}

void Image0StaticNoise::StaticMethod() {
	SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0,
		width, height, 32, SDL_PIXELFORMAT_RGBA32);

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, color.size() - 1); // define the range

	for (int i = 0; i < surf->w; i++) {
		for (int j = 0; j < surf->h; j++) {

			SurfaceModify::PutPixel32(i, j, ImGui::ColorConvertFloat4ToU32(color[distr(gen)]), surf);
		}
	}
	staticnoise.setSurface(surf);
	staticnoise.textureFromSurface();
}

Image1::Image1(void) {}
//-------------------------------------------------------------------------------------

/*
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

	imOut = imseged;
}*/

void Image1::setImage(Image im) {

	SDL_Surface* source = im.getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);

	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}

	imOut.setSurface(destination);
	imOut.textureFromSurface();
}

Image1Magnify::Image1Magnify(void){
	zoomTimes = 2.f;
	zoomW = 100;
	zoomH = 50;
	smallX = 0;
	smallY = 0;
	smallChange = true;
	bigX = 0;
	bigY = 0;
	upd = false;
}


void Image1Magnify::Reset() {
	//SDL_Surface* source = imOut.getSurface();  //put this in image1
	//SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0, source->w, source->h, source->format->BitsPerPixel, source->format->format);
	//SDL_BlitSurface(source, nullptr, destination, nullptr);

	//surface = destination;

	zoomTimes = 2.f;
	zoomW = 100;
	if (zoomW > imOut.getSurface()->w / (zoomTimes + 1)) {
		zoomW = imOut.getSurface()->w / (zoomTimes + 1);
	}
	zoomH = 50;
	if (zoomH > imOut.getSurface()->h / (zoomTimes + 1)) {
		zoomH = imOut.getSurface()->h / (zoomTimes + 1);
	}
	smallX = 0;
	smallY = 0;
	smallChange = true;
	bigX = 0;
	bigY = 0;
	upd = false;

	//MagnifyMethod();
}


void Image1Magnify::editableDrawImage(Image im) {

	ImFont* imFo = ImGui::GetFont();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::Image((void*)(intptr_t)imOut.getTexture(), ImVec2(imOut.getSurface()->w, imOut.getSurface()->h));
	if (ImGui::IsItemClicked()) {
		if (io.MousePos.x - pos.x >= (im.getSurface()->w - bigX) - zoomW * zoomTimes && io.MousePos.x - pos.x <= (im.getSurface()->w - bigX) && io.MousePos.y - pos.y >= (im.getSurface()->h - bigY) - zoomH * zoomTimes && io.MousePos.y - pos.y <= (im.getSurface()->h - bigY)) {
			smallChange = false;
			upd = true;
		}
		else if (io.MousePos.x - pos.x <= smallX + zoomW && io.MousePos.x - pos.x >= smallX && io.MousePos.y - pos.y <= smallY + zoomH && io.MousePos.y - pos.y >= smallY) {
			smallChange = true;
			upd = true;
		}
		else {
			upd = false;
		}
	}
	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
	{
		if (upd) {

			MagnifyMethod(im);


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
			float focus_x = io.MousePos.x - pos.x - focus_sz_x * 0.5;
			if (smallChange && focus_x < 0.0f) focus_x = 0.0f;
			else if (smallChange && focus_x > im.getSurface()->w - focus_sz_x) focus_x = im.getSurface()->w - focus_sz_x;
			else if (!smallChange && focus_x < zoomW * zoomTimes + 1) focus_x = zoomW * zoomTimes + 1;
			else if (!smallChange && focus_x > im.getSurface()->w) focus_x = im.getSurface()->w;
			float focus_y = io.MousePos.y - pos.y - focus_sz_y * 0.5f; //(smallChange ? focus_sz_y : 0); // change here for cursor.y relative to the rectangles
			if (smallChange && focus_y < 0.0f) focus_y = 0.0f;
			else if (smallChange && focus_y > im.getSurface()->h - focus_sz_y) focus_y = im.getSurface()->h - focus_sz_y;
			else if (!smallChange && focus_y < zoomH * zoomTimes + 1) focus_y = zoomH * zoomTimes + 1;
			else if (!smallChange && focus_y > im.getSurface()->h) focus_y = im.getSurface()->h;
			ImVec2 uv0 = ImVec2((focus_x) / im.getSurface()->w, (focus_y) / im.getSurface()->h);
			ImVec2 uv1 = ImVec2((focus_x + focus_sz_x) / im.getSurface()->w, (focus_y + focus_sz_y) / im.getSurface()->h);
			if (smallChange) {
				smallX = focus_x;
				smallY = focus_y;
			}
			else {
				bigX = im.getSurface()->w - focus_x;
				bigY = im.getSurface()->h - focus_y;
			}
		}
	}
	/*else {
		upd = false;
	}
	if (ImGui::IsItemClicked()) {
		upd = !upd;
	}*/
}

//Draws
void Image1Magnify::MagnifyMethod(Image im) {
	Uint32 red = (255 << 24) | (0 << 16) | (0 << 8) | 255;

	//can be moved out
	//draw bg
	for (int i = 0; i < imOut.getSurface()->w; i++) {
		for (int j = 0; j < imOut.getSurface()->h; j++) {
			SurfaceModify::PutPixel32(i, j, SurfaceModify::GetColor(i, j, im.getSurface()), imOut.getSurface());
		}
	}

	//little outline
	for (int i = 0 + smallX; i < zoomW + smallX; i++) {
		for (int j = 0 + smallY; j < zoomH + smallY; j++) {
			if (i == 0 + smallX || j == 0 + smallY || i == zoomW + smallX - 1 || j == zoomH + smallY - 1) {
				SurfaceModify::PutPixel32(i, j, red, imOut.getSurface());
			}
		}
	}

	ImVec2 small1 = ImVec2(smallX, zoomH + smallY);
	ImVec2 small2 = ImVec2(zoomW + smallX, smallY);
	ImVec2 big1 = ImVec2(im.getSurface()->w - bigX - zoomW * zoomTimes, im.getSurface()->h - bigY);
	ImVec2 big2 = ImVec2(im.getSurface()->w - bigX, im.getSurface()->h - bigY - zoomH * zoomTimes);

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

		SurfaceModify::plotLine(small1.x, small1.y, big1.x, big1.y, imOut.getSurface());
		SurfaceModify::plotLine(small2.x, small2.y, big2.x, big2.y, imOut.getSurface());
	}

	//big
	for (int i = zoomW * zoomTimes; i > 0; i--) {
		for (int j = zoomH * zoomTimes; j > 0; j--) {
			if (i == 1 || i == std::floor(zoomW * zoomTimes) || j == 1 || j == std::floor(zoomH * zoomTimes)) {
				//outline
				SurfaceModify::PutPixel32(im.getSurface()->w - i - bigX - 1, im.getSurface()->h - j - bigY - 1, red, imOut.getSurface());
			}
			else {
				//fill
				SurfaceModify::PutPixel32(im.getSurface()->w - i - bigX - 1, im.getSurface()->h - j - bigY - 1, SurfaceModify::GetColor(int((zoomW * zoomTimes - i) / zoomTimes + smallX), int((zoomH * zoomTimes - j) / zoomTimes + smallY), im.getSurface()), imOut.getSurface());
			}
		}
	}

	imOut.textureFromSurface();
}

Image1Blur::Image1Blur(void) {
	blurSize = 2;
	blurType = 0;
}

void Image1Blur::Reset() {
	blurSize = 2;
	blurType = 0;
}

void Image1Blur::BlurMethod(Image im) { //if <0 than = 0; if < w
	if (blurType == 0) {
		for (int i = 0; i < imOut.getSurface()->w; i+=blurSize) {
			for (int j = 0; j < imOut.getSurface()->h; j+=blurSize) {

				int rSum=0, gSum=0, bSum=0, aSum=0;
				int count = 0;
				for (int x = 0; x < blurSize; x++) {
					for (int y = 0; y < blurSize; y++) {
						Uint8 r, g, b, a;
						SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, im.getSurface()), im.getSurface()->format, &r, &g, &b, &a);
						rSum += r;
						gSum += g;
						bSum += b;
						aSum += a;
						count++;
					}
				}
				rSum /= count;
				gSum /= count;
				bSum /= count;
				aSum /= count;

				for (int x = 0; x < blurSize; x++) {
					for (int y = 0; y < blurSize; y++) {
						Uint32 color = SDL_MapRGBA(imOut.getSurface()->format, rSum, gSum, bSum, aSum);
						SurfaceModify::PutPixel32(x + i, y + j, color, imOut.getSurface());
					}
				}
			}

			
		}
	}

	else if (blurType == 1) {

		for (int i = 0; i < imOut.getSurface()->w; i++) {
			for (int j = 0; j < imOut.getSurface()->h; j++) {
				float rSum = 0, gSum = 0, bSum = 0, aSum = 0;
				float count = 0;

				for (int x = -((blurSize - 1) / 2); x <= ((blurSize - 1) / 2); x++) {
					for (int y = -((blurSize - 1) / 2); y <= ((blurSize - 1) / 2); y++) {

						float onthepower = -1 * ((x * x + y * y) / (2.0f * blurSize * blurSize));
						float value = ((1.0f) / (2.0f * M_PI * blurSize * blurSize)) * std::exp(onthepower);

						if (0 < i + x && i + x < imOut.getSurface()->w - 1 && 0 < j + y && j + y < imOut.getSurface()->h - 1) {
							Uint8 r, g, b, a;
							SDL_GetRGBA(SurfaceModify::GetColor(i+x, j+y, im.getSurface()), im.getSurface()->format, &r, &g, &b, &a);
							rSum += r * value;
							gSum += g * value;
							bSum += b * value;
							aSum += a * value;
							count += value;
						}

					}
				}
				rSum /= count;
				gSum /= count;
				bSum /= count;
				aSum /= count;

				Uint32 color = SDL_MapRGBA(imOut.getSurface()->format, rSum, gSum, bSum, aSum);
				SurfaceModify::PutPixel32(i, j, color, imOut.getSurface());

			}
		}
	}

	imOut.textureFromSurface();
}

Image1Color::Image1Color(void) {
	imctype = Null;
}

void Image1Color::Reset() {
	imctype = Null;
}

void Image1Color::ColorMethod(Image im) { //sometimes red line???
	for (int i = 0; i < imOut.getSurface()->w; i++) {
		for (int j = 0; j < imOut.getSurface()->h; j++) {
			switch (imctype) {
				case Null: {
					SurfaceModify::PutPixel32(i, j, SurfaceModify::GetColor(i, j, im.getSurface()), imOut.getSurface());
					break;
				}
				case GreyScale: {
					Uint8 grey8 = RegularModify::greyScale(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format);
					Uint32 grey32 = (grey8 << 0) | (grey8 << 8) | (grey8 << 16) |  (255 << 24);
					SurfaceModify::PutPixel32(i, j, grey32 , imOut.getSurface());
					break;
				}
				case Red: {
					Uint8 red8,g,b,alpha8;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &red8,&g,&b,&alpha8);
					Uint32 grey32 = (red8 << 0) | (0 << 8) | (0 << 16) | (alpha8 << 24);
					SurfaceModify::PutPixel32(i, j, grey32, imOut.getSurface());
					break;
				}
				case Green: {
					Uint8 r, green8, b, alpha8;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &r, &green8, &b, &alpha8);
					Uint32 grey32 = (0 << 0) | (green8 << 8) | (0 << 16) | (alpha8 << 24);
					SurfaceModify::PutPixel32(i, j, grey32, imOut.getSurface());
					break;
				}
				case Blue: {
					Uint8 r, g, blue8, alpha8;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &r, &g, &blue8, &alpha8);
					Uint32 grey32 = (0 << 0) | (0 << 8) | (blue8 << 16) | (alpha8 << 24);
					SurfaceModify::PutPixel32(i, j, grey32, imOut.getSurface());
					break;
				}
				case Inverted: {
					Uint8 r, g, b, a;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &r, &g, &b, &a);
					r = 255 - r;
					g = 255 - g;
					b = 255 - b;
					Uint32 grey32 = (r << 0) | (g << 8) | (b << 16) | (a << 24);
					SurfaceModify::PutPixel32(i, j, grey32, imOut.getSurface());
					break;
				}
			}
		}
	}
	imOut.textureFromSurface();
}

bool Image1Save::SaveFolder(Folder f, std::string path, int n/*, StoredOperaionsClass storedOperationsVector*/) { //todo: get every image here in here
	/*SDL_Surface* source = f.images[j].getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);
	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}
	imOut.setSurface(destination);
	

	if (j != 0) {
		for (int k = 0; k < storedOperationsVector.storedOperationsElement.size(); k++) {// go through the operations
			if (std::find(storedOperationsVector.storedOperationsElement[k].affectedElements.begin(), storedOperationsVector.storedOperationsElement[k].affectedElements.end(), j) != storedOperationsVector.storedOperationsElement[k].affectedElements.end()) {//if current image is affected
				switch (storedOperationsVector.storedOperationsElement[k].ote) {
					case StoredOperaionsClass::oteImage1Magnify: {

						if (f.images[0].getSurface()->w <= f.images[j].getSurface()->w && f.images[0].getSurface()->h <= f.images[j].getSurface()->h) {
							storedOperationsVector.storedOperationsElement[k].i1m.setImage(imOut);
							storedOperationsVector.storedOperationsElement[k].i1m.MagnifyMethod(imOut);
							imOut = storedOperationsVector.storedOperationsElement[k].i1m.imOut;
						}

						else {
							//error
						}
						break;
					}
					case StoredOperaionsClass::oteImage1Blur: {

						storedOperationsVector.storedOperationsElement[k].i1b.setImage(imOut);
						storedOperationsVector.storedOperationsElement[k].i1b.BlurMethod(imOut);
						imOut = storedOperationsVector.storedOperationsElement[k].i1b.imOut;

						break;
					}
					case StoredOperaionsClass::oteImage1Color: {

						storedOperationsVector.storedOperationsElement[k].i1c.setImage(imOut);
						storedOperationsVector.storedOperationsElement[k].i1c.ColorMethod(imOut);
						imOut = storedOperationsVector.storedOperationsElement[k].i1c.imOut;

						break;
					}
					case StoredOperaionsClass::oteImage2SSIM: {

						if (storedOperationsVector.storedOperationsElement[k].i2s.storediof.iof == iofImage) {
							storedOperationsVector.storedOperationsElement[k].i2s.setImage(imOut);
							storedOperationsVector.storedOperationsElement[k].i2s.SSIMSurface(imOut, storedOperationsVector.storedOperationsElement[k].i2s.storediof.im);
							imOut = storedOperationsVector.storedOperationsElement[k].i2s.imOut;
						}
						else if (storedOperationsVector.storedOperationsElement[k].i2s.storediof.iof == iofFolder) {
							storedOperationsVector.storedOperationsElement[k].i2s.setImage(imOut);
							storedOperationsVector.storedOperationsElement[k].i2s.SSIMSurface(imOut, storedOperationsVector.storedOperationsElement[k].i2s.storediof.f.images[j]);
							imOut = storedOperationsVector.storedOperationsElement[k].i2s.imOut;
						}

						break;
					}

					case StoredOperaionsClass::oteImage2Merge: {

						if (storedOperationsVector.storedOperationsElement[k].i2m.storediof.iof == iofImage) {
							storedOperationsVector.storedOperationsElement[k].i2m.setImage(imOut);
							if (storedOperationsVector.storedOperationsElement[k].i2m.swap) {
								storedOperationsVector.storedOperationsElement[k].i2m.plotLineMerge(storedOperationsVector.storedOperationsElement[k].i2m.getUx(), storedOperationsVector.storedOperationsElement[k].i2m.getUy(), storedOperationsVector.storedOperationsElement[k].i2m.storediof.im ,imOut);
							}
							else {
								storedOperationsVector.storedOperationsElement[k].i2m.plotLineMerge(storedOperationsVector.storedOperationsElement[k].i2m.getUx(), storedOperationsVector.storedOperationsElement[k].i2m.getUy(), imOut, storedOperationsVector.storedOperationsElement[k].i2m.storediof.im);
							}
							imOut = storedOperationsVector.storedOperationsElement[k].i2m.imOut;
						}
						else if (storedOperationsVector.storedOperationsElement[k].i2m.storediof.iof == iofFolder) {
							storedOperationsVector.storedOperationsElement[k].i2m.setImage(imOut);
							if (storedOperationsVector.storedOperationsElement[k].i2m.swap) {
								storedOperationsVector.storedOperationsElement[k].i2m.plotLineMerge(storedOperationsVector.storedOperationsElement[k].i2m.getUx(), storedOperationsVector.storedOperationsElement[k].i2m.getUy(), storedOperationsVector.storedOperationsElement[k].i2m.storediof.f.images[j], imOut);
							}
							else {
								storedOperationsVector.storedOperationsElement[k].i2m.plotLineMerge(storedOperationsVector.storedOperationsElement[k].i2m.getUx(), storedOperationsVector.storedOperationsElement[k].i2m.getUy(), imOut, storedOperationsVector.storedOperationsElement[k].i2m.storediof.f.images[j]);
							}
							imOut = storedOperationsVector.storedOperationsElement[k].i2m.imOut;
						}

						break;
					}
				}
			}

		}
	}
	IMG_SavePNG(imOut.getSurface(), cstr);*/
	struct stat sb;
	bool allSucceeded=true;

	for (int i = 0; i < f.images.size(); i++) {
		std::string seged = path + "/image" + std::to_string(n) + ".png";
		char* cstr = new char[seged.size() + 1];
		std::strcpy(cstr, seged.c_str());

		if (IMG_SavePNG(f.images[i].getSurface(), cstr) != 0) {
			allSucceeded = false;
		}

		if (stat(cstr, &sb) != 0) {
			allSucceeded = false;
		}

		delete[] cstr;
		n++;
	}

	return allSucceeded;
}

bool Image1Save::Save(Image im, char* cstr) {
	struct stat sb;
	
	if (IMG_SavePNG(im.getSurface(), cstr) != 0) {
		return false;
	}

	return stat(cstr, &sb) == 0;
}

Image2::Image2(void) {}

/*
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
*/

void Image2::setImage(Image im) {
	SDL_Surface* source = im.getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);

	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}

	Image imseged;
	imseged.setSurface(destination);
	imseged.textureFromSurface();

	imOut = imseged;
}

Image2SSIM::Image2SSIM(void){
	ssimSize = 1;
	ssimColor = 0;
	ssimOsszeg = 0;
}

void Image2SSIM::Reset() {
	ssimSize = 1;
	ssimColor = 0;
	ssimOsszeg = 0;
}

/*
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
*/

//No need
void Image2SSIM::editableDrawImage() {

	ImGui::Image((void*)(intptr_t)imOut.getTexture(), ImVec2(imOut.getSurface()->w, imOut.getSurface()->h));
}

void Image2SSIM::SSIMSurface(Image im1, Image im2) {

	//SDL_LockSurface(ssimSurface);
	//			SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, imIn1.getSurface()), imIn1.getSurface()->format, &r, &g, &b, &a);
	//			SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, imIn2.getSurface()), imIn2.getSurface()->format, &r2, &g2, &b2, &a2);


	ssimOsszeg = 0.f;
	int num = 0;

	for (int x = 0; x < imOut.getSurface()->w; x += ssimSize) {
		for (int y = 0; y < imOut.getSurface()->h; y += ssimSize) {

			static float mean1[colorResult::NumberOfTypes]{};
			static float mean2[colorResult::NumberOfTypes]{};
			static float var1[colorResult::NumberOfTypes]{};
			static float var2[colorResult::NumberOfTypes]{};
			static float covar[colorResult::NumberOfTypes]{};
			for (int i = 0; i < colorResult::NumberOfTypes; i++) {
				mean1[i] = mean2[i] = var1[i] = var2[i] = covar[i] = 0.0f;
			}

			//mean
			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize ; j++) {

					Uint8 seged1[colorResult::NumberOfTypes]{};
					Uint8 seged2[colorResult::NumberOfTypes]{};
					SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, im1.getSurface()), im1.getSurface()->format, &seged1[colorResult::Red], &seged1[colorResult::Green], &seged1[colorResult::Blue], &seged1[colorResult::Alpha]);
					SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, im2.getSurface()), im2.getSurface()->format, &seged2[colorResult::Red], &seged2[colorResult::Green], &seged2[colorResult::Blue], &seged2[colorResult::Alpha]);
					for (int k = 0; k < colorResult::NumberOfTypes; k++) { //r g b a
						if (k != colorResult::Grey) {
							mean1[k] += seged1[k];
							mean2[k] += seged2[k];
						}
					}
					mean1[colorResult::Grey] += RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, im1.getSurface()), im1.getSurface()->format);
					mean2[colorResult::Grey] += RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, im2.getSurface()), im2.getSurface()->format);

				}
			}
			for (int i = 0; i < colorResult::NumberOfTypes; i++) {
				mean1[i] /= pow(ssimSize, 2);
				mean2[i] /= pow(ssimSize, 2);
			}

			//var
			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize; j++) {
					Uint8 seged1[colorResult::NumberOfTypes]{};
					Uint8 seged2[colorResult::NumberOfTypes]{};
					SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, im1.getSurface()), im1.getSurface()->format, &seged1[colorResult::Red], &seged1[colorResult::Green], &seged1[colorResult::Blue], &seged1[colorResult::Alpha]);
					SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, im2.getSurface()), im2.getSurface()->format, &seged2[colorResult::Red], &seged2[colorResult::Green], &seged2[colorResult::Blue], &seged2[colorResult::Alpha]);
					for (int k = 0; k < colorResult::NumberOfTypes - 1; k++) { //r g b a
						var1[k] += (seged1[k] - mean1[k]);
						var2[k] += (seged2[k] - mean2[k]);
					}
					var1[colorResult::Grey] += (RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, im1.getSurface()), im1.getSurface()->format) - mean1[colorResult::Grey]);
					var2[colorResult::Grey] += (RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, im2.getSurface()), im2.getSurface()->format) - mean2[colorResult::Grey]);
				}
			}
			for (int i = 0; i < colorResult::NumberOfTypes; i++) {
				var1[i] /= pow(ssimSize, 2);
				var2[i] /= pow(ssimSize, 2);
			}

			//covar
			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize; j++) {
					for (int k = 0; k < colorResult::NumberOfTypes; k++) {
						covar[k] += (var1[k] * var2[k]);
					}
				}
			}
			for (int i = 0; i < colorResult::NumberOfTypes; i++) {
				covar[i] /= pow(ssimSize, 2);
			}

			//end
			static std::vector<float> resultFinal(colorResult::NumberOfTypes);

			for (int i = 0; i < colorResult::NumberOfTypes; i++) {
				resultFinal[i] = ((2 * mean1[i] * mean2[i] + C1) * (2 * covar[i] + C2)) / ((pow(mean1[i], 2) + pow(mean2[i], 2) + C1) * (pow(var1[i], 2) + pow(var2[i], 2) + C2));
			}
			

			Uint32 putColor{};
			Uint8 seged;

			switch (ssimColor)
			{
			case 0:
				//ssimGrey = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Grey];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Grey]) * 255);
				putColor = (seged << 24) + (seged << 16) + (seged << 8);
				break;
			case 1:
				//ssimRed = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Red];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Red]) * 255);
				putColor = (seged << 24) + (seged << 16) + (255 << 8);
				break;
			case 2:
				//ssimGreen = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Green];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Green]) * 255);
				putColor = (seged << 24) + (255 << 16) + (seged << 8);
				break;
			case 3:
				//ssimBlue = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Blue];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Blue]) * 255);
				putColor = (255 << 24) + (seged << 16) + (seged << 8);
				break;
			case 4:
				//ssimRed = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Red];
				num++;
				//ssimGreen = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Green];
				num++;
				//ssimBlue = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Blue];
				num++;
				putColor = ((Uint8)((resultFinal[colorResult::Red]) * 255) << 24) + ((Uint8)((resultFinal[colorResult::Green]) * 255) << 16) + ((Uint8)((resultFinal[colorResult::Blue]) * 255) << 8);
				break;
			case 5:
				//ssimGrey = SSIMmethod(colorVectFinal1, colorVectFinal2);
				ssimOsszeg += resultFinal[colorResult::Grey];
				num++;
				putColor = RegularModify::heatmapColor(resultFinal[colorResult::Grey]);
				break;
			}


			//red 0 0 255   green 0 255 0   blue 255 0 0     
			//r = -y -m	, g = -y -c , b = -m -c 

			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize; j++) {
					if (x + i < imOut.getSurface()->w && y + j < imOut.getSurface()->h) {

						Uint32 color = SDL_MapRGBA(imOut.getSurface()->format, (putColor >> 24) & 0xFF, (putColor >> 16) & 0xFF, (putColor >> 8) & 0xFF, 255 /*alpha1[i][j] + alpha2[i][j]) / 2*/);
						SurfaceModify::PutPixel32(x + i, y + j, color, imOut.getSurface()); //alpha?
					}
				}
			}
		}
	}

	ssimOsszeg = ssimOsszeg / num;

	imOut.textureFromSurface();

}

std::vector<float> Image2SSIM::SSIMmethod(std::vector<std::vector<Uint32>> window1, std::vector<std::vector<Uint32>> window2, Image im1, Image im2)  {

	float mean1[colorResult::NumberOfTypes]{};
	float mean2[colorResult::NumberOfTypes]{};
	float var1[colorResult::NumberOfTypes]{};
	float var2[colorResult::NumberOfTypes]{};
	float covar[colorResult::NumberOfTypes]{};
	for (int i = 0; i < colorResult::NumberOfTypes; i++) {
		mean1[i] = mean2[i] = var1[i] = var2[i] = covar[i] = 0.0f;
	}

	//mean
	for (int i = 0; i < ssimSize; i++) {
		for (int j = 0; j < ssimSize; j++) {
			Uint8 seged1[colorResult::NumberOfTypes]{};
			Uint8 seged2[colorResult::NumberOfTypes]{};
			SDL_GetRGBA(window1[i][j], im1.getSurface()->format, &seged1[colorResult::Red], &seged1[colorResult::Green], &seged1[colorResult::Blue], &seged1[colorResult::Alpha]);
			SDL_GetRGBA(window2[i][j], im2.getSurface()->format, &seged2[colorResult::Red], &seged2[colorResult::Green], &seged2[colorResult::Blue], &seged2[colorResult::Alpha]);
			for (int k = 0; k < colorResult::NumberOfTypes - 1; k++) { //r g b a
				mean1[k] += seged1[k];
				mean2[k] += seged2[k];
			}
			mean1[colorResult::Grey] += RegularModify::greyScale(window1[i][j], im1.getSurface()->format);
			mean2[colorResult::Grey] += RegularModify::greyScale(window2[i][j], im2.getSurface()->format);;
		}
	}
	for (int i = 0; i < colorResult::NumberOfTypes; i++) {
		mean1[i] /= pow(ssimSize,2);
		mean2[i] /= pow(ssimSize,2);
	}

	//var
	for (int i = 0; i < ssimSize; i++) {
		for (int j = 0; j < ssimSize; j++) {
			Uint8 seged1[colorResult::NumberOfTypes]{};
			Uint8 seged2[colorResult::NumberOfTypes]{};
			SDL_GetRGBA(window1[i][j], im1.getSurface()->format, &seged1[colorResult::Red], &seged1[colorResult::Green], &seged1[colorResult::Blue], &seged1[colorResult::Alpha]);
			SDL_GetRGBA(window2[i][j], im2.getSurface()->format, &seged2[colorResult::Red], &seged2[colorResult::Green], &seged2[colorResult::Blue], &seged2[colorResult::Alpha]);
			for (int k = 0; k < colorResult::NumberOfTypes - 1; k++) { //r g b a
				var1[k] += (seged1[k] - mean1[k]);
				var2[k] += (seged2[k] - mean2[k]);
			}
			var1[colorResult::Grey] += (RegularModify::greyScale(window1[i][j], im1.getSurface()->format) - mean1[colorResult::Grey]);
			var2[colorResult::Grey] += (RegularModify::greyScale(window2[i][j], im2.getSurface()->format) - mean2[colorResult::Grey]);
		}
	}
	for (int i = 0; i < colorResult::NumberOfTypes; i++) {
		var1[i] /= pow(ssimSize,2);
		var2[i] /= pow(ssimSize,2);
	}

	//covar
	for (int i = 0; i < ssimSize; i++) {
		for (int j = 0; j < ssimSize; j++) {
			for (int k = 0; k < colorResult::NumberOfTypes; k++) {
				covar[k] += (var1[k] * var2[k]);
			}
		}
	}
	for (int i = 0; i < colorResult::NumberOfTypes; i++) {
		covar[i] /= pow(ssimSize,2);
	}

	std::vector<float> res(colorResult::NumberOfTypes);

	for (int i = 0; i < colorResult::NumberOfTypes; i++) {
		res[i] = ((2 * mean1[i] * mean2[i] + C1) * (2 * covar[i] + C2)) / ((pow(mean1[i], 2) + pow(mean2[i], 2) + C1) * (pow(var1[i], 2) + pow(var2[i], 2) + C2));
	}

	return res;
}

Image2Merge::Image2Merge(void){
	slope = 45.0f;
	ux = uy = 0;
	upd = false;
}

void Image2Merge::Reset() {
	slope = 45.0f;
	if (imOut.getSurface() != nullptr) {
		ux = imOut.getSurface()->w / 2;
		uy = imOut.getSurface()->h / 2;
	}
	else {
		ux = uy = 0;
	}
	upd = false;
	swap = false;
}

/*
Image2Merge::Image2Merge(Image im1, Image im2) : Image2(im1, im2) {
	SDL_Surface* source = imIn1.getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);

	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}

	surface = destination;
	if (surface != nullptr) {
		ux = surface->w / 2;
		uy = surface->h / 2;
	}
	else {
		ux = uy = 0;
	}
	slope = 45.0f;
	plotLineMerge(ux, uy);
	textureFromSurface();

	upd = false;
}
*/

void Image2Merge::editableDrawImage(Image im1, Image im2) {

	//ImFont* imFo = ImGui::GetFont();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::Image((void*)(intptr_t)imOut.getTexture(), ImVec2(imOut.getSurface()->w, imOut.getSurface()->h));
	if (ImGui::IsItemHovered())
	{
		if (upd) {
			ux = io.MousePos.x - pos.x;
			uy = io.MousePos.y - pos.y;
			plotLineMerge(ux,uy,im1,im2);
			//imOut.textureFromSurface();
		}
	}
	else {
		upd = false;
	}
	if (ImGui::IsItemClicked()) {
		upd = !upd;
	}

}

void Image2Merge::plotLineMerge(int x, int y, Image im1, Image im2) {
	for (int i = 0; i < im1.getSurface()->w; i++) {
		for (int j = 0; j < im1.getSurface()->h; j++) {
			if ((j <= tan(slope/180*M_PI) * (i - x) + y  && (slope <= 90 || slope >270)) || (j > tan(slope / 180 * M_PI) * (i - x) + y &&  slope>90 && slope <=270 )) {
				SurfaceModify::PutPixel32(i, j, SurfaceModify::GetColor(i, j, im1.getSurface()), imOut.getSurface());
			}
			else {
				SurfaceModify::PutPixel32(i, j, SurfaceModify::GetColor(i, j, im2.getSurface()), imOut.getSurface());
			}
		}
	}

	imOut.textureFromSurface();
}


void SurfaceModify::plotLine(int x0, int y0, int x1, int y1, SDL_Surface* sur) {

	if ( (abs(y1 - y0) < abs(x1 - x0) && x0 > x1) || (abs(y1 - y0) >= abs(x1 - x0)) && (y0 > y1)){
		plotLineMixed(x1, y1, x0, y0, sur);
	}
	else {
		plotLineMixed(x0, y0, x1, y1, sur);
	}
}
	/*if (abs(y1 - y0) < abs(x1 - x0)) {
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
	}*/



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

void SurfaceModify::plotLineMixed(int x0, int y0, int x1, int y1, SDL_Surface* sur) {
	int dBig,dSmall;
	int from,to,otherAxis;
	int i = 1;

	if(abs(y1 - y0) < abs(x1 - x0)){
		dBig = x1 - x0;
		dSmall = y1 - y0;
		from = x0;
		to = x1;
		otherAxis = y0;

	}
	else {
		dBig = y1 - y0;
		dSmall = x1 - x0;
		from = y0;
		to = y1;
		otherAxis = x0;
	}

	if (dSmall < 0) {
		i = -1;
		dSmall = -dSmall;
	}

	int D = (2 * dSmall) - dBig;

	for (int f = from; f < to; f++) {
		if (abs(y1 - y0) < abs(x1 - x0)) {
			PutPixel32(f, otherAxis, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);
		}
		else {
			PutPixel32(otherAxis, f, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);
		}

		if (D > 0) {
			otherAxis = otherAxis + i;
			D = D + (2 * (dSmall - dBig));
		}
		else {
			D = D + 2 * dSmall;
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

	Uint8* pixelPtr = (Uint8*)sur->pixels+ y * sur->pitch+ x * sur->format->BytesPerPixel;

	// Read 32-bit pixel value
	Uint32 pixel = *(Uint32*)pixelPtr;

	Uint8 r, g, b, a;
	SDL_GetRGBA(pixel, sur->format, &r, &g, &b, &a);
	
	return ImColor(r, g, b, a);
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
		g = static_cast<int>(255 * (value / 0.5f));
		b = 0;
	}
	else {
		// yellow (255,255,0) -> blue (0,0,255)
		r = g = static_cast<int>(255 * (1.0f - (value / 0.5f)));
		b = static_cast<int>(255 * (value / 0.5f));
	}

	return (r << 24) + (g << 16) + (b << 8) ;
}

bool RegularModify::Verify(char* filePath) {

	SDL_Surface* imageSurface = IMG_Load(filePath);
	if (!imageSurface) {
		/*imageSurface = IMG_Load("texture.bmp");
		strcpy(filePathv, "texture.bmp");*/
		return false;
	}

	/*if (!imageSurface) {
		printf("Failed to load image (also backup image got deleted): %s\n", IMG_GetError());
		return false;
	}*/
	SDL_FreeSurface(imageSurface);
	return true;
}

/*
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		TASKDIALOGCONFIG taskDialogParams = { sizeof(taskDialogParams) };
		taskDialogParams.dwFlags = TDF_USE_COMMAND_LINKS | TDF_ALLOW_DIALOG_CANCELLATION;

		TASKDIALOG_BUTTON const buttons[] =
		{
			{ IDC_BASICFILEOPEN,                       L"Basic File Open" },
			{ IDC_ADDITEMSTOCUSTOMPLACES,              L"Add Items to Common Places" },
			{ IDC_ADDCUSTOMCONTROLS,                   L"Add Custom Controls" },
			{ IDC_SETDEFAULTVALUESFORPROPERTIES,       L"Change Property Order" },
			{ IDC_WRITEPROPERTIESUSINGHANDLERS,        L"Write Properties Using Handlers" },
			{ IDC_WRITEPROPERTIESWITHOUTUSINGHANDLERS, L"Write Properties without Using Handlers" },
		};

		taskDialogParams.pButtons = buttons;
		taskDialogParams.cButtons = ARRAYSIZE(buttons);
		taskDialogParams.pszMainInstruction = L"Pick the file dialog sample you want to try";
		taskDialogParams.pszWindowTitle = L"Common File Dialog";

		while (SUCCEEDED(hr))
		{
			int selectedId;
			hr = TaskDialogIndirect(&taskDialogParams, &selectedId, NULL, NULL);
			if (SUCCEEDED(hr))
			{
				if (selectedId == IDCANCEL)
				{
					break;
				}
				else if (selectedId == IDC_BASICFILEOPEN)
				{
					BasicFileOpen();
				}
				else if (selectedId == IDC_ADDITEMSTOCUSTOMPLACES)
				{
					AddItemsToCommonPlaces();
				}
				else if (selectedId == IDC_ADDCUSTOMCONTROLS)
				{
					AddCustomControls();
				}
				else if (selectedId == IDC_SETDEFAULTVALUESFORPROPERTIES)
				{
					SetDefaultValuesForProperties();
				}
				else if (selectedId == IDC_WRITEPROPERTIESUSINGHANDLERS)
				{
					WritePropertiesUsingHandlers();
				}
				else if (selectedId == IDC_WRITEPROPERTIESWITHOUTUSINGHANDLERS)
				{
					WritePropertiesWithoutUsingHandlers();
				}
			}
		}
		CoUninitialize();
	}
	return 0;
}
*/
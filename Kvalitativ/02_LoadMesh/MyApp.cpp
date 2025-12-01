#define STB_IMAGE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <imgui/imgui.h>
#include "stb_image.h"
#include <random>

#include <filesystem>

#include <windows.h>

CMyApp::CMyApp(void)
{
	window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar;

	strcpy(stradd, "C:/Users/User/Pictures/ac2.jpg");
	RegularModify::Verify(stradd);

	currentMenuEnum = OPERATIONSENUM;
	strcpy(outstr, "C:/Users/User/Pictures/save.png");


	ImGuiIO& io = ImGui::GetIO();

	ImVector<ImWchar> ranges;
	ImFontAtlas::GlyphRangesBuilder builder;
	builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); 
	builder.BuildRanges(&ranges); 
	float dpiScale = 3.0f;
	arial = io.Fonts->AddFontFromFileTTF("ARIAL.ttf", 12.0f * dpiScale, NULL, ranges.Data);
	io.FontGlobalScale = 1.0f / dpiScale;
	io.Fonts->Build();
	
	arrow_left = im0load.Load("Arrow_left.png", false).im;
	arrow_left.textureFromSurface();
	arrow_right = im0load.Load("Arrow_right.png", false).im;
	arrow_right.textureFromSurface();
	arrow_left_unavailable = im0load.Load("Arrow_left_unavailable.png", false).im;
	arrow_left_unavailable.textureFromSurface();
	arrow_right_unavailable = im0load.Load("Arrow_right_unavailable.png", false).im;
	arrow_right_unavailable.textureFromSurface();

}

CMyApp::~CMyApp(void)
{
}


bool CMyApp::Init()
{
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

	Colors[ColorEnum::TEXT_LIGHT] = ImColor(183, 201, 226, 255);
	Colors[ColorEnum::TEXT_DARK] = ImColor(16, 26, 40, 255);
	Colors[ColorEnum::TEXT_HINT] = ImColor(39, 63, 96, 255);
	Colors[ColorEnum::CHILD_BG] = ImColor(183, 201, 226, 255);
	Colors[ColorEnum::POPUP_BG] = ImColor(169, 191, 220, 255);
	Colors[ColorEnum::POPUP_BORDER] = ImColor(39, 63, 96, 255);
	Colors[ColorEnum::SCROLL_GRAB] = ImColor(44, 72, 110, 255);
	Colors[ColorEnum::SCROLL_GRAB_HOVERED] = ImColor(50, 81, 124, 255);
	Colors[ColorEnum::SCROLL_GRAB_ACTIVE] = ImColor(55, 90, 138, 255);
	Colors[ColorEnum::SCROLL_BG] = ImColor(27, 22, 68, 255);
	Colors[ColorEnum::HEADER] = ImColor(183, 201, 226, 255);
	Colors[ColorEnum::HEADER_HOVERED] = ImColor(211, 222, 237, 255);
	Colors[ColorEnum::HEADER_ACTIVE] = ImColor(239, 243, 249, 255);
	Colors[ColorEnum::INPUT_BG] = ImColor(127, 159, 203, 255);
	Colors[ColorEnum::BUTTON_BLUE] = ImColor(55, 90, 138, 255);
	Colors[ColorEnum::BUTTON_BLUE_HOVERED] = ImColor(61, 99, 152, 255);
	Colors[ColorEnum::BUTTON_BLUE_ACTIVE] = ImColor(67, 108, 166, 255);
	Colors[ColorEnum::BUTTON_GREEN] = ImColor(50, 200, 50, 255);
	Colors[ColorEnum::BUTTON_GREEN_HOVERED] = ImColor(63, 207, 63, 255);
	Colors[ColorEnum::BUTTON_GREEN_ACTIVE] = ImColor(78, 211, 78, 255);
	Colors[ColorEnum::BUTTON_RED] = ImColor(200, 50, 50, 255);
	Colors[ColorEnum::BUTTON_RED_HOVERED] = ImColor(207, 63, 63, 255);
	Colors[ColorEnum::BUTTON_RED_ACTIVE] = ImColor(211, 78, 78, 255);
	Colors[ColorEnum::BUTTON_GREY] = ImColor(128, 128, 128, 255);
	Colors[ColorEnum::BUTTON_GREY_HOVERED] = ImColor(141, 141, 141, 255);
	Colors[ColorEnum::BUTTON_GREY_ACTIVE] = ImColor(160, 160, 160, 255);
	Colors[ColorEnum::COLORPICKER_FRAMEBG] = ImColor(28, 44, 68, 255);

	//TestMethod();

	return true;
}

void CMyApp::Clean()
{
	im0stat.getImOut().freeSurface();
	im1blur.getImOut().freeSurface();
	im1col.getImOut().freeSurface();
	im1mag.getImOut().freeSurface();
	im1sav.getImOut().freeSurface();
	im2merge.getImOut().freeSurface();
	im2ssim.getImOut().freeSurface();

	for (int i = 0; i < imfVec.size(); i++) {
		if (imfVec[i].iof == iofImage) {
			imfVec[i].im.freeSurface();
		}
		else if (imfVec[i].iof == iofFolder) {
			for (int j = 0; j < imfVec[i].f.getImages().size(); j++) {
				imfVec[i].f.getImages()[j].freeSurface();
			}
		}
	}

}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

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

	SetBasicUI_5Col_2Var();
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

	ImGui::BeginChild("Pictures", ImVec2(0, imfVec.size() == 0 ? 100 : isFolder? 455 : 395), false); {

		imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
		ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Images: ");
		imFo->Scale = 1.f;	ImGui::PopFont();
		RegularModify::ShowHelpMarker("The loaded images and folders will appear here. Click on them to select them."); ImGui::NewLine();

		if (imfVec.size() > 0) {


			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));

			ImGui::BeginChild("scrolling", ImVec2(0, isFolder? 390 : 330), false, ImGuiWindowFlags_HorizontalScrollbar); {
				for (int i = 0; i < imfVec.size(); i++) {

					switch (imfVec[i].iof) {
						case iofImage: {
							RegularModify::CursorPos(20 + 315 * i);
							imfVec[i].im.drawImage(300, std::find(selectedImFVec.begin(), selectedImFVec.end(), i) != selectedImFVec.end());
							break;
						}
						case iofFolder: {
							RegularModify::CursorPos(20 + 315 * i);
							imfVec[i].f.getIcon().drawImage(300, std::find(selectedImFVec.begin(), selectedImFVec.end(), i) != selectedImFVec.end());
							break;
						}
						default: {
							break;
						}
					}

					if (ImGui::IsItemClicked() && (currentMenuEnum == OPERATIONSENUM || currentMenuEnum == LOADENUM))
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
 
				for (int i = 0; i < imfVec.size(); i++) {

					switch (imfVec[i].iof) {
						case iofFolder: {
							ImGui::SameLine();
							RegularModify::CursorPos(20 + 315 * i);
							
							ImGui::PushID(i);
							if (ImGui::ImageButton(imfVec[i].f.getIconN() != 0 ? (void*)(intptr_t)arrow_left.getTexture(): (void*)(intptr_t)arrow_left_unavailable.getTexture(), ImVec2(147, 50), ImVec2(0, 0), ImVec2(1, 1))) {
								if (imfVec[i].f.getIconN() != 0) {
									imfVec[i].f.setIconN(imfVec[i].f.getIconN()-1);
									if (imfVec[i].f.getIcon().getSurface() != nullptr) {
										SDL_FreeSurface(imfVec[i].f.getIcon().getSurface());
										imfVec[i].f.getIcon().setSurface(nullptr);
									}
									imfVec[i].f.createIconImageFromImages();
								}
							}
							ImGui::PopID();
							ImGui::SameLine();
							ImGui::PushID(i);
							if (ImGui::ImageButton((imfVec[i].f.getIconN() != imfVec[i].f.getImages().size() - 1) ? (void*)(intptr_t)arrow_right.getTexture() : (void*)(intptr_t)arrow_right_unavailable.getTexture(), ImVec2(147, 50), ImVec2(0, 0), ImVec2(1, 1))) {
								if (imfVec[i].f.getIconN() != imfVec[i].f.getImages().size() - 1) {
									imfVec[i].f.setIconN(imfVec[i].f.getIconN() + 1);
									if (imfVec[i].f.getIcon().getSurface() != nullptr) {
										SDL_FreeSurface(imfVec[i].f.getIcon().getSurface());
										imfVec[i].f.getIcon().setSurface(nullptr);
									}
									imfVec[i].f.createIconImageFromImages();
								}
							}
							ImGui::PopID();
							break;
						}
						default: {
							break;
						}
					}
				}
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

	switch (currentMenuEnum)
	{
	case CMyApp::OPERATIONSENUM: //--------------------------------------------------------------------------------------------------
	{

			ImGui::PushStyleColor(ImGuiCol_Header, Colors[ColorEnum::HEADER]);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Colors[ColorEnum::HEADER_HOVERED]);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, Colors[ColorEnum::HEADER_ACTIVE]);
			ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_DARK]);

			if (ImGui::CollapsingHeader("Selected images in full size", ImGuiTreeNodeFlags_DefaultOpen)) {

				RegularModify::CursorPos(20);
				if (selectedImFVec.size() > 0) {
					for (int i = 0; i < selectedImFVec.size(); i++) {

						switch (imfVec[selectedImFVec[i]].iof) {
							case iofImage: {
								imfVec[selectedImFVec[i]].im.drawImage();
								break;
							}
							case iofFolder: {
								imfVec[selectedImFVec[i]].f.getIcon().drawImage();
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
			}
			ImGui::PopStyleColor(4);

			ImGui::NewLine();ImGui::NewLine();



		SetBasicUI_5Col_2Var();
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
					currentMenuEnum = LOADENUM;
				}
				ImGui::NextColumn();
				if (ImGui::Button("Static", ImVec2(150, 50))) {

					im0stat.Reset();

					im0stat.StaticMethod();

					currentMenuEnum = STATICNOISEENUM;
				}
				ImGui::NextColumn();
			}

			if (selectedImFVec.size() >= 1) {

				RegularModify::CursorPos(20);
				if (ImGui::Button("Magnify", ImVec2(150, 50))) {
					currentMenuEnum = MAGNIFYENUM;

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1mag.setImage(imfVec[selectedImFVec[0]].im);
						im1mag.Reset();
						im1mag.MagnifyMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1mag.setImage(imfVec[selectedImFVec[0]].f.getImages()[0]);
						im1mag.Reset();
						im1mag.MagnifyMethod(imfVec[selectedImFVec[0]].f.getImages()[0]);
					}
				}
				
				ImGui::NextColumn();
				if (ImGui::Button("Blur", ImVec2(150, 50))) {
					currentMenuEnum = BLURENUM;

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1blur.setImage(imfVec[selectedImFVec[0]].im);
						im1blur.Reset();
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1blur.setImage(imfVec[selectedImFVec[0]].f.getImages()[0]);
						im1blur.Reset();
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].f.getImages()[0]);
					}
				}

				ImGui::NextColumn();
				if (ImGui::Button("Color", ImVec2(150, 50))) {
					currentMenuEnum = COLORENUM;

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1col.setImage(imfVec[selectedImFVec[0]].im);
						im1col.Reset();
						im1col.ColorMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1col.setImage(imfVec[selectedImFVec[0]].f.getImages()[0]);
						im1col.Reset();
						im1col.ColorMethod(imfVec[selectedImFVec[0]].f.getImages()[0]);
					}
				}

				ImGui::NextColumn();

			}

			if (selectedImFVec.size() == 2) {

				if ((imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofImage && imfVec[selectedImFVec[0]].im.getSurface()->w == imfVec[selectedImFVec[1]].im.getSurface()->w && imfVec[selectedImFVec[0]].im.getSurface()->h == imfVec[selectedImFVec[1]].im.getSurface()->h) ||
					(imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofFolder && imfVec[selectedImFVec[0]].im.getSurface()->w == imfVec[selectedImFVec[1]].f.getImages()[0].getSurface()->w && imfVec[selectedImFVec[0]].im.getSurface()->h == imfVec[selectedImFVec[1]].f.getImages()[0].getSurface()->h) ||
					(imfVec[selectedImFVec[0]].iof == iofFolder && imfVec[selectedImFVec[1]].iof == iofImage && imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->w == imfVec[selectedImFVec[1]].im.getSurface()->w && imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->h == imfVec[selectedImFVec[1]].im.getSurface()->h) ||
					(imfVec[selectedImFVec[0]].iof == iofFolder && imfVec[selectedImFVec[1]].iof == iofFolder && imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->w == imfVec[selectedImFVec[1]].f.getImages()[0].getSurface()->w && imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->h == imfVec[selectedImFVec[1]].f.getImages()[0].getSurface()->h)) {


					if (ImGui::Button("SSIM", ImVec2(150, 50))) {
						currentMenuEnum = SSIMENUM;
						if (imfVec[selectedImFVec[0]].iof == iofImage) {
							im2ssim.setImage(imfVec[selectedImFVec[0]].im);
							im2ssim.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.getImages()[0]);
							}
						}
						else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							im2ssim.setImage(imfVec[selectedImFVec[0]].f.getImages()[0]);
							im2ssim.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].f.getImages()[0]);
							}
						}
					}
					ImGui::NextColumn();

					if (ImGui::Button("Merge", ImVec2(150, 50))) {
						currentMenuEnum = MERGEENUM;
						if (imfVec[selectedImFVec[0]].iof == iofImage) {
							im2merge.setImage(imfVec[selectedImFVec[0]].im);
							im2merge.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2merge.MergeMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2merge.MergeMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.getImages()[0]);
							}
						}
						else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							im2merge.setImage(imfVec[selectedImFVec[0]].f.getImages()[0]);
							im2merge.Reset();
							if (imfVec[selectedImFVec[1]].iof == iofImage) {
								im2merge.MergeMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].im);
							}
							else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
								im2merge.MergeMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].f.getImages()[0]);
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

			if (selectedImFVec.size() >= 1) {
				if (ImGui::Button("Save", ImVec2(150, 50))) {
					currentMenuEnum = SAVEENUM;
				}
				ImGui::NextColumn();
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
			SetBasicUI_5Col_2Var();
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
				PushStyleColorGreenButton_3Col();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo); RegularModify::CursorPos(20);
				if (ImGui::Button("Load##Load", ImVec2(150, 50))) {

					ImageFolder loadImFolder = im0load.Load(stradd,true);
					if (loadImFolder.iof != iofEmpty) {
						imfVec.push_back(loadImFolder);
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
			
			SetBasicUI_5Col_2Var();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_StaticNoise", ImVec2(max(im0stat.getImOut().getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Static Noise");
				imFo->Scale = 1.f;	ImGui::PopFont();
			
			}

			ImGui::EndChild();

			ImGui::NewLine();
			im0stat.getImOut().drawImage();
			ImGui::NewLine();

			ImGui::BeginChild("Values_StaticNoise", ImVec2(max(im0stat.getImOut().getSurface()->w, 400), (215 + im0stat.color.size() * 76)), false); {

				ImGui::NewLine();

				ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_BLUE]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_BLUE_HOVERED]);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_BLUE_ACTIVE]);
				ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				RegularModify::CursorPos((max(im0stat.getImOut().getSurface()->w, 400) / 2.0f) - (150.0f / 2.0f));
				if (ImGui::Button("Regenerate##StaticNoise", ImVec2(150, 50))) {
					im0stat.StaticMethod();
				}
				ImGui::PopStyleColor(4);
				imFo->Scale = 1.f; ImGui::PopFont();
				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Width:");
				ImGui::SameLine();
				RegularModify::CursorPos(270);
				ImGui::PushItemWidth(fmax(im0stat.getImOut().getSurface()->w, 400) - 290);
				int segedim0statwidth = im0stat.getWidth();
				if (ImGui::InputInt("##staticnoideWidth", &segedim0statwidth, 0)) {
					if (segedim0statwidth > 10) {
						im0stat.setWidth(segedim0statwidth);
					}
					else {
						im0stat.setWidth(10);
					}
				}
				ImGui::PopItemWidth();

				RegularModify::CursorPos(20); ImGui::Text("Height:");
				ImGui::SameLine();
				RegularModify::CursorPos(270);
				ImGui::PushItemWidth(fmax(im0stat.getImOut().getSurface()->w, 400) - 290);
				int segedim0statheight = im0stat.getHeight();
				if (ImGui::InputInt("##staticnoideHeight", &segedim0statheight, 0)) {
					if (segedim0statheight > 10) {
						im0stat.setHeight(segedim0statheight);
					}
					else {
						im0stat.setHeight(10);
					}
				}
				ImGui::PopItemWidth();

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Colors:");
				int misc_flags = ImGuiColorEditFlags_AlphaPreviewHalf;

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

					ImGui::PushStyleColor(ImGuiCol_PopupBg, Colors[ColorEnum::TEXT_DARK]);
					ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::COLORPICKER_FRAMEBG]);
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
								im0stat.color[i] = ImColor(im0stat.saved_palette[n].x, im0stat.saved_palette[n].y, im0stat.saved_palette[n].z, im0stat.color[i].w);
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

			ImGui::BeginChild("Buttons_StaticNoise", ImVec2(max(im0stat.getImOut().getSurface()->w, 400), 80)); {
				//--------button

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton_3Col();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##StaticNoise", ImVec2(150, 50))) {
					SDL_Surface* source = im0stat.getImOut().getSurface();
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
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im0stat.getImOut().getSurface()->w - 170));
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
			SetBasicUI_5Col_2Var();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Magnify", ImVec2(max(im1mag.getImOut().getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Magnify");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();
			ImGui::NewLine();

			im1mag.editableDrawImage(imfVec[selectedImFVec[0]].iof==iofImage ? imfVec[selectedImFVec[0]].im : imfVec[selectedImFVec[0]].f.getImages()[0]);

			ImGui::NewLine();

			ImGui::BeginChild("Buttons_Magnify", ImVec2(max(im1mag.getImOut().getSurface()->w, 400), 80)); {
				//--------button

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton_3Col();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##Magnify", ImVec2(150, 50))) {

					//image
					if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {

						SDL_Surface* source = im1mag.getImOut().getSurface();
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
					}

					//folder
					else {
						Folder segedFolder;
						for (int i = 0; i < selectedImFVec.size(); i++) {
							if (imfVec[selectedImFVec[i]].iof == iofImage) {
								segedFolder.Append(imfVec[selectedImFVec[i]].im);
							}
							else if (imfVec[selectedImFVec[i]].iof == iofFolder) {
								segedFolder.Append(imfVec[selectedImFVec[i]].f);
							}
						}

						Folder savedFolder;

						for (int i = 0; i < segedFolder.getImages().size(); i++) {

							if (segedFolder.getImages()[i].getSurface()->w >= segedFolder.getImages()[0].getSurface()->w &&
								segedFolder.getImages()[i].getSurface()->h >= segedFolder.getImages()[0].getSurface()->h) {

								im1mag.setImage(segedFolder.getImages()[i]);
								im1mag.MagnifyMethod(segedFolder.getImages()[i]);

								SDL_Surface* source = im1mag.getImOut().getSurface();
								SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
									source->w, source->h, source->format->BitsPerPixel, source->format->format);
								if (destination != nullptr) {
									SDL_BlitSurface(source, nullptr, destination, nullptr);
								}

								Image imseged;
								imseged.setSurface(destination);
								imseged.textureFromSurface();

								savedFolder.pushImages(imseged);
							}
						}

						savedFolder.createIconImageFromImages();

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						imfseged.f = savedFolder;

						imfVec.push_back(imfseged);

						im1mag.setImage(segedFolder.getImages()[0]);
						im1mag.MagnifyMethod(segedFolder.getImages()[0]);
					}
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im1mag.getImOut().getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::BLURENUM: {//---------------------------------------------------------------------------------------------------
			SetBasicUI_5Col_2Var();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Blur", ImVec2(max(im1blur.getImOut().getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Blur");
				imFo->Scale = 1.f;	ImGui::PopFont();

			}

			ImGui::EndChild();

			ImGui::NewLine();
			im1blur.getImOut().drawImage();
			ImGui::NewLine();

			ImGui::BeginChild("Values_Blur", ImVec2(max(im1blur.getImOut().getSurface()->w, 400), 200), false); {

				ImGui::NewLine();


				ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_BLUE]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_BLUE_HOVERED]);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_BLUE_ACTIVE]);
				ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				RegularModify::CursorPos((max(im1blur.getImOut().getSurface()->w, 400) / 2.0f) - (150.0f / 2.0f));

				if (ImGui::Button("Regenerate##Blur", ImVec2(150, 50))) {

					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].im);
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						im1blur.BlurMethod(imfVec[selectedImFVec[0]].f.getImages()[0]);
					}
				}

				ImGui::PopStyleColor(4);
				imFo->Scale = 1.f; ImGui::PopFont();
				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Blur's type: "); ImGui::NewLine();
				RegularModify::CursorPos(20);

				char* segedBlurRadioNames[] = { "Box","Gaussian" };
				int segedBlurType = im1blur.getBlurType();
				for (int i = 0; i < sizeof(segedBlurRadioNames) / sizeof(char*); i++) {
					ImGui::SameLine();
					if (ImGui::RadioButton(segedBlurRadioNames[i], &segedBlurType, i)) {
						im1blur.setBlurType((Image1Blur::BlurTypeEnum)segedBlurType);
					}
				}

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("The size of the Blur radius:");
				ImGui::SameLine();
				RegularModify::CursorPos(220);
				ImGui::PushItemWidth(fmax(im1blur.getImOut().getSurface()->w, 400) - 240);

				int segedBlurSize = im1blur.getBlurSize();
				if (ImGui::InputInt("##BlurSize", &segedBlurSize, 0)) {
					if (segedBlurSize < 1) {
						segedBlurSize = 1;
					}
					if (300 * 300 < im1blur.getImOut().getSurface()->w * im1blur.getImOut().getSurface()->h && segedBlurSize > 5) {
						segedBlurSize = 5;
					}
					else if (segedBlurSize > 9) {
						segedBlurSize = 9;
					}
					im1blur.setBlurSize(segedBlurSize);
				}
				
				ImGui::NewLine();
			}

			ImGui::EndChild();
			ImGui::NewLine();

			//--------button

			ImGui::BeginChild("Buttons_Blur", ImVec2(max(im1blur.getImOut().getSurface()->w, 400), 80)); {

				ImGui::NewLine();

				RegularModify::CursorPos(20);

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);

				PushStyleColorGreenButton_3Col();

				if (ImGui::Button("Load##Blur", ImVec2(150, 50))) {

					//image
					if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {

						SDL_Surface* source = im1blur.getImOut().getSurface();
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
					}

					//folder
					else {
						Folder segedFolder;
						for (int i = 0; i < selectedImFVec.size(); i++) {
							if (imfVec[selectedImFVec[i]].iof == iofImage) {
								segedFolder.Append(imfVec[selectedImFVec[i]].im);
							}
							else if (imfVec[selectedImFVec[i]].iof == iofFolder) {
								segedFolder.Append(imfVec[selectedImFVec[i]].f);
							}
						}

						Folder savedFolder;

						for (int i = 0; i < segedFolder.getImages().size(); i++) {

							im1blur.setImage(segedFolder.getImages()[i]);
							im1blur.BlurMethod(segedFolder.getImages()[i]);

							SDL_Surface* source = im1blur.getImOut().getSurface();
							SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
								source->w, source->h, source->format->BitsPerPixel, source->format->format);
							if (destination != nullptr) {
								SDL_BlitSurface(source, nullptr, destination, nullptr);
							}

							Image imseged;
							imseged.setSurface(destination);
							imseged.textureFromSurface();

							savedFolder.pushImages(imseged);

						}

						savedFolder.createIconImageFromImages();

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						imfseged.f = savedFolder;

						imfVec.push_back(imfseged);

						im1blur.setImage(segedFolder.getImages()[0]);
						im1blur.BlurMethod(segedFolder.getImages()[0]);
					}
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im1blur.getImOut().getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}

		case CMyApp::COLORENUM: {//---------------------------------------------------------------------------------------------------
			SetBasicUI_5Col_2Var();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Color", ImVec2(max(im1col.getImOut().getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Modify image's color");
				imFo->Scale = 1.f;	ImGui::PopFont();

			}

			ImGui::EndChild();

			ImGui::NewLine();
			im1col.getImOut().drawImage();
			ImGui::NewLine();

			ImGui::BeginChild("Color_Values", ImVec2(max(im1col.getImOut().getSurface()->w, 400), 75), false); {

				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Color modificatin type: "); ImGui::NewLine();
				RegularModify::CursorPos(20);

				char* segedColorRadioNames[] = { "Null","GreyScale","Red","Green","Blue","Invert"};
				int segedColorType = im1col.getImcType();
				for (int i = 0; i < sizeof(segedColorRadioNames) / sizeof(char*); i++) {
					ImGui::SameLine();
					if (ImGui::RadioButton(segedColorRadioNames[i], &segedColorType, i)) {
						im1col.setImcType((Image1Color::ImageColorType)segedColorType);
						if (imfVec[selectedImFVec[0]].iof == iofImage) {
							im1col.ColorMethod(imfVec[selectedImFVec[0]].im);
						}
						else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							im1col.ColorMethod(imfVec[selectedImFVec[0]].f.getImages()[0]);
						}
					}
				}

			}
			ImGui::EndChild();
			ImGui::NewLine();

			ImGui::BeginChild("Buttons_Color", ImVec2(max(im1col.getImOut().getSurface()->w, 400), 80)); {
				//--------button

				ImGui::NewLine();

				RegularModify::CursorPos(20);

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);

				PushStyleColorGreenButton_3Col();

				if (ImGui::Button("Load##Color", ImVec2(150, 50))) {

					//image
					if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {

						SDL_Surface* source = im1col.getImOut().getSurface();
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
					}

					//folder
					else {
						Folder segedFolder;
						for (int i = 0; i < selectedImFVec.size(); i++) {
							if (imfVec[selectedImFVec[i]].iof == iofImage) {
								segedFolder.Append(imfVec[selectedImFVec[i]].im);
							}
							else if (imfVec[selectedImFVec[i]].iof == iofFolder) {
								segedFolder.Append(imfVec[selectedImFVec[i]].f);
							}
						}

						Folder savedFolder;

						for (int i = 0; i < segedFolder.getImages().size(); i++) {

								im1col.setImage(segedFolder.getImages()[i]);
								im1col.ColorMethod(segedFolder.getImages()[i]);

								SDL_Surface* source = im1col.getImOut().getSurface();
								SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
									source->w, source->h, source->format->BitsPerPixel, source->format->format);
								if (destination != nullptr) {
									SDL_BlitSurface(source, nullptr, destination, nullptr);
								}

								Image imseged;
								imseged.setSurface(destination);
								imseged.textureFromSurface();

								savedFolder.pushImages(imseged);
						}

						savedFolder.createIconImageFromImages();

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						imfseged.f = savedFolder;

						imfVec.push_back(imfseged);

						im1col.setImage(segedFolder.getImages()[0]);
						im1col.ColorMethod(segedFolder.getImages()[0]);
					}					
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im1col.getImOut().getSurface()->w - 170));
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
			SetBasicUI_5Col_2Var();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name_Save", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Save");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::NewLine();
			if (imfVec[selectedImFVec[0]].iof == iofImage) {
				imfVec[selectedImFVec[0]].im.drawImage();
			}
			else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
				imfVec[selectedImFVec[0]].f.getImages()[0].drawImage();
			}
			ImGui::NewLine();

			ImGui::BeginChild("Values_Save", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->w, 400), 75), false); {

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Path:");
				ImGui::PushItemWidth(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w - 40 : imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->w - 40, 360));
				RegularModify::CursorPos(20); ImGui::InputText("##SavePath", outstr, IM_ARRAYSIZE(outstr));
				ImGui::PopItemWidth();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			ImGui::BeginChild("Buttons_Save", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->w, 400), 80)); {
				//--------button

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton_3Col();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Save", ImVec2(150, 50))) {

					struct stat sb;
					//available path (there isn't a file with this name already)
					if (stat(outstr, &sb) != 0) {
						if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {
							if (im1sav.Save(imfVec[selectedImFVec[0]].im,outstr)) { 
								currentMenuEnum = OPERATIONSENUM;
							}
							else {
								ImGui::OpenPopup("Save##Error");
							}
						}
						else {
							ImGui::OpenPopup("Save##InvalidPath");
						}
					}
					//folder path
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

									im1sav.Save(imfVec[selectedImFVec[i]].im, cstr); 

									delete[] cstr;
									n++;
								}
								else if (imfVec[selectedImFVec[i]].iof == iofFolder) {

									im1sav.SaveFolder(imfVec[selectedImFVec[i]].f, path,n);
									n += imfVec[selectedImFVec[i]].f.getImages().size();
								}
							}
							currentMenuEnum = OPERATIONSENUM;
						}
					}
					//already existing image path
					else {
						ImGui::OpenPopup("Save##ImageOverritePath");
					}
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w - 170 : imfVec[selectedImFVec[0]].f.getImages()[0].getSurface()->w - 170));
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
			SetBasicUI_5Col_2Var();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);
			ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_BLUE]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_BLUE_HOVERED]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_BLUE_ACTIVE]);

			ImGui::BeginChild("Name_SSIM", ImVec2(max(im2ssim.getImOut().getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("SSIM");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();
			ImGui::NewLine();

			im2ssim.getImOut().drawImage();

			ImGui::NewLine();

			ImGui::BeginChild("Values_SSIM", ImVec2(max(im2ssim.getImOut().getSurface()->w, 400), 210), false); {

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

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("The size of the SSIM slices:");
				ImGui::SameLine();
				RegularModify::CursorPos(max(im2ssim.getImOut().getSurface()->w, 400) - 120);
				ImGui::PushItemWidth(80);
				int segedSsimSize = im2ssim.getSsimSize();
				if (ImGui::InputInt("##Size", &segedSsimSize, 0)) {
					if (segedSsimSize < 1) {
						segedSsimSize = 1;
					}
					if (segedSsimSize > fmin(im2ssim.getImOut().getSurface()->w / 2, im2ssim.getImOut().getSurface()->h / 2)) {
						segedSsimSize = fmin(im2ssim.getImOut().getSurface()->w / 2, im2ssim.getImOut().getSurface()->h / 2);
					}

					im2ssim.setSsimSize(segedSsimSize);
				}
				RegularModify::ShowHelpMarker("What should be the size of the slices that the SSIM algorithm uses");

				ImGui::NewLine();
				ImGui::PopItemWidth();

				ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
				RegularModify::CursorPos((max(im2ssim.getImOut().getSurface()->w, 400) / 2.0f) - (150.0f / 2.0f));
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Regenerate##SSIM", ImVec2(150, 50))) {
					if (imfVec[selectedImFVec[0]].iof == iofImage) {
						if (imfVec[selectedImFVec[1]].iof == iofImage) {
							im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].im);
						}
						else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
							im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.getImages()[0]);
						}
					}
					else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
						if (imfVec[selectedImFVec[1]].iof == iofImage) {
							im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].im);
						}
						else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
							im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].f.getImages()[0]);
						}
					}
				}
				imFo->Scale = 1.f;
				ImGui::PopStyleColor();
				ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			ImGui::BeginChild("Buttons_SSIM", ImVec2(max(im2ssim.getImOut().getSurface()->w, 400), 80)); {
				//--------button

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton_3Col();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);

				if (ImGui::Button("Load##SSIM", ImVec2(150, 50))) {

					//images
					if (imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofImage) {

						SDL_Surface* source = im2ssim.getImOut().getSurface();
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

					}

					//folder (+image/folder)
					else {

						Folder segedFolder;

						if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							segedFolder.Append(imfVec[selectedImFVec[0]].f);
						}
						else if (imfVec[selectedImFVec[0]].iof == iofImage) {
							segedFolder.Append(imfVec[selectedImFVec[1]].f);
						}

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

							for (int i = 0; i < fPointer->getImages().size(); i++) {
								if (fPointer->getImages()[i].getSurface()->w == imPointer->getSurface()->w && fPointer->getImages()[i].getSurface()->h == imPointer->getSurface()->h) {

									im2ssim.setImage(*imPointer);
									im2ssim.SSIMMethod(*imPointer, fPointer->getImages()[i]);
									
									SDL_Surface* source = im2ssim.getImOut().getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.pushImages(imseged);
								}
							}

							im2ssim.setImage(segedFolder.getImages()[0]);
							if (imfVec[selectedImFVec[0]].iof == iofImage) {
								im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.getImages()[0]);
							}
							else {
								im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].im);
							}
						}

						//folders
						else {

							for (int i = 0; i < (imfVec[selectedImFVec[0]].f.getImages().size() <= imfVec[selectedImFVec[1]].f.getImages().size() ? imfVec[selectedImFVec[0]].f.getImages().size() : imfVec[selectedImFVec[1]].f.getImages().size()); i++) {
								if (imfVec[selectedImFVec[0]].f.getImages()[i].getSurface()->w == imfVec[selectedImFVec[1]].f.getImages()[i].getSurface()->w && imfVec[selectedImFVec[0]].f.getImages()[i].getSurface()->h == imfVec[selectedImFVec[1]].f.getImages()[i].getSurface()->h) {

									im2ssim.setImage(imfVec[selectedImFVec[0]].f.getImages()[i]);
									im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].f.getImages()[i], imfVec[selectedImFVec[1]].f.getImages()[i]);

									SDL_Surface* source = im2ssim.getImOut().getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.pushImages(imseged);
								}
							}

							im2ssim.setImage(segedFolder.getImages()[0]);
							im2ssim.SSIMMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].f.getImages()[0]);
						}

						ImageFolder imfseged; 
						imfseged.iof = iofFolder;
						savedFolder.createIconImageFromImages();
						imfseged.f = savedFolder;

						imfVec.push_back(imfseged);

					}
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im2ssim.getImOut().getSurface()->w - 170));
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
			SetBasicUI_5Col_2Var();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);
			
			ImGui::BeginChild("Name_Merge", ImVec2(max(im2merge.getImOut().getSurface()->w, 400), 65), false); {

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Merge");
				imFo->Scale = 1.f;	ImGui::PopFont();
			}
			ImGui::EndChild();

			ImGui::NewLine();

			Image* im1 = nullptr;
			Image* im2 = nullptr;
			if (imfVec[selectedImFVec[0]].iof == iofImage) {
				if (imfVec[selectedImFVec[1]].iof == iofImage) {
					im1 = &imfVec[selectedImFVec[0]].im;
					im2 = &imfVec[selectedImFVec[1]].im;
				}
				else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
					im1 = &imfVec[selectedImFVec[0]].im;
					Image segedMerge1 = imfVec[selectedImFVec[1]].f.getImages()[0];
					im2 = &segedMerge1;
				}
			}
			else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
				if (imfVec[selectedImFVec[1]].iof == iofImage) {
					Image segedMerge2 = imfVec[selectedImFVec[0]].f.getImages()[0];
					im1 = &segedMerge2;
					im2 = &imfVec[selectedImFVec[1]].im;
				}
				else if (imfVec[selectedImFVec[1]].iof == iofFolder) {
					Image segedMerge3 = imfVec[selectedImFVec[0]].f.getImages()[0];
					im1 = &segedMerge3;
					Image segedMerge4 = imfVec[selectedImFVec[1]].f.getImages()[0];
					im2 = &segedMerge4;
				}
			}

			im2merge.editableDrawImage(*im1, *im2);

			ImGui::NewLine();

			ImGui::BeginChild("Values_Merge", ImVec2(max(im2merge.getImOut().getSurface()->w, 400), 70), false); {

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("The slope of the transition:");
				ImGui::SameLine();
				RegularModify::CursorPos(200);
				ImGui::PushItemWidth(max(im2merge.getImOut().getSurface()->w, 400) - 220);
				float segedSlope = im2merge.getSlope();
				if (ImGui::SliderFloat("##slope", &segedSlope, 0.0f, 360.0f, "%.4f")) {
					im2merge.setSlope(segedSlope);
					im2merge.MergeMethod(*im1, *im2);
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
					im2merge.MergeMethod(*im1, *im2);
				}
				ImGui::PopItemWidth();
			}

			ImGui::EndChild();
			ImGui::NewLine();

			ImGui::BeginChild("Buttons_Merge", ImVec2(max(im2merge.getImOut().getSurface()->w, 400), 80)); {
				//--------button

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton_3Col();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##Merge", ImVec2(150, 50))) {

					//images
					if (imfVec[selectedImFVec[0]].iof == iofImage && imfVec[selectedImFVec[1]].iof == iofImage) {

						SDL_Surface* source = im2merge.getImOut().getSurface();
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

					}

					//folder (+image/folder)
					else {

						Folder segedFolder;

						if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							segedFolder.Append(imfVec[selectedImFVec[0]].f);
						}
						else if (imfVec[selectedImFVec[0]].iof == iofImage) {
							segedFolder.Append(imfVec[selectedImFVec[1]].f);
						}

						Folder savedFolder;

						//folder + image
						if ((imfVec[selectedImFVec[0]].iof == iofImage) || (imfVec[selectedImFVec[1]].iof == iofImage)) {
							Image* imPointer;
							Folder* fPointer;
							if (imfVec[selectedImFVec[0]].iof == iofImage) {
								imPointer = &imfVec[selectedImFVec[0]].im;
								fPointer = &imfVec[selectedImFVec[1]].f;
							}
							else if(imfVec[selectedImFVec[0]].iof == iofFolder){
								imPointer = &imfVec[selectedImFVec[1]].im;
								fPointer = &imfVec[selectedImFVec[0]].f;
							}

							for (int i = 0; i < fPointer->getImages().size(); i++) {
								if (fPointer->getImages()[i].getSurface()->w == imPointer->getSurface()->w && fPointer->getImages()[i].getSurface()->h == imPointer->getSurface()->h) {

									im2merge.setImage(*imPointer);
									if (imfVec[selectedImFVec[0]].iof == iofImage) {
										im2merge.MergeMethod(*imPointer, fPointer->getImages()[i]);
									}
									else if(imfVec[selectedImFVec[0]].iof == iofFolder){
										im2merge.MergeMethod(fPointer->getImages()[i], *imPointer);
									}

									SDL_Surface* source = im2merge.getImOut().getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.pushImages(imseged);
								}
							}

							im2merge.setImage(segedFolder.getImages()[0]);
							if (imfVec[selectedImFVec[0]].iof == iofImage) {
								im2merge.MergeMethod(imfVec[selectedImFVec[0]].im, imfVec[selectedImFVec[1]].f.getImages()[0]);
							}
							else {
								im2merge.MergeMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].im);
							}
						}

						//folders
						else {

							for (int i = 0; i < (imfVec[selectedImFVec[0]].f.getImages().size() <= imfVec[selectedImFVec[1]].f.getImages().size() ? imfVec[selectedImFVec[0]].f.getImages().size() : imfVec[selectedImFVec[1]].f.getImages().size()); i++) {
								if (imfVec[selectedImFVec[0]].f.getImages()[i].getSurface()->w == imfVec[selectedImFVec[1]].f.getImages()[i].getSurface()->w && imfVec[selectedImFVec[0]].f.getImages()[i].getSurface()->h == imfVec[selectedImFVec[1]].f.getImages()[i].getSurface()->h) {

									im2merge.setImage(imfVec[selectedImFVec[0]].f.getImages()[i]);
									im2merge.MergeMethod(imfVec[selectedImFVec[0]].f.getImages()[i], imfVec[selectedImFVec[1]].f.getImages()[i]);

									SDL_Surface* source = im2merge.getImOut().getSurface();
									SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
										source->w, source->h, source->format->BitsPerPixel, source->format->format);
									if (destination != nullptr) {
										SDL_BlitSurface(source, nullptr, destination, nullptr);
									}

									Image imseged;
									imseged.setSurface(destination);
									imseged.textureFromSurface();

									savedFolder.pushImages(imseged);
								}
							}
							im2merge.setImage(segedFolder.getImages()[0]);
							im2merge.MergeMethod(imfVec[selectedImFVec[0]].f.getImages()[0], imfVec[selectedImFVec[1]].f.getImages()[0]);
						}

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						savedFolder.createIconImageFromImages();
						imfseged.f = savedFolder;

						imfVec.push_back(imfseged);

					}
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(400 - 170, im2merge.getImOut().getSurface()->w - 170));
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

		currentMenuEnum = OPERATIONSENUM;
	}
	ImGui::PopStyleColor(3);
	imFo->Scale = 1.f;
	ImGui::PopFont();
}


void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

}

void CMyApp::SetBasicUI_5Col_2Var() const {

	ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors[ColorEnum::CHILD_BG]);
	ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_DARK]);
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, Colors[ColorEnum::TEXT_HINT]);
	ImGui::PushStyleColor(ImGuiCol_Border, Colors[ColorEnum::POPUP_BORDER]);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, Colors[ColorEnum::POPUP_BG]);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);
}

void CMyApp::PushStyleColorGreenButton_3Col() const {

	ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREEN]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREEN_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_GREEN_ACTIVE]);
}

void CMyApp::TestMethod() {

	//1.1
	char* textWhite = "White.jpg";

	im0load.setLoadType(Image0FromFile::PICTURE);
	ImageFolder imagefolderWhite = im0load.Load(textWhite,false);

	if (imagefolderWhite.im.getSurface() != nullptr) {
		std::cout << "1.1 Success" << std::endl;
	}
	else {
		std::cout << "1.1 Failed" << std::endl;
	}

	//1.2
	char* textFolder = "TestImages";
	bool success = true;

	im0load.setLoadType(Image0FromFile::FOLDER);
	ImageFolder imagefolderFolder = im0load.Load(textFolder,false);

	if (imagefolderFolder.f.getImages().size() == 0) {
		success = false;
	}
	for (Image i : imagefolderFolder.f.getImages()) {
		if (i.getSurface() == nullptr) {
			success = false;
		}
	}

	if (success) {
		std::cout << "1.2 Success" << std::endl;
	}
	else {
		std::cout << "1.2 Failed" << std::endl;
	}
	success = true;
	
	//1.3
	char* textIncorrect = "IncorrectPath.jpg";

	im0load.setLoadType(Image0FromFile::PICTURE);
	ImageFolder imagefolderIncorrect = im0load.Load(textIncorrect,false);

	if (imagefolderIncorrect.iof == iofEmpty) {
		std::cout << "1.3 Success" << std::endl;
	}
	else {
		std::cout << "1.3 Failed" << std::endl;
	}

	//2
	ImColor white = ImColor(255, 255, 255, 255);
	ImColor black = ImColor(0, 0, 0, 255);

	im0stat.StaticMethod();
	if (im0stat.getImOut().getSurface()->w != im0stat.getWidth() || im0stat.getImOut().getSurface()->h != im0stat.getHeight()) {
		success = false;
	}

	for (int i = 0; i < im0stat.getWidth(); i++) {
		for (int j = 0; j < im0stat.getHeight(); j++) {
			if (SurfaceModify::GetColor(i, j, im0stat.getImOut().getSurface()) != white && SurfaceModify::GetColor(i, j, im0stat.getImOut().getSurface()) != black  )   {
				success = false;
			}
		}
	}

	Image staticBlackWhite = im0stat.getImOut();

	if (success) {
		std::cout << "2 Success" << std::endl;
	}
	else {
		std::cout << "2 Failed" << std::endl;
	}
	success = true;

	//3
	ImColor red = ImColor(255, 0, 0, 255);

	im1mag.setImage(imagefolderWhite.im);
	im1mag.MagnifyMethod(imagefolderWhite.im);

	if (SurfaceModify::GetColor(0, 0, im1mag.getImOut().getSurface()) == red) {
		std::cout << "3 Success" << std::endl;
	}
	else {
		std::cout << "3 Failed" << std::endl;
	}

	//4.1
	im1blur.setBlurSize(5);
	im1blur.setBlurType(im1blur.blurBox);
	im1blur.setImage(staticBlackWhite);
	im1blur.BlurMethod(staticBlackWhite);
	Uint8 r, g, b;

	bool notonlyBlackWhite = false;

	for (int i = 0; i < im1blur.getImOut().getSurface()->w ; i++) {
		for (int j = 0; j < im1blur.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i,j, im1blur.getImOut().getSurface()), im1blur.getImOut().getSurface()->format, &r, &g, &b);
			if (r!=b || b!=g) {
				success = false;
			}
			if (r != 255 && r != 0) {
				notonlyBlackWhite = true;
			}
		}
	}

	success = success && notonlyBlackWhite;

	if (success) {
		std::cout << "4.1 Success" << std::endl;
	}
	else {
		std::cout << "4.1 Failed" << std::endl;
	}
	success = true;
	notonlyBlackWhite = false;

	//4.2
	im1blur.setBlurType(im1blur.blurGauss);
	im1blur.BlurMethod(staticBlackWhite);

	for (int i = 0; i < im1blur.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im1blur.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im1blur.getImOut().getSurface()), im1blur.getImOut().getSurface()->format, &r, &g, &b);
			if (r != b || b != g) {
				success = false;
			}
			if (r != 255 && r != 0) {
				notonlyBlackWhite = true;
			}
		}
	}

	success = success && notonlyBlackWhite;

	if (success) {
		std::cout << "4.2 Success" << std::endl;
	}
	else {
		std::cout << "4.2 Failed" << std::endl;
	}
	success = true;
	notonlyBlackWhite = false;

	//5.1
	im1col.setImcType(im1col.ImageColorType::GreyScale);
	im1col.setImage(staticBlackWhite);
	im1col.ColorMethod(staticBlackWhite);

	for (int i = 0; i < im1col.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im1col.getImOut().getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) != SurfaceModify::GetColor(i, j, staticBlackWhite.getSurface())) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.1 Success" << std::endl;
	}
	else {
		std::cout << "5.1 Failed" << std::endl;
	}
	success = true;

	//5.2
	im0stat.setWidth(imagefolderWhite.im.getSurface()->w);
	im0stat.setHeight(imagefolderWhite.im.getSurface()->h);

	im0stat.StaticMethod();
	Image staticBlackWhite5 = im0stat.getImOut();

	im0stat.color[0] =  red;
	im0stat.StaticMethod();
	Image staticRedWhite = im0stat.getImOut();

	im1col.setImage(staticRedWhite);
	im1col.ColorMethod(staticRedWhite);

	for (int i = 0; i < im1col.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im1col.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()), im1col.getImOut().getSurface()->format, &r, &g, &b);
			if (r != b || b != g) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.2 Success" << std::endl;
	}
	else {
		std::cout << "5.2 Failed" << std::endl;
	}
	success = true;

	//5.3.a
	im1col.setImcType(im1col.ImageColorType::Red);
	im1col.setImage(imagefolderWhite.im);
	im1col.ColorMethod(imagefolderWhite.im);

	for (int i = 0; i < im1col.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im1col.getImOut().getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) != red) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.3.a Success" << std::endl;
	}
	else {
		std::cout << "5.3.a Failed" << std::endl;
	}
	success = true;

	//5.3.b
	ImColor green = ImColor(0, 255, 0, 255);
	im1col.setImcType(im1col.ImageColorType::Green);
	im1col.ColorMethod(imagefolderWhite.im);
		
	for (int i = 0; i < im1col.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im1col.getImOut().getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) != green) {
				success = false;
			}
		}
	}



	if (success) {
		std::cout << "5.3.b Success" << std::endl;
	}
	else {
		std::cout << "5.3.b Failed" << std::endl;
	}
	success = true;

	//5.3.c
	ImColor blue = ImColor(0, 0, 255, 255);
	im1col.setImcType(im1col.Blue);
	im1col.ColorMethod(imagefolderWhite.im);

	for (int i = 0; i < im1col.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im1col.getImOut().getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) != blue) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.3.c Success" << std::endl;
	}
	else {
		std::cout << "5.3.c Failed" << std::endl;
	}
	success = true;

	//5.4
	im1col.setImcType(im1col.Inverted);
	im1col.setImage(staticBlackWhite);
	im1col.ColorMethod(staticBlackWhite);

	for (int i = 0; i < im1col.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im1col.getImOut().getSurface()->h; j++) {

			if (!((SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) == white && SurfaceModify::GetColor(i, j, staticBlackWhite.getSurface()) == black) || (SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) == black && SurfaceModify::GetColor(i, j, staticBlackWhite.getSurface()) == white)) ||
				(SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) != white && SurfaceModify::GetColor(i, j, im1col.getImOut().getSurface()) != black)) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "5.4 Success" << std::endl;
	}
	else {
		std::cout << "5.4 Failed" << std::endl;
	}
	success = true;

	//6.1
	im2ssim.setImage(staticRedWhite);

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(0);

	im2ssim.SSIMMethod(staticRedWhite, imagefolderWhite.im);

	for (int i = 0; i < im2ssim.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface()), im2ssim.getImOut().getSurface()->format, &r, &g, &b);
			if (r != b || b != g || (SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface())==white && SurfaceModify::GetColor(i, j, staticRedWhite.getSurface())!=white )) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "6.1 Success" << std::endl;
	}
	else {
		std::cout << "6.1 Failed" << std::endl;
	}
	success = true;

	//6.2.a

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(1);

	im2ssim.SSIMMethod(staticBlackWhite5, imagefolderWhite.im);

	for (int i = 0; i < im2ssim.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface()), im2ssim.getImOut().getSurface()->format, &r, &g, &b);
			if (SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white && (r!=255)) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "6.2.a Success" << std::endl;
	}
	else {
		std::cout << "6.2.a Failed" << std::endl;
	}
	success = true;

	//6.2.b

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(2);

	im2ssim.SSIMMethod(staticBlackWhite5, imagefolderWhite.im);

	for (int i = 0; i < im2ssim.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface()), im2ssim.getImOut().getSurface()->format, &r, &g, &b);
			if (SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white && (g != 255)) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "6.2.b Success" << std::endl;
	}
	else {
		std::cout << "6.2.b Failed" << std::endl;
	}
	success = true;

	//6.2.c

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(3);

	im2ssim.SSIMMethod(staticBlackWhite5, imagefolderWhite.im);

	for (int i = 0; i < im2ssim.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface()), im2ssim.getImOut().getSurface()->format, &r, &g, &b);
			if (SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white && (b != 255)) {
				success = false;
			}
		}
	}

	if (success) {
		std::cout << "6.2.c Success" << std::endl;
	}
	else {
		std::cout << "6.2.c Failed" << std::endl;
	}
	success = true;

	//6.3

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(4);

	im2ssim.SSIMMethod(staticRedWhite, imagefolderWhite.im);

	for (int i = 0; i < im2ssim.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.getImOut().getSurface()->h; j++) {

			if (SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface()) != SurfaceModify::GetColor(i, j, staticRedWhite.getSurface())) {
				success = false;
			}
			
		}
	}

	if (success) {
		std::cout << "6.3 Success" << std::endl;
	}
	else {
		std::cout << "6.3 Failed" << std::endl;
	}
	success = true;

	//6.4

	im2ssim.setSsimSize(1);
	im2ssim.setSsimColor(5);

	im2ssim.SSIMMethod(staticBlackWhite5, imagefolderWhite.im);

	for (int i = 0; i < im2ssim.getImOut().getSurface()->w; i++) {
		
		for (int j = 0; j < im2ssim.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface()), im2ssim.getImOut().getSurface()->format, &r, &g, &b);
			if ((SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == white && b - (g + r) < 245) ||
				(SurfaceModify::GetColor(i, j, staticBlackWhite5.getSurface()) == black && r - (g + b) < 245)) {
				success = false;
			}

		}
	}

	if (success) {
		std::cout << "6.4 Success" << std::endl;
	}
	else {
		std::cout << "6.4 Failed" << std::endl;
	}
	success = true;

	//6.5

	im2ssim.setSsimSize(5);
	im2ssim.setSsimColor(0);

	im2ssim.SSIMMethod(staticBlackWhite5, imagefolderWhite.im);

	for (int i = 0; i < im2ssim.getImOut().getSurface()->w; i++) {
		for (int j = 0; j < im2ssim.getImOut().getSurface()->h; j++) {

			SDL_GetRGB(SurfaceModify::GetColor(i, j, im2ssim.getImOut().getSurface()), im2ssim.getImOut().getSurface()->format, &r, &g, &b);
			if (r != b || b != g) {
				success = false;
			}
			if (r != 255 && r != 0) {
				notonlyBlackWhite = true;
			}

		}
	}

	success = success && notonlyBlackWhite;

	if (success) {
		std::cout << "6.5 Success" << std::endl;
	}
	else {
		std::cout << "6.5 Failed" << std::endl;
	}
	success = true;
	notonlyBlackWhite = false;

	//7.1
	char* blackchar = "Black.jpg";
	im0load.setLoadType(Image0FromFile::PICTURE);
	ImageFolder blackTest = im0load.Load(blackchar,false);

	im2merge.setImage(imagefolderWhite.im);
	im2merge.setSlope(0);
	im2merge.MergeMethod(blackTest.im, imagefolderWhite.im);

	if (SurfaceModify::GetColor(0, 0, im2merge.getImOut().getSurface()) != black || SurfaceModify::GetColor(im2merge.getImOut().getSurface()->w-1, im2merge.getImOut().getSurface()->h-1, im2merge.getImOut().getSurface()) != white) {
		success = false;
	}

	if (success) {
		std::cout << "7.1 Success" << std::endl;
	}
	else {
		std::cout << "7.1 Failed" << std::endl;
	}
	success = true;

	//7.2
	im2merge.setSlope(180);
	im2merge.MergeMethod(blackTest.im, imagefolderWhite.im);

	if (SurfaceModify::GetColor(0, 0, im2merge.getImOut().getSurface()) != white || SurfaceModify::GetColor(im2merge.getImOut().getSurface()->w-1, im2merge.getImOut().getSurface()->h-1, im2merge.getImOut().getSurface()) != black) {
		success = false;
	}

	if (success) {
		std::cout << "7.2 Success" << std::endl;
	}
	else {
		std::cout << "7.2 Failed" << std::endl;
	}
	success = true;

	//8.1

	if (im1sav.Save(imagefolderWhite.im, "SaveTest/save.png")) {
		std::cout << "8.1 Success" << std::endl;
	}
	else {
		std::cout << "8.1 Failed" << std::endl;
	}

	//8.2

	if (im1sav.Save(imagefolderWhite.im, "SaveTest") == false) {
		std::cout << "8.2 Success" << std::endl;
	}
	else {
		std::cout << "8.2 Failed" << std::endl;
	}

	//8.3

	if (im1sav.Save(imagefolderWhite.im, "NotValidPath/save.png") == false) {
		std::cout << "8.3 Success" << std::endl;
	}
	else {
		std::cout << "8.3 Failed" << std::endl;
	}

	//8.4

	int n = 1;
	if (im1sav.SaveFolder(imagefolderFolder.f, "SaveTest",n)) {
		std::cout << "8.4 Success" << std::endl;
	}
	else {
		std::cout << "8.4 Failed" << std::endl;
	}

	//8.5

	if (im1sav.SaveFolder(imagefolderFolder.f, "SaveTest/save2.png",n) == false) {
		std::cout << "8.5 Success" << std::endl;
	}
	else {
		std::cout << "8.5 Failed" << std::endl;
	}

	//8.6

	if (im1sav.SaveFolder(imagefolderFolder.f, "NotValidPath", n) == false) {
		std::cout << "8.6 Success" << std::endl;
	}
	else {
		std::cout << "8.6 Failed" << std::endl;
	}

} 

void Image::setSurface(SDL_Surface* surface) {
	this->surface = surface;
}

void Image::textureFromSurface() {
	if (texture!=0) {
		glDeleteTextures(1, &texture); 
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

void Image::drawImage() const {
	ImGui::Image((void*)(intptr_t)texture, ImVec2(surface->w, surface->h));
}

void Image::drawImage(int size, bool selected) const {
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));
	ImGui::ImageButton((void*)(intptr_t)texture, ImVec2(size, size), ImVec2(0, 0), ImVec2(1,1), selected ? 5 : 0, ImColor(0, 0, 0, 255));
	ImGui::PopStyleColor(2);
}

void Image::freeSurface() {
	if (surface != nullptr) {
		SDL_FreeSurface(surface);
		surface = nullptr;
	}
}

Folder::Folder(void){
	iconN = 0;
}

void Folder::Append(Image app) {
	images.push_back(app);
}

void Folder::Append(Folder app) {
	for (int i = 0; i < app.images.size(); i++) {
		images.push_back(app.images[i]);
	}
}

void Folder::createIconImageFromImages() {

	SDL_Surface* source = images[iconN].getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);
	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}
	icon.setSurface(destination);

	int border = (icon.getSurface()->w < 40 || icon.getSurface()->h < 40) ? 1 : 20;
	for (int i = 0; i < icon.getSurface()->w; i++) {
		for (int j = 0; j < icon.getSurface()->h; j++) {
			if (i < border || j < border || icon.getSurface()->w - (i + 1) < border || icon.getSurface()->h - (j + 1) < border) {
				SurfaceModify::PutPixel(i, j, ImColor(255,255,0,255), icon.getSurface());
			}
		}
	}
	icon.textureFromSurface();
}

Image0FromFile::Image0FromFile(void) {
	loadType = LoadTypeEnum::PICTURE;
}

ImageFolder Image0FromFile::Load(char* s, bool popup) {
	ImageFolder imfo;

	switch (loadType)
	{
	case Image0FromFile::LoadTypeEnum::PICTURE: {

		if (RegularModify::Verify(s)) {
			Image sizeVerify;
			sizeVerify.setSurface(IMG_Load(s));
			if (sizeVerify.getSurface() != nullptr && sizeVerify.getSurface()->h >= 10 && sizeVerify.getSurface()->w >= 10) {
				sizeVerify.textureFromSurface();
				imfo.im = sizeVerify;
				imfo.iof = iofImage;
			}
			else if(popup){
				ImGui::OpenPopup("Load##InvalidSize");
			}

		}
		else if(popup){
			ImGui::OpenPopup("Load##InvalidPath");
		}
		break;
	}

	case Image0FromFile::LoadTypeEnum::FOLDER: {

		Folder fold;
		std::string path = s;
		path += "/*";
		std::string path2 = s;

		WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFileA(path.c_str(), &ffd);

		if (hFind == INVALID_HANDLE_VALUE) {
			if (popup) {
				ImGui::OpenPopup("Load##InvalidPath");
			}
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
				if (sizeVerify.getSurface() != nullptr && sizeVerify.getSurface()->h >= 10 && sizeVerify.getSurface()->w >= 10) {
					fold.pushImages(sizeVerify);
				}
				else if(popup){
					ImGui::OpenPopup("Load##FolderInvalidSize");
				}
			}
			delete[] cstr;
		}
		FindClose(hFind);

		if (fold.getImages().size() == 0) {
			if (popup) {
				ImGui::OpenPopup("Load##EmptyFolder");
			}
		}
		else {

			for (int i = 0; i < fold.getImages().size();i++) {
				fold.getImages()[i].textureFromSurface();
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

	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distr(0, color.size() - 1); 

	for (int i = 0; i < surf->w; i++) {
		for (int j = 0; j < surf->h; j++) {

			SurfaceModify::PutPixel(i, j, ImGui::ColorConvertFloat4ToU32(color[distr(gen)]), surf);
		}
	}
	imOut.setSurface(surf);
	imOut.textureFromSurface();
}

Image1::Image1(void) {}

void Image1::setImage(Image im) {

	imOut.freeSurface();

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
	updMove = false;
	updResize = false;
}


void Image1Magnify::Reset() {

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
	updMove = false;
	updResize = false;
}


void Image1Magnify::editableDrawImage(Image im) {

	ImFont* imFo = ImGui::GetFont();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::Image((void*)(intptr_t)imOut.getTexture(), ImVec2(imOut.getSurface()->w, imOut.getSurface()->h));
	if (ImGui::IsItemClicked()) {

		if (sqrt(pow( (io.MousePos.x - pos.x) - ((im.getSurface()->w - bigX)-(zoomTimes*zoomW)), 2) + pow((io.MousePos.y - pos.y) - ((im.getSurface()->h - bigY) - (zoomTimes * zoomH)), 2)) < 20) {
			smallChange = false;
			updResize = true;
			updMove = false;
		}
		else if(io.MousePos.x - pos.x >= (im.getSurface()->w - bigX) - zoomW * zoomTimes && io.MousePos.x - pos.x <= (im.getSurface()->w - bigX) && io.MousePos.y - pos.y >= (im.getSurface()->h - bigY) - zoomH * zoomTimes && io.MousePos.y - pos.y <= (im.getSurface()->h - bigY)) {
			smallChange = false;
			updMove = true;
			updResize = false;
		}
		else if (sqrt(pow((io.MousePos.x - pos.x) - (smallX + zoomW), 2) + pow((io.MousePos.y - pos.y) - (smallY + zoomH), 2)) < 20) {
			smallChange = true;
			updResize = true;
			updMove = false;
		}
		else if (io.MousePos.x - pos.x <= smallX + zoomW && io.MousePos.x - pos.x >= smallX && io.MousePos.y - pos.y <= smallY + zoomH && io.MousePos.y - pos.y >= smallY) {
			smallChange = true;
			updMove = true;
			updResize = false;
		}
		else {
			updMove = false;
			updResize = false;
		}
	}
	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
	{
		if (updMove) {

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
			float focus_y = io.MousePos.y - pos.y - focus_sz_y * 0.5f;
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
		else if (updResize) {
			
			MagnifyMethod(im);

			if (smallChange) {
				int segedZoomW = zoomW, segedZoomH = zoomH;
				segedZoomW = io.MousePos.x - pos.x - smallX;
				segedZoomH = io.MousePos.y - pos.y - smallY;

				if (segedZoomW < 10) segedZoomW = 10;
				else if (((float)im.getSurface()->w - (float)bigX) - ((float)segedZoomW * zoomTimes) <= 0) segedZoomW = ((im.getSurface()->w - bigX) / zoomTimes) - 1;

				if (segedZoomH < 10) segedZoomH = 10;
				else if (((float)im.getSurface()->h - (float)bigY) - ((float)segedZoomH * zoomTimes) <= 0) segedZoomH = ((im.getSurface()->h - bigY) / zoomTimes) - 1;

				zoomW = segedZoomW;
				zoomH = segedZoomH;
			}
			else {
				float segedZoomTimes = zoomTimes;
				segedZoomTimes = max( ((im.getSurface()->w-bigX)-(io.MousePos.x - pos.x)) /zoomW , ((im.getSurface()->h-bigY)-(io.MousePos.y - pos.y)) / zoomH);

				if (segedZoomTimes < 1) segedZoomTimes = 1;

				if (((float)im.getSurface()->w - (float)bigX) - ((float)zoomW * segedZoomTimes) <= 0) segedZoomTimes = (((float)im.getSurface()->w - (float)bigX) / (float)zoomW) - 0.05f;

				if (((float)im.getSurface()->h - (float)bigY) - ((float)zoomH * segedZoomTimes) <= 0) segedZoomTimes = (((float)im.getSurface()->h - (float)bigY) / (float)zoomH) - 0.05f;

				zoomTimes = segedZoomTimes;
			}
		}
	}
}

void Image1Magnify::MagnifyMethod(Image im) {
	ImColor red = (255,0,0,255);

	for (int i = 0; i < imOut.getSurface()->w; i++) {
		for (int j = 0; j < imOut.getSurface()->h; j++) {
			SurfaceModify::PutPixel(i, j, SurfaceModify::GetColor(i, j, im.getSurface()), imOut.getSurface());
		}
	}

	//small outline
	for (int i = 0 + smallX; i < zoomW + smallX; i++) {
		for (int j = 0 + smallY; j < zoomH + smallY; j++) {
			if (i == 0 + smallX || j == 0 + smallY || i == zoomW + smallX - 1 || j == zoomH + smallY - 1) {
				SurfaceModify::PutPixel(i, j, red, imOut.getSurface());
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
				SurfaceModify::PutPixel(im.getSurface()->w - i - bigX - 1, im.getSurface()->h - j - bigY - 1, red, imOut.getSurface());
			}
			else {
				//fill
				SurfaceModify::PutPixel(im.getSurface()->w - i - bigX - 1, im.getSurface()->h - j - bigY - 1, SurfaceModify::GetColor(int((zoomW * zoomTimes - i) / zoomTimes + smallX), int((zoomH * zoomTimes - j) / zoomTimes + smallY), im.getSurface()), imOut.getSurface());
			}
		}
	}

	imOut.textureFromSurface();
}

Image1Blur::Image1Blur(void) {
	blurSize = 2;
	blurType = blurBox;
}

void Image1Blur::Reset() {
	blurSize = 2;
	blurType = blurBox;
}

void Image1Blur::BlurMethod(Image im) { 
	if (blurType == blurBox) {
		for (int i = 0; i < imOut.getSurface()->w; i+=(blurSize*2+1)) {
			for (int j = 0; j < imOut.getSurface()->h; j+=(blurSize*2+1)) {
				int rSum=0, gSum=0, bSum=0, aSum=0;
				int count = 0;
				for (int x = 0; x < (blurSize*2+1); x++) {
					for (int y = 0; y < (blurSize*2+1); y++) {

						if (x + i < imOut.getSurface()->w && y + j < imOut.getSurface()->h) {

							Uint8 r, g, b, a;
							SDL_GetRGBA(SurfaceModify::GetColor(x+i, y+j, im.getSurface()), im.getSurface()->format, &r, &g, &b, &a);
							rSum += r;
							gSum += g;
							bSum += b;
							aSum += a;
							count++;
						}
					}
				}
				rSum /= count;
				gSum /= count;
				bSum /= count;
				aSum /= count;

				for (int x = 0; x < (blurSize*2+1); x++) {
					for (int y = 0; y < (blurSize*2+1); y++) {
						if (x + i < imOut.getSurface()->w && y + j < imOut.getSurface()->h) {

							Uint32 color = SDL_MapRGBA(imOut.getSurface()->format, rSum, gSum, bSum, aSum);
							SurfaceModify::PutPixel(x + i, y + j, color, imOut.getSurface());

						}
					}
				}
			}

			
		}
	}

	else if (blurType == blurGauss) {

		for (int i = 0; i < imOut.getSurface()->w; i++) {
			for (int j = 0; j < imOut.getSurface()->h; j++) {
				float rSum = 0, gSum = 0, bSum = 0, aSum = 0;
				float count = 0;

				for (int x = -blurSize; x <= blurSize; x++) {
					for (int y = -blurSize; y <= blurSize; y++) {

						float onthepower = -1 * ((x * x + y * y) / (2.0f * blurSize * blurSize));
						float value = ((1.0f) / (2.0f * M_PI * blurSize * blurSize)) * std::exp(onthepower);

						int x0 = i + x, y0 = j + y;
						if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0; if (x0 > imOut.getSurface()->w - 1) x0 = imOut.getSurface()->w - 1; if (y0 > imOut.getSurface()->h - 1) y0 = imOut.getSurface()->h - 1;
					
						Uint8 r, g, b, a;
						SDL_GetRGBA(SurfaceModify::GetColor(x0, y0, im.getSurface()), im.getSurface()->format, &r, &g, &b, &a);
						rSum += r * value;
						gSum += g * value;
						bSum += b * value;
						aSum += a * value;
						count += value;

					}
				}
				rSum /= count;
				gSum /= count;
				bSum /= count;
				aSum /= count;

				Uint32 color = SDL_MapRGBA(imOut.getSurface()->format, rSum, gSum, bSum, aSum);
				SurfaceModify::PutPixel(i, j, color, imOut.getSurface());

			}
		}
	}

	imOut.textureFromSurface();
}

Image1Color::Image1Color(void) {
	imctype = GreyScale;
}

void Image1Color::Reset() {
	imctype = GreyScale;
}

void Image1Color::ColorMethod(Image im) { 
	for (int i = 0; i < imOut.getSurface()->w; i++) {
		for (int j = 0; j < imOut.getSurface()->h; j++) {
			switch (imctype) {
				case Null: {
					SurfaceModify::PutPixel(i, j, SurfaceModify::GetColor(i, j, im.getSurface()), imOut.getSurface());
					break;
				}
				case GreyScale: {
					Uint8 grey8 = RegularModify::greyScale(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format);
					Uint32 grey32 = (grey8 << 0) | (grey8 << 8) | (grey8 << 16) |  (255 << 24);
					SurfaceModify::PutPixel(i, j, grey32 , imOut.getSurface());
					break;
				}
				case Red: {
					Uint8 red8,g,b,alpha8;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &red8,&g,&b,&alpha8);
					Uint32 grey32 = (red8 << 0) | (0 << 8) | (0 << 16) | (alpha8 << 24);
					SurfaceModify::PutPixel(i, j, grey32, imOut.getSurface());
					break;
				}
				case Green: {
					Uint8 r, green8, b, alpha8;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &r, &green8, &b, &alpha8);
					Uint32 grey32 = (0 << 0) | (green8 << 8) | (0 << 16) | (alpha8 << 24);
					SurfaceModify::PutPixel(i, j, grey32, imOut.getSurface());
					break;
				}
				case Blue: {
					Uint8 r, g, blue8, alpha8;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &r, &g, &blue8, &alpha8);
					Uint32 grey32 = (0 << 0) | (0 << 8) | (blue8 << 16) | (alpha8 << 24);
					SurfaceModify::PutPixel(i, j, grey32, imOut.getSurface());
					break;
				}
				case Inverted: {
					Uint8 r, g, b, a;
					SDL_GetRGBA(SurfaceModify::GetColor(i, j, im.getSurface()), im.getSurface()->format, &r, &g, &b, &a);
					r = 255 - r;
					g = 255 - g;
					b = 255 - b;
					Uint32 grey32 = (r << 0) | (g << 8) | (b << 16) | (a << 24);
					SurfaceModify::PutPixel(i, j, grey32, imOut.getSurface());
					break;
				}
			}
		}
	}
	imOut.textureFromSurface();
}

bool Image1Save::SaveFolder(Folder f, std::string path, int n) { 
	struct stat sb;
	bool allSucceeded=true;

	for (int i = 0; i < f.getImages().size(); i++) {
		std::string seged = path + "/image" + std::to_string(n) + ".png";
		char* cstr = new char[seged.size() + 1];
		std::strcpy(cstr, seged.c_str());

		if (RegularModify::Verify(cstr) == false) {

			if (IMG_SavePNG(f.getImages()[i].getSurface(), cstr) != 0) {
				allSucceeded = false;
			}

			if (stat(cstr, &sb) != 0) {
				allSucceeded = false;
			}
		}
		else {
			allSucceeded = false;
		}

		delete[] cstr;
		n++;
	}

	return allSucceeded;
}

bool Image1Save::Save(Image im, char* cstr) {

	if (RegularModify::Verify(cstr) == false) {

		struct stat sb;
	
		if (IMG_SavePNG(im.getSurface(), cstr) != 0) {
			return false;
		}

		return stat(cstr, &sb) == 0;
	}
	return false;
}

Image2::Image2(void) {}

void Image2::setImage(Image im) {

	imOut.freeSurface();

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

void Image2SSIM::SSIMMethod(Image im1, Image im2) {

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
					int x0 = i + x, y0 = j + y;
					if (x0 > imOut.getSurface()->w - 1) x0 = imOut.getSurface()->w - 1; if (y0 > imOut.getSurface()->h - 1) y0 = imOut.getSurface()->h - 1;

					Uint8 seged1[colorResult::NumberOfTypes]{};
					Uint8 seged2[colorResult::NumberOfTypes]{};
					SDL_GetRGBA(SurfaceModify::GetColor(x0, y0, im1.getSurface()), im1.getSurface()->format, &seged1[colorResult::Red], &seged1[colorResult::Green], &seged1[colorResult::Blue], &seged1[colorResult::Alpha]);
					SDL_GetRGBA(SurfaceModify::GetColor(x0, y0, im2.getSurface()), im2.getSurface()->format, &seged2[colorResult::Red], &seged2[colorResult::Green], &seged2[colorResult::Blue], &seged2[colorResult::Alpha]);
					for (int k = 0; k < colorResult::NumberOfTypes; k++) { //r g b a
						if (k != colorResult::Grey) {
							mean1[k] += seged1[k];
							mean2[k] += seged2[k];
						}
					}
					mean1[colorResult::Grey] += RegularModify::greyScale(SurfaceModify::GetColor(x0, y0, im1.getSurface()), im1.getSurface()->format);
					mean2[colorResult::Grey] += RegularModify::greyScale(SurfaceModify::GetColor(x0, y0, im2.getSurface()), im2.getSurface()->format);

				}
			}
			for (int i = 0; i < colorResult::NumberOfTypes; i++) {
				mean1[i] /= pow(ssimSize, 2);
				mean2[i] /= pow(ssimSize, 2);
			}

			//var
			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize; j++) {
					int x0 = i + x, y0 = j + y;
					if (x0 > imOut.getSurface()->w - 1) x0 = imOut.getSurface()->w - 1; if (y0 > imOut.getSurface()->h - 1) y0 = imOut.getSurface()->h - 1;

					Uint8 seged1[colorResult::NumberOfTypes]{};
					Uint8 seged2[colorResult::NumberOfTypes]{};
					SDL_GetRGBA(SurfaceModify::GetColor(x0, y0, im1.getSurface()), im1.getSurface()->format, &seged1[colorResult::Red], &seged1[colorResult::Green], &seged1[colorResult::Blue], &seged1[colorResult::Alpha]);
					SDL_GetRGBA(SurfaceModify::GetColor(x0, y0, im2.getSurface()), im2.getSurface()->format, &seged2[colorResult::Red], &seged2[colorResult::Green], &seged2[colorResult::Blue], &seged2[colorResult::Alpha]);
					for (int k = 0; k < colorResult::NumberOfTypes - 1; k++) { //r g b a
						var1[k] += (seged1[k] - mean1[k]);
						var2[k] += (seged2[k] - mean2[k]);
					}
					var1[colorResult::Grey] += (RegularModify::greyScale(SurfaceModify::GetColor(x0, y0, im1.getSurface()), im1.getSurface()->format) - mean1[colorResult::Grey]);
					var2[colorResult::Grey] += (RegularModify::greyScale(SurfaceModify::GetColor(x0, y0, im2.getSurface()), im2.getSurface()->format) - mean2[colorResult::Grey]);
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
				ssimOsszeg += resultFinal[colorResult::Grey];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Grey]) * 255);
				putColor = (seged << 24) + (seged << 16) + (seged << 8);
				break;
			case 1:
				ssimOsszeg += resultFinal[colorResult::Red];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Red]) * 255);
				putColor = (seged << 24) + (seged << 16) + (255 << 8);
				break;
			case 2:
				ssimOsszeg += resultFinal[colorResult::Green];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Green]) * 255);
				putColor = (seged << 24) + (255 << 16) + (seged << 8);
				break;
			case 3:
				ssimOsszeg += resultFinal[colorResult::Blue];
				num++;
				seged = (Uint8)((resultFinal[colorResult::Blue]) * 255);
				putColor = (255 << 24) + (seged << 16) + (seged << 8);
				break;
			case 4:
				ssimOsszeg += resultFinal[colorResult::Red];
				num++;
				ssimOsszeg += resultFinal[colorResult::Green];
				num++;
				ssimOsszeg += resultFinal[colorResult::Blue];
				num++;
				putColor = ((Uint8)((resultFinal[colorResult::Red]) * 255) << 24) + ((Uint8)((resultFinal[colorResult::Green]) * 255) << 16) + ((Uint8)((resultFinal[colorResult::Blue]) * 255) << 8);
				break;
			case 5:
				ssimOsszeg += resultFinal[colorResult::Grey];
				num++;
				putColor = RegularModify::heatmapColor(resultFinal[colorResult::Grey]);
				break;
			}

			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize; j++) {
					if (x + i < imOut.getSurface()->w && y + j < imOut.getSurface()->h) {

						Uint32 color = SDL_MapRGBA(imOut.getSurface()->format, (putColor >> 24) & 0xFF, (putColor >> 16) & 0xFF, (putColor >> 8) & 0xFF, 255);
						SurfaceModify::PutPixel(x + i, y + j, color, imOut.getSurface()); 
					}
				}
			}
		}
	}

	ssimOsszeg = ssimOsszeg / num;

	imOut.textureFromSurface();

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
}

void Image2Merge::editableDrawImage(Image im1, Image im2) {

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::Image((void*)(intptr_t)imOut.getTexture(), ImVec2(imOut.getSurface()->w, imOut.getSurface()->h));
	if (ImGui::IsItemClicked()) {

		upd = std::abs(-(io.MousePos.x - pos.x - ux) * std::sin(slope * M_PI / 180.0) + (io.MousePos.y - pos.y - uy) * std::cos(slope * M_PI / 180.0)) < 25;

	}
	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
	{
		if (upd) {
			ux = io.MousePos.x - pos.x;
			uy = io.MousePos.y - pos.y;
			MergeMethod(im1,im2);
		}
	}

}

void Image2Merge::MergeMethod(Image im1, Image im2) {
	for (int i = 0; i < im1.getSurface()->w; i++) {
		for (int j = 0; j < im1.getSurface()->h; j++) {
			if ((j <= tan(slope/180*M_PI) * (i - ux) + uy  && (slope <= 90 || slope >270)) || (j > tan(slope / 180 * M_PI) * (i - ux) + uy &&  slope>90 && slope <=270 )) {
				SurfaceModify::PutPixel(i, j, SurfaceModify::GetColor(i, j, im1.getSurface()), imOut.getSurface());
			}
			else {
				SurfaceModify::PutPixel(i, j, SurfaceModify::GetColor(i, j, im2.getSurface()), imOut.getSurface());
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
		PutPixel(x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);

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
			if (f >= 0 && otherAxis >= 0 && f < sur->w && otherAxis < sur->h) {
				PutPixel(f, otherAxis, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);
			}
		}
		else {
			if (f >= 0 && otherAxis >= 0 && otherAxis < sur->w && f < sur->h) {
				PutPixel(otherAxis, f, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);
			}
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
		PutPixel(x, y, (255 << 24) | (0 << 16) | (0 << 8) | 255, sur);

		if (D > 0) {
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else {
			D = D + 2 * dx;
		}
	}
}

void SurfaceModify::PutPixel(int x, int y, Uint32 color, SDL_Surface* sur)
{
	if (SDL_MUSTLOCK(sur))
		SDL_LockSurface(sur);
	PutPixel_nolock(x, y, color,sur);
	if (SDL_MUSTLOCK(sur))
		SDL_UnlockSurface(sur);
}

void SurfaceModify::PutPixel_nolock(int x, int y, Uint32 color, SDL_Surface* sur)
{
	if (sur->format->BitsPerPixel == 8) {
		Uint8* pixel = (Uint8*)sur->pixels;
		pixel += (y * sur->pitch) + (x * sizeof(Uint8));
		*pixel = color & 0xFF;
	}
	else if (sur->format->BitsPerPixel == 16) {
		Uint8* pixel = (Uint8*)sur->pixels;
		pixel += (y * sur->pitch) + (x * sizeof(Uint16));
		*((Uint16*)pixel) = color & 0xFFFF;
	}
	else if (sur->format->BitsPerPixel == 24) {
		Uint8* pixel = (Uint8*)sur->pixels;
		pixel += (y * sur->pitch) + (x * sizeof(Uint8) * 3);
		if( SDL_BYTEORDER == SDL_BIG_ENDIAN){
				pixel[0] = (color >> 24) & 0xFF;
				pixel[1] = (color >> 16) & 0xFF;
				pixel[2] = (color >> 8) & 0xFF;
		}
		else{
				pixel[0] = color & 0xFF;
				pixel[1] = (color >> 8) & 0xFF;
				pixel[2] = (color >> 16) & 0xFF;
		}
	}
	else if (sur->format->BitsPerPixel == 32) {
		Uint8* pixel = (Uint8*)sur->pixels;
		pixel += (y * sur->pitch) + (x * sizeof(Uint32));
		*((Uint32*)pixel) = color;
	}

}

Uint32 SurfaceModify::GetColor(int x, int y, SDL_Surface* sur) {

	Uint8* pixelPtr = (Uint8*)sur->pixels+ y * sur->pitch+ x * sur->format->BytesPerPixel;

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
		return false;
	}
	SDL_FreeSurface(imageSurface);
	return true;
}
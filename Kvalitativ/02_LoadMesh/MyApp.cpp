#define STB_IMAGE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <imgui/imgui.h>
#include "stb_image.h"

#include <filesystem>

#include <windows.h>
//#include "CommonFileDialogApp.h"

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

	return true;
}

void CMyApp::Clean()
{
	//delete[] stradd;
	//delete[] straddverified;
	//delete[] outstr;
}

void CMyApp::Update()
{

}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/);

	ImGuiIO io = ImGui::GetIO();

	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Begin("Qualitative comparison", 0, window_flags); 

		//ImGui::PushFont(arial);
		//ImGui::PopFont();
		
	ImFont* imFo = ImGui::GetFont();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::NewLine();
	imFo->Scale = 2.f;	ImGui::PushFont(imFo);
	ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
	RegularModify::CursorPos(max((int)(io.DisplaySize.x / 2 - 240),10));
	ImGui::Text("Qualitative comparison of images");
	imFo->Scale = 1.f;	ImGui::PopFont(); ImGui::PopStyleColor();
	ImGui::NewLine(); ImGui::NewLine();


	/*int maxHeightVecIndex = 0;
	for (int i = 0; i < imageVec.size(); ++i) {
		if (imageVec[i].getSurface()->h > imageVec[maxHeightVecIndex].getSurface()->h) {
			maxHeightVecIndex = i;
		}
	}*/

	SetBasicUI();
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, Colors[ColorEnum::SCROLL_BG]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, Colors[ColorEnum::SCROLL_GRAB]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, Colors[ColorEnum::SCROLL_GRAB_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, Colors[ColorEnum::SCROLL_GRAB_ACTIVE]);

	ImGui::BeginChild("Pictures", ImVec2(0, imfVec.size() == 0 ? 100 : 395), false);

	imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
	ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("FoldersIms: ");
	imFo->Scale = 1.f;	ImGui::PopFont();
	RegularModify::ShowHelpMarker("The loaded images will appear here. Click on them to select them."); ImGui::NewLine();

	if (imfVec.size() > 0) {

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));

		ImGui::BeginChild("scrolling", ImVec2(0, 330), false, ImGuiWindowFlags_HorizontalScrollbar);
		RegularModify::CursorPos(20);
		for (int i = 0; i < imfVec.size(); i++) {

			switch (imfVec[i].iof) {
				case iofImage: {
					imfVec[i].im.drawImage(300 , std::find(selectedImFVec.begin(), selectedImFVec.end(), i) != selectedImFVec.end());
					break;
				}
				case iofFolder: {
					imfVec[i].f.drawImage(300 , std::find(selectedImFVec.begin(), selectedImFVec.end(), i) != selectedImFVec.end());
					break;
				}
				default: {
					break;
				}
			}

			if (ImGui::IsItemClicked() && (currentImageEnum == SEMMIENUM || currentImageEnum == LOADENUM))
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

		ImGui::EndChild();

		ImGui::PopStyleVar(2);
	}
	else {
		RegularModify::CursorPos(20); ImGui::Text("No loaded image.");
		ImGui::NewLine();
	}

	ImGui::EndChild();

	 

	ImGui::PopStyleVar(2);ImGui::PopStyleColor(9);

	ImGui::NewLine(); ImGui::NewLine();

	switch (currentImageEnum)
	{
		case CMyApp::SEMMIENUM: //--------------------------------------------------------------------------------------------------
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

			ImGui::BeginChild("Operations", ImVec2(0, 140), false);

			imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
			ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Operations:");
			imFo->Scale = 1.f;	ImGui::PopFont();
			RegularModify::ShowHelpMarker("The operations that can be done with the selected images will appear here.");

			ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);

			ImGui::NewLine();

			int rowNo = 0;
			if (selectedImFVec.size() == 0) { rowNo = 1; }
			else if (selectedImFVec.size() == 1) { rowNo = 2; }
			else if (selectedImFVec.size() == 2) { rowNo = 4; }
			else { rowNo = 2; }

			ImGui::Columns(rowNo, "mycolumnsselected", false);
			imFo->Scale = 1.3f;
			ImGui::PushFont(imFo);

			if (selectedImFVec.size() == 0) {

				RegularModify::CursorPos(20);
				if (ImGui::Button("Load", ImVec2(150, 50))) {
					currentImageEnum = LOADENUM;
				}
				ImGui::NextColumn();
			}

			if (selectedImFVec.size() >= 1) { //!!selectedImFVec imfVec

				RegularModify::CursorPos(20);
			//	if (imfVec[selectedImFVec[0]].getSurface()->w >= 10 && imfVec[selectedImFVec[0]].getSurface()->h >= 10) {
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
			//	}
			/*	else {
					ImGui::PushStyleColor(ImGuiCol_Button, Colors[ColorEnum::BUTTON_GREY]);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors[ColorEnum::BUTTON_GREY_HOVERED]);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors[ColorEnum::BUTTON_RED_ACTIVE]);
					ImGui::Button("Magnify", ImVec2(150, 50));
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
					RegularModify::ShowHelpMarker("The magnify operation requires the image to be at least 10 pixels in both width and height.");
					ImGui::PopStyleColor(4);
				}*/
				ImGui::NextColumn();

				if (ImGui::Button("Save", ImVec2(150, 50))) {
					currentImageEnum = SAVEENUM;
				}
				ImGui::NextColumn();

			}

			if (selectedImFVec.size() == 2) {

				if (ImGui::Button("SSIM", ImVec2(150, 50))) {
					currentImageEnum = SSIMENUM;
					im2ssim.setImage(imageVec[selectedImageVec[0]]);
					im2ssim.Reset();
					im2ssim.SSIMSurface(imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);
				}
				ImGui::NextColumn();

				if (ImGui::Button("Merge", ImVec2(150, 50))) {
					currentImageEnum = MERGEENUM;
					im2merge.setImage(imageVec[selectedImageVec[0]]);
					im2merge.Reset();
					im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);
				}
				ImGui::NextColumn();

			}

			ImGui::Columns(1);
			imFo->Scale = 1.f;
			ImGui::PopFont();

			ImGui::PopStyleColor();

			ImGui::EndChild();
			ImGui::PopStyleVar(2); ImGui::PopStyleColor(8);

			break;
		}
		case CMyApp::LOADENUM: //-------------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Load", ImVec2(0, 200), false);

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Load");
				imFo->Scale = 1.f;	ImGui::PopFont();
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
				RegularModify::CursorPos(20); ImGui::Text(im0load.getLoadType() == im0load.PICTURE ? "The image's path:" : im0load.getLoadType() == im0load.FOLDER ? "The folder's path:" :  "");
				ImGui::PushItemWidth(320);
				RegularModify::CursorPos(20); ImGui::InputText("##StrAdd", stradd, IM_ARRAYSIZE(stradd));
				ImGui::PopItemWidth();

				if (currentErrors[0]) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
					RegularModify::CursorPos(20); ImGui::Text("Incorrect path");
					ImGui::PopStyleColor();
				}
				else ImGui::NewLine();

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo); RegularModify::CursorPos(20);

				//--------button

				if (ImGui::Button("Load##stradd", ImVec2(150, 50))) {
					switch (im0load.getLoadType())
					{
					case Image0FromFile::loadTypeEnum::PICTURE :{

						if (RegularModify::Verify(stradd, straddverified)) {
							Image sizeVerify = Image0FromFile::Load(straddverified); //fix!!
							if (sizeVerify.getSurface() != nullptr && sizeVerify.getSurface()->h > 10 && sizeVerify.getSurface()->w > 10) {
								ImageFolder seged;
								seged.im = sizeVerify; //Image0FromFile::Load(straddverified);
								seged.iof = iofImage;
								imfVec.push_back(seged);
								currentErrors[0] = false;
							}
							else {
								//errors 
							}
							
						}
						else {
							currentErrors[0] = true;
						}
						break;
					}

					case Image0FromFile::loadTypeEnum::FOLDER :{

						Folder fold;
						std::string path = stradd;
						path += "/*";
						std::string path2 = stradd;

						WIN32_FIND_DATAA ffd;
						HANDLE hFind = FindFirstFileA(path.c_str(), &ffd);

						if (hFind == INVALID_HANDLE_VALUE) {
							currentErrors[0] = true;
							break;
						}
						
						currentErrors[0] = false;
						while (FindNextFileA(hFind, &ffd) != 0)
						{	
							std::string seged = path2 + "/" + (std::string)ffd.cFileName;
							char* cstr = new char[seged.size() + 1];
							std::strcpy(cstr, seged.c_str());

							if (RegularModify::Verify(cstr, straddverified)) {
								if (!fold.Load(straddverified)) {
									//error
								}
								else {
									std::cout << cstr << std::endl;
								}
							}
							delete[] cstr;
						}
						FindClose(hFind);

						if (fold.images.size() == 0) {
							//error
						}
						else {

							for (Image im : fold.images) {
								im.textureFromSurface();
							}
							
							fold.createIconImageFromImages();

							ImageFolder seged;
							seged.f = fold;
							seged.iof = iofFolder;
							imfVec.push_back(seged);

						}

						break;
					}
					default:
						break;
					}
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(190);
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();

			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}
		case CMyApp::MAGNIFYENUM: //------------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name1", ImVec2(max(im1mag.imOut.getSurface()->w, 360), 65), false);

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Magnify");
				imFo->Scale = 1.f;	ImGui::PopFont();

			ImGui::EndChild();
			ImGui::NewLine();

			im1mag.editableDrawImage(imfVec[selectedImFVec[0]].iof==iofImage ? imfVec[selectedImFVec[0]].im : imfVec[selectedImFVec[0]].f.images[0]);

			ImGui::NewLine();

			ImGui::BeginChild("Magnify", ImVec2(max(im1mag.imOut.getSurface()->w, 360), 180), false);

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(1 * fmax(im1mag.imOut.getSurface()->w, 360) / 4 - 70);

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
				RegularModify::CursorPos(3 * fmax(im1mag.imOut.getSurface()->w, 360) / 4 - 70);
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


				ImGui::NewLine();

				RegularModify::CursorPos(20); ImGui::Text("Width:");
				ImGui::SameLine();
				RegularModify::CursorPos(270);
				ImGui::PushItemWidth(fmax(im1mag.imOut.getSurface()->w, 360) - 290);
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
				ImGui::PushItemWidth(fmax(im1mag.imOut.getSurface()->w, 360) - 290);
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
				RegularModify::CursorPos(190);
				ImGui::PushItemWidth(fmax(im1mag.imOut.getSurface()->w, 360) - 210);
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
				ImGui::PopItemWidth();

			ImGui::EndChild();
			ImGui::NewLine();

			//--------button

			ImGui::BeginChild("EndButtons_1", ImVec2(max(im1mag.imOut.getSurface()->w, 360), 80));
				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##stradd", ImVec2(150, 50))) {

					//image
					if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof==iofImage) {
						
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

						/*//+ exit on load
						Image imseged;
						imseged.setSurface(im1mag.imOut.getSurface());
						imseged.textureFromSurface();
						ImageFolder imfseged;
						imfseged.iof = iofImage;
						imfseged.im = imseged;

						imfVec.push_back(imfseged);*/

					}

					//folder !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! recently changed, if error its probably here
					else {
						Folder segedFolder;
						int offset = 0;
						for (int i = 0; i < selectedImFVec.size();i++) {
							if (imfVec[selectedImFVec[i]].iof == iofImage) {
								segedFolder.Append(imfVec[selectedImFVec[i]].im);
								offset++;
							}
							else if (imfVec[selectedImFVec[i]].iof == iofFolder) {
								segedFolder.Append(imfVec[selectedImFVec[i]].f);

								for (int j = 0; j < imfVec[selectedImFVec[i]].f.storedOperationsVector.size();j++) {
									for (int k = 0; k < imfVec[selectedImFVec[i]].f.storedOperationsVector[j].affectedElements.size();k++) {
										imfVec[selectedImFVec[i]].f.storedOperationsVector[j].affectedElements[k] += offset;
									}
									segedFolder.storedOperationsVector.push_back(imfVec[selectedImFVec[i]].f.storedOperationsVector[j]);
								}
								offset += imfVec[selectedImFVec[i]].f.images.size();
							}
						}
						//err end
						SDL_Surface* source = im1mag.imOut.getSurface();
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

						Folder::storedOperation segedOp;
						segedOp.ote = Folder::oteImage1Magnify;
						segedOp.i1m = im1mag;

						//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! changed as well
						for (int i = 0; i < segedFolder.images.size(); i++) {
							segedOp.affectedElements.push_back(i);
						}

						segedFolder.storedOperationsVector.push_back(segedOp);

						ImageFolder imfseged;
						imfseged.iof = iofFolder;
						imfseged.f = segedFolder;

						imfVec.push_back(imfseged);

						/*
						if (imfVec[selectedImFVec[0]].iof == iofImage) {
							im1mag.setImage(imfVec[selectedImFVec[0]].im);
							im1mag.MagnifyMethod(imfVec[selectedImFVec[0]].im);
						}
						else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
							im1mag.setImage(imfVec[selectedImFVec[0]].f.images[0]);
							im1mag.MagnifyMethod(imfVec[selectedImFVec[0]].f.images[0]);
						}*/
					}
					ImGui::OpenPopup("Betoltes##Pop"); //!!!!!!!!!!!!!!
				}

				ImGui::PopStyleColor(3);

				ImGui::SameLine(); RegularModify::CursorPos(max(170, im1mag.imOut.getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();

			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}
		case CMyApp::SAVEENUM: //-----------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);

			ImGui::BeginChild("Name2", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, 360), 65), false);

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Save");
				imFo->Scale = 1.f;	ImGui::PopFont();

			ImGui::EndChild();

			ImGui::NewLine();
			if (imfVec[selectedImFVec[0]].iof == iofImage) {
				ImGui::Image((void*)(intptr_t)imfVec[selectedImFVec[0]].im.getTexture(), ImVec2(imfVec[selectedImFVec[0]].im.getSurface()->w, imfVec[selectedImFVec[0]].im.getSurface()->h));
			}
			else if (imfVec[selectedImFVec[0]].iof == iofFolder) {
				ImGui::Image((void*)(intptr_t)imfVec[selectedImFVec[0]].f.images[0].getTexture(), ImVec2(imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, imfVec[selectedImFVec[0]].f.images[0].getSurface()->h));
			}
			ImGui::NewLine();

			ImGui::BeginChild("Save", ImVec2(max(imfVec[selectedImFVec[0]].iof==iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, 360), 75), false);

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("Path:");
				ImGui::PushItemWidth(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w -40 : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w - 40, 260));
				RegularModify::CursorPos(20); ImGui::InputText("##SavePath", outstr, IM_ARRAYSIZE(outstr));
				ImGui::PopItemWidth();
				if (currentErrors[1]) {
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
					RegularModify::CursorPos(20); ImGui::Text("Incorrect path");
					ImGui::PopStyleColor();
				}

			ImGui::EndChild();

			ImGui::NewLine();

			ImGui::BeginChild("EndButtons_2", ImVec2(max(imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w, 360), 80));

				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Save", ImVec2(150, 50))) {

					struct stat sb;
					//incorrect path (there isn't a file with this name already)
					if (stat(outstr, &sb) != 0) {
						if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {
							if (IMG_SavePNG(imfVec[selectedImFVec[0]].im.getSurface(), outstr) == 0) {
								if (stat(outstr, &sb) == 0) { //check, might not needed
									currentImageEnum = SEMMIENUM;
									currentErrors[1] = false;
									ImGui::OpenPopup("Mentes##Pop");
								}
								else {
									currentErrors[1] = true;
								}
							}
							else {
								currentErrors[1] = true;
							}
						}
						else {
							currentErrors[1] = true;
						}
					}
					//folder path  !!!!!!!!!!!!!!!!!!!!!!!!!!check if name isnt already taken
					else if (sb.st_mode & S_IFDIR) {
						if (selectedImFVec.size() == 1 && imfVec[selectedImFVec[0]].iof == iofImage) {
							currentErrors[1] = true;
						}
						else {
							int n = 1;
							std::string path = outstr;

							for (int i = 0; i < selectedImFVec.size(); i++) {
								if (imfVec[selectedImFVec[i]].iof == iofImage) {

									std::string seged = path + "/image" + std::to_string(n) + ".png";
									char* cstr = new char[seged.size() + 1];
									std::strcpy(cstr, seged.c_str());

									IMG_SavePNG(imfVec[selectedImFVec[i]].im.getSurface(), cstr);

									delete[] cstr;

									n++;
								}
								else if (imfVec[selectedImFVec[i]].iof == iofFolder) { //shorten stuff
									for (int j = 0; j < imfVec[selectedImFVec[i]].f.images.size(); j++) {

										std::string seged = path + "/image" + std::to_string(n) + ".png";
										char* cstr = new char[seged.size() + 1];
										std::strcpy(cstr, seged.c_str());

										//----------------------------------------------------------

										//im1sav.setImage(imfVec[selectedImFVec[i]].f.images[j]);
										im1sav.SaveFolder(imfVec[selectedImFVec[i]].f, cstr, j);

										//freeup imseged

										//----------------------------------------------------------

										delete[] cstr;

										n++;
									}
								}
							}
							currentImageEnum = SEMMIENUM;
							currentErrors[1] = false;

							ImGui::OpenPopup("Mentes##Pop");
						}
					}
					//already existing image path
					else {
						currentErrors[1] = true;
					}
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(170, imfVec[selectedImFVec[0]].iof == iofImage ? imfVec[selectedImFVec[0]].im.getSurface()->w -170 : imfVec[selectedImFVec[0]].f.images[0].getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f; ImGui::PopFont();
				ImGui::PopStyleColor(3);

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

			ImGui::BeginChild("Name3", ImVec2(max(imageVec[selectedImageVec[0]].getSurface()->w, 360), 65), false);

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("SSIM");
				imFo->Scale = 1.f;	ImGui::PopFont();

			ImGui::EndChild();
			ImGui::NewLine();

			im2ssim.editableDrawImage(); //?

			ImGui::NewLine();

			ImGui::BeginChild("SSIM", ImVec2(max(imageVec[selectedImageVec[0]].getSurface()->w, 360), 210), false);

				ImGui::NewLine();
				float segedSsimOsszeg = im2ssim.getSsimOsszeg();
				RegularModify::CursorPos(20); ImGui::Text("Overall SSIM value: %f", segedSsimOsszeg);
				RegularModify::ShowHelpMarker("The average value of the SSIM algorithm on the two image.");

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("SSIM's color: "); ImGui::NewLine();
				RegularModify::CursorPos(20);

				char* segedSsimRadioNames[] = {"Black-White","Blue","Green","Red","All color","Heatmap"};
				int segedSsimColor = im2ssim.getSsimColor();
				for (int i = 0; i < sizeof(segedSsimRadioNames)/sizeof(char*); i++) {
					ImGui::SameLine();
					if (ImGui::RadioButton(segedSsimRadioNames[i], &segedSsimColor, i)) {
						im2ssim.setSsimColor(segedSsimColor);
					}
				}

				RegularModify::ShowHelpMarker("Here you can select the color/style of the SSIM.");

				ImGui::NewLine();
				RegularModify::CursorPos(20); ImGui::Text("The size of the SSIM slices:");
				ImGui::SameLine();
				RegularModify::CursorPos(220);
				ImGui::PushItemWidth(80);
				int segedSsimSize = im2ssim.getSsimSize();
				if (ImGui::InputInt("##Size", &segedSsimSize, 0)) {
					if (segedSsimSize < 1) {
						segedSsimSize = 1;
					}
					if (segedSsimSize > fmin(imageVec[selectedImageVec[0]].getSurface()->w / 2, imageVec[selectedImageVec[0]].getSurface()->h / 2)) {
						segedSsimSize = fmin(imageVec[selectedImageVec[0]].getSurface()->w / 2, imageVec[selectedImageVec[0]].getSurface()->h / 2);
					}

					im2ssim.setSsimSize(segedSsimSize);
				}
				RegularModify::ShowHelpMarker("What times what should be the size of the slices that the SSIM algorithm uses");

				ImGui::NewLine();
				ImGui::PopItemWidth();

				ImGui::PushStyleColor(ImGuiCol_Text, Colors[ColorEnum::TEXT_LIGHT]);
				RegularModify::CursorPos((imageVec[selectedImageVec[0]].getSurface()->w / 2.0f) - (150.0f / 2.0f));
				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Regenerate", ImVec2(150, 50))) {

					im2ssim.SSIMSurface(imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);
				}
				imFo->Scale = 1.f;
				ImGui::PopStyleColor();
				ImGui::PopFont();

			ImGui::EndChild();

			ImGui::NewLine();

			ImGui::BeginChild("EndButtons_3", ImVec2(max(imageVec[selectedImageVec[0]].getSurface()->w, 360), 80));
				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##stradd", ImVec2(150, 50))) {
					
					// this or exit on betoltes
					SDL_Surface* source = im2ssim.imOut.getSurface();
					SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
						source->w, source->h, source->format->BitsPerPixel, source->format->format);
					if (destination != nullptr) {
						SDL_BlitSurface(source, nullptr, destination, nullptr);
					}
					Image imseged;
					imseged.setSurface(destination);
					imseged.textureFromSurface();
					
					imageVec.push_back(imseged);

					ImGui::OpenPopup("Betoltes##Pop");
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(170, imageVec[selectedImageVec[0]].getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
				ImGui::PopStyleColor(3);

			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(9);

			break;
		}
		case CMyApp::MERGEENUM: //-----------------------------------------------------------------------------------------------------
		{
			SetBasicUI();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors[ColorEnum::INPUT_BG]);
			
			ImGui::BeginChild("Name4", ImVec2(max(imageVec[selectedImageVec[0]].getSurface()->w, 360), 65), false);

				imFo->Scale = 1.5f;	ImGui::PushFont(imFo);
				ImGui::NewLine(); RegularModify::CursorPos(20); ImGui::Text("Merge");
				imFo->Scale = 1.f;	ImGui::PopFont();

			ImGui::EndChild();

			ImGui::NewLine();

			im2merge.editableDrawImage(imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);

			ImGui::NewLine();

			ImGui::BeginChild("Merge", ImVec2(max(imageVec[selectedImageVec[0]].getSurface()->w, 360), 70), false);

			ImGui::NewLine();
			RegularModify::CursorPos(20); ImGui::Text("The slope of the transition:");
			ImGui::SameLine();
			RegularModify::CursorPos(200);
			ImGui::PushItemWidth(max(imageVec[selectedImageVec[0]].getSurface()->w, 360) - 220);
			float segedSlope = im2merge.getSlope();
			if (ImGui::SliderFloat("##slope", &segedSlope, 0.0f, 360.0f, "%.4f")) {
				im2merge.setSlope(segedSlope);
				im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);
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
				im2merge.plotLineMerge(im2merge.getUx(), im2merge.getUy(), imageVec[selectedImageVec[0]], imageVec[selectedImageVec[1]]);
			}
			ImGui::PopItemWidth();

			ImGui::EndChild();
			ImGui::NewLine();

			ImGui::BeginChild("EndButtons_4", ImVec2(max(imageVec[selectedImageVec[0]].getSurface()->w, 360), 80));
				ImGui::NewLine();
				RegularModify::CursorPos(20);

				PushStyleColorGreenButton();

				imFo->Scale = 1.3f;
				ImGui::PushFont(imFo);
				if (ImGui::Button("Load##stradd", ImVec2(150, 50))) {
					
					// this or exit on betoltes
					SDL_Surface* source = im2merge.imOut.getSurface();
					SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
						source->w, source->h, source->format->BitsPerPixel, source->format->format);
					if (destination != nullptr) {
						SDL_BlitSurface(source, nullptr, destination, nullptr);
					}
					Image imseged;
					imseged.setSurface(destination);
					imseged.textureFromSurface();
					
					imageVec.push_back(imseged);

					ImGui::OpenPopup("Betoltes##Pop");
				}

				ImGui::SameLine(); RegularModify::CursorPos(max(170, imageVec[selectedImageVec[0]].getSurface()->w - 170));
				Back();

				imFo->Scale = 1.f;
				ImGui::PopFont();
				ImGui::PopStyleColor(3);

			ImGui::EndChild();

			ImGui::PopStyleVar(2); ImGui::PopStyleColor(6);

			break;
		}
		default:
		{
			break;
		}
	}

	ImGui::NewLine();

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
	}

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
		currentImageEnum = SEMMIENUM;
		currentErrors[0] = false;
		currentErrors[1] = false;
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

Image0FromFile::Image0FromFile(void) {
	loadType = loadTypeEnum::PICTURE;
}

Image Image0FromFile::Load(char* s) {
	Image im;
	im.setSurface(IMG_Load(s));
	im.textureFromSurface();
	return im;
}

Folder::Folder(void){}

bool Folder::Load(char* s) {
	Image sizeVerify = Image0FromFile::Load(s);
	if (sizeVerify.getSurface()!=NULL && sizeVerify.getSurface()->h > 10 && sizeVerify.getSurface()->w > 10) {
		images.push_back(sizeVerify);
		return true;
	}
	else {
		return false;
	}
}

void Folder::Append(Image app) {
	images.push_back(app);
}

void Folder::Append(Folder app) {
	for (Image im : app.images) {
		images.push_back(im);
	}
}

void Folder::createIconImageFromImages() {
	SDL_Surface* source = images[0].getSurface();
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
	if (ImGui::IsItemHovered())
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
	else {
		upd = false;
	}
	if (ImGui::IsItemClicked()) {
		upd = !upd;
	}
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

void Image1Save::SaveFolder(Folder f, char* cstr, int j) {
	SDL_Surface* source = f.images[j].getSurface();
	SDL_Surface* destination = SDL_CreateRGBSurfaceWithFormat(0,
		source->w, source->h, source->format->BitsPerPixel, source->format->format);
	if (destination != nullptr) {
		SDL_BlitSurface(source, nullptr, destination, nullptr);
	}
	imOut.setSurface(destination);
	

	if (j != 0) {
		for (int k = 0; k < f.storedOperationsVector.size(); k++) {
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! edited
			if (std::find(f.storedOperationsVector[k].affectedElements.begin(), f.storedOperationsVector[k].affectedElements.end(), j) != f.storedOperationsVector[k].affectedElements.end()) {
				switch (f.storedOperationsVector[k].ote) {
					case Folder::oteImage1Magnify: {

						if (f.images[0].getSurface()->w <= f.images[j].getSurface()->w && f.images[0].getSurface()->h <= f.images[j].getSurface()->h) {
							f.storedOperationsVector[k].i1m.setImage(imOut);
							f.storedOperationsVector[k].i1m.MagnifyMethod(imOut);
							imOut = f.storedOperationsVector[k].i1m.imOut;
						}

						else {
							//error
						}
						break;
					}
					case Folder::oteImage2SSIM: {
						break;
					}
				}
			}

		}
	}
	IMG_SavePNG(imOut.getSurface(), cstr);
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

/*	std::vector<std::vector<colorsStruckt>> colorVect(ssimSize);
	for (int i = 0; i < ssimSize; i++) {
		colorVect[i].resize(ssimSize);
	}
*/

	/*std::vector<std::vector<colorsStruckt*>> colorVect2(ssimSize);
	for (int i = 0; i < ssimSize; i++) {
		colorVect[i].resize(ssimSize);
	}*/

	//std::vector<std::vector<colorsStruckt*>> colorVect2(ssimSize, std::vector<colorsStruckt*>(ssimSize)); 

	std::vector<std::vector<Uint32>> colorVectFinal1(ssimSize, std::vector<Uint32>(ssimSize));
	std::vector<std::vector<Uint32>> colorVectFinal2(ssimSize, std::vector<Uint32>(ssimSize));

	//const int seged = ssimSize;
	//std::array<std::array<Uint32,seged)>,seged> colorVectFinal1(ssimSize, std::vector<Uint32>(ssimSize));

	//std::vector<std::vector<Uint8>> colorVectFinal1(ssimSize, std::vector<Uint8>(ssimSize));
	//std::vector<std::vector<Uint8>> grey2(ssimSize, std::vector<Uint8>(ssimSize));

	//std::vector<std::vector<Uint8>> red1(ssimSize, std::vector<Uint8>(ssimSize));
	//std::vector<std::vector<Uint8>> red2(ssimSize, std::vector<Uint8>(ssimSize));

	//std::vector<std::vector<Uint8>> green1(ssimSize, std::vector<Uint8>(ssimSize));
	//std::vector<std::vector<Uint8>> green2(ssimSize, std::vector<Uint8>(ssimSize));

	//std::vector<std::vector<Uint8>> blue1(ssimSize, std::vector<Uint8>(ssimSize));
	//std::vector<std::vector<Uint8>> blue2(ssimSize, std::vector<Uint8>(ssimSize));

	//std::vector<std::vector<Uint8>> alpha1(ssimSize, std::vector<Uint8>(ssimSize));
	//std::vector<std::vector<Uint8>> alpha2(ssimSize, std::vector<Uint8>(ssimSize));

	ssimOsszeg = 0.f;
	int num = 0;

	for (int x = 0; x < imOut.getSurface()->w; x += ssimSize) {
		for (int y = 0; y < imOut.getSurface()->h; y += ssimSize) {


			for (int i = 0; i < ssimSize; i++) {
				for (int j = 0; j < ssimSize ; j++) {

		//			SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, imIn1.getSurface()), imIn1.getSurface()->format, &r, &g, &b, &a);
		//			SDL_GetRGBA(SurfaceModify::GetColor(x + i, y + j, imIn2.getSurface()), imIn2.getSurface()->format, &r2, &g2, &b2, &a2);

					//grey1[i][j] = RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, imIn1.getSurface()), imIn1.getSurface()->format);
					//grey2[i][j] = RegularModify::greyScale(SurfaceModify::GetColor(x + i, y + j, imIn2.getSurface()), imIn2.getSurface()->format);

					colorVectFinal1[i][j] = SurfaceModify::GetColor(x + i, y + j, im1.getSurface());
					colorVectFinal2[i][j] = SurfaceModify::GetColor(x + i, y + j, im2.getSurface());


				}
			}

			Uint32 putColor{};
			Uint8 seged;

			std::vector<float> resultFinal = SSIMmethod(colorVectFinal1, colorVectFinal2,im1,im2);

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

						SurfaceModify::PutPixel32(x + i, y + j, SDL_MapRGBA(imOut.getSurface()->format, (putColor >> 24) & 0xFF, (putColor >> 16) & 0xFF, (putColor >> 8) & 0xFF, 255 /*alpha1[i][j] + alpha2[i][j]) / 2*/), imOut.getSurface()); //alpha?
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
		g = static_cast<int>(255 * (value / 0.5f));
		b = 0;
	}
	else {
		// yellow (255,255,0) -> blue (0,0,255)
		r = g = static_cast<int>(255 * (1.0f - (value / 0.5f)));
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
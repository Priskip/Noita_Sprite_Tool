//Standards
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
//Libraries
#include "imgui.h"
#include "lodepng.h"
#include "tinyxml2.h"
//Priskip's
#include "utils.h"
#include "Textures.h"
#include "File_Browser.h"
#include "PriskipAsepriteEncoderDecoder.h"

void performMainGUILoop() {
	//Variables
	static bool show_demo_window = false;
	static bool show_file_browser = true;
	static bool test_window = true;
	static bool test_window2 = false;

	static std::string file_browser_starting_location =
		"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Noita\\mods";
	static FileBrowser file_browser(file_browser_starting_location);

	static const char bin_file[] = "files/test_data.bin";
	static AsepriteFile aseprite_file(bin_file);


	//Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Demo Window", enabledOrDisabled(show_demo_window).c_str())) {
				show_demo_window = !show_demo_window;
			}
			if (ImGui::MenuItem("test_window", enabledOrDisabled(test_window).c_str())) {
				test_window = !test_window;
			}
			if (ImGui::MenuItem("test_window2", enabledOrDisabled(test_window2).c_str())) {
				test_window2 = !test_window2;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("File Broswer"))
		{
			if (ImGui::MenuItem("File Browser", enabledOrDisabled(show_file_browser).c_str())) {
				show_file_browser = !show_file_browser;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//Demo Window
	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	//Test Window
	if (test_window) {
		ImGui::Begin("Testing Window");

		if (ImGui::Button("Button")) {
			aseprite_file.printBinaryDataToConsole();
		}

		if (ImGui::Button("Button 2")) {
			aseprite_file.testy();
		}

		ImGui::End();
	}

	//Test Window
	if (test_window2) {
		ImGui::Begin("Testing Window 2");

		ImGui::End();
	}

	//File Browser
	if (show_file_browser) {
		ImGui::Begin("File Browser");

		file_browser.doTheImGuiThing();

		ImGui::End();
	}
}

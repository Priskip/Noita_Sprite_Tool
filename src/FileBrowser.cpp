#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "imgui.h"
#include "Textures.h"
#include "File_Browser.h"

//Helper functions
bool compFunc(std::string str_a, std::string str_b) {
	return str_a < str_b;
}

//CONSTRUCTORS
FileBrowser::FileBrowser(const std::string& path_str) {
	//Initialize Members
	path = path_str;
	std::replace(path.begin(), path.end(), '/', '\\'); //change all '/'s to '\\'
	display_mode = ALL;

	//Initialize path_chunks
	chunkifyPath();

	//Initialize drives
	findDrives();

	//Initialize files && file_types
	findFiles();
	sortFiles();

	//Find texture pointers
	icon_unknown = textures.findTexture("files/icons/unknown.png");
	icon_folder = textures.findTexture("files/icons/folder.png");
	icon_txt = textures.findTexture("files/icons/txt.png");
	icon_png = textures.findTexture("files/icons/png.png");
	icon_xml = textures.findTexture("files/icons/xml.png");
	icon_lua = textures.findTexture("files/icons/lua.png");
	icon_md = textures.findTexture("files/icons/md.png");
	icon_this_pc = textures.findTexture("files/icons/this_pc.png");
	icon_back = textures.findTexture("files/icons/back.png");
	icon_forward = textures.findTexture("files/icons/forward.png");
	icon_history = textures.findTexture("files/icons/history.png");
	icon_aseprite = textures.findTexture("files/icons/aseprite.png");

	file_icons = {
		icon_unknown,
		icon_folder,
		icon_txt,
		icon_png,
		icon_xml,
		icon_lua,
		icon_md,
		icon_aseprite
	};

	//Add initial path to history
	file_history.push_back(path);
	display_mode_history.push_back(ALL);
	history_position = 0;
}

//PRIVATE METHODS

//Takes the obj.path and splits it on '\\' to create a list of subdirectories and stores it to obj.path_chunks
//Example:
// "C:\Program Files (x86)\Steam\steamapps\common\Noita\mods\purgatory"
// becomes
// {"C:", "Program Files (x86)", "Steam", "steamapps", "common", "Noita", "mods", "purgatory"}
void FileBrowser::chunkifyPath() {
	path_chunks.clear();
	std::string temp;

	for (int i = 0; i < path.length(); i++) {
		if (path.substr(i, 1) == "\\") {
			path_chunks.push_back(temp);
			temp.clear();
		}
		else
		{
			temp.append(path.substr(i, 1));
		}
	}
	if (temp != "")
		path_chunks.push_back(temp);
}

//Tests letter drives A-Z to see if they are available.
//Stores available drives as strings in obj.drives
//Example: drives = {"C:\\", "D:\\"}
void FileBrowser::findDrives() {
	//Clear drives list just in case it already had data in it
	drives.clear();

	//Test for drives A to Z
	std::vector<std::string> drives_to_test;
	for (char c = 65; c <= 90; c++) {
		std::string temp;
		temp.append(1, c);
		temp.append(":\\");
		drives_to_test.push_back(temp);
	}

	for (std::string drive : drives_to_test) {
		std::filesystem::path temp_path_var(drive);
		if (std::filesystem::exists(temp_path_var))
			drives.push_back(drive);
	}
}

//Finds all files located in obj.path and stores the information in obj.files and obj.file_types
void FileBrowser::findFiles() {
	//clear files and file_types vectors of their previous values
	files.clear();
	file_types.clear();

	//Iterate through directory iterator
	for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
		//Turn entry into a path object
		std::filesystem::path entry_path = entry.path();

		//Get the file name and type form path
		std::string filename = entry_path.filename().string(); //filename() returns the "name.txt" from "name.txt"
		std::string fileextension = entry_path.extension().string(); //extension() returns the ".txt" from "name.txt"

		//Check if is a folder
		bool is_folder = std::filesystem::is_directory(entry_path);

		//Add filename to vector
		files.push_back(filename);

		//Determine what type of file it is.
		FileType ft = getFileTypeEnum(entry_path);

		//Add filetype to list
		file_types.push_back(ft);
	}
}

//Sorts the files in current folder.
//Folders will be on top. Folders will be sorted alphebetically.
//All other file types will be beneath the folders and sorted alphebetically.
void FileBrowser::sortFiles() {
	//First, bubble up the folders from the files
	unsigned folder_count = 0;
	for (int i = 0; i < files.size(); i++) {
		if (file_types[i] == FOLDER) {

			std::swap(files[folder_count], files[i]);
			std::swap(file_types[folder_count], file_types[i]);

			folder_count++;
		}
	}

	//Then sort the files.
	std::sort(files.begin(), files.begin() + folder_count, compFunc); //sorts folders alphebetically
	std::sort(files.begin() + folder_count, files.end(), compFunc); //sorts files alphebetically

	//Previous lines only sort files, need to manually sort file types again.
	for (int i = 0; i < file_types.size(); i++) {
		file_types[i] = getFileTypeEnum(path + "\\" + files[i]);
	}
}

//Gets the file type from file location
FileType FileBrowser::getFileTypeEnum(const std::string& path_str) {
	//Turn str into a path object
	std::filesystem::path path_var(path_str);

	//returns the ".txt" from "name.txt"
	std::string fileextension = path_var.extension().string();

	//Check if is a folder
	bool is_folder = std::filesystem::is_directory(path_var);

	//Determine what type of file it is.
	FileType ft = UNKNOWN;
	if (is_folder)
		ft = FOLDER;
	if (fileextension == ".txt")
		ft = TXT;
	if (fileextension == ".png")
		ft = PNG;
	if (fileextension == ".xml")
		ft = XML;
	if (fileextension == ".lua")
		ft = LUA;
	if (fileextension == ".md")
		ft = MD;
	if (fileextension == ".aseprite")
		ft = ASEPRITE;

	return ft;
}

FileType FileBrowser::getFileTypeEnum(const std::filesystem::path& path_var) {
	//returns the ".txt" from "name.txt"
	std::string fileextension = path_var.extension().string();

	//Check if is a folder
	bool is_folder = std::filesystem::is_directory(path_var);

	//Determine what type of file it is.
	FileType ft = UNKNOWN;
	if (is_folder)
		ft = FOLDER;
	if (fileextension == ".txt")
		ft = TXT;
	if (fileextension == ".png")
		ft = PNG;
	if (fileextension == ".xml")
		ft = XML;
	if (fileextension == ".lua")
		ft = LUA;
	if (fileextension == ".md")
		ft = MD;
	if (fileextension == ".aseprite")
		ft = ASEPRITE;

	return ft;
} //ToDo: clean this up a bit so one overload calls the other

//Update Path (WARNING: DOES NOT UPDATE HISTORY - use pushBackHistory() for that)
void FileBrowser::updatePath(std::string& path_str) {
	path = path_str;
	std::replace(path.begin(), path.end(), '/', '\\'); //change all '/'s to '\\'
	chunkifyPath();
	findDrives();
	findFiles();
	sortFiles();
}

//Update Display Mode (WARNING: DOES NOT UPDATE HISTORY - use pushBackHistory() for that)
void FileBrowser::updateDisplayMode(DisplayMode disp_mode) {
	display_mode = disp_mode;
}

//Respective ImGUI Displays for different display modes
void FileBrowser::displayMode_ALL() {
	//Display Files
	for (int i = 0; i < files.size(); i++) {
		FileType ft = file_types[i];
		ImGui::Image(file_icons[ft]->imgui_texture, file_icons[ft]->imgui_size);
		ImGui::SameLine();

		if (ImGui::Button(files[i].c_str())) {
			//If the selected button is a folder, go into folder.
			std::string new_path_str;
			for (int j = 0; j < path_chunks.size(); j++) {
				new_path_str.append(path_chunks[j]);
				new_path_str.append("/");
			}
			new_path_str.append(files[i]);

			if (ft == FOLDER) {
				pushBackHistory(new_path_str, ALL);
			}
			else {
				//This is the selected file, add to file queue
				//std::cout << "Selected File :\"" << new_path_str << "\"\n";
				file_queue.push_back(new_path_str);
			}
		}
	}
}

void FileBrowser::displayMode_ONLY_DRIVES() {
	//Display all drives the user has available
	ImGui::Text("Select Drive:");
	for (std::string drive : drives) {
		if (ImGui::Button(drive.c_str())) {
			updatePath(drive);
			display_mode = ALL;
		}
	}
}

void FileBrowser::displayMode_ONLY_PNG() {
	ImGui::Text("How did you get here?");
}

void FileBrowser::displayMode_ONLY_XML() {
	ImGui::Text("How did you get here?");
}

void FileBrowser::displayMode_ONLY_LUA() {
	ImGui::Text("How did you get here?");
}

void FileBrowser::displayMode_HISTORY() {
	ImGui::Text("Select point in history to go back to.");
	for (int i = 0; i < file_history.size(); i++) {
		if (ImGui::Button(file_history[i].c_str())) {
			history_position = i;
			updatePath(file_history[history_position]);
			updateDisplayMode(display_mode_history[history_position]);
		}
	}
}

void FileBrowser::displayMode_ONLY_ASEPRITE() {
	ImGui::Text("How did you get here?");
}

//PUBLIC METHODS

//Pop back once in path history
void FileBrowser::popBackHistory() {
	if (history_position >= 1) {
		history_position--;
		updatePath(file_history[history_position]);
		updateDisplayMode(display_mode_history[history_position]);
	}
}

//Push item to history
void FileBrowser::pushBackHistory(std::string path, DisplayMode disp_mode) {
	//If we're back in history and go forward, delete the history we traversed back through.
	if (history_position < file_history.size() - 1) {
		file_history.erase(file_history.begin() + history_position + 1, file_history.end());
		display_mode_history.erase(display_mode_history.begin() + history_position + 1, display_mode_history.end());
	}

	//Add new path and display mode onto history if the last history element isn't the same as the one we're adding
	if (file_history.size() > 0 && display_mode_history.size() > 0) {
		if (!(path == file_history.back() && disp_mode == display_mode_history.back())) {
			file_history.push_back(path);
			display_mode_history.push_back(disp_mode);
			updatePath(path);
			updateDisplayMode(disp_mode);
			history_position++;
		}
		else {
			//std::cout << "Trying to add the same thing to the history\n";
		}
	}
	else {
		//std::cout << "History vectors are size 0... What?\n";
	}
}

//Great name there Priskip
void FileBrowser::doTheImGuiThing() {

	//BEGIN This PC and History

	//This PC
	if (icon_this_pc != nullptr)
		if (ImGui::ImageButton(icon_this_pc->imgui_texture, icon_this_pc->imgui_size))
			pushBackHistory(file_history.back() , ONLY_DRIVES);
	
	//Back
	ImGui::SameLine();
	if (icon_back != nullptr)
		if (ImGui::ImageButton(icon_back->imgui_texture, icon_back->imgui_size))
			popBackHistory();

	//Forward
	ImGui::SameLine();
	if (icon_forward != nullptr) {

		ImVec2 size = icon_forward->imgui_size;
		ImVec2 uv0 = ImVec2(0.0f, 0.0f);
		ImVec2 uv1 = ImVec2(1.0f, 1.0f);
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImVec4 tint_col_grayed_out = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		ImVec4 tint_col_no_tint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

		if (history_position < file_history.size() - 1) {
			//We're back in the history, allow user to go forward
			if (ImGui::ImageButton("forward button", icon_forward->imgui_texture, size, uv0, uv1, bg_col, tint_col_no_tint)) {
				history_position++;
				updatePath(file_history[history_position]);
				updateDisplayMode(display_mode_history[history_position]);
			}
		}
		else {
			//We're at the latest point in history. Gray out forward button.
			if (ImGui::ImageButton("forward button", icon_forward->imgui_texture, size, uv0, uv1, bg_col, tint_col_grayed_out)) {
				//std::cout << "Nothing to go forward to.";
			}
		}
	}

	//History
	ImGui::SameLine();
	if (icon_history != nullptr)
		if (ImGui::ImageButton(icon_history->imgui_texture, icon_history->imgui_size))
			pushBackHistory(file_history.back(), HISTORY);

	//END This PC and History

	//Current path and allows user to back track in it
	for (int i = 0; i < path_chunks.size(); i++) {
		if (i != 0)
			ImGui::SameLine();
		if (ImGui::Button(path_chunks[i].c_str())) {
			std::string new_path_str;
			for (int j = 0; j <= i; j++) {
				new_path_str.append(path_chunks[j]);
				new_path_str.append("/");
			}
			pushBackHistory(new_path_str, display_mode_history.back());
		}
	}

	//Switch on display mode
	ImGui::SeparatorText("");
	switch (display_mode) {
	case ALL:
		displayMode_ALL();
		break;
	case ONLY_DRIVES:
		displayMode_ONLY_DRIVES();
		break;
	case ONLY_PNG:
		displayMode_ONLY_PNG();
		break;
	case ONLY_XML:
		displayMode_ONLY_XML();
		break;
	case ONLY_LUA:
		displayMode_ONLY_LUA();
		break;
	case HISTORY:
		displayMode_HISTORY();
		break;
	}
}

//Prints history information to an imgui window for debugging purposes
void FileBrowser::displayDebugInfo() {
	ImGui::Text("int history_position = %d", history_position);
	for (int i = 0; i < file_history.size(); i++) {
		if (i == history_position) {
			//Display in colored text
			ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);
			ImGui::TextColored(color, file_history[i].c_str());
			ImGui::SameLine();
			ImGui::TextColored(color, "     %d", display_mode_history[i]);
		}
		else {
			//Display in normal text
			ImGui::Text(file_history[i].c_str());
			ImGui::SameLine();
			ImGui::Text("     %d", display_mode_history[i]);
		}
	}
}

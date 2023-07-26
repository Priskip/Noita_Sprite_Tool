#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "imgui.h"
#include "Textures.h"

//ENUMS
enum DisplayMode { ALL, ONLY_DRIVES, ONLY_PNG, ONLY_XML, ONLY_LUA, HISTORY, ONLY_ASEPRITE };
enum FileType { UNKNOWN, FOLDER, TXT, PNG, XML, LUA, MD, ASEPRITE };

class FileBrowser {
private:
	//DATA VALUES
	std::string path;
	std::vector<std::string> path_chunks;
	std::vector<std::string> files;
	std::vector<FileType> file_types;
	std::vector<std::string> drives;
	DisplayMode display_mode = ALL;

	//TEXTURES
	Texture* icon_unknown;
	Texture* icon_folder;
	Texture* icon_txt;
	Texture* icon_png;
	Texture* icon_xml;
	Texture* icon_lua;
	Texture* icon_md;
	Texture* icon_this_pc;
	Texture* icon_back;
	Texture* icon_forward;
	Texture* icon_history;
	Texture* icon_aseprite;

	std::vector<Texture*> file_icons;

	//History
	std::vector<std::string> file_history;
	std::vector<DisplayMode> display_mode_history;
	int history_position;

	//METHODS

	//Takes the obj.path and splits it on '\\' to create a list of 
	//subdirectories and stores it to obj.path_chunks
	void chunkifyPath();

	//Tests letter drives A-Z to see if they are available.
	//Stores available drives as strings in obj.drives
	//Example: drives = {"C:\\", "D:\\"}
	void findDrives();

	//Finds all files located in obj.path and stores the information in obj.files and obj.file_types
	void findFiles();

	//Sorts the files in current folder.
	//Folders will be on top. Folders will be sorted alphebetically.
	//All other file types will be beneath the folders and sorted alphebetically.
	void sortFiles();

	//Gets the file type from file location
	FileType getFileTypeEnum(const std::string &path_str);
	FileType getFileTypeEnum(const std::filesystem::path& path_var);

	//Update Path (WARNING: DOES NOT UPDATE HISTORY - use pushBackHistory() for that)
	void updatePath(std::string& path_str);
	
	//Update Display Mode (WARNING: DOES NOT UPDATE HISTORY - use pushBackHistory() for that)
	void updateDisplayMode(DisplayMode disp_mode);

	//Respective ImGUI Displays for different display modes
	void displayMode_ALL();
	void displayMode_ONLY_DRIVES();
	void displayMode_ONLY_PNG();
	void displayMode_ONLY_XML();
	void displayMode_ONLY_LUA();
	void displayMode_HISTORY();
	void displayMode_ONLY_ASEPRITE();

public:
	//Queued Files
	std::vector<std::string> file_queue;

	//CONSTRUCTORS
	FileBrowser(const std::string& path_str);

	//METHODS

	//Pop back once in path history
	void popBackHistory();

	//Push item to history
	void pushBackHistory(std::string path, DisplayMode disp_mode);

	//Great name there Priskip
	void doTheImGuiThing();

	//Prints history information to an imgui window for debugging purposes
	void displayDebugInfo();

};
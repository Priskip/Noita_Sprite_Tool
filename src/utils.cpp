#include <string>
#include <fstream>
#include <filesystem>

//returns a bool if file exists
bool doesFileExist(std::string file_path) {
	bool exists = false;

	std::ifstream file;
	file.open(file_path);
	if (file.is_open()) {
		exists = true;
	}

	file.close();

	return exists;
}

//String to Bool
//"true" || "1" --> true
//"false" || "0" --> false
//Anything else --> false
bool stringToBool(std::string str) {
	bool boolean = false;

	if (str == "true" || str == "1") {
		boolean = true;
	}

	return boolean;
}

//Returns string "Enabled" or "Disabled" based on bool
std::string enabledOrDisabled(bool enabled) {
	if (enabled) {
		return "Enabled";
	}
	else {
		return "Disabled";
	}
}

//Returns where or not a png image is real and exists
bool isPNGThatExists(const char name[]) {
	
	if (doesFileExist(name)) {
		std::filesystem::path path_var(name);
		std::string file_extension = path_var.extension().string();

		if (file_extension == ".png") {
			return true;
		}
	}
	return false;
}
#pragma once
#include <string>

//returns a bool if file exists
bool doesFileExist(std::string file_path);

//String to Bool
//"true" || "1" --> true
//"false" || "0" --> false
//Anything else --> false
bool stringToBool(std::string str);

//Returns string "Enabled" or "Disabled" based on bool
std::string enabledOrDisabled(bool enabled);

//Returns where or not a png image is real and exists
bool isPNGThatExists(const char name[]);
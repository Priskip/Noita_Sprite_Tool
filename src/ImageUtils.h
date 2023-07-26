#pragma once
#include <string>
#include <vector>

/*
To Make
- rgba to hsv
- hsv to rgba

- rgba to hex string
- hex string to rgba
*/

//Transforms an rgba vector to an hsv vector.
//HSV data with H in degrees [0,360), S [0,1], V [0,1]
void rgba2hsv(const std::vector<uint8_t>& rgba, std::vector<double>& hsv);

//Transforms an hsv vector to an rgba vector.
//WARNING: Alpha will always be returned as 255
void hsv2rgba(const std::vector<double>& hsv, std::vector<uint8_t>& rgba);

//Transforms an rgba vector to a hex string
void rgba2hexstr(const std::vector<uint8_t>& rgba, std::string* hex);

//Transforms a hex string to an rgba vector
void hexstr2rgba(const std::string* hex, std::vector<uint8_t>& rgba);

//Compares two RGBA vectors
bool compRGBA(const std::vector<uint8_t>& rgba1, const std::vector<uint8_t>& rgba2);

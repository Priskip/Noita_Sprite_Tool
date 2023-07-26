//Standards
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
//Priskip's
#include "ImageUtils.h"


//Transforms an rgba vector to an hsv vector.
//HSV data with H in degrees [0,360), S [0,1], V [0,1]
void rgba2hsv(const std::vector<uint8_t>& rgba, std::vector<double>& hsv) {
	//Convert to floats
	double rgb_prime[3];
	rgb_prime[0] = (double)rgba[0] / 255;
	rgb_prime[1] = (double)rgba[1] / 255;
	rgb_prime[2] = (double)rgba[2] / 255;

	//Calc c max min and delta
	double c_max = fmax(fmax(rgb_prime[0], rgb_prime[1]), rgb_prime[2]);
	double c_min = fmin(fmin(rgb_prime[0], rgb_prime[1]), rgb_prime[2]);;
	double delta = c_max - c_min;

	//Calc H
	if (delta == 0) {
		hsv[0] = 0;
	}
	else {
		if (c_max == rgb_prime[0]) {
			hsv[0] = 60 * ((rgb_prime[1] - rgb_prime[2]) / delta);
		}

		if (c_max == rgb_prime[1]) {
			hsv[0] = 60 * ((rgb_prime[2] - rgb_prime[0]) / delta + 2);
		}

		if (c_max == rgb_prime[2]) {
			hsv[0] = 60 * ((rgb_prime[0] - rgb_prime[1]) / delta + 4);
		}
	}

	//Make sure to return H in range of [0,360)
	while (hsv[0] < 0 || hsv[0] >= 360) {
		if (hsv[0] < 0) {
			hsv[0] = hsv[0] + 360;
		}

		if (hsv[0] >= 360) {
			hsv[0] = hsv[0] - 360;
		}
	}

	//Calc S
	if (c_max == 0) {
		hsv[1] = 0;
	}
	else {
		hsv[1] = delta / c_max;
	}

	//Calc V
	hsv[2] = c_max;
}

//Transforms an hsv vector to an rgba vector.
//WARNING: Alpha will always be returned as 255
void hsv2rgba(const std::vector<double>& hsv, std::vector<uint8_t>& rgba) {
	double c = hsv[1] * hsv[2]; //c = SV
	double m = hsv[2] - c; //m = V-c
	double x = c * (1 - fabs(fmod((hsv[0] / 60), 2) - 1));
	double rgb_prime[3] = { 0, 0, 0 };

	if (hsv[0] >= 0 && hsv[0] < 60) {
		rgb_prime[0] = c;
		rgb_prime[1] = x;
		rgb_prime[2] = 0;
	}
	if (hsv[0] >= 60 && hsv[0] < 120) {
		rgb_prime[0] = x;
		rgb_prime[1] = c;
		rgb_prime[2] = 0;
	}
	if (hsv[0] >= 120 && hsv[0] < 180) {
		rgb_prime[0] = 0;
		rgb_prime[1] = c;
		rgb_prime[2] = x;
	}
	if (hsv[0] >= 180 && hsv[0] < 240) {
		rgb_prime[0] = 0;
		rgb_prime[1] = x;
		rgb_prime[2] = c;
	}
	if (hsv[0] >= 240 && hsv[0] < 300) {
		rgb_prime[0] = x;
		rgb_prime[1] = 0;
		rgb_prime[2] = c;
	}
	if (hsv[0] >= 300 && hsv[0] < 360) {
		rgb_prime[0] = c;
		rgb_prime[1] = 0;
		rgb_prime[2] = x;
	}

	rgba[0] = (uint8_t)round((255 * (rgb_prime[0] + m)));
	rgba[1] = (uint8_t)round((255 * (rgb_prime[1] + m)));
	rgba[2] = (uint8_t)round((255 * (rgb_prime[2] + m)));

	rgba[3] = (uint8_t)255;
}

//Transforms an rgba vector to a hex string
void rgba2hexstr(const std::vector<uint8_t>& rgba, std::string* hex) {
	std::stringstream str_stream;
	str_stream << std::hex << (int)rgba[0] << (int)rgba[1] << (int)rgba[2] << (int)rgba[3];
	*hex = str_stream.str();
}

//Transforms a hex string to an rgba vector
void hexstr2rgba(const std::string* hex, std::vector<uint8_t>& rgba) {
	unsigned int x;

	for (int i = 0; i < 4; i++) {
		std::stringstream str_stream;
		str_stream << std::hex << (*hex).substr(2 * i, 2);
		str_stream >> x;
		rgba[i] = x;
	}
}

//Compares two RGBA vectors
bool compRGBA(const std::vector<uint8_t>& rgba1, const std::vector<uint8_t>& rgba2) {
	return (rgba1[0] == rgba2[0]) && (rgba1[1] == rgba2[1]) && (rgba1[2] == rgba2[2]) && (rgba1[3] == rgba2[3]);
}

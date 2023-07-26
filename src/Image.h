#pragma once
#include <vector>
#include <string>

enum BlendMode { NONE, ADDITIVE };

class Image {
private:
	// --- MEMEBER VARIABLES ---
	// -------------------------
	std::string file_name;
	std::vector<uint8_t> pixel_data;
	unsigned width; //Width of the Image
	unsigned height; //Height of the Image
	unsigned error; //LodePNG Error Codes

	// --- MEMBER FUNCTIONS ---
	// ------------------------

	//Is x position in the bounds of the image?
	bool x_inBounds(unsigned x);

	//Is y position in the bounds of the image?
	bool y_inBounds(unsigned y);

public:
	// --- MEMEBER VARIABLES ---
	// -------------------------


	// --- CONSTRUCTORS ---
	// --------------------

	//Initializes the image class using LodePNG.
	Image(std::string& file_path);

	//Initializes a blank image of size given to it.
	Image(unsigned x, unsigned y);

	//Initialized a blank image
	Image();

	// --- MEMBER FUNCTIONS ---
	// ------------------------

	//Reads a png image to pixel_data
	void readImage(std::string& file_path);

	//Writes stored pixel_data to a png image
	void writeImage(std::string& file_path);

	//Reads color from pixel_data to rgba vector provided
	void getPixel(unsigned pos_x, unsigned pos_y, std::vector<uint8_t>& rgba);

	//Reads color from pixel_data to rgba vector provided
	void setPixel(unsigned pos_x, unsigned pos_y, const std::vector<uint8_t>& rgba);

	//Is the position in the bounds of the image?
	bool isInImage(unsigned pos_x, unsigned pos_y);

	//Crops image to specified x and y pairs
	void cropImage(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

	//Writes an image on top of the base image
	void overlayImage(Image* im, unsigned pos_x, unsigned pos_y, BlendMode blend_mode);

	//Writes an image on top of the base image and swaps one particular color with another.
	void overlayImage(Image* im, unsigned pos_x, unsigned pos_y, BlendMode blend_mode, const std::vector<uint8_t>& swap_color_from, const std::vector<uint8_t>& swap_color_to);

	//Get Width
	unsigned getWidth();

	//Get Height
	unsigned getHeight();
};
//Standards
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

//Libraries
#include "lodepng.h"

//Priskip's
#include "Image.h"
#include "ImageUtils.h"

// --- ------ ---
// --- PRIVATE ---
// --- ------ ---

//Is x position in the bounds of the image?
bool Image::x_inBounds(unsigned x) {
	return 0 <= x && x < width;
}

//Is y position in the bounds of the image?
bool Image::y_inBounds(unsigned y) {
	return 0 <= y && y < height;
}

// --- ------ ---
// --- PUBLIC ---
// --- ------ ---

// --- CONSTRUCTORS ---
// --------------------

//Initializes the image class using LodePNG.
Image::Image(std::string& file_path) {
	readImage(file_path);
}

//Initializes a blank image of size given to it.
Image::Image(unsigned x, unsigned y) {
	width = x;
	height = y;
	pixel_data.resize(4 * width * height);
	error = 0;
	file_name = "";
}

//Initialized a blank image
Image::Image() {
	width = 0;
	height = 0;
	pixel_data.resize(0);
	error = 0;
	file_name = "";
}

// --- MEMBER FUNCTIONS ---
// ------------------------

//Reads a png image to pixel_data
void Image::readImage(std::string& file_path) {
	//Set file name
	file_name = file_path;

	//Read image with LodePNG
	error = lodepng::decode(pixel_data, width, height, file_name.c_str());

	//Display any LodePNG Error
	if (error) {
		std::cout << "Image::Image(" << file_path << ") - lodepng decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}
}

//Writes stored pixel_data to a png image
void Image::writeImage(std::string& file_path) {
	//Write image with LodePNG
	error = lodepng::encode(file_path.c_str(), pixel_data, width, height);

	//Display lodePNG error if any
	if (error) {
		std::cout << "Image::writeImageToFile(" << file_path << ") - lodepng encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}
}

//Reads color from pixel_data to rgba vector provided
void Image::getPixel(unsigned pos_x, unsigned pos_y, std::vector<uint8_t>& rgba) {
	rgba[0] = pixel_data[4 * width * pos_y + 4 * pos_x + 0];
	rgba[1] = pixel_data[4 * width * pos_y + 4 * pos_x + 1];
	rgba[2] = pixel_data[4 * width * pos_y + 4 * pos_x + 2];
	rgba[3] = pixel_data[4 * width * pos_y + 4 * pos_x + 3];
}

//Reads color from pixel_data to rgba vector provided
void Image::setPixel(unsigned pos_x, unsigned pos_y, const std::vector<uint8_t>& rgba) {
	pixel_data[4 * width * pos_y + 4 * pos_x + 0] = rgba[0];
	pixel_data[4 * width * pos_y + 4 * pos_x + 1] = rgba[1];
	pixel_data[4 * width * pos_y + 4 * pos_x + 2] = rgba[2];
	pixel_data[4 * width * pos_y + 4 * pos_x + 3] = rgba[3];
}

//Is the position in the bounds of the image?
bool Image::isInImage(unsigned pos_x, unsigned pos_y) {
	return x_inBounds(pos_x) && x_inBounds(pos_y);
}

//Crops image to specified x and y pairs
void Image::cropImage(unsigned x1, unsigned y1, unsigned x2, unsigned y2) {

	//First, make sure that x1 < x2 and y1 < y2
	if (x1 > x2) {
		std::swap(x1, x2);
	}

	if (y1 < y2) {
		std::swap(y1, y2);
	}

	//Next, check if all bounds are in image.
	bool bounds_are_in_image = x_inBounds(x1) && x_inBounds(x2) && y_inBounds(y1) && y_inBounds(y2);

	//Now we can crop the image.
	if (bounds_are_in_image) {
		//Store old width
		unsigned old_width = width;

		//Set Width and Height
		width = x2 - x1 + 1;
		height = y2 - y1 + 1;

		//New pixeldata vector that we write the subselection of old data into
		std::vector<unsigned char> new_pixel_data;
		new_pixel_data.resize(4 * width * height);

		//Read old pixel data into new pixel data.
		int x_this = 0;
		int y_this = 0;
		for (int x_that = x1; x_that < x1 + width; x_that++) {
			y_this = 0; //reset y_this each time in the loop
			for (int y_that = y1; y_that < y1 + height; y_that++) {
				new_pixel_data[4 * width * y_this + 4 * x_this + 0] = pixel_data[4 * old_width * y_that + 4 * x_that + 0];
				new_pixel_data[4 * width * y_this + 4 * x_this + 1] = pixel_data[4 * old_width * y_that + 4 * x_that + 1];
				new_pixel_data[4 * width * y_this + 4 * x_this + 2] = pixel_data[4 * old_width * y_that + 4 * x_that + 2];
				new_pixel_data[4 * width * y_this + 4 * x_this + 3] = pixel_data[4 * old_width * y_that + 4 * x_that + 3];
				y_this++;
			}
			x_this++;
		}

		//Set Pixel Data
		pixel_data = new_pixel_data;
	}

}


//Writes an image on top of the base image
void Image::overlayImage(Image* im, unsigned pos_x, unsigned pos_y, BlendMode blend_mode) {

	//Read rgba in image
	//if transparent - ignore
	//if not transparent - overwrite what's in rgba of this image if it's within this image's bounds
	int x_im = 0;
	int y_im = 0;

	std::vector<uint8_t> rgba;
	rgba.push_back(0);
	rgba.push_back(0);
	rgba.push_back(0);
	rgba.push_back(0);

	for (unsigned x = pos_x; x < pos_x + im->width; x++) {
		y_im = 0;
		for (unsigned y = pos_y; y < pos_y + im->height; y++) {
			//If we're in the scope of our image
			if (isInImage(x, y)) {
				//Read pixel data at point
				im->getPixel(x_im, y_im, rgba);

				//If fully colored pixel
				if (rgba[3] == 255) {
					//write pixel into our image
					setPixel(x, y, rgba);
				}
			}
			y_im++;
		}
		x_im++;
	}
}

//Writes an image on top of the base image and swaps one particular color with another.
void Image::overlayImage(Image* im, unsigned pos_x, unsigned pos_y, BlendMode blend_mode, const std::vector<uint8_t>& swap_color_from, const std::vector<uint8_t>& swap_color_to) {

	//Read rgba in image
	//if transparent - ignore
	//if not transparent - overwrite what's in rgba of this image if it's within this image's bounds
	int x_im = 0;
	int y_im = 0;

	std::vector<uint8_t> rgba;
	rgba.push_back(0);
	rgba.push_back(0);
	rgba.push_back(0);
	rgba.push_back(0);

	for (unsigned x = pos_x; x < pos_x + im->width; x++) {
		y_im = 0;
		for (unsigned y = pos_y; y < pos_y + im->height; y++) {
			//If we're in the scope of our image
			if (isInImage(x, y)) {
				//Read pixel data at point
				im->getPixel(x_im, y_im, rgba);

				//Check if color is swap color
				if (compRGBA(rgba, swap_color_from)) {
					rgba = swap_color_to;
				}

				//If fully colored pixel
				if (rgba[3] == 255) {
					//write pixel into our image
					setPixel(x, y, rgba);
				}
			}
			y_im++;
		}
		x_im++;
	}
}

//Get Width
unsigned Image::getWidth() {
	return width;
}

//Get Height
unsigned Image::getHeight() {
	return height;
}
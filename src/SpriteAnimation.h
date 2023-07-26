#pragma once
#include <string>
#include <vector>
#include "Image.h"

class SpriteAnimation {
	//Custom Structs
	struct Sprite
	{
		std::string default_animation = "";
		std::string filename = "";
		int offset_x = 0;
		int offset_y = 0;
	};

	struct RectAnimation
	{
		std::string name = "";
		std::string next_animation = "";
		unsigned frame_count = 0;
		unsigned frame_width = 0;
		unsigned frame_height = 0;
		unsigned frames_per_row = 0;
		unsigned pos_x = 0;
		unsigned pos_y = 0;
		float frame_wait = 0.0;
		bool loop = false;
		bool shrink_by_one_pixel = false; //Removes furthest right column and lowest row of each animation frame
		bool has_offset = false; //needs to be set to true for offset to take place
		int offset_x = 0; //positive x offset pushes frame left, negative pushes right
		int offset_y = 0; //positive y offset pushes frame up, negative pushes down
	};

private:

public:
	//MEMBER VARIABLES
	Sprite sprite_info;
	std::vector<RectAnimation> rect_animations;
	Image sprite_image;

	//CONSTRUCTORS
	SpriteAnimation(std::string& path_str_to_xml);
	

	//METHODS

	//Dump all info to console
	void dumpInfo();

	//Reads all information stored within a Noita sprite animation xml file.
	void readSpriteXMLData(std::string& path_str_to_xml);

	//Returns the index in the vector of where the rect animation with name "name" is. Returns -1 if rect animation with name "name" does not exist.
	int getRectAnimationIndexFromName(std::string& name);

	//Returns the name of rect animation[i]. Returns "unknown" if index is not in rect_animations.
	std::string getRectAnimationNameFromIndex(int index);

};
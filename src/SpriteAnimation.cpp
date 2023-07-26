#include "SpriteAnimation.h"
#include <string>
#include <vector>
#include <iostream>
#include "tinyxml2.h"
#include "Image.h"
#include "utils.h"

//CONSTRUCTORS
SpriteAnimation::SpriteAnimation(std::string& path_str_to_xml) {
	//Read in sprite animation info
	readSpriteXMLData(path_str_to_xml);

	//Reads image defined by sprite_info
	sprite_image.readImage(sprite_info.filename);
}

//PUBLICS

//Dump all info to console
void SpriteAnimation::dumpInfo() {
	std::cout << "Sprite" << "\n";
	std::cout << "\t" << "default_animation = \"" << sprite_info.default_animation << "\"\n";
	std::cout << "\t" << "filename = \"" << sprite_info.filename << "\"\n";
	std::cout << "\t" << "offset_x = \"" << sprite_info.offset_x << "\"\n";
	std::cout << "\t" << "offset_y = \"" << sprite_info.offset_y << "\"\n";
	std::cout << "\n";

	for (int i = 0; i < rect_animations.size(); i++) {
		std::cout << "RectAnimation[" << i << "]\n";
		std::cout << "\t" << "name = \"" << rect_animations[i].name << "\"\n";
		std::cout << "\t" << "next_animation = \"" << rect_animations[i].next_animation << "\"\n";
		std::cout << "\t" << "frame_count = \"" << rect_animations[i].frame_count << "\"\n";
		std::cout << "\t" << "frame_width = \"" << rect_animations[i].frame_width << "\"\n";
		std::cout << "\t" << "frame_height = \"" << rect_animations[i].frame_height << "\"\n";
		std::cout << "\t" << "frames_per_row = \"" << rect_animations[i].frames_per_row << "\"\n";
		std::cout << "\t" << "pos_x = \"" << rect_animations[i].pos_x << "\"\n";
		std::cout << "\t" << "pos_y = \"" << rect_animations[i].pos_y << "\"\n";
		std::cout << "\t" << "frame_wait = \"" << rect_animations[i].frame_wait << "\"\n";
		std::cout << "\t" << "loop = \"" << rect_animations[i].loop << "\"\n";
		std::cout << "\t" << "shrink_by_one_pixel = \"" << rect_animations[i].shrink_by_one_pixel << "\"\n";
		std::cout << "\t" << "has_offset = \"" << rect_animations[i].has_offset << "\"\n";
		std::cout << "\t" << "offset_x = \"" << rect_animations[i].offset_x << "\"\n";
		std::cout << "\t" << "offset_y = \"" << rect_animations[i].offset_y << "\"\n";
		std::cout << "\n";
	}
}

//Reads all information stored within a Noita sprite animation xml file.
void SpriteAnimation::readSpriteXMLData(std::string& path_str_to_xml) {
	//Clear previous rect animation info
	rect_animations.clear();

	//Check to see if file exists before attempting to read info
	if (doesFileExist(path_str_to_xml)) {

		//Load XML file with tinyxml2
		tinyxml2::XMLDocument sprite_XML_doc;
		sprite_XML_doc.LoadFile(path_str_to_xml.c_str());

		//BEGIN Root Element Information (struct Sprite)
		tinyxml2::XMLElement* pRootElement = sprite_XML_doc.RootElement();

		if (pRootElement->FindAttribute("default_animation") != nullptr)
			sprite_info.default_animation = pRootElement->FindAttribute("default_animation")->Value();

		if (pRootElement->FindAttribute("filename") != nullptr)
			sprite_info.filename = pRootElement->FindAttribute("filename")->Value();

		if (pRootElement->FindAttribute("offset_x") != nullptr)
			sprite_info.offset_x = std::stoi(pRootElement->FindAttribute("offset_x")->Value());

		if (pRootElement->FindAttribute("offset_y") != nullptr)
			sprite_info.offset_y = std::stoi(pRootElement->FindAttribute("offset_y")->Value());
		//END Root Element Information

		//BEGIN Child Element Information (struct RectAnimation)
		for (
			tinyxml2::XMLElement* child_element = pRootElement->FirstChildElement();
			child_element != nullptr;
			child_element = child_element->NextSiblingElement()
			)
		{
			//Test if we're a RectAnimation
			std::string child_elem_name = child_element->Name();
			if (child_elem_name == "RectAnimation") {
				//Read Data
				RectAnimation frame;

				if (child_element->FindAttribute("name") != nullptr)
					frame.name = child_element->FindAttribute("name")->Value();

				if (child_element->FindAttribute("next_animation") != nullptr)
					frame.next_animation = child_element->FindAttribute("next_animation")->Value();

				if (child_element->FindAttribute("frame_count") != nullptr)
					frame.frame_count = std::stoi(child_element->FindAttribute("frame_count")->Value());

				if (child_element->FindAttribute("frame_width") != nullptr)
					frame.frame_width = std::stoi(child_element->FindAttribute("frame_width")->Value());

				if (child_element->FindAttribute("frame_height") != nullptr)
					frame.frame_height = std::stoi(child_element->FindAttribute("frame_height")->Value());

				if (child_element->FindAttribute("frames_per_row") != nullptr)
					frame.frames_per_row = std::stoi(child_element->FindAttribute("frames_per_row")->Value());

				if (child_element->FindAttribute("pos_x") != nullptr)
					frame.pos_x = std::stoi(child_element->FindAttribute("pos_x")->Value());

				if (child_element->FindAttribute("pos_y") != nullptr)
					frame.pos_y = std::stoi(child_element->FindAttribute("pos_y")->Value());

				if (child_element->FindAttribute("pos_y") != nullptr)
					frame.pos_y = std::stoi(child_element->FindAttribute("pos_y")->Value());

				if (child_element->FindAttribute("frame_wait") != nullptr)
					frame.frame_wait = std::stof(child_element->FindAttribute("frame_wait")->Value());

				if (child_element->FindAttribute("loop") != nullptr)
					frame.loop = stringToBool(child_element->FindAttribute("loop")->Value());

				if (child_element->FindAttribute("shrink_by_one_pixel") != nullptr)
					frame.shrink_by_one_pixel = stringToBool(child_element->FindAttribute("shrink_by_one_pixel")->Value());

				if (child_element->FindAttribute("has_offset") != nullptr)
					frame.has_offset = stringToBool(child_element->FindAttribute("has_offset")->Value());

				if (child_element->FindAttribute("offset_x") != nullptr)
					frame.offset_x = std::stoi(child_element->FindAttribute("offset_x")->Value());

				if (child_element->FindAttribute("offset_y") != nullptr)
					frame.offset_y = std::stoi(child_element->FindAttribute("offset_y")->Value());

				//Add this frame to the vector of animation_states
				rect_animations.push_back(frame);
			}
		}
		//END Child Element Information (struct RectAnimation)
	}

}

//Returns the index in the vector of where the rect animation with name "name" is. Returns -1 if rect animation with name "name" does not exist.
int SpriteAnimation::getRectAnimationIndexFromName(std::string& name) {
	int ra_pos = -1;
	for (RectAnimation ra : rect_animations) {
		ra_pos++;
		if (ra.name == name)
			break;
	}
	return ra_pos;
}

//Returns the name of rect animation[i]. Returns "unknown" if index is not in rect_animations.
std::string SpriteAnimation::getRectAnimationNameFromIndex(int index) {
	std::string str = "unknown";

	if (index >= 0 && index < rect_animations.size()) {
		str = rect_animations[index].name;
	}

	return str;
}
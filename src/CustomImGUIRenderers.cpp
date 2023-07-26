#include "CustomImGUIRenderers.h"
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "imgui.h"
#include "Image.h"


//Displays a type Image from Image.h inside an imgui window.
void displayImage(Image& im) {
	//ImGui Draw List and Screen Pos
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();

	//RGBA Vector for storing pixel colors to
	std::vector<uint8_t> rgba = { 0, 0, 0, 0 };

	//ImGui Color Variables
	ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImU32 col = ImColor(colf);

	//Loop through each pixel
	for (unsigned y = 0; y < im.getHeight(); y++) {
		for (unsigned x = 0; x < im.getWidth(); x++) {
			//Get pixel color
			im.getPixel(x, y, rgba);

			//Translate the color to ImGui's standards
			col = ImColor(rgba[0], rgba[1], rgba[2], rgba[3]);

			//Add to draw list
			draw_list->AddRectFilled(ImVec2(p.x + x, p.y + y), ImVec2(p.x + x + 1, p.y + y + 1), col);
		}
	}
}

void displayImage(Image& im, unsigned scale) {
	//ImGui Draw List and Screen Pos
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();

	//RGBA Vector for storing pixel colors to
	std::vector<uint8_t> rgba = { 0, 0, 0, 0 };

	//ImGui Color Variables
	ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImU32 col = ImColor(colf);

	//Loop through each pixel
	for (unsigned y = 0; y < im.getHeight(); y++) {
		for (unsigned x = 0; x < im.getWidth(); x++) {
			//Get pixel color
			im.getPixel(x, y, rgba);

			//Translate the color to ImGui's standards
			col = ImColor(rgba[0], rgba[1], rgba[2], rgba[3]);

			//Add to draw list
			draw_list->AddRectFilled(
				ImVec2(p.x + scale * x, p.y + scale * y),
				ImVec2(p.x + scale * (x + 1), p.y + scale * (y + 1)),
				col);
		}
	}
}


//Displays a subsection defined by x1,y1, x2,y2 of type Image from Image.h inside an imgui window.
void displayImage(Image& im, unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
	//Check if x1>x2 or y1>y2 and flip if they are.
	if (x1 > x2) {
		unsigned temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 > y2) {
		unsigned temp = y1;
		y1 = y2;
		y2 = temp;
	}

	//Check if values provided are in bounds of image
	bool x_in_bounds = x1 >= 0 && x1 <= im.getWidth() && x2 >= 0 && x2 <= im.getWidth();
	bool y_in_bounds = y1 >= 0 && y1 <= im.getHeight() && y2 >= 0 && y2 <= im.getHeight();

	//If bounds are good, then draw
	if (x_in_bounds && y_in_bounds) {
		//ImGui Draw List and Screen Pos
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 p = ImGui::GetCursorScreenPos();

		//RGBA Vector for storing pixel colors to
		std::vector<uint8_t> rgba = { 0, 0, 0, 0 };

		//ImGui Color Variables
		ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImU32 col = ImColor(colf);

		//Loop through each pixel
		int sx = 0; //used for tracking where in the window to print to
		int sy = 0;
		for (unsigned y = y1; y < y2; y++) {
			for (unsigned x = x1; x < x2; x++) {
				//Get pixel color
				im.getPixel(x, y, rgba);

				//Translate the color to ImGui's standards
				col = ImColor(rgba[0], rgba[1], rgba[2], rgba[3]);

				//Add to draw list
				draw_list->AddRectFilled(ImVec2(p.x + sx, p.y + sy), ImVec2(p.x + sx + 1, p.y + sy + 1), col);

				//Increment sx
				sx++;
			}
			//Increment sy
			sx = 0;
			sy++;
		}
	}
}

void displayImage(Image& im, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned scale) {
	//Check if x1>x2 or y1>y2 and flip if they are.
	if (x1 > x2) {
		unsigned temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 > y2) {
		unsigned temp = y1;
		y1 = y2;
		y2 = temp;
	}

	//Check if values provided are in bounds of image
	bool x_in_bounds = x1 >= 0 && x1 <= im.getWidth() && x2 >= 0 && x2 <= im.getWidth();
	bool y_in_bounds = y1 >= 0 && y1 <= im.getHeight() && y2 >= 0 && y2 <= im.getHeight();

	//If bounds are good, then draw
	if (x_in_bounds && y_in_bounds) {
		//ImGui Draw List and Screen Pos
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 p = ImGui::GetCursorScreenPos();

		//RGBA Vector for storing pixel colors to
		std::vector<uint8_t> rgba = { 0, 0, 0, 0 };

		//ImGui Color Variables
		ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImU32 col = ImColor(colf);

		//Loop through each pixel
		int sx = 0; //used for tracking where in the window to print to
		int sy = 0;
		for (unsigned y = y1; y < y2; y++) {
			for (unsigned x = x1; x < x2; x++) {
				//Get pixel color
				im.getPixel(x, y, rgba);

				//Translate the color to ImGui's standards
				col = ImColor(rgba[0], rgba[1], rgba[2], rgba[3]);

				//Add to draw list
				draw_list->AddRectFilled(
					ImVec2(p.x + scale * sx, p.y + scale * sy),
					ImVec2(p.x + scale * (sx + 1), p.y + scale * (sy + 1)),
					col);

				//Increment sx
				sx++;
			}
			//Increment sy
			sx = 0;
			sy++;
		}
	}
}



//Draws the frame of animation based off the imgui app's runtime (ImGui is 60fps)
void drawSpriteAnimation(SpriteAnimation& sprite_animation, unsigned ra_index, int imgui_frame_count) {
	//Calculate which frame of the animation to display based off the imgui_frame_count, rect_animation.frame_wait, and rect_animation.frame_count
	// frame_to_display = floor( mod( imgui_frame_count / round(frame_wait * 60), frame_count))
	unsigned temp = round(sprite_animation.rect_animations[ra_index].frame_wait * 60);
	unsigned frame_to_display = floor((imgui_frame_count / temp) % (sprite_animation.rect_animations[ra_index].frame_count));

	//Calc sub image positions
	unsigned grid_x = frame_to_display % sprite_animation.rect_animations[ra_index].frames_per_row;
	unsigned grid_y = frame_to_display / sprite_animation.rect_animations[ra_index].frames_per_row;

	unsigned x1 = sprite_animation.rect_animations[ra_index].pos_x + sprite_animation.rect_animations[ra_index].frame_width * grid_x;
	unsigned x2 = x1 + sprite_animation.rect_animations[ra_index].frame_width;
	unsigned y1 = sprite_animation.rect_animations[ra_index].pos_y + sprite_animation.rect_animations[ra_index].frame_height * grid_y;
	unsigned y2 = y1 + sprite_animation.rect_animations[ra_index].frame_height;

	//Display
	displayImage(sprite_animation.sprite_image, x1, y1, x2, y2);
}

void drawSpriteAnimation(SpriteAnimation& sprite_animation, std::string& animation_name, int imgui_frame_count) {
	//Get index
	int ra_index = sprite_animation.getRectAnimationIndexFromName(animation_name);
	drawSpriteAnimation(sprite_animation, ra_index, imgui_frame_count);
}

void drawSpriteAnimation(SpriteAnimation& sprite_animation, unsigned ra_index, int imgui_frame_count, unsigned scale) {
	//Calculate which frame of the animation to display based off the imgui_frame_count, rect_animation.frame_wait, and rect_animation.frame_count
	// frame_to_display = floor( mod( imgui_frame_count / round(frame_wait * 60), frame_count))
	unsigned temp = round(sprite_animation.rect_animations[ra_index].frame_wait * 60);
	unsigned frame_to_display = floor((imgui_frame_count / temp) % (sprite_animation.rect_animations[ra_index].frame_count));

	//Calc sub image positions
	unsigned grid_x = frame_to_display % sprite_animation.rect_animations[ra_index].frames_per_row;
	unsigned grid_y = frame_to_display / sprite_animation.rect_animations[ra_index].frames_per_row;

	unsigned x1 = sprite_animation.rect_animations[ra_index].pos_x + sprite_animation.rect_animations[ra_index].frame_width * grid_x;
	unsigned x2 = x1 + sprite_animation.rect_animations[ra_index].frame_width;
	unsigned y1 = sprite_animation.rect_animations[ra_index].pos_y + sprite_animation.rect_animations[ra_index].frame_height * grid_y;
	unsigned y2 = y1 + sprite_animation.rect_animations[ra_index].frame_height;

	//Display
	displayImage(sprite_animation.sprite_image, x1, y1, x2, y2, scale);
}

void drawSpriteAnimation(SpriteAnimation& sprite_animation, std::string& animation_name, int imgui_frame_count, unsigned scale) {
	//Get index
	int ra_index = sprite_animation.getRectAnimationIndexFromName(animation_name);
	drawSpriteAnimation(sprite_animation, ra_index, imgui_frame_count, scale);
}



//Draws one frame of a specified animation
void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, unsigned ra_index, unsigned frame_num) {
	//Calc sub image positions
	unsigned grid_x = frame_num % sprite_animation.rect_animations[ra_index].frames_per_row;
	unsigned grid_y = frame_num / sprite_animation.rect_animations[ra_index].frames_per_row;

	unsigned x1 = sprite_animation.rect_animations[ra_index].pos_x + sprite_animation.rect_animations[ra_index].frame_width * grid_x;
	unsigned x2 = x1 + sprite_animation.rect_animations[ra_index].frame_width;
	unsigned y1 = sprite_animation.rect_animations[ra_index].pos_y + sprite_animation.rect_animations[ra_index].frame_height * grid_y;
	unsigned y2 = y1 + sprite_animation.rect_animations[ra_index].frame_height;

	//Display
	displayImage(sprite_animation.sprite_image, x1, y1, x2, y2);
}

void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, std::string& animation_name, unsigned frame_num) {
	int ra_index = sprite_animation.getRectAnimationIndexFromName(animation_name);
	drawSpriteAnimationFrame(sprite_animation, ra_index, frame_num);
}

void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, unsigned ra_index, unsigned frame_num, unsigned scale) {
	//Calc sub image positions
	unsigned grid_x = frame_num % sprite_animation.rect_animations[ra_index].frames_per_row;
	unsigned grid_y = frame_num / sprite_animation.rect_animations[ra_index].frames_per_row;

	unsigned x1 = sprite_animation.rect_animations[ra_index].pos_x + sprite_animation.rect_animations[ra_index].frame_width * grid_x;
	unsigned x2 = x1 + sprite_animation.rect_animations[ra_index].frame_width;
	unsigned y1 = sprite_animation.rect_animations[ra_index].pos_y + sprite_animation.rect_animations[ra_index].frame_height * grid_y;
	unsigned y2 = y1 + sprite_animation.rect_animations[ra_index].frame_height;

	//Display
	displayImage(sprite_animation.sprite_image, x1, y1, x2, y2, scale);
}

void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, std::string& animation_name, unsigned frame_num, unsigned scale) {
	int ra_index = sprite_animation.getRectAnimationIndexFromName(animation_name);
	drawSpriteAnimationFrame(sprite_animation, ra_index, frame_num, scale);
}


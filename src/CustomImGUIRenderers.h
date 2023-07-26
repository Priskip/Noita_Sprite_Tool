#pragma once
#include <string>
#include <vector>
#include "Image.h"
#include "SpriteAnimation.h"
#include "imgui.h"

//Displays a type Image from Image.h inside an imgui window.
void displayImage(Image& im);
void displayImage(Image& im, unsigned scale);
void displayImage(Image& im, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
void displayImage(Image& im, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned scale);

//Draws the frame of animation based off the imgui app's runtime (ImGui is 60fps)
void drawSpriteAnimation(SpriteAnimation& sprite_animation, unsigned ra_index, int imgui_frame_count);
void drawSpriteAnimation(SpriteAnimation& sprite_animation, std::string& animation_name, int imgui_frame_count);
void drawSpriteAnimation(SpriteAnimation& sprite_animation, unsigned ra_index, int imgui_frame_count, unsigned scale);
void drawSpriteAnimation(SpriteAnimation& sprite_animation, std::string& animation_name, int imgui_frame_count, unsigned scale);

//Draws one frame of a specified animation
void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, unsigned ra_index, unsigned frame_num);
void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, std::string& animation_name, unsigned frame_num);
void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, unsigned ra_index, unsigned frame_num, unsigned scale);
void drawSpriteAnimationFrame(SpriteAnimation& sprite_animation, std::string& animation_name, unsigned frame_num, unsigned scale);
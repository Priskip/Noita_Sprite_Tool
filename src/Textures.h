#pragma once
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <vector>
#include "imgui.h"


//Load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

//Locations for Textures
const unsigned BUFFER_WIDTH = 64;

//Enum for Type
enum TextureCategory { FILEBROWSER_ICONS };

//Struct of texture information
struct Texture {
	char file_name[BUFFER_WIDTH];
	GLuint gl_texture;
	int width;
	int height;
	ImTextureID imgui_texture;
	ImVec2 imgui_size;

	Texture() {
		strcpy_s(file_name, "unitialized");
		gl_texture = -1;
		width = -1;
		height = -1;
		imgui_texture = 0;
		imgui_size = ImVec2(0.0f, 0.0f);
	}

	Texture(const char file_path[]) {
		strcpy_s(file_name, file_path);
		gl_texture = 0;
		width = 0;
		height = 0;

		LoadTextureFromFile(file_name, &gl_texture, &width, &height);

		imgui_texture = (void*)(intptr_t)gl_texture;
		imgui_size = ImVec2((float)width, (float)height);
	}
};

//Class for holding all the textures
class AllTextures {
private:
		std::vector<Texture*> filebrowser_icons;
public:
	//Constructor
	AllTextures();

	//Destructor
	~AllTextures();

	//Methods

	//Returns a pointer to the texture with the name provided. Returns nullptr if name cannot be found.
	Texture* findTexture(const char name[]);

	//Returns a pointer to the texture a newly initialized texture.
	//If texture is already initialized, will return pointer to existing texture.
	//If file does not exist, returns nullptr.
	//TextureCategory specifies which vector to store the new texture to.
	//Invalid texture will result in no texture being created and a nullptr returned.
	Texture* registerTexture(const char name[], TextureCategory type = FILEBROWSER_ICONS);

	//Release texture from memory
	void releaseTexture(const char name[], TextureCategory type = FILEBROWSER_ICONS);
};

extern AllTextures textures;
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GL_CLAMP_TO_EDGE 0x812F

#include "Textures.h"
#include "utils.h"
#include <vector>

//Load an image into a OpenGL texture with common settings
//Has to be called AFTER OpenGL has initialized
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

//Initialize a member variable

//BEGIN AllTextures Class
AllTextures::AllTextures() {

}

//Destroy all heap allocated memory
AllTextures::~AllTextures() {
	//filebrowser_icons
	for (int i = 0; i < filebrowser_icons.size(); i++) {
		delete filebrowser_icons[i];
		filebrowser_icons[i] = nullptr;
	}
	filebrowser_icons.clear();
}

//Returns a pointer to the texture with the name provided. Returns nullptr if name cannot be found.
Texture* AllTextures::findTexture(const char name[]) {
	Texture* ptr = nullptr;

	for (int i = 0; i < filebrowser_icons.size(); i++) {
		if (strcmp(filebrowser_icons[i]->file_name, name) == 0) {
			ptr = filebrowser_icons[i];
			break;
		}
	}

	return ptr;
}

//Returns a pointer to the texture a newly initialized texture.
//If texture is already initialized, will return pointer to existing texture.
//If file does not exist, returns nullptr.
//TextureCategory specifies which vector to store the new texture to.
//Invalid texture will result in no texture being created and a nullptr returned.
Texture* AllTextures::registerTexture(const char name[], TextureCategory type) {
	//Test if file exists and is type PNG, else return nullptr
	if (isPNGThatExists(name)) {
		//File Exists and is PNG
		//Now need to test stored textures to see if any match the name provided.

		//Test filebrowser_icons
		for (Texture* t_ptr : filebrowser_icons) {
			if (strcmp(t_ptr->file_name, name) == 0) {
				return t_ptr;
			}
		}

		//If we escape above loop/s, then we need to register new texture.
		Texture* new_texture = nullptr;
		switch (type) {
		case FILEBROWSER_ICONS:
			new_texture = new Texture(name);
			filebrowser_icons.push_back(new_texture);
			break;
		default:
			//Unknown type category inputted
			break;
		}
		return new_texture;
	}
	return nullptr;
}

//Release texture from memory
void AllTextures::releaseTexture(const char name[], TextureCategory type) {
	switch (type) {
	case FILEBROWSER_ICONS:
		for (int i = 0; i < filebrowser_icons.size(); i++) {
			if (strcmp(filebrowser_icons[i]->file_name, name) == 0) {
				delete filebrowser_icons[i];
				filebrowser_icons[i] = nullptr;
				filebrowser_icons.erase(filebrowser_icons.begin() + i);
				break;
			}
		}
		break;
	default:
		//Idk chief, you shouldn't have hit this case.
		break;
	}
}
//END AllTextures Class

//Object to hold all the textures
AllTextures textures;
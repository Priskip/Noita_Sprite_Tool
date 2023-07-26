#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

template <typename T> void printHexValue(T num);
template <typename T> void printBinValue(T num);

namespace priskip_aseprite_encoder_decoder {
	//Type Defs
	typedef uint8_t BYTE;             //typeid().name() = "unsigned char"
	typedef uint16_t WORD;            //typeid().name() = "unsigned short"
	typedef int16_t SHORT;            //typeid().name() = "short"
	typedef uint32_t DWORD;           //typeid().name() = "unsigned int"
	typedef int32_t LONG;             //typeid().name() = "int"
	typedef int32_t FIXED;            //typeid().name() = "int"
	typedef float FLOAT;              //typeid().name() = "float"
	typedef double DOUBLE;            //typeid().name() = "double"
	typedef long long unsigned QWORD; //typeid().name() = "unsigned __int64"
	typedef long long int LONG64;     //typeid().name() = "__int64"

	struct STRING {
		WORD length; //string length (number of bytes)
		BYTE* str; //characters (in UTF-8) The '\0' character is not included.

		STRING(WORD input_l, BYTE input_str[])
			: length(input_l), str(input_str)
		{
			//validate size
			std::string temp = reinterpret_cast<char*>(input_str);
			if (length != temp.size()) {
				std::cout << "WARNING: STRUCT SIZE NOT SAME AS STRING SIZE\n";
				length = (WORD)temp.size();
				//why am I playing games like this?
				//I could just get the length from the string by casting it to std::string like this
			}
		}
	};

	struct POINT {
		LONG x;
		LONG y;

		POINT(LONG input_x, LONG input_y) : x(input_x), y(input_y) {};
	};

	struct SIZE {
		LONG width;
		LONG height;

		SIZE(LONG w, LONG h) : width(w), height(h) {};
	};

	struct RECT {
		POINT og_coords;
		SIZE rect_size;

		RECT(POINT point, SIZE size) : og_coords(point), rect_size(size) {}
	};

	//PIXEL (RGBA, Grayscale, or Indexed)
	struct PIXEL_RGBA {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE a;

		PIXEL_RGBA(BYTE red, BYTE green, BYTE blue, BYTE alpha)
			: r(red), g(green), b(blue), a(alpha) {}
	};

	struct PIXEL_GRAYSCALED {
		BYTE v;
		BYTE a;

		PIXEL_GRAYSCALED(BYTE value, BYTE alpha) : v(value), a(alpha) {}
	};

	typedef BYTE PIXEL_INDEXED;

	//TILE Each tile can be a 8-bit (BYTE), 16-bit (WORD),
	// or 32-bit (DWORD) value and there are masks
	// related to the meaning of each bit.

	//UUID BYTE[16]

	struct HEADER {
		DWORD	FILE_SIZE;
		WORD	MAGIC_NUMBER = 0xA5E0;
		WORD	FRAMES;
		WORD	WIDTH;
		WORD	HEIGHT;
		WORD	COLOR_DEPTH;		//Color depth (bits per pixel) 32 bpp = RGBA, 16 bpp = , 8 bpp = Indexed
		DWORD	Flags;				//1 = Layer opacity has valid value
		WORD	SPEED;				//Speed (milliseconds between frame, like in FLC files) DEPRECATED: You should use the frame duration field from each frame header
		BYTE	PALETTE_ENTRY;		//Palette entry (index) which represent transparent color in all non - background layers(only for Indexed sprites).
		WORD	NUMBER_OF_COLORS;	//Number of colors (0 means 256 for old sprites)
		BYTE	PIXEL_WIDTH;
		BYTE	PIXEL_HEIGHT;		//pixel ratio is "pixel width/pixel height". If this or pixel height field is zero, pixel ratio is 1:1
		SHORT	X_POS_IN_GRID;
		SHORT	Y_POS_IN_GRID;
		WORD	GRID_WIDTH;			//zero if there is no grid, grid size is 16x16 on Aseprite by default
		WORD	GRID_HEIGHT;		//zero if there is no grid

		//TO DO: define initializing values for all of these
	};
}

//Aseprite File
class AsepriteFile {
private:
	// --- MEMBER VARIABLES ---
	std::vector<priskip_aseprite_encoder_decoder::BYTE> binary_data;
	uintmax_t file_size;

	// --- MEMBER FUNCTIONS ---

	//Reads binary file into binary data vector
	void readBinaryFile(const char file_name[]);

	template <typename T> void readBinaryValueAtIndex(T& _type, uintmax_t index);

public:

	// --- CONSTRUCTORS ---
	AsepriteFile(const char file_name[]);

	// --- MEMBER FUNCTIONS ---

	//prints binary data to console
	void printBinaryDataToConsole();

	void testy();

};

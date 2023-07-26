#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <bitset>
#include "PriskipAsepriteEncoderDecoder.h"

//template for printing hex number
template <typename T> void printHexValue(T num) {
	//store old cout state so we can restory it when we're done.
	std::ios old_state(nullptr);
	old_state.copyfmt(std::cout);

	//Print
	std::cout << "0x" << std::internal << std::setfill('0')
		<< std::hex << std::setw(2 * sizeof(T)) << (uintmax_t)num;

	//Restore old ios state
	std::cout.copyfmt(old_state);
}

//template for printing binary number
template <typename T> void printBinValue(T num) {
	//store old cout state so we can restory it when we're done.
	std::ios old_state(nullptr);
	old_state.copyfmt(std::cout);

	//Print
	std::cout << "0b" << std::bitset<8 * sizeof(num)>(num);

	//Restore old ios state
	std::cout.copyfmt(old_state);
}

//BEGIN ASEPRITE FILE CLASS

// --- CONSTRUCTORS ---
AsepriteFile::AsepriteFile(const char file_name[]) {
	//BINARY DATA
	readBinaryFile(file_name); //Sets obj.binary_data and obj.file_size


}

//PRIVATE MEMBER FUNCTIONS

//Reads a binary file into obj.binary_data.
void AsepriteFile::readBinaryFile(const char file_name[]) {
	//If file exists
	if (std::filesystem::exists(std::filesystem::path(file_name))) {
		//Read Size of File
		file_size = std::filesystem::file_size(file_name);

		//Create an File Stream
		std::ifstream infile(file_name, std::ifstream::binary);

		//Create Heap Allocation to read to
		char* binary_chars = new char[file_size];

		//Read into newly created buffer
		infile.read(binary_chars, file_size);

		//Convert from SIGNED chars to UNSIGNED chars and store to vector
		binary_data.clear();
		binary_data.reserve(file_size);
		for (int i = 0; i < file_size; i++) {
			int num = (int)*(binary_chars + i);
			if (num < 0)
				num += 256; //cast from being a SIGNED char to an UNSIGNED char if need be

			binary_data.push_back((uint8_t)num);
		}

		//Free Heap Memory
		delete[] binary_chars;
		binary_chars = nullptr;
	}
	else {
		std::string file_name_str = file_name;
		std::string error_message = "Error in AsepriteFile::readBinaryFile(const char file_name[]): Could not open file \"" + file_name_str + "\"";
		std::cout << error_message << std::endl;
	}
}

//Read Binary Value at index
template <typename T> void AsepriteFile::readBinaryValueAtIndex(T& _type, uintmax_t index) {
	_type = 0;
	for (int i = 0; i < sizeof(T); i++) {
		_type += (((T)binary_data[index + i]) << (8 * i));
	}
}

//PUBLIC MEMBER FUNCTIONS

//Prints obj's binary_data to console
void AsepriteFile::printBinaryDataToConsole() {
	//store old cout state so we can restory it when we're done.
	std::ios old_state(nullptr);
	old_state.copyfmt(std::cout);

	//display info in vector
	for (int i = 0; i < binary_data.size(); i++) {
		std::cout << std::internal
			<< std::setfill('0')
			<< std::hex
			<< std::setw(2)
			<< std::uppercase
			<< (int)binary_data[i] << " ";

		//if after 8th byte, add an extra ' '
		if ((i + 1) % 8 == 0)
			std::cout << " ";

		//if after the 16th byte, add an '\n'
		if ((i + 1) % 16 == 0)
			std::cout << "\n";
	}

	//Restore old ios state
	std::cout.copyfmt(old_state);
}

void AsepriteFile::testy() {
	namespace PAED = priskip_aseprite_encoder_decoder;
	PAED::LONG _long = 0;

	readBinaryValueAtIndex(_long, 2);
	printHexValue(_long);
	std::cout << "\n";
}

//END ASEPRITE FILE CLASS


//Notes
/*
ASEPRITE TYPE DEFINITIONS
ASE files use Intel (little-endian) byte order.
https://github.com/aseprite/aseprite/blob/main/docs/ase-file-specs.md#introduction

BYTE: An 8-bit unsigned integer value		[typedef uint8_t BYTE;]

WORD: A 16-bit unsigned integer value		[typedef uint16_t WORD;]

SHORT: A 16-bit signed integer value		[typedef int16_t SHORT;]

DWORD: A 32-bit unsigned integer value		[typedef uint32_t DWORD;]

LONG: A 32-bit signed integer value			[typedef int32_t LONG;]

FIXED: A 32-bit fixed point (16.16) value	[typedef int32_t FIXED;]
Conversion functions:
int32_t doubleTofixed16_16(double d) {
	return int32_t(d * double(1 << 16) + (d >= 0 ? 0.5 : -0.5));
}

double fixed16_16Todouble(int32_t d) {
	return double(d) / double(1 << 16);
}


FLOAT: A 32-bit single-precision value		[typedef float FLOAT;]

DOUBLE: A 64-bit double-precision value		[typedef double DOUBLE;]

QWORD: A 64-bit unsigned integer value		[typedef long long unsigned QWORD;]

LONG64: A 64-bit signed integer value		[typedef long long int LONG64;]

BYTE[n]: "n" bytes.							[That's just an array of type BYTE silly]

STRING:
WORD: string length (number of bytes)
BYTE[length]: characters (in UTF-8) The '\0' character is not included.

POINT:
LONG: X coordinate value
LONG: Y coordinate value

SIZE:
LONG: Width value
LONG: Height value

RECT:
POINT: Origin coordinates
SIZE: Rectangle size

PIXEL: One pixel, depending on the image pixel format:
RGBA: BYTE[4], each pixel have 4 bytes in this order Red, Green, Blue, Alpha.
Grayscale: BYTE[2], each pixel have 2 bytes in the order Value, Alpha.
Indexed: BYTE, each pixel uses 1 byte (the index).

TILE: Tilemaps: Each tile can be a 8-bit (BYTE), 16-bit (WORD), or 32-bit (DWORD) value and there are masks related to the meaning of each bit.

UUID: A Universally Unique Identifier stored as BYTE[16].
*/

/*
Header
A 128-byte header (same as FLC/FLI header, but with other magic number):

DWORD       File size
WORD        Magic number (0xA5E0)
WORD        Frames
WORD        Width in pixels
WORD        Height in pixels
WORD        Color depth (bits per pixel)
			  32 bpp = RGBA
			  16 bpp = Grayscale
			  8 bpp = Indexed
DWORD       Flags:
			  1 = Layer opacity has valid value
WORD        Speed (milliseconds between frame, like in FLC files)
			DEPRECATED: You should use the frame duration field
			from each frame header
DWORD       Set be 0
DWORD       Set be 0
BYTE        Palette entry (index) which represent transparent color
			in all non-background layers (only for Indexed sprites).
BYTE[3]     Ignore these bytes
WORD        Number of colors (0 means 256 for old sprites)
BYTE        Pixel width (pixel ratio is "pixel width/pixel height").
			If this or pixel height field is zero, pixel ratio is 1:1
BYTE        Pixel height
SHORT       X position of the grid
SHORT       Y position of the grid
WORD        Grid width (zero if there is no grid, grid size
			is 16x16 on Aseprite by default)
WORD        Grid height (zero if there is no grid)
BYTE[84]    For future (set to zero)
*/

/*
//Type Defs
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef int16_t SHORT;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef int32_t FIXED;
typedef float FLOAT;
typedef double DOUBLE;
typedef long long unsigned QWORD;
typedef long long int LONG64;
struct STRING {
	WORD length; //string length (number of bytes)
	std::vector<BYTE> str; //characters (in UTF-8) The '\0' character is not included.
	// that ^ should be an array but arrays can't be dynamically allocated easily in a struct call like this
	// I'll fix that later
};
struct POINT {
	LONG x;
	LONG y;
};
struct SIZE {
	LONG width;
	LONG height;
};
struct RECT {
	POINT og_coords;
	SIZE rect_size;
};
//PIXEL (RGBA, Grayscale, or Indexed)
struct PIXEL_RGBA {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
};
struct PIXEL_GRAYSCALED{
	BYTE value;
	BYTE alpha;
};
typedef BYTE PIXEL_INDEXED;

//TILE Each tile can be a 8-bit (BYTE), 16-bit (WORD),
// or 32-bit (DWORD) value and there are masks
// related to the meaning of each bit.

//UUID BYTE[16]

//Templates
template <typename T> T printSizeInfo(T value) {
	std::cout << sizeof(T) << " bytes = " << sizeof(T) * 8 << " bits" << "\n";
	return 0;
}

template <typename T> T printHex(T value, unsigned num_of_digits) {
	std::cout << std::internal << std::setfill('0');
	std::cout << std::hex << std::setw(num_of_digits) << std::uppercase << value;
	return 0;
}

//Fixed point conversion functions.
//thank you https://www.youtube.com/watch?v=ZMsrZvBmQnU ;)
FIXED doubleTofixed16_16(double d) {
	return FIXED(d * double(1 << 16) + (d >= 0 ? 0.5 : -0.5));
}

double fixed16_16Todouble(FIXED d) {
	return double(d) / double(1 << 16);
}

//Main
int main(int argc, char** argv[]) {

	FIXED xf = doubleTofixed16_16(4.4);
	double xd = fixed16_16Todouble(xf);

	printHex(xf, 16); std::cout << "\n";
	std::cout << xd << "\n";

	return 0;
}
*/

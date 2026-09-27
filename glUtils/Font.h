#pragma once

#ifndef __gl_h_
#include <glad/gl.h>
#endif

#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

class BitmapFont {
private:
	unsigned int texture_ID;
	int texture_width;
	int texture_height;
	int num_color_channels;

	struct Info {
		std::string name;
		const char* file_path;
		unsigned int char_width;
		unsigned int char_height;
		unsigned char min_char;
		unsigned char max_char;
	};
	const Info info;

	static std::map<std::string, BitmapFont::Info> fonts_info;
	static std::map<std::string, BitmapFont*> fonts;

	BitmapFont(Info info);

public:
	struct CharBounds {
		float u0, v0, u1, v1;
	};
	glm::vec2 getCharOffset(unsigned char c);

	unsigned int getCharWidth();
	unsigned int getCharHeight();
	float getAspectRatio();

	unsigned int getTextureID();

	glm::mat3 getFontTransform();

	static void loadFont(std::string fontName);
	static BitmapFont* getFont(std::string fontName);
};


class Font {
	static FT_Library library;	
	FT_Face face;

public:
	static FT_Error initLibrary();
	FT_Error loadFace(const char* file_path);
};
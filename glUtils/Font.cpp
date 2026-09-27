#include "Font.h"

#ifdef _DEBUG
#include <iostream>
#endif //_DEBUG

std::map<std::string, BitmapFont::Info> BitmapFont::fonts_info = {
	{"kfont", BitmapFont::Info {
		"kfont", "./Assets/font.bmp", 6, 10, 32, 126,
	}},
	{"consolas", BitmapFont::Info {
		"consolas", "./Assets/consolas.bmp", 9, 15, 32, 126,
	}},
	{"consolas-aa", BitmapFont::Info {
		"consolas-aa", "./Assets/consolas-aa.bmp", 9, 15, 32, 126,
	}}
};

std::map<std::string, BitmapFont*> BitmapFont::fonts;

BitmapFont::BitmapFont(BitmapFont::Info info) : info(info) {}

void BitmapFont::loadFont(std::string fontName) {
	if (fonts.count(fontName) != 0)
		return;

	fonts.insert({ fontName, new BitmapFont(fonts_info[fontName]) });
	BitmapFont* font = fonts[fontName];

	unsigned char* source = stbi_load(fonts_info[fontName].file_path,
		&font->texture_width, &font->texture_height, &font->num_color_channels, 0);

	unsigned int TEX;
	glGenTextures(1, &TEX);

	glBindTexture(GL_TEXTURE_2D, TEX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, font->texture_width, font->texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, source);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	font->texture_ID = TEX;

	stbi_image_free(source);
}

BitmapFont* BitmapFont::getFont(std::string fontName) {
	return fonts[fontName];
}

glm::vec2 BitmapFont::getCharOffset(unsigned char c) {
	unsigned int per_row = (unsigned int)texture_width / info.char_width;
	unsigned char c_0 = info.min_char;

	glm::vec2 offset;
	offset.s = (((c - c_0) % per_row) * info.char_width) / (float)texture_width;
	offset.t = (((c - c_0) / per_row) * info.char_height) / (float)texture_height;

	return offset;
}

unsigned int BitmapFont::getCharWidth() {
	return info.char_width;
}

unsigned int BitmapFont::getCharHeight() {
	return info.char_height;
}

float BitmapFont::getAspectRatio() {
	return info.char_width / (float)info.char_height;
}

unsigned int BitmapFont::getTextureID() {
	return texture_ID;
}

glm::mat3 BitmapFont::getFontTransform() {
	glm::mat3 font_transform = glm::mat3(0.f);
	font_transform[0][0] = info.char_width / (float)texture_width;
	font_transform[1][1] = info.char_height / (float)texture_height;
	font_transform[2][2] = 1.f;

	return font_transform;
}





FT_Library Font::library;

FT_Error Font::initLibrary() {
	FT_Error error = FT_Init_FreeType(&library);
#ifdef _DEBUG
	if (error)
		std::cerr << "Unable to initialize FreeType: " << error << std::endl;
#endif //_DEBUG
	return error;
}

FT_Error Font::loadFace(const char* file_path) {
	FT_Error error = FT_New_Face(library, file_path, 0, &face);
#ifdef _DEBUG
	if (error == FT_Err_Unknown_File_Format)
		std::cerr << "Font [" << file_path << "] of unknown format" << std::endl;
	else if (error)
		std::cerr << "Font [" << file_path << "] could not be loaded: " << error << std::endl;
#endif //_DEBUG
	return error;
}
#pragma once

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STB_FAILURE_USERMSG
#include "external/stb_image.h"

#include <cstdlib>
#include <iostream>

class Image {
private:
	const int bytesPerPixel = 3;
	float* fdata = nullptr;
	unsigned char* bdata = nullptr;
	int imageWidth = 0;
	int imageHeight = 0;
	int bytesPerScanline = 0;

public:


	Image() {}

	Image(const char* imageFilename) {
		auto filename = std::string(imageFilename);
		auto imagedir = getenv("RTW_IMAGES");

		if (imagedir && load(std::string(imagedir) + "/" + imageFilename)) {
			return;
		}

		if (load(filename)) return;

		std::cerr << "ERROR : unable to load texture '" << imageFilename << "'" << std::endl;
	}

	~Image() {
		delete[] bdata;
		STBI_FREE(fdata);
	}

	bool load(const std::string& filname) {
		auto n = bytesPerPixel;
		fdata = stbi_loadf(filname.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);

		if (fdata == nullptr) return false;

		bytesPerScanline = imageWidth * bytesPerPixel;

		convertToBytes();

		return true;
	}

	int width() const {
		return (fdata == nullptr) ? 0 : imageWidth;
	}

	int height() const {
		return (fdata == nullptr) ? 0 : imageHeight;
	}

	const unsigned char* pixelData(int x, int y) const {
		static unsigned char magenta[] = { 255,0,255 };
		if (bdata == nullptr) return magenta;

		x = clamp(x, 0, imageWidth);
		y = clamp(y, 0, imageHeight);

		return bdata + y * bytesPerScanline + x * bytesPerPixel;

	}

private : 
	static int clamp(int x, int low, int high) {
		if (x < low) return low;
		if (x < high) return x;
		return high - 1;
	}

	static unsigned char floatToByte(float value) {
		if (value <= 0.0)
			return 0;
		if (1.0 <= value)
			return 255;

		return static_cast<unsigned char>(256.0 * value);
	}

	void convertToBytes() {
		int totalBytes = imageWidth * imageHeight * bytesPerPixel;
		bdata = new unsigned char[totalBytes];

		auto* bptr = bdata;
		auto* fptr = fdata;

		for (auto i = 0; i < totalBytes; ++i, ++fptr, ++bptr) {
			*bptr = floatToByte(*fptr);
		}
	}

};

#ifdef _MSC_VER
#pragma warning (pop)
#endif

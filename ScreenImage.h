#pragma once

#include <Windows.h>
#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <FreeImage.h>

class ScreenImage
{
private:
    std::vector<std::uint8_t> Pixels;
    std::uint32_t width, height;
    std::uint16_t BitsPerPixel;

    void Flip(void* In, void* Out, int width, int height, unsigned int Bpp);

public:
    explicit ScreenImage(HDC DC, int X, int Y, int Width, int Height);

    inline std::uint16_t GetBitsPerPixel() { return this->BitsPerPixel; }
    inline std::uint16_t GetBytesPerPixel() { return this->BitsPerPixel / 8; }
    inline std::uint16_t GetBytesPerScanLine() { return (this->BitsPerPixel / 8) * this->width; }
    inline int GetWidth() const { return this->width; }
    inline int GetHeight() const { return this->height; }
    inline const std::uint8_t* GetPixels() { return this->Pixels.data(); }

    void Save();
};


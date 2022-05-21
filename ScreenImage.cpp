#include "ScreenImage.h"

ScreenImage::ScreenImage(HDC DC, int X, int Y, int Width, int Height) : Pixels(), width(Width), height(Height), BitsPerPixel(32)
{
    BITMAP Bmp = { 0 };
    HBITMAP hBmp = reinterpret_cast<HBITMAP>(GetCurrentObject(DC, OBJ_BITMAP));

    if (GetObject(hBmp, sizeof(BITMAP), &Bmp) == 0)
        throw std::runtime_error("BITMAP DC NOT FOUND.");

    RECT area = { X, Y, X + Width, Y + Height };
    HWND Window = WindowFromDC(DC);
    GetClientRect(Window, &area);

    HDC MemDC = GetDC(nullptr);
    HDC SDC = CreateCompatibleDC(MemDC);
    HBITMAP hSBmp = CreateCompatibleBitmap(MemDC, width, height);
    DeleteObject(SelectObject(SDC, hSBmp));

    BitBlt(SDC, 0, 0, width, height, DC, X, Y, SRCCOPY);
    unsigned int data_size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    std::vector<std::uint8_t> Data(data_size);
    this->Pixels.resize(data_size);

    BITMAPINFO Info = { sizeof(BITMAPINFOHEADER), static_cast<long>(width), static_cast<long>(height), 1, BitsPerPixel, BI_RGB, data_size, 0, 0, 0, 0 };
    GetDIBits(SDC, hSBmp, 0, height, &Data[0], &Info, DIB_RGB_COLORS);
    this->Flip(&Data[0], &Pixels[0], width, height, BitsPerPixel);

    DeleteDC(SDC);
    DeleteObject(hSBmp);
    ReleaseDC(nullptr, MemDC);
}

void ScreenImage::Flip(void* In, void* Out, int width, int height, unsigned int Bpp)
{
    unsigned long Chunk = (Bpp > 24 ? width * 4 : width * 3 + width % 4);
    unsigned char* Destination = static_cast<unsigned char*>(Out);
    unsigned char* Source = static_cast<unsigned char*>(In) + Chunk * (height - 1);

    while (Source != In)
    {
        std::memcpy(Destination, Source, Chunk);
        Destination += Chunk;
        Source -= Chunk;
    }
}

void ScreenImage::Save() {
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(GetWidth(), GetHeight(), BitsPerPixel);
    RGBQUAD color;

    if (!bitmap)
        exit(1);

    int x = GetSystemMetrics(SM_XVIRTUALSCREEN);  //left (e.g. -1024)
    int y = GetSystemMetrics(SM_YVIRTUALSCREEN);  //top (e.g. -34)
    int cx = GetSystemMetrics(SM_CXVIRTUALSCREEN); //entire width (e.g. 2704)
    int cy = GetSystemMetrics(SM_CYVIRTUALSCREEN); //entire height (e.g. 1050)

    HDC dcScreen = GetDC(0);
    HDC dcTarget = CreateCompatibleDC(dcScreen);

    HBITMAP bmpTarget = CreateCompatibleBitmap(dcScreen, cx, cy);
    HGDIOBJ oldBmp = SelectObject(dcTarget, bmpTarget);

    BitBlt(dcTarget, 0, 0, cx, cy, dcScreen, x, y, SRCCOPY | CAPTUREBLT);
    SelectObject(dcTarget, oldBmp);
    DeleteDC(dcTarget);

    FreeImage_Save(FIF_JPEG, bitmap, "test.png", 0);
    FreeImage_DeInitialise();
}
#include "Main.h"
#include "Macro.h"

int main() {
	bool running = true;

	UINT recordToggleKey = VK_NUMPAD1;
	UINT playToggleKey = VK_NUMPAD2;

	std::thread gui(onPaint);

	Macro macro = Macro();
	while (running) {
		Sleep(10);
	
		if (GetAsyncKeyState(recordToggleKey)) {
			msgInformer.assign(L"Record");

			Helper::waitForTargerKeyUp(recordToggleKey);

			std::cout << "Record is started" << std::endl;
			macro.record();
			std::cout << "Record is stopped" << std::endl;

			msgInformer.assign(L"Idle mode");
		}

		if (GetAsyncKeyState(playToggleKey)) {
			msgInformer.assign(L"Playing");

			Helper::waitForTargerKeyUp(playToggleKey);

			std::cout << "Playing" << std::endl;
			macro.run();
			std::cout << "Stoped" << std::endl;

			msgInformer.assign(L"Idle mode");
		}
	}
#pragma region In progress
	//HWND SomeWindowHandle = GetDesktopWindow();
	//HDC DC = GetDC(SomeWindowHandle);

	//ScreenImage Img = ScreenImage(DC, 500, 500, 1500, 1500); //screenshot of 0, 0, 200, 200..

	//ReleaseDC(SomeWindowHandle, DC);

	//std::unique_ptr<tesseract::TessBaseAPI> tesseract_ptr(new tesseract::TessBaseAPI());

	//const DWORD path_size = 100;
	//std::wstring buff;
	//buff.resize(path_size);
	//GetEnvironmentVariable(L"TESSDATA_PREFIX", &buff[0], path_size);

	//tesseract_ptr->Init("C:\\Users\\Vova\\tessdata", "rus");
	//tesseract_ptr->SetImage(Img.GetPixels(), Img.GetWidth(), Img.GetHeight(), Img.GetBytesPerPixel(), Img.GetBytesPerScanLine()); //Fixed this line..

	//std::unique_ptr<char[]> utf8_text_ptr(tesseract_ptr->GetUTF8Text());

	//std::cout << utf8_text_ptr.get() << "\n";

	//Img.Save();
#pragma endregion

	return 0;
}

void onPaint()
{
	while (true) {
		Sleep(10);

		RECT rect;
		HWND hwnd = GetActiveWindow();
		HDC wdc = GetWindowDC(hwnd);

		GetClientRect(hwnd, &rect);
		SetTextColor(wdc, WHITENESS);
		SetBkMode(wdc, BLACKNESS);

		rect.left = 0;
		rect.top = 0;

		auto size = msgInformer.size();

		BitBlt(wdc, 0, 0, 70, 16, nullptr, 0, 0, BLACKNESS);
		DrawText(wdc, msgInformer.c_str(), (int)size, &rect, DT_SINGLELINE | DT_NOCLIP);

		DeleteDC(wdc);
		ReleaseDC(hwnd, wdc);
	}
}
/*
* Desciption: This screenshot function utilizes the GDIPlus library and saves the entire desktop screenshot to ScreenShot.png.
*             The window size to be captured can be changed by setting screenx, screeny, width and height inside GdiPlusScreenCapture function
*             The output file format can be changed via changing the dataFormat argument when calling the saveToMemory function
* 
*             Currently, the executable GDIPlus_ScreenCap.exe captures screenshot continuously and outputs latency to the terminal.
*/
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <atlimage.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <chrono>
#include <iomanip>

using namespace Gdiplus;

BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER  bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}

HBITMAP GdiPlusScreenCapture(HWND hWnd)
{
    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(hWnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // define scale, height and width
    int scale = 1;
    int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that call HeapAlloc using a handle to the process's default heap.
    // Therefore, GlobalAlloc and LocalAlloc have greater overhead than HeapAlloc.
    DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);   //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // avoid memory leak
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hWnd, hwindowDC);
    GlobalFree(hDIB);

    return hbwindow;
}

bool saveToMemory(HBITMAP* hbitmap, std::vector<BYTE>& data, std::string dataFormat = "png")
{
    Gdiplus::Bitmap bmp(*hbitmap, nullptr);
    // write to IStream
    IStream* istream = nullptr;
    CreateStreamOnHGlobal(NULL, TRUE, &istream);

    // define encoding
    CLSID clsid;
    if (dataFormat.compare("bmp") == 0) { CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("jpg") == 0) { CLSIDFromString(L"{557cf401-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("gif") == 0) { CLSIDFromString(L"{557cf402-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("tif") == 0) { CLSIDFromString(L"{557cf405-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("png") == 0) { CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsid); }

    Gdiplus::Status status = bmp.Save(istream, &clsid, NULL);
    if (status != Gdiplus::Status::Ok)
        return false;

    // get memory handle associated with istream
    HGLOBAL hg = NULL;
    GetHGlobalFromStream(istream, &hg);

    // copy IStream to buffer
    int bufsize = GlobalSize(hg);
    data.resize(bufsize);

    // lock & unlock memory
    LPVOID pimage = GlobalLock(hg);
    memcpy(&data[0], pimage, bufsize);
    GlobalUnlock(hg);
    istream->Release();
    return true;
}


int main()
{
   
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    

    // get the bitmap handle to the bitmap screenshot
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        HWND hWnd = GetDesktopWindow();
        HBITMAP hBmp = GdiPlusScreenCapture(hWnd);


        // save as png to memory
        std::vector<BYTE> data;
        std::string dataFormat = "png";

        if (saveToMemory(&hBmp, data, dataFormat))
        {
            std::wcout << "Screenshot saved to memory" << std::endl;

            // save from memory to file
            std::ofstream fout("ScreenShot." + dataFormat, std::ios::binary);
            fout.write((char*)data.data(), data.size());
        }
        else
            std::wcout << "Error: Couldn't save screenshot to memory" << std::endl;

        GdiplusShutdown(gdiplusToken);
        auto end = std::chrono::high_resolution_clock::now();

        // Calculating total time taken by the program. 
        double time_taken =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        std::cout << "Time taken by this screen capture is : " << std::fixed
            << time_taken << std::setprecision(9);
        std::cout << " sec" << std::endl;
    }
    


    // save as png (method 2)
    //CImage image;
    //image.Attach(hBmp);
    //image.Save(L"Screenshot-m2.png");

    return 0;
}

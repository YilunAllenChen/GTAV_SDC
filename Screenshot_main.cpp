/*
* Author: Jingxuan Wang
* Class: ECE6122
* Last Date Modified: 2020/11/27
* 
* Description: Screencapture module using the GDIPLUS library. The executable takes screenshot of the top-left 800*600 pixel area of the desktop
*              and outputs a png file to the local directory. The screenshots are numbered 0-9 to avoid file read/write races and thus improve latency.
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

/*
* Desciption: This function creates a BitmapHeader using with the screencapture.
*             @param width: the width of the BitmapHeader
*             @param height: defines the orientation of the BitmapHeader
              @return the created BitmapHeader
*/
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

/*
* Desciption: This function actually captures the window
*             @param hWnd: The window handle to take the screencapture
*             @param the BITMAP
*/
HBITMAP GdiPlusScreenCapture(HWND hWnd)
{
    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(hWnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // define scale, height and width
    int scale = 1;
    //int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    //int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    //int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    //int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    //Defines the width and height of the screenshot and resulting file
    int width = 800;
    int height = 600;
    int screenx = 800;
    int screeny = 600;

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
    //StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);   //change SRCCOPY to NOTSRCCOPY for wacky colors !
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 40, width, height, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // avoid memory leak
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hWnd, hwindowDC);
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    return hbwindow;
}


/*
* Desciption: This function saves the resulting bitmap to file.
*             @param hbitmap: The bitmap to write.
*             @param data: the data which can be saved to
*             @param dataFormat: dataFormat of the file
*             @return if the saving is complete or not
*/
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

//Global counter used to loop through avoid file conflict
static int count = 0;
int main()
{
   
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    

    // get the bitmap handle to the bitmap screenshot
    while (true) {
        count++;
        if (count >= 10) count = 0;
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
            std::ofstream fout("ScreenShot" + std::to_string(count) + "." + dataFormat, std::ios::binary);
            fout.write((char*)data.data(), data.size());
        }
        else
            std::wcout << "Error: Couldn't save screenshot to memory" << std::endl;

        DeleteObject(hBmp);
        DeleteObject(hWnd);
        GdiplusShutdown(gdiplusToken);
        auto end = std::chrono::high_resolution_clock::now();

        // Calculating total time taken by the program. 
        double time_taken =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        std::cout << "Time taken by this screen capture is : " << std::fixed
            << time_taken << std::setprecision(9);
        std::cout << " sec" << std::endl;
        //Sleep(1000);
    }
    


    // save as png (method 2)
    //CImage image;
    //image.Attach(hBmp);
    //image.Save(L"Screenshot-m2.png");

    return 0;
}

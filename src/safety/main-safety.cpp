#include <windows.h>
#include <mmsystem.h>
#include <functional>
#include <iostream>
#include <cmath>
#include <TlHelp32.h>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")

// --< Sounds >--
using SndCalc = std::function<char(DWORD)>;

void play(SndCalc calc) {
    HWAVEOUT Wave = 0;
    WAVEFORMATEX Format = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&Wave, WAVE_MAPPER, &Format, 0, 0, CALLBACK_NULL);
    char Sound[8000 * 30];

    for (DWORD t = 0; t < sizeof(Sound); ++t) {
		Sound[t] = calc(t);
    }

    WAVEHDR Head = { Sound, sizeof(Sound), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(Wave, &Head, sizeof(WAVEHDR));
    waveOutWrite(Wave, &Head, sizeof(WAVEHDR));
    waveOutUnprepareHeader(Wave, &Head, sizeof(WAVEHDR));
    waveOutClose(Wave);
}

VOID WINAPI sound1() {
    play([](DWORD t) -> char {
		return static_cast<char>(t*(t>>10)&(t>>4));
	});
}

VOID WINAPI sound2() {
    play([](DWORD t) -> char {
        return static_cast<char>(t * (t >> 6 & t >> 10) ^ (t >> 4));
    });
}

VOID WINAPI sound3() {
    play([](DWORD t) -> char {
        return static_cast<char>(t * (t & t>>10));
    });
}

VOID WINAPI sound4() {
    play([](DWORD t) -> char {
        return static_cast<char>(t * (t&t >> 13) >> 4);
    });
}

VOID WINAPI sound5() {
    play([](DWORD t) -> char {
        return static_cast<char>(t * (t&t >> 13) >> 4);
    });
}

VOID WINAPI sound6() {
    play([](DWORD t) -> char {
        // long meowing of a cat, by SthepanShi and remixed by RealZynx92
        return static_cast<char>(t*((t>>11|t%16*t>>8)&t>>9&18)|-t/16+64);
    });
}

VOID WINAPI sound7() {
    play([](DWORD t) -> char {
        return static_cast<char>((t >> 4) * (t >> 6) | t >> 2);
    });
}

VOID WINAPI sound8() {
    play([](DWORD t) -> char {
        return static_cast<char>(t * (3 + (1 ^ 5 & t >> 10)) * (t >> 9));
    });
}

VOID WINAPI sound9() {
    play([](DWORD t) -> char {
        return static_cast<char>(t*(((t>>8)&(t>>10))^((t>>9)&(t>>11))));
    });
}

VOID WINAPI sound10() {
    play([](DWORD t) -> char {
        // neuro funk remixed from me
        return static_cast<char>(t * ((t & 4096 ? t % 65536 < 60000 ? 9 : t & 5 : 12) ^ (42 & t >> 10)) >> (4 & -t >> (t & 512 ? 4 : 8)));
    });
}

VOID WINAPI sound11() {
    play([](DWORD t) -> char {
        // Credits: lhphr
		return static_cast<char>((t >> 10 ^ t >> 11) % 5 * ((t >> 14 & 3 ^ t >> 15 & 1) + 1) * t % 99 + ((3 + (t >> 14 & 3) - (t >> 16 & 1)) / 3 * t % 99 & 64));
	});
}

// --< Sounds >--

// --< Visual >--
DWORD WINAPI Shake(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    while (true) {
        HDC Screen = GetDC(0);
        BitBlt(Screen, 0, 0, width, height, Screen, -20, 0, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, width - 20, 0, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, -20, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, height - 20, SRCCOPY);
        ReleaseDC(0, Screen);
        Sleep(100);
    }
}

DWORD WINAPI Grey_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i <= size; i++) {
            BYTE gray = (BYTE)(Rgb[i].rgbtBlue * 0.11 + Rgb[i].rgbtGreen * 0.59 + Rgb[i].rgbtRed * 0.3); 
            // smwhere took from google or the corner of the internet ^^^
            Rgb[i].rgbtBlue = gray + 5;
            Rgb[i].rgbtGreen = gray + 5;
            Rgb[i].rgbtRed = gray + 5;
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(1);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI sepia_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i < size; i++) {
            BYTE Blue = Rgb[i].rgbtBlue;
            BYTE Green = Rgb[i].rgbtGreen;
            BYTE Red = Rgb[i].rgbtRed;

            int tr = (int)(0.393 * Red + 0.769 * Green + 0.189 * Blue);
            int tg = (int)(0.349 * Red + 0.686 * Green + 0.168 * Blue);
            int tb = (int)(0.272 * Red + 0.534 * Green + 0.131 * Blue);

            Rgb[i].rgbtRed = (tr > 255) ? 255 : tr;
            Rgb[i].rgbtGreen = (tg > 255) ? 255 : tg;
            Rgb[i].rgbtBlue = (tb > 255) ? 255 : tb;
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(50);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI glitched_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);
    int size = width * height;

    int shiftIndex = 0;

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0);

        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i < size; i++) {
            BYTE B = Rgb[i].rgbtBlue;
            BYTE G = Rgb[i].rgbtGreen;
            BYTE R = Rgb[i].rgbtRed;

            switch (shiftIndex) {
                case 0:
                    Rgb[i].rgbtBlue = G;
                    Rgb[i].rgbtGreen = R;
                    Rgb[i].rgbtRed = B;
                    break;
                case 1:
                    Rgb[i].rgbtBlue = B;
                    Rgb[i].rgbtGreen = R;
                    Rgb[i].rgbtRed = G;
                    break;
                case 2:
                    Rgb[i].rgbtBlue = R;
                    Rgb[i].rgbtGreen = B;
                    Rgb[i].rgbtRed = G;
                    break;
            }
            shiftIndex = (shiftIndex + 1) % 3;
        }
        
        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(1);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteDC(Memory);
    }
    return 0;
}

DWORD WINAPI holyshit(LPVOID lpParam) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    int size = width * height;

    int phase = 0;

    while (true) {
        HDC hScreenDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);
        BITMAPINFO bInfo = { sizeof(BITMAPINFOHEADER), width, -height, 1, 24, 0, 0, 0, 0, 0, 0 };
        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hScreenDC, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hMemDC, hBitmap);

        BitBlt(hMemDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

        PRGBTRIPLE pixels = (PRGBTRIPLE)pBits;
        for (int y = 0; y < height; y++) {
            BYTE waveOffset = (BYTE)(127 + 128 * sin((phase + y) * 0.02));
            for (int x = 0; x < width; x++) {
                int index = y * width + x;
                BYTE B = pixels[index].rgbtBlue;
                BYTE G = pixels[index].rgbtGreen;
                BYTE R = pixels[index].rgbtRed;
                pixels[index].rgbtBlue = (BYTE)((B + waveOffset) % 256);
                pixels[index].rgbtGreen = (BYTE)((G + waveOffset) % 256);
                pixels[index].rgbtRed = (BYTE)((R + waveOffset) % 256);
            }
        }

        HDC hDestDC = GetDC(NULL);
        BitBlt(hDestDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hDestDC);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);

        phase += 5;
        if (phase > 360) phase -= 360;

        Sleep(10);
    }
    return 0;
}

DWORD WINAPI blurryass(LPVOID lpParam) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    while (true) {
        HDC hScreenDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);

        BITMAPINFO bInfo = { sizeof(BITMAPINFOHEADER), width, -height, 1, 24, 0, 0, 0, 0, 0, 0 };
        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hScreenDC, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hMemDC, hBitmap);

        BitBlt(hMemDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                int index = (y * width + x) * 3;
                int prevIndex = (y - 1) * width + (x - 1);
                int nextIndex = (y + 1) * width + (x + 1);
                int randOffset = rand() % 3 - 1;
                BYTE B = ((BYTE*)pBits)[index];
                BYTE G = ((BYTE*)pBits)[index + 1];
                BYTE R = ((BYTE*)pBits)[index + 2];

                int neighborIndex = ((y + randOffset) * width + (x + randOffset)) * 3;
                if (neighborIndex >= 0 && neighborIndex < width * height * 3) {
                    BYTE nb = ((BYTE*)pBits)[neighborIndex];
                    BYTE ng = ((BYTE*)pBits)[neighborIndex + 1];
                    BYTE nr = ((BYTE*)pBits)[neighborIndex + 2];

                    ((BYTE*)pBits)[index] = (BYTE)((B + nb) / 2);
                    ((BYTE*)pBits)[index + 1] = (BYTE)((G + ng) / 2);
                    ((BYTE*)pBits)[index + 2] = (BYTE)((R + nr) / 2);
                }
            }
        }

        HDC hDestDC = GetDC(NULL);
        BitBlt(hDestDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hDestDC);

        Sleep(50);
    }
    return 0;
}

DWORD WINAPI distort(LPVOID lpParam) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    float time = 0.0f;

    while (true) {
        time += 0.1f;

        HDC hScreenDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);

        BITMAPINFO bInfo = { sizeof(BITMAPINFOHEADER), width, -height, 1, 24, 0, 0, 0, 0, 0, 0 };
        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hScreenDC, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hMemDC, hBitmap);

        BitBlt(hMemDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
        memcpy(pBits, pBits, width * height * 3);

        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                int index = (y * width + x) * 3;

                float ripple = sin((x + y) + time) * 5;
                int dx = (int)ripple;
                int dy = (int)ripple;

                int srcX = x + dx;
                int srcY = y + dy;

                if (srcX >= 0 && srcX < width && srcY >= 0 && srcY < height) {
                    int srcIndex = (srcY * width + srcX) * 3;

                    BYTE B = ((BYTE*)pBits)[srcIndex];
                    BYTE G = ((BYTE*)pBits)[srcIndex + 1];
                    BYTE R = ((BYTE*)pBits)[srcIndex + 2];

                    ((BYTE*)pBits)[index] = (BYTE)((B + rand() % 50) % 256);
                    ((BYTE*)pBits)[index + 1] = (BYTE)((G + rand() % 50) % 256);
                    ((BYTE*)pBits)[index + 2] = (BYTE)((R + rand() % 50) % 256);
                }
            }
        }

        HDC hDestDC = GetDC(NULL);
        BitBlt(hDestDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hDestDC);

        Sleep(100);
    }
    return 0;
}

DWORD WINAPI smthg(LPVOID lpParam) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    int size = width * height;
    while (true) {
        HDC hScreenDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);
        BITMAPINFO bInfo = { sizeof(BITMAPINFOHEADER), width, -height, 1, 24, 0, 0, 0, 0, 0, 0 };
        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hScreenDC, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hMemDC, hBitmap);

        BitBlt(hMemDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

        PRGBTRIPLE pixels = (PRGBTRIPLE)pBits;
        for (int y = 1; y < height; y++) {
            for (int x = 1; x < width; x++) {
                int index = y * width + x;
                pixels[index] = pixels[index + (rand() % 10)];
            }
        }

        HDC hDestDC = GetDC(NULL);
        BitBlt(hDestDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hDestDC);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI wavedistort(LPVOID lpParam) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    float time = 0.0f;

    while (true) {
        time += 0.1f;

        HDC hScreenDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);

        BITMAPINFO bInfo = { 0 };
        bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bInfo.bmiHeader.biWidth = width;
        bInfo.bmiHeader.biHeight = -height;
        bInfo.bmiHeader.biPlanes = 1;
        bInfo.bmiHeader.biBitCount = 24;
        bInfo.bmiHeader.biCompression = BI_RGB;

        void* pBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hScreenDC, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hMemDC, hBitmap);

        BitBlt(hMemDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

        BYTE* pixels = (BYTE*)pBits;
        const int stride = width * 3;

        BYTE* srcPixels = new BYTE[stride * height];
        memcpy(srcPixels, pixels, stride * height);

        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                int index = y * stride + x * 3;

                float waveX = sinf((float)y * 0.05f + time) * 10.0f;
                float waveY = cosf((float)x * 0.05f + time) * 10.0f;

                int srcX = x + (int)waveX;
                int srcY = y + (int)waveY;

                if (srcX >= 0 && srcX < width && srcY >= 0 && srcY < height) {
                    int srcIndex = srcY * stride + srcX * 3;

                    BYTE B = srcPixels[srcIndex];
                    BYTE G = srcPixels[srcIndex + 1];
                    BYTE R = srcPixels[srcIndex + 2];

                    pixels[index] = (BYTE)((B + rand() % 30) % 256);
                    pixels[index + 1] = (BYTE)((G + rand() % 30) % 256);
                    pixels[index + 2] = (BYTE)((R + rand() % 30) % 256);
                }
            }
        }

        delete[] srcPixels;

        HDC hDestDC = GetDC(NULL);
        BitBlt(hDestDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hDestDC);

        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);

        Sleep(30);
    }
    return 0;
}

DWORD WINAPI idksmthg(LPVOID lpParam) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    float time = 0.0f;

    while (true) {
        time += 0.1f;

        HDC hScreenDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);

        BITMAPINFO bInfo = { 0 };
        bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bInfo.bmiHeader.biWidth = width;
        bInfo.bmiHeader.biHeight = -height;
        bInfo.bmiHeader.biPlanes = 1;
        bInfo.bmiHeader.biBitCount = 24;
        bInfo.bmiHeader.biCompression = BI_RGB;

        void* pBits;
        HBITMAP hBitmap = CreateDIBSection(hScreenDC, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
        SelectObject(hMemDC, hBitmap);

        BitBlt(hMemDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

        BYTE* pixels = (BYTE*)pBits;
        int stride = width * 3;

        BYTE* tempBuffer = new BYTE[height * stride];
        memcpy(tempBuffer, pixels, height * stride);

        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                float waveX = 10.0f * sinf(2.0f * 6.14159f * y / 31.0f + time); // fuck pi
                float waveY = 10.0f * cosf(2.0f * 6.143159f * x / 30.0f + time);

                int srcX = x + (int)waveX;
                int srcY = y + (int)waveY;

                if (srcX < 0) srcX = 0;
                if (srcX >= width) srcX = width - 1;
                if (srcY < 0) srcY = 0;
                if (srcY >= height) srcY = height - 1;

                int dstIndex = (y * width + x) * 3;
                int srcIndex = (srcY * width + srcX) * 3;

                tempBuffer[dstIndex] = (BYTE)((pixels[srcIndex] + rand() % 20) % 256);
                tempBuffer[dstIndex + 1] = (BYTE)((pixels[srcIndex + 1] + rand() % 20) % 256);
                tempBuffer[dstIndex + 2] = (BYTE)((pixels[srcIndex + 2] + rand() % 20) % 256);
            }
        }

        memcpy(pixels, tempBuffer, height * stride);
        delete[] tempBuffer;

        HDC hDestDC = GetDC(NULL);
        BitBlt(hDestDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hDestDC);

        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);

        Sleep(30);
    }
    return 0;
}

DWORD WINAPI crazyColor(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i < size; i++) {
            BYTE Blue = Rgb[i].rgbtBlue;
            BYTE Green = Rgb[i].rgbtGreen;
            BYTE Red = Rgb[i].rgbtRed;

            float factor = sin(GetTickCount() * 0.001 + i * 0.01);
            BYTE newRed = (BYTE)(Red * 0.5 + 127.5 + 127.5 * sin(factor));
            BYTE newGreen = (BYTE)(Green * 0.5 + 127.5 + 127.5 * sin(factor + 2));
            BYTE newBlue = (BYTE)(Blue * 0.5 + 127.5 + 127.5 * sin(factor + 4));

            Rgb[i].rgbtRed = newRed;
            Rgb[i].rgbtGreen = newGreen;
            Rgb[i].rgbtBlue = newBlue;
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(50);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI Color2(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i < size; i++) {
            BYTE Blue = Rgb[i].rgbtBlue;
            BYTE Green = Rgb[i].rgbtGreen;
            BYTE Red = Rgb[i].rgbtRed;

            float factor = sin(GetTickCount() * 0.5 + i * 0.2);
            BYTE newRed = (BYTE)(Red * 0.5 + 127.5 + 127.5 * cos(factor));
            BYTE newGreen = (BYTE)(Green * 0.5 + 107.5 + 127.5 * sin(factor + 2));
            BYTE newBlue = (BYTE)(Blue * 0.5 + 143.5 + 57.5 * cos(factor + 4));

            Rgb[i].rgbtRed = newRed;
            Rgb[i].rgbtGreen = newGreen;
            Rgb[i].rgbtBlue = newBlue;
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(50);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI Color3(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i < size; i++) {
            BYTE Blue = Rgb[i].rgbtBlue;
            BYTE Green = Rgb[i].rgbtGreen;
            BYTE Red = Rgb[i].rgbtRed;
            BYTE newRed = static_cast<BYTE>(Red * 0.5 + 127.5);
            BYTE newGreen = static_cast<BYTE>(Green * 0.5 + 27.5 + 117.5);
            BYTE newBlue = static_cast<BYTE>(Blue * 0.5 + 123.5 + 55.5);

            Rgb[i].rgbtRed = newGreen;
            Rgb[i].rgbtGreen = newBlue;
            Rgb[i].rgbtBlue = newRed;
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(50);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

// --< Visual >--

// --< Main >--
int main() {
    HANDLE shake = CreateThread(0, 0, Shake, 0, 0, 0);
    HANDLE GreyShader = CreateThread(0, 0, Grey_Shader, 0, 0, 0);
    sound1();
    Sleep(30000);
    TerminateThread(GreyShader, 0);
    HANDLE sepia = CreateThread(0, 0, sepia_Shader, 0, 0, 0);
    sound2();
    Sleep(30000);
    TerminateThread(sepia, 0);
    HANDLE glitch = CreateThread(0, 0, glitched_Shader, 0, 0, 0);
    sound3();
    Sleep(30000);
    TerminateThread(glitch, 0);
    HANDLE shit = CreateThread(0, 0, holyshit, 0, 0, 0);
    sound4();
    Sleep(30000);
    HANDLE blur = CreateThread(0, 0, blurryass, 0, 0, 0);
    sound5();
    Sleep(30000);
    TerminateThread(shit, 0);
    TerminateThread(blur, 0);
    HANDLE distortt = CreateThread(0, 0, distort, 0, 0, 0);
    sound6();
    Sleep(30000);
    TerminateThread(distortt, 0);
    HANDLE wavedistortt = CreateThread(0, 0, wavedistort, 0, 0, 0);
    HANDLE rotate = CreateThread(0, 0, smthg, 0, 0, 0);
    sound7();
    Sleep(30000);
    TerminateThread(wavedistortt, 0);
    HANDLE idkbro = CreateThread(0, 0, idksmthg, 0, 0, 0);
    sound8();
    Sleep(30000);
    TerminateThread(idkbro, 0);
    TerminateThread(rotate, 0);
    HANDLE hmm = CreateThread(0, 0, crazyColor, 0, 0, 0);
    sound9();
    Sleep(30000);
    TerminateThread(hmm, 0);
    HANDLE hmm2 = CreateThread(0, 0, Color2, 0, 0, 0);
    sound10();
    Sleep(30000);
    TerminateThread(hmm2, 0);
    HANDLE hmm3 = CreateThread(0, 0, Color3, 0, 0, 0);
    sound11();
    Sleep(30000);
    TerminateThread(hmm3, 0);
    return 1;
}
// --< Main >--

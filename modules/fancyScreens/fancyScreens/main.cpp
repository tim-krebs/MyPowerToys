#include <iostream>
#include <Windows.h>
#include <Dbt.h>
#include "fancyScreens.h"

/// <summary>
/// ToastNotification
/// </summary>

void CheckMonitorConfiguration();

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DISPLAYCHANGE:
        // Rufen Sie hier Ihre SetDisplay-Funktion auf
        FancyScreens::SetDisplay(FALSE);
        CheckMonitorConfiguration();

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Registrieren der Fensterklasse
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"FancyScreensClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassExW(&wcex))
    {
        MessageBox(NULL, L"Call to RegisterClassEx failed!", L"Win32 Guided Tour", NULL);
        return 1;
    }

    // Erstellen des Hauptfensters
    HWND hWnd = CreateWindowW(
        wcex.lpszClassName,
        L"Fancy Screens App",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd)
    {
        MessageBox(NULL, L"Call to CreateWindow failed!", L"Win32 Guided Tour", NULL);
        return 1;
    }

    // Anzeigen und Aktualisieren des Hauptfensters
    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);

    // Hauptnachrichtenschleife:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


void CheckMonitorConfiguration()
{
    int msgBoxResult = MessageBox(
        NULL,
        L"Sind die Monitore korrekt angeordnet?",
        L"Monitore überprüfen",
        MB_YESNO | MB_ICONQUESTION
    );

    if (msgBoxResult == IDYES)
    {
        // Die Monitore sind korrekt angeordnet, führen Sie hier die gewünschten Aktionen aus
        exit;
    }
    else
    {
        // Die Monitore sind nicht korrekt angeordnet, führen Sie hier die gewünschten Aktionen aus
        FancyScreens::SetDisplay(TRUE);
    }
}
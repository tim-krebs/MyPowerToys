#include "fancyScreens.h"


void FancyScreens::SwapDisplays(DEVMODE* display1, DEVMODE* display2)
{
    DEVMODE tempDisplay;
    memcpy(&tempDisplay, display1, sizeof(DEVMODE));
    memcpy(display1, display2, sizeof(DEVMODE));
    memcpy(display2, &tempDisplay, sizeof(DEVMODE));

    ChangeDisplaySettingsEx(NULL, display1, NULL, CDS_UPDATEREGISTRY, NULL);
}

void FancyScreens::SetDisplay(bool swapSecondaryDisplays)
{
    BOOL            FoundSecondaryDisp = FALSE;
    DWORD           DispNum = 0;
    DISPLAY_DEVICE  DisplayDevice;
    LONG            Result;
    TCHAR           szTemp[200];
    DEVMODE         defaultMode;
    DEVMODE         primaryDevMode;
    DEVMODE         secondaryDevModes[2];
    POINTL          primaryPosition;
    int             secondaryDisplays = 0;

    // initialize DisplayDevice
    ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
    DisplayDevice.cb = sizeof(DisplayDevice);

    // Set primary display device first
    while (EnumDisplayDevices(NULL, DispNum, &DisplayDevice, 0))
    {
        if ((DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) &&
            (DisplayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
        {
            ZeroMemory(&primaryDevMode, sizeof(DEVMODE));
            primaryDevMode.dmSize = sizeof(DEVMODE);
            primaryDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_POSITION
                | DM_DISPLAYFREQUENCY | DM_DISPLAYFLAGS;

            primaryDevMode.dmPosition.x = 0;
            primaryDevMode.dmPosition.y = 0;

            Result = ChangeDisplaySettingsEx((LPWSTR)DisplayDevice.DeviceName,
                &primaryDevMode,
                NULL,
                CDS_UPDATEREGISTRY,
                NULL);

            EnumDisplaySettings(DisplayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &primaryDevMode);
            primaryDevMode.dmPelsWidth = primaryDevMode.dmPelsWidth;
            primaryPosition = primaryDevMode.dmPosition;

            break;
        }
        DispNum++;
    }

    // Reset the DispNum and reinitialize DisplayDevice
    DispNum = 0;
    ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
    DisplayDevice.cb = sizeof(DisplayDevice);

    // Align other displays regarding the primary display device
    while (EnumDisplayDevices(NULL, DispNum, &DisplayDevice, 0))
    {
        if (secondaryDisplays < 2 && (DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) &&
            !(DisplayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
        {
            DEVMODE    DevMode;
            ZeroMemory(&DevMode, sizeof(DevMode));
            DevMode.dmSize = sizeof(DevMode);
            DevMode.dmFields = DM_POSITION;

            if (secondaryDisplays == 0)
            {
                EnumDisplaySettings(DisplayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &DevMode);
                DevMode.dmPosition.x = primaryPosition.x + primaryDevMode.dmPelsWidth;
                DevMode.dmPosition.y = primaryPosition.y;

                memcpy(&secondaryDevModes[secondaryDisplays], &DevMode, sizeof(DEVMODE));
            }
            else
            {
                DevMode.dmPosition.x = primaryPosition.x + primaryDevMode.dmPelsWidth + secondaryDevModes[secondaryDisplays - 1].dmPelsWidth;
                DevMode.dmPosition.y = primaryPosition.y;

                memcpy(&secondaryDevModes[secondaryDisplays], &DevMode, sizeof(DEVMODE));
            }


            secondaryDisplays++;
        }

        // Reinitialize DisplayDevice for the next iteration
        ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
        DisplayDevice.cb = sizeof(DisplayDevice);
        DispNum++;
    }

    if (swapSecondaryDisplays && secondaryDisplays == 2)
    {
        FancyScreens::SwapDisplays(&secondaryDevModes[0], &secondaryDevModes[1]);
    }

    DispNum = 0;
    secondaryDisplays = 0;
    ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
    DisplayDevice.cb = sizeof(DisplayDevice);

    while (EnumDisplayDevices(NULL, DispNum, &DisplayDevice, 0))
    {
        if (secondaryDisplays < 2 && (DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) &&
            !(DisplayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
        {
            Result = ChangeDisplaySettingsEx((LPWSTR)DisplayDevice.DeviceName,
                &secondaryDevModes[secondaryDisplays],
                NULL,
                CDS_UPDATEREGISTRY,
                NULL);
            secondaryDisplays++;
        }
        // Reinitialize DisplayDevice for the next iteration
        ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
        DisplayDevice.cb = sizeof(DisplayDevice);
        DispNum++;
    }
}
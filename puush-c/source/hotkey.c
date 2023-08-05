#include "hotkey.h"

const KeyMapping keyMappings[] = {
    {L"None", L"None", 0, FALSE, FALSE},
    {L"Back", L"Back", VK_BACK, FALSE, FALSE},
    {L"Tab", L"Tab", VK_TAB, FALSE, FALSE},
    {L"Enter", L"Enter", VK_RETURN, FALSE, FALSE},
    {L"Escape", L"Escape", VK_ESCAPE, FALSE, FALSE},
    {L"Space", L"Space", VK_SPACE, FALSE, FALSE},
    {L"PageUp", L"Page Up", VK_PRIOR, FALSE, FALSE},
    {L"PageDown", L"Page Down", VK_NEXT, FALSE, FALSE},
    {L"End", L"End", VK_END, FALSE, FALSE},
    {L"Home", L"Home", VK_HOME, FALSE, FALSE},
    {L"Left", L"Left", VK_LEFT, FALSE, FALSE},
    {L"Up", L"Up", VK_UP, FALSE, FALSE},
    {L"Right", L"Right", VK_RIGHT, FALSE, FALSE},
    {L"Down", L"Down", VK_DOWN, FALSE, FALSE},
    {L"Insert", L"Insert", VK_INSERT, FALSE, FALSE},
    {L"Delete", L"Delete", VK_DELETE, FALSE, FALSE},
    {L"LControlKey", L"Ctrl", VK_LCONTROL, TRUE, FALSE},
    {L"RControlKey", L"Ctrl", VK_RCONTROL, TRUE, TRUE},
    {L"LShiftKey", L"Shift", VK_LSHIFT, TRUE, FALSE},
    {L"RShiftKey", L"Shift", VK_RSHIFT, TRUE, TRUE},
    {L"LMenu", L"Alt", VK_LMENU, TRUE, FALSE},
    {L"RMenu", L"Alt", VK_RMENU, TRUE, TRUE},
    {L"LWin", L"Win", VK_LWIN, TRUE, FALSE},
    {L"RWin", L"Win", VK_RWIN, TRUE, TRUE},
    {L"A", L"A", 'A', FALSE, FALSE},
    {L"B", L"B", 'B', FALSE, FALSE},
    {L"C", L"C", 'C', FALSE, FALSE},
    {L"D", L"D", 'D', FALSE, FALSE},
    {L"E", L"E", 'E', FALSE, FALSE},
    {L"F", L"F", 'F', FALSE, FALSE},
    {L"G", L"G", 'G', FALSE, FALSE},
    {L"H", L"H", 'H', FALSE, FALSE},
    {L"I", L"I", 'I', FALSE, FALSE},
    {L"J", L"J", 'J', FALSE, FALSE},
    {L"K", L"K", 'K', FALSE, FALSE},
    {L"L", L"L", 'L', FALSE, FALSE},
    {L"M", L"M", 'M', FALSE, FALSE},
    {L"N", L"N", 'N', FALSE, FALSE},
    {L"O", L"O", 'O', FALSE, FALSE},
    {L"P", L"P", 'P', FALSE, FALSE},
    {L"Q", L"Q", 'Q', FALSE, FALSE},
    {L"R", L"R", 'R', FALSE, FALSE},
    {L"S", L"S", 'S', FALSE, FALSE},
    {L"T", L"T", 'T', FALSE, FALSE},
    {L"U", L"U", 'U', FALSE, FALSE},
    {L"V", L"V", 'V', FALSE, FALSE},
    {L"W", L"W", 'W', FALSE, FALSE},
    {L"X", L"X", 'X', FALSE, FALSE},
    {L"Y", L"Y", 'Y', FALSE, FALSE},
    {L"Z", L"Z", 'Z', FALSE, FALSE},
    {L"D0", L"0", '0', FALSE, FALSE},
    {L"D1", L"1", '1', FALSE, FALSE},
    {L"D2", L"2", '2', FALSE, FALSE},
    {L"D3", L"3", '3', FALSE, FALSE},
    {L"D4", L"4", '4', FALSE, FALSE},
    {L"D5", L"5", '5', FALSE, FALSE},
    {L"D6", L"6", '6', FALSE, FALSE},
    {L"D7", L"7", '7', FALSE, FALSE},
    {L"D8", L"8", '8', FALSE, FALSE},
    {L"D9", L"9", '9', FALSE, FALSE},
    {L"F1", L"F1", VK_F1, FALSE, FALSE},
    {L"F2", L"F2", VK_F2, FALSE, FALSE},
    {L"F3", L"F3", VK_F3, FALSE, FALSE},
    {L"F4", L"F4", VK_F4, FALSE, FALSE},
    {L"F5", L"F5", VK_F5, FALSE, FALSE},
    {L"F6", L"F6", VK_F6, FALSE, FALSE},
    {L"F7", L"F7", VK_F7, FALSE, FALSE},
    {L"F8", L"F8", VK_F8, FALSE, FALSE},
    {L"F9", L"F9", VK_F9, FALSE, FALSE},
    {L"F10", L"F10", VK_F10, FALSE, FALSE},
    {L"F11", L"F11", VK_F11, FALSE, FALSE},
    {L"F12", L"F12", VK_F12, FALSE, FALSE},
    {L"NumLock", L"NumLock", VK_NUMLOCK, FALSE, FALSE},
    {L"ScrollLock", L"ScrollLock", VK_SCROLL, FALSE, FALSE},
    {L"Pause", L"Pause", VK_PAUSE, FALSE, FALSE},
};

KeyMapping* StringToKeyMapping(LPCWSTR lpwstr) {
    for (UINT i = 0; i < KEY_MAPPINGS_LENGTH; i++) {
        if (wcscmp(lpwstr, keyMappings[i].configName) == 0) {
            return &keyMappings[i];
        }
    }
    return NULL;
}

LPCWSTR VkCodeToConfigName(UINT vkCode) {
    for (UINT i = 0; i < KEY_MAPPINGS_LENGTH; i++) {
        if (vkCode == keyMappings[i].vkCode) {
            return keyMappings[i].configName;
        }
    }
    return NULL;
}

LPCWSTR VkCodeToKeyName(UINT vkCode) {
    for (UINT i = 0; i < KEY_MAPPINGS_LENGTH; i++) {
        if (vkCode == keyMappings[i].vkCode) {
            return keyMappings[i].prettyName;
        }
    }
    return NULL;
}

KeyMapping* VkCodeToKeyMapping(UINT vkCode) {
    for (UINT i = 0; i < KEY_MAPPINGS_LENGTH; i++) {
        if (vkCode == keyMappings[i].vkCode) {
            return &keyMappings[i];
        }
    }
    return NULL;
}

Hotkey ParseHotkeyFromIniFileString(const WCHAR* hotKeyString) {
    Hotkey hotkey = { 0 };

    WCHAR* context = NULL;
    WCHAR* keyToken = wcstok_s((WCHAR*)hotKeyString, L"-", &context);
    while (keyToken != NULL && hotkey.keyCount < HOTKEY_MAX_KEYS) {
        KeyMapping* km = StringToKeyMapping(keyToken);
        if (km) {
            hotkey.keys[hotkey.keyCount++] = km->vkCode;
        }
        keyToken = wcstok_s(NULL, L"-", &context);
    }

    return hotkey;
}

void HotkeyToIniFileString(const Hotkey* hotkey, WCHAR* outputBuffer, size_t bufferSize) {
    if (!hotkey || hotkey->keyCount < 1 || hotkey->keyCount > 3) {
        return; // Invalid hotkey, do nothing
    }

    outputBuffer[0] = '\0'; // Initialize the buffer with an empty string

    for (int i = 0; i < hotkey->keyCount; i++) {
        KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[i]);
        if (km && km->isModifier) {
            wcscat_s(outputBuffer, bufferSize, km->configName);
            wcscat_s(outputBuffer, bufferSize, L"-");
        }
    }

    KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[hotkey->keyCount - 1]);
    if (km) {
        wcscat_s(outputBuffer, bufferSize, km->configName);
    }
}

void HotkeyToPrettyString(const Hotkey* hotkey, WCHAR* outputBuffer, size_t bufferSize) {
    if (!hotkey || hotkey->keyCount < 1 || hotkey->keyCount > 3) {
        return; // Invalid hotkey, do nothing
    }

    outputBuffer[0] = '\0';

    for (int i = 0; i < hotkey->keyCount; i++) {
        KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[i]);
        if (km && km->isModifier) {
            wcscat_s(outputBuffer, bufferSize, km->prettyName);
            wcscat_s(outputBuffer, bufferSize, L" + ");
        }
    }

    KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[hotkey->keyCount - 1]);
    if (km) {
        wcscat_s(outputBuffer, bufferSize, km->prettyName);
    }
}

BOOL IsHotkeyMatch(KBDLLHOOKSTRUCT* kbdStruct, const Hotkey* hotkey) {
    // Check if the last pressed key matches the provided virtual key code
    if (kbdStruct->vkCode != hotkey->keys[hotkey->keyCount - 1]) {
        return FALSE;  // Last key does not match
    }

    for (int i = 0; i < hotkey->keyCount - 1; ++i) {
        if ((GetAsyncKeyState(hotkey->keys[i]) & 0x8000) == 0) {
            return FALSE;  // Key not pressed
        }
    }

    return TRUE;  // All keys pressed, last key matched
}
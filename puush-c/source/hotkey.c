#include "hotkey.h"

const KeyMapping keyMappings[] = {
    {_T("None"), _T("None"), 0, FALSE, FALSE},
    {_T("Back"), _T("Back"), VK_BACK, FALSE, FALSE},
    {_T("Tab"), _T("Tab"), VK_TAB, FALSE, FALSE},
    {_T("Enter"), _T("Enter"), VK_RETURN, FALSE, FALSE},
    {_T("Escape"), _T("Escape"), VK_ESCAPE, FALSE, FALSE},
    {_T("Space"), _T("Space"), VK_SPACE, FALSE, FALSE},
    {_T("PageUp"), _T("Page Up"), VK_PRIOR, FALSE, FALSE},
    {_T("PageDown"), _T("Page Down"), VK_NEXT, FALSE, FALSE},
    {_T("End"), _T("End"), VK_END, FALSE, FALSE},
    {_T("Home"), _T("Home"), VK_HOME, FALSE, FALSE},
    {_T("Left"), _T("Left"), VK_LEFT, FALSE, FALSE},
    {_T("Up"), _T("Up"), VK_UP, FALSE, FALSE},
    {_T("Right"), _T("Right"), VK_RIGHT, FALSE, FALSE},
    {_T("Down"), _T("Down"), VK_DOWN, FALSE, FALSE},
    {_T("Insert"), _T("Insert"), VK_INSERT, FALSE, FALSE},
    {_T("Delete"), _T("Delete"), VK_DELETE, FALSE, FALSE},
    {_T("LControlKey"), _T("Ctrl"), VK_LCONTROL, TRUE, FALSE},
    {_T("RControlKey"), _T("Ctrl"), VK_RCONTROL, TRUE, TRUE},
    {_T("LShiftKey"), _T("Shift"), VK_LSHIFT, TRUE, FALSE},
    {_T("RShiftKey"), _T("Shift"), VK_RSHIFT, TRUE, TRUE},
    {_T("LMenu"), _T("Alt"), VK_LMENU, TRUE, FALSE},
    {_T("RMenu"), _T("Alt"), VK_RMENU, TRUE, TRUE},
    {_T("LWin"), _T("Win"), VK_LWIN, TRUE, FALSE},
    {_T("RWin"), _T("Win"), VK_RWIN, TRUE, TRUE},
    {_T("A"), _T("A"), 'A', FALSE, FALSE},
    {_T("B"), _T("B"), 'B', FALSE, FALSE},
    {_T("C"), _T("C"), 'C', FALSE, FALSE},
    {_T("D"), _T("D"), 'D', FALSE, FALSE},
    {_T("E"), _T("E"), 'E', FALSE, FALSE},
    {_T("F"), _T("F"), 'F', FALSE, FALSE},
    {_T("G"), _T("G"), 'G', FALSE, FALSE},
    {_T("H"), _T("H"), 'H', FALSE, FALSE},
    {_T("I"), _T("I"), 'I', FALSE, FALSE},
    {_T("J"), _T("J"), 'J', FALSE, FALSE},
    {_T("K"), _T("K"), 'K', FALSE, FALSE},
    {_T("L"), _T("L"), 'L', FALSE, FALSE},
    {_T("M"), _T("M"), 'M', FALSE, FALSE},
    {_T("N"), _T("N"), 'N', FALSE, FALSE},
    {_T("O"), _T("O"), 'O', FALSE, FALSE},
    {_T("P"), _T("P"), 'P', FALSE, FALSE},
    {_T("Q"), _T("Q"), 'Q', FALSE, FALSE},
    {_T("R"), _T("R"), 'R', FALSE, FALSE},
    {_T("S"), _T("S"), 'S', FALSE, FALSE},
    {_T("T"), _T("T"), 'T', FALSE, FALSE},
    {_T("U"), _T("U"), 'U', FALSE, FALSE},
    {_T("V"), _T("V"), 'V', FALSE, FALSE},
    {_T("W"), _T("W"), 'W', FALSE, FALSE},
    {_T("X"), _T("X"), 'X', FALSE, FALSE},
    {_T("Y"), _T("Y"), 'Y', FALSE, FALSE},
    {_T("Z"), _T("Z"), 'Z', FALSE, FALSE},
    {_T("D0"), _T("0"), '0', FALSE, FALSE},
    {_T("D1"), _T("1"), '1', FALSE, FALSE},
    {_T("D2"), _T("2"), '2', FALSE, FALSE},
    {_T("D3"), _T("3"), '3', FALSE, FALSE},
    {_T("D4"), _T("4"), '4', FALSE, FALSE},
    {_T("D5"), _T("5"), '5', FALSE, FALSE},
    {_T("D6"), _T("6"), '6', FALSE, FALSE},
    {_T("D7"), _T("7"), '7', FALSE, FALSE},
    {_T("D8"), _T("8"), '8', FALSE, FALSE},
    {_T("D9"), _T("9"), '9', FALSE, FALSE},
    {_T("F1"), _T("F1"), VK_F1, FALSE, FALSE},
    {_T("F2"), _T("F2"), VK_F2, FALSE, FALSE},
    {_T("F3"), _T("F3"), VK_F3, FALSE, FALSE},
    {_T("F4"), _T("F4"), VK_F4, FALSE, FALSE},
    {_T("F5"), _T("F5"), VK_F5, FALSE, FALSE},
    {_T("F6"), _T("F6"), VK_F6, FALSE, FALSE},
    {_T("F7"), _T("F7"), VK_F7, FALSE, FALSE},
    {_T("F8"), _T("F8"), VK_F8, FALSE, FALSE},
    {_T("F9"), _T("F9"), VK_F9, FALSE, FALSE},
    {_T("F10"), _T("F10"), VK_F10, FALSE, FALSE},
    {_T("F11"), _T("F11"), VK_F11, FALSE, FALSE},
    {_T("F12"), _T("F12"), VK_F12, FALSE, FALSE},
    {_T("NumLock"), _T("NumLock"), VK_NUMLOCK, FALSE, FALSE},
    {_T("ScrollLock"), _T("ScrollLock"), VK_SCROLL, FALSE, FALSE},
    {_T("Pause"), _T("Pause"), VK_PAUSE, FALSE, FALSE},
};

KeyMapping* StringToKeyMapping(LPCWSTR lpwstr) {
    for (UINT i = 0; i < KEY_MAPPINGS_LENGTH; i++) {
        if (wcscmp(lpwstr, keyMappings[i].configName) == 0) {
            return &keyMappings[i];
        }
    }
    return NULL;
}

LPCTSTR VkCodeToConfigName(UINT vkCode) {
    for (UINT i = 0; i < KEY_MAPPINGS_LENGTH; i++) {
        if (vkCode == keyMappings[i].vkCode) {
            return keyMappings[i].configName;
        }
    }
    return NULL;
}

LPCTSTR VkCodeToKeyName(UINT vkCode) {
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

Hotkey ParseHotkeyFromIniFileString(const TCHAR* hotKeyString) {
    Hotkey hotkey = { 0 };

    TCHAR* context = NULL;
    TCHAR* keyToken = _tcstok_s((TCHAR*)hotKeyString, _T("-"), &context);
    while (keyToken != NULL && hotkey.keyCount < HOTKEY_MAX_KEYS) {
        KeyMapping* km = StringToKeyMapping(keyToken);
        if (km) {
            hotkey.keys[hotkey.keyCount++] = km->vkCode;
        }
        keyToken = _tcstok_s(NULL, _T("-"), &context);
    }

    return hotkey;
}

void HotkeyToIniFileString(const Hotkey* hotkey, TCHAR* outputBuffer, size_t bufferSize) {
    if (!hotkey || hotkey->keyCount < 1 || hotkey->keyCount > 3) {
        return; // Invalid hotkey, do nothing
    }

    outputBuffer[0] = '\0'; // Initialize the buffer with an empty string

    for (int i = 0; i < hotkey->keyCount; i++) {
        KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[i]);
        if (km && km->isModifier) {
            _tcscat_s(outputBuffer, bufferSize, km->configName);
            _tcscat_s(outputBuffer, bufferSize, _T("-"));
        }
    }

    KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[hotkey->keyCount - 1]);
    if (km) {
        _tcscat_s(outputBuffer, bufferSize, km->configName);
    }
}

void HotkeyToPrettyString(const Hotkey* hotkey, TCHAR* outputBuffer, size_t bufferSize) {
    if (!hotkey || hotkey->keyCount < 1 || hotkey->keyCount > 3) {
        return; // Invalid hotkey, do nothing
    }

    outputBuffer[0] = '\0';

    for (int i = 0; i < hotkey->keyCount; i++) {
        KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[i]);
        if (km && km->isModifier) {
            _tcscat_s(outputBuffer, bufferSize, km->prettyName);
            _tcscat_s(outputBuffer, bufferSize, _T(" + "));
        }
    }

    KeyMapping* km = VkCodeToKeyMapping(hotkey->keys[hotkey->keyCount - 1]);
    if (km) {
        _tcscat_s(outputBuffer, bufferSize, km->prettyName);
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
#pragma once

#include <windows.h>
#include <string.h>
#include <stdio.h>

#define HOTKEY_MAX_KEYS 3
#define KEY_MAPPINGS_LENGTH (sizeof(keyMappings) / sizeof(KeyMapping))

typedef struct {
    UINT keys[3];
    int keyCount;
} Hotkey;

typedef struct {
    LPCWSTR configName;  // for use in config files
    LPCWSTR prettyName;  // for display to the user
    UINT vkCode;         // virtual-key code
    BOOL isModifier;     // TRUE for modifier keys, FALSE otherwise
    BOOL rightSide;      // TRUE for right-side keys, FALSE for left-side or non-side-specific keys
} KeyMapping;

KeyMapping* StringToKeyMapping(LPCWSTR lpwstr);
LPCTSTR VkCodeToConfigName(UINT vkCode);
LPCTSTR VkCodeToKeyName(UINT vkCode);
KeyMapping* VkCodeToKeyMapping(UINT vkCode);
Hotkey ParseHotkeyFromIniFileString(const WCHAR* hotKeyString);
void HotkeyToIniFileString(const Hotkey* hotkey, WCHAR* outputBuffer, size_t bufferSize);
void HotkeyToPrettyString(const Hotkey* hotkey, WCHAR* outputBuffer, size_t bufferSize);
BOOL IsHotkeyMatch(KBDLLHOOKSTRUCT* kbdStruct, const Hotkey* hotkey);

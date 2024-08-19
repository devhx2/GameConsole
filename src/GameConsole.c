#include "GameConsole.h"

#include <Windows.h>

/// @brief スクリーンバッファ1
HANDLE g_screen_buffer1 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
/// @brief スクリーンバッファ2
HANDLE g_screen_buffer2 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

/// @brief 初期化処理
void Initialize()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
}

/// @brief 終了処理
void Finalize()
{
    CloseHandle(g_screen_buffer1);
    CloseHandle(g_screen_buffer2);
}

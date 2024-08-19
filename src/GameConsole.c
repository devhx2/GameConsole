#include "GameConsole.h"

#include <Windows.h>

/// @brief スクリーンバッファ1
HANDLE g_screen_buffer1;
/// @brief スクリーンバッファ2
HANDLE g_screen_buffer2;

/// @brief 初期化処理
void Initialize()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    g_screen_buffer1 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    g_screen_buffer2 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    {
        DWORD console_mode;
        const HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(input_handle, &console_mode);
        // 入力された文字を非表示にする
        console_mode &= ~ENABLE_ECHO_INPUT;
        // Ctrl+Cなどのシグナルを無効にする
        console_mode &= ~ENABLE_PROCESSED_INPUT;
        SetConsoleMode(input_handle, console_mode);
    }
}

/// @brief 終了処理
void Finalize()
{
    CloseHandle(g_screen_buffer1);
    CloseHandle(g_screen_buffer2);
}

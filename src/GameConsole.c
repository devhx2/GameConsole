
#include "GameConsole.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vadefs.h>
#include <windows.h>

HANDLE g_screen_buffer1;     /* スクリーンバッファ1 */
HANDLE g_screen_buffer2;     /* スクリーンバッファ2 */
const char *ESCAPE = "\033"; /* エスケープシーケンス */

void print(const char *format, va_list args) {
  char buffer[256];

  vsprintf_s(buffer, (size_t)(sizeof(buffer)), format, args);

  DWORD written = strlen(buffer);
  WriteConsoleA(g_screen_buffer1, buffer, written, &written, NULL);
}

void sprint(const char *format, ...) {
  va_list args;
  va_start(args, format);
  print(format, args);
  va_end(args);
}

/******************************************************************************
 * @brief ライブラリの初期化処理
 *
 ******************************************************************************/
void Initialize() {
  g_screen_buffer1 = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
  g_screen_buffer2 = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

  {
    DWORD console_mode;
    const HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(input_handle, &console_mode);
    // 入力された文字を非表示にする
    console_mode &= ~ENABLE_ECHO_INPUT;
    // Ctrl+Cなどのシグナルを無効にする
    console_mode &= ~ENABLE_PROCESSED_INPUT;
    // コンソール上でのマウス操作を無効にする
    console_mode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(input_handle, console_mode);
  }

  {
    // 仮想コンソールモードをオン ->
    // Debugはデフォルトでオンだが,Releaseはオフのためプログラムでオン
    DWORD console_mode;
    GetConsoleMode(g_screen_buffer1, &console_mode);
    console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(g_screen_buffer1, console_mode);

    GetConsoleMode(g_screen_buffer2, &console_mode);
    console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(g_screen_buffer2, console_mode);
  }

  SetConsoleActiveScreenBuffer(g_screen_buffer1);

  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
}

/******************************************************************************
 * @brief ライブラリの終了処理
 *
 ******************************************************************************/
void Finalize() {
  CloseHandle(g_screen_buffer1);
  CloseHandle(g_screen_buffer2);
}

void Print(int x, int y, const char *format, ...) {
  va_list args;
  va_start(args, format);
  sprint("%s[%d;%dH", ESCAPE, y + 1, x + 1);
  print(format, args);
  va_end(args);
}

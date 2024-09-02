#include "GameConsole.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vadefs.h>
#include <windows.h>

///=============================================================================
/// 内部変数
///=============================================================================

struct Buffer {
  HANDLE Front; // 表のスクリーンバッファ
  HANDLE Back;  // 裏のスクリーンバッファ
} Screen;

const char *ESCAPE = "\033"; // エスケープシーケンス

///=============================================================================
/// 内部関数
///=============================================================================

/// @brief 入力バッファのモードを初期化
void initInputMode() {
  DWORD mode;
  const HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(handle, &mode);

  // 入力された文字を非表示にする
  mode &= ~ENABLE_ECHO_INPUT;
  // Ctrl+Cなどのシグナルを無効にする
  mode &= ~ENABLE_PROCESSED_INPUT;
  // コンソール上でのマウス操作を無効にする
  mode &= ~ENABLE_QUICK_EDIT_MODE;

  SetConsoleMode(handle, mode);
}

/// @brief 出力バッファのモードを初期化
void initOutputMode() {
  DWORD mode1, mode2;
  GetConsoleMode(Screen.Front, &mode1);
  GetConsoleMode(Screen.Back, &mode2);

  // 仮想コンソールモードを有効にする
  mode1 |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  mode2 |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  SetConsoleMode(Screen.Front, mode1);
  SetConsoleMode(Screen.Back, mode2);
}

void vprintBuffer(HANDLE handle, const char *format, va_list args) {
  char buffer[256];

  vsprintf_s(buffer, (size_t)(sizeof(buffer)), format, args);

  DWORD length = strlen(buffer);
  WriteConsoleA(handle, buffer, length, &length, NULL);
}

void printBuffer(HANDLE handle, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintBuffer(handle, format, args);
  va_end(args);
}

void clearBuffer(HANDLE handle) {
  printBuffer(handle, "%s[%d;%dH", ESCAPE, 1, 1);
  printBuffer(handle, "%s[2J", ESCAPE);
}

///=============================================================================
/// 外部関数
///=============================================================================

void Initialize() {
  Screen.Front = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
  Screen.Back = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

  initInputMode();
  initOutputMode();

  Flip();

  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
}

void Finalize() {
  CloseHandle(Screen.Front);
  CloseHandle(Screen.Back);
}

void Flip() {
  SetConsoleActiveScreenBuffer(Screen.Back);

  // 古いスクリーンバッファをクリア
  clearBuffer(Screen.Front);

  HANDLE tmp = Screen.Front;
  Screen.Front = Screen.Back;
  Screen.Back = tmp;
}

void Print(int x, int y, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printBuffer(Screen.Back, "%s[%d;%dH", ESCAPE, y + 1, x + 1);
  vprintBuffer(Screen.Back, format, args);
  va_end(args);
}

void Clear() { clearBuffer(Screen.Back); }

#include "GameConsole.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vadefs.h>
#include <windows.h>

///=============================================================================
/// 内部変数
///=============================================================================

int Index = 0;               // スクリーン番号
HANDLE Screen[2];            // スクリーンバッファ
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
  DWORD mode;
  for (int i = 0; i < 2; i++) {
    GetConsoleMode(Screen[i], &mode);

    // 仮想コンソールモードを有効にする
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(Screen[i], mode);
  }
}

void vprintBuffer(const char *format, va_list args) {
  char buffer[256];

  vsprintf_s(buffer, (size_t)(sizeof(buffer)), format, args);

  DWORD length = strlen(buffer);
  WriteConsoleA(Screen[Index], buffer, length, &length, NULL);
}

void printBuffer(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintBuffer(format, args);
  va_end(args);
}

///=============================================================================
/// 外部関数
///=============================================================================

void Initialize() {
  Screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
  Screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

  initInputMode();
  initOutputMode();

  Flip();

  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
}

void Finalize() {
  CloseHandle(Screen[0]);
  CloseHandle(Screen[1]);
}

void Flip() {
  SetConsoleActiveScreenBuffer(Screen[Index]);

  // 1,0を順に繰り返す
  Index = (Index + 1) % 2;

  // 新しいスクリーンバッファをクリア
  Clear();
}

void Print(int x, int y, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printBuffer("%s[%d;%dH", ESCAPE, y + 1, x + 1);
  vprintBuffer(format, args);
  va_end(args);
}

void Clear() {
  // 現在のスクリーンバッファをクリア
  printBuffer("%s[%d;%dH", ESCAPE, 1, 1);
  printBuffer("%s[2J", ESCAPE);
}

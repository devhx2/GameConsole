#include "GameConsole.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vadefs.h>
#include <windows.h>

///=============================================================================
/// 内部変数
///=============================================================================

/// @brief ダブルバッファ
struct DoubleBuffer {
  HANDLE Front; // 表のスクリーンバッファ
  HANDLE Back;  // 裏のスクリーンバッファ
} Screen;

const char *ESCAPE = "\x1b"; // エスケープシーケンス

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
  GetConsoleMode(Screen.Front, &mode);

  // 仮想コンソールモードを有効にする
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  SetConsoleMode(Screen.Front, mode);
  SetConsoleMode(Screen.Back, mode);
}

/// @brief スクリーンバッファに出力
/// @param handle スクリーンバッファのハンドル
/// @param format 書式文字列
/// @param args 引数リスト
void vprintBuffer(HANDLE handle, const char *format, va_list args) {
  char buffer[256];

  vsprintf_s(buffer, (size_t)(sizeof(buffer)), format, args);

  DWORD length = strlen(buffer);
  WriteConsoleA(handle, buffer, length, &length, NULL);
}

/// @brief スクリーンバッファに書き込み
/// @param handle スクリーンバッファのハンドル
/// @param format 書式文字列
/// @param ... 可変長引数
void printBuffer(HANDLE handle, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintBuffer(handle, format, args);
  va_end(args);
}

/// @brief カーソルを移動
/// @param handle スクリーンバッファのハンドル
/// @param x x座標
/// @param y y座標
void moveCursor(HANDLE handle, int x, int y) {
  // 引数は原点が(0,0)の座標系だが,ANSIエスケープシーケンスの原点が(1,1)の座標系
  // ANSIの仕様に合わせるために各座標に+1
  printBuffer(handle, "%s[%d;%dH", ESCAPE, y + 1, x + 1);
}

/// @brief スクリーンバッファをクリア
/// @param handle スクリーンバッファのハンドル
void clearBuffer(HANDLE handle) {
  moveCursor(handle, 0, 0);
  printBuffer(handle, "%s[0m", ESCAPE);
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
  HANDLE tmp = Screen.Front;
  Screen.Front = Screen.Back;
  Screen.Back = tmp;

  SetConsoleActiveScreenBuffer(Screen.Front);
  clearBuffer(Screen.Back);
}

void Print(int x, int y, const char *format, ...) {
  va_list args;
  va_start(args, format);
  moveCursor(Screen.Back, x, y);
  vprintBuffer(Screen.Back, format, args);
  va_end(args);
}

void SetFgColor(int r, int g, int b) { printBuffer(Screen.Back, "%s[38;2;%d;%d;%dm", ESCAPE, r, g, b); }

void SetBgColor(int r, int g, int b) { printBuffer(Screen.Back, "%s[48;2;%d;%d;%dm", ESCAPE, r, g, b); }

void Clear() { clearBuffer(Screen.Back); }

#include "GameConsole.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vadefs.h>
#include <windows.h>

HANDLE Screen[2];            /* スクリーンバッファ */
const char *ESCAPE = "\033"; /* エスケープシーケンス */

/******************************************************************************
 * @brief 入力バッファのモードを初期化
 *
 ******************************************************************************/
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

/******************************************************************************
 * @brief 出力バッファのモードを初期化
 *
 ******************************************************************************/
void initOutputMode() {
  DWORD mode;
  const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleMode(handle, &mode);

  // 仮想コンソールモードを有効にする
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  SetConsoleMode(handle, mode);
}

/******************************************************************************
 * @brief ライブラリの初期化
 *
 ******************************************************************************/
void Initialize() {
  Screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
  Screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

  initInputMode();
  initOutputMode();

  Flip();

  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
}

/******************************************************************************
 * @brief ライブラリの終了
 *
 ******************************************************************************/
void Finalize() {
  CloseHandle(Screen[0]);
  CloseHandle(Screen[1]);
}

void Flip() {
  static int flag = 0;
  SetStdHandle(STD_OUTPUT_HANDLE, Screen[flag]);

  // 1,0を順に繰り返す
  flag = (flag + 1) % 2;
}

void Print(int x, int y, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf_s("%s[%d;%dH", ESCAPE, y + 1, x + 1);
  vprintf_s(format, args);
  va_end(args);
}

void Clear() { printf_s("%s[2J", ESCAPE); }

#ifndef GAME_CONSOLE
#define GAME_CONSOLE

/// @brief ライブラリを初期化
void Initialize();

/// @brief ライブラリを終了
void Finalize();

/// @brief スクリーンバッファを入れ替え
void Flip();

/// @brief @brief スクリーンバッファに書き込み
/// @param x x座標
/// @param y y座標
/// @param format 書式文字列
/// @param ... 可変長引数
void Print(int x, int y, const char *format, ...);

/// @brief 前景色を設定
/// @param r 赤要素
/// @param g 緑要素
/// @param b 青要素
void SetFgColor(int r, int g, int b);

/// @brief 後景色を設定
/// @param r 赤要素
/// @param g 緑要素
/// @param b 青要素
void SetBgColor(int r, int g, int b);

/// @brief スクリーンバッファをクリア
void Clear();

#endif

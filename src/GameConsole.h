#ifndef GAME_CONSOLE
#define GAME_CONSOLE

void Initialize();
void Finalize();
void Flip();
void Print(int x, int y, const char *format, ...);
void Clear();

#endif

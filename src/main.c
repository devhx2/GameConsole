#include "GameConsole.h"
#include <windows.h>

int main() {
  Initialize();

  const char *str = "🎲🗝️⛏️🧱⚙️🔗💣";
  for (int i = 0; i < 100; i++) {
    Clear();
    Print(i, 1, "%s", str);
    Flip();
    Sleep(16);
  }

  Finalize();

  return 0;
}

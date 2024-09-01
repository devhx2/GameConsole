#include "GameConsole.h"
#include <windows.h>

int main() {
  Initialize();

  const char *str = "🎲🗝️⛏️🧱⚙️🔗💣";
  for (int i = 0; i < 100; i++) {
    Print(i, 1, "%s", str);
    Sleep(33);
    Flip();
  }

  Finalize();

  return 0;
}

#include "GameConsole.h"
#include <windows.h>

int main() {
  Initialize();

  const char *str = "🎲🗝️⛏️🧱⚙️🔗💣";
  Print("\033[2J\033[%d;%dH%s", 1, 1, str);
  Sleep(1000);

  Finalize();

  return 0;
}

#include "GameConsole.h"
#include <windows.h>

int main() {
  Initialize();

  const char *str = "🎲🗝️⛏️🧱⚙️🔗💣";
  Print(2, 1, "%s", str);
  Sleep(1000);

  Finalize();

  return 0;
}

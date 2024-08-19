#include <stdio.h>

#include "GameConsole.h"

int main()
{
    Initialize();

    const char *str = "🎲🗝️⛏️🧱⚙️🔗💣";
    printf_s(str);

    getchar();
    return 0;
}

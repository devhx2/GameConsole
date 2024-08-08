#include <stdio.h>
#include <Windows.h>

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    const char *str = "🎲🗝️⛏️🧱⚙️🔗💣";
    printf_s(str);
    getchar();
    return 0;
}

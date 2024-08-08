@echo off

@REM 相対パスで移動
cd %~dp0\..\src\

@REM 対象ファイルをコンパイル
cl /c /utf-8 GameConsole.c
if %ERRORLEVEL% neq 0 (
    echo "error cl /c /utf-8 GameConsole.c"
    exit /b
)

@REM ライブラリを作成
lib /out:GameConsole.lib GameConsole.obj
if %ERRORLEVEL% neq 0 (
    echo "error lib /out:GameConsole.lib GameConsole.obj"
    exit /b
)

@REM ライブラリを移動
move *.lib ..\out\ > nul

@REM 中間ファイルを削除
del *.obj

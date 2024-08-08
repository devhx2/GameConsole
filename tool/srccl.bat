@echo off

@REM 相対パスで移動
cd %~dp0/../src

@REM 対象ファイルをコンパイル
for %%s in (%*) do (
    cl /c /utf-8 %%~nxs
    if %ERRORLEVEL% neq 0 (
        echo "error cl /c /utf-8 %%~nxs"
        exit /b
    )
)

@REM 対象ファイルをリンク
link /out:%~n1.exe *.obj ../out/GameConsole.lib
if %ERRORLEVEL% neq 0 (
    echo "error link /out:%~n1.exe *.obj ../out/GameConsole.lib"
    exit /b
)

@REM 実行ファイルを移動
move *.exe ../out/ > nul

@REM 中間ファイルを削除
del *.obj

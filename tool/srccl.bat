@echo off

@REM ���΃p�X�ňړ�
cd %~dp0/../src

@REM �Ώۃt�@�C�����R���p�C��
for %%s in (%*) do (
    cl /c /utf-8 %%~nxs
    if %ERRORLEVEL% neq 0 (
        echo "error cl /c /utf-8 %%~nxs"
        exit /b
    )
)

@REM �Ώۃt�@�C���������N
link /out:%~n1.exe *.obj ../out/GameConsole.lib
if %ERRORLEVEL% neq 0 (
    echo "error link /out:%~n1.exe *.obj ../out/GameConsole.lib"
    exit /b
)

@REM ���s�t�@�C�����ړ�
move *.exe ../out/ > nul

@REM ���ԃt�@�C�����폜
del *.obj

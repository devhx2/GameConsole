@echo off

@REM ���΃p�X�ňړ�
cd %~dp0\..\src\

@REM �Ώۃt�@�C�����R���p�C��
cl /c /utf-8 GameConsole.c
if %ERRORLEVEL% neq 0 (
    echo "error cl /c /utf-8 GameConsole.c"
    exit /b
)

@REM ���C�u�������쐬
lib /out:GameConsole.lib GameConsole.obj
if %ERRORLEVEL% neq 0 (
    echo "error lib /out:GameConsole.lib GameConsole.obj"
    exit /b
)

@REM ���C�u�������ړ�
move *.lib ..\out\ > nul

@REM ���ԃt�@�C�����폜
del *.obj

@echo off
cd /d "%~dp0"

:CheckUAC
cacls.exe "%SystemDrive%\System Volume Information" >nul 2>nul
set retVal=%errorlevel%
if %retVal%==0 goto Admin
if not %retVal%==0 goto WarningNoUAC

:GetUAC
echo Try to get the Administrator Control
if exist "%temp%\getadmin.vbs" del /f /q "%temp%\getadmin.vbs"
echo Set RequestUAC = CreateObject^("Shell.Application"^)>"%temp%\getadmin.vbs"
echo RequestUAC.ShellExecute "%~s0","","","runas",1 >>"%temp%\getadmin.vbs"
echo WScript.Quit >>"%temp%\getadmin.vbs"
"%temp%\getadmin.vbs" /f

:Admin
echo Start install MySql 9.0.11 Win64
if not exist %CD%data (mkdir data)
cd bin
mysqld -install
if %retVal%==0 exit

:CleanTempFile
if exist "%temp%\getadmin.vbs" del /f /q "%temp%\getadmin.vbs"


:WarningNoUAC
echo Permission Denied
echo Please run in Administrator Mode
exit
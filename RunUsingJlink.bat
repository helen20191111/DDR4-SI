
@echo off

Set JLINK_PATH="C:\Program Files (x86)\SEGGER\JLink_V632e";
if exist %JLINK_PATH% goto NewJlink 

Set JLINK_PATH="C:\Program Files (x86)\SEGGER\JLink_V614d"; 
if exist %JLINK_PATH% goto NewJlink 

Set JLINK_PATH="C:\Program Files (x86)\SEGGER\JLink_V614a"; 
if exist %JLINK_PATH% goto NewJlink 

Set JLINK_PATH="C:\Program Files (x86)\SEGGER\JLink_V512g"; 
if exist %JLINK_PATH% goto NewJlink 

Set JLINK_PATH="C:\Program Files (x86)\SEGGER\JLinkARM_V492"; 
if exist %JLINK_PATH% goto OldJlink

Set JLINK_PATH="C:\Program Files (x86)\SEGGER\JLinkARM_V486a";
if exist %JLINK_PATH% goto OldJlink

Set JLINK_PATH="C:\Program Files (x86)\SEGGER\JLinkARM_V480g";
if exist %JLINK_PATH% goto OldJlink


echo. & echo ERROR: Fail to find SEGGER JLink path. Pls update 'RunUsingJlink.bat' file. & echo. & echo.
pause
exit


:OldJlink
set PATH=%JLINK_PATH%
echo. & echo Old JLink Ver %JLINK_PATH% & echo.
Jlink.exe    JlinkLoader.scr -autoconnect 1 -If JTAG  -Device Cortex-A9 -Speed 1000 
rem pause
goto EOF


:NewJlink
set PATH=%JLINK_PATH%
echo. & echo New JLink Ver %JLINK_PATH% & echo.
Jlink.exe   JlinkLoader.scr -autoconnect 1 -If JTAG  -Device Cortex-A9 -Speed 1000 -JTAGConf -1,-1  
rem pause
goto EOF





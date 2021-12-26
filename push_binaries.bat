@echo off
set P4CLIENT=%P4USER%_BUILD

rem Reconcile
p4 reconcile -ea //%P4CLIENT%/Binaries/...
p4 sync -k //%P4CLIENT%/Binaries/...
p4 resolve -ay //%P4CLIENT%/Binaries/...
p4 submit -d "Binary build"

rem In case of errors, revert everything that stuck around...
p4 revert -k //%P4CLIENT%/Binaries/...

echo.
echo Done! :)
pause
SET CMAKE="..\..\cmake-2.8.11.2-win32-x86\bin\cmake.exe"
SET SOL=unit-test.sln
SET PROJ=unit-test
SET CONF=Release
SET EXE=Release\unit-test.exe

rem cleanup from previous runs
rd /s /q build-borland build-vc8 build-vc9 build-vc10 build-vc11 build-vc12 build-vc12-c++11

setlocal
  mkdir build-borland
  cd build-borland
  bcc32 -O2 ..\..\cpp-copy-elision-NRVO-URVO.cpp
  cpp-copy-elision-NRVO-URVO.exe 2> bcc32.txt
  bcc64 -O3 ..\..\cpp-copy-elision-NRVO-URVO.cpp -DALLOW_MOVE -o test-64.exe
  test-64.exe 2> bcc64.txt
  cd ..
endlocal

rem VC8 needs an administrator cmd shell
setlocal
  mkdir build-vc8
  cd build-vc8
  call "%VS80COMNTOOLS%"\vsvars32.bat
  %CMAKE% -G "Visual Studio 8 2005" ..
  devenv %SOL% /Build %CONF% /Project %PROJ%
  %EXE% > nul 2>output.log
  cd ..
endlocal
 
setlocal
  mkdir build-vc9
  cd build-vc9
  call "%VS90COMNTOOLS%"\vsvars32.bat
  %CMAKE% -G "Visual Studio 9 2008" ..
  devenv %SOL% /Build %CONF% /Project %PROJ%
  %EXE% > nul 2>output.log
  cd ..
endlocal
 
setlocal
  mkdir build-vc10
  cd build-vc10
  call "%VS100COMNTOOLS%"\vsvars32.bat
  %CMAKE% -G "Visual Studio 10" ..
  devenv %SOL% /Build %CONF% /Project %PROJ%
  %EXE% > nul 2>output.log
  cd ..
endlocal

rem VC11 needs a fresh cmd shell otherwise cmake fails
setlocal
  mkdir build-vc11
  cd build-vc11
  call "%VS110COMNTOOLS%"\vsvars32.bat
  %CMAKE% -G "Visual Studio 11" ..
  msbuild /maxcpucount /p:Configuration=%CONF% /p:VisualStudioVersion=11.0 %PROJ%.vcxproj
  %EXE% > nul 2>output.log
  cd ..
endlocal
 
setlocal
  mkdir build-vc12
  cd build-vc12
  call "%VS120COMNTOOLS%"\vsvars32.bat
  %CMAKE% -G "Visual Studio 12" -DALLOW_MOVE=ON ..
  msbuild /maxcpucount /p:Configuration=%CONF% /p:VisualStudioVersion=12.0 %PROJ%.vcxproj
  %EXE% > nul 2>output.log
  cd ..
endlocal

rem show results
type build-vc8\output.log build-vc9\output.log build-vc10\output.log build-vc11\output.log build-vc12\output.log

dm857c\bin\dmc.exe -Ae ..\cpp-copy-elision-NRVO-URVO.cpp && cpp-copy-elision-NRVO-URVO.exe 2> digital-mars-8.57.txt 

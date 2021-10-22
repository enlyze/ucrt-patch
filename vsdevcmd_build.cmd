:: Builds the fixed thread.cpp and replaces it in libucrt.lib
:: Oh, it would have been so great if Microsoft had not removed the CRT makefiles from Visual Studio...
::
:: Written by Colin Finck for ENLYZE GmbH <c.finck@enlyze.com>
::

@echo off

:: All inputs are specified explicitly.
set INCLUDE=
set LIB=

:: Build thread.obj
cl /c /I glue /I "%VSINSTALLDIR%\VC\Tools\MSVC\14.29.30133\include" /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt" /I "C:\Program Files (x86)\Windows Kits\10\include\10.0.20348.0\shared" /I "C:\Program Files (x86)\Windows Kits\10\include\10.0.20348.0\um" /I "C:\Program Files (x86)\Windows Kits\10\include\10.0.20348.0\winrt" ucrt\startup\thread.cpp

:: Patch libucrt.lib
copy "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.20348.0\ucrt\x86\libucrt.lib"
lib /out:libucrt-removed.lib /remove:d:\os\obj\x86fre\minkernel\crts\ucrt\src\appcrt\dll\mt\..\..\startup\mt\objfre\i386\thread.obj libucrt.lib
lib /out:libucrt-patched.lib libucrt-removed.lib thread.obj

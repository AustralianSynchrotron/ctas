@ECHO OFF


setlocal

set PWD=%~dp0

PATH %PWD%\BIN;%PWD%\DLL;%path%

SET description=Interface to all tools in the '${PACKAGE}' package.
SET usagemsg=CTAS [TOOL] [TOOL SPECIFIC ... ]

IF !%1==! GOTO USAGE

IF "%1%"=="l" GOTO LIST
IF "%1%"=="list" GOTO LIST
IF "%1%"=="-l" GOTO LIST
IF "%1%"=="--list" GOTO LIST

IF "%1%"=="u" GOTO USAGE
IF "%1%"=="usage" GOTO USAGE
IF "%1%"=="-u" GOTO USAGE
IF "%1%"=="--usage" GOTO USAGE

IF "%1%"=="h" GOTO HELP
IF "%1%"=="help" GOTO HELP
IF "%1%"=="-h" GOTO HELP
IF "%1%"=="--help" GOTO HELP

SHIFT
CALL %*
GOTO EXIT0


:USAGE
ECHO %description%
ECHO Use '-h ^| --help' option for more information.
GOTO EXIT0

:HELP
ECHO Interface to all tools in the 'ctas' package.
ECHO Usage: ctas ^<tool^> [TOOL SPECIFIC ... ]
ECHO TOOLS:
ECHO     ct         CT reconstruction of one sinogram.
ECHO     ct-abs     All-in-one absorption-based CT reconstruction.
ECHO     ct-dei     All-in-one refraction-based CT reconstruction.
ECHO     ct-edei    All-in-one EDEI-based CT reconstruction.
ECHO     ct-ipc     All-in-one IPC-based CT reconstruction.
ECHO     ct-line    CT reconstruction of one sinogram picked from the series of images.
ECHO     dei        DEI contrast processing.
ECHO     edei       EDEI contrast processing.
ECHO     f2i        Converts float-point image to the integer one.
ECHO     ff         Flat field correction.
ECHO     ipc        IPC contrast processing
ECHO     rc2fd      Converts the rocking curve of the analyzer to the function used in the EDEI.
ECHO     sino       Prepares sinogram from the stack of files.
ECHO     sino-abs   Forms absorption-based sinogram(s).
ECHO     sino-dei   Forms DEI-based sinogram(s).
ECHO     sino-edei  Forms EDEI-based sinogram(s).
ECHO     sino-ipc   Forms IPC-based sinogram(s).
ECHO     ts         Tomosynthesis reconstruction.
ECHO     list       Lists all tools.
ECHO     help       Prints this help message.
ECHO     usage      Outputs brief usage message.
ECHO TOOL SPECIFIC:
ECHO     Use command's help or usage for specific information:
ECHO     ctas ^<command^> -h ^| --help ^| -? ^| --usage
GOTO EXIT0

:LIST
FOR /F %%C in ('DIR /B /O %PWD%\BIN') do (
	ECHO %%~nC
)
GOTO EXIT0




:EXIT0
endlocal
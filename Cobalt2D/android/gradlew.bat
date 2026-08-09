@if "%DEBUG%"==\"\" @echo off
@rem ##########################################################################
@rem
@rem  Gradle startup script for Windows
@rem
@rem ##########################################################################

@rem Set local scope for the variables with windows NT shell
if "%OS%"==\"Windows_NT\" setlocal

set DIRNAME=%~dp0
if \"%DIRNAME:~-1%\"==\"\" set DIRNAME=%DIRNAME:~0,-1%
set APP_BASE_NAME=%~n0
set APP_HOME=%DIRNAME%

@rem Resolve any \"...\" program files not to interpret \"...\" as a path separator
for %%i in (\"%APP_HOME%\") do set APP_HOME=%%~fi

@rem Add default JVM options here. You can also use JAVA_OPTS and GRADLE_OPTS to pass JVM options to this script.
set DEFAULT_JVM_OPTS=-Xmx64m -Xms64m

@rem Find java.exe
if defined JAVA_HOME goto findJavaFromJavaHome

set JAVA_EXE=java.exe
%JAVA_EXE% -version >nul 2>&1
if %ERRORLEVEL% equ 0 goto execute

echo.
echo Error: JAVA_HOME is not set and no 'java' command could be found in your PATH.
echo.
echo Please set the JAVA_HOME variable in your environment to match the
echo location of your Java installation.

goto fail

:findJavaFromJavaHome
set JAVA_HOME=%JAVA_HOME:\=/%
set JAVA_EXE=%JAVA_HOME%/bin/java.exe

if exist \"%JAVA_EXE%\" goto execute

echo.
echo Error: JAVA_HOME is set to an invalid directory: %JAVA_HOME%
echo.
echo Please set the JAVA_HOME variable in your environment to match the
echo location of your Java installation.

goto fail

:execute
@rem Setup the command line

set CLASSPATH=%APP_HOME%\gradle\wrapper\gradle-wrapper.jar

@rem Execute Gradle
if \"%1\"==\"\" (
    \"%JAVA_EXE%\" %DEFAULT_JVM_OPTS% %JAVA_OPTS% %GRADLE_OPTS% -classpath \"%CLASSPATH%\" org.gradle.wrapper.GradleWrapperMain
) else (
    \"%JAVA_EXE%\" %DEFAULT_JVM_OPTS% %JAVA_OPTS% %GRADLE_OPTS% -classpath \"%CLASSPATH%\" org.gradle.wrapper.GradleWrapperMain %*
)

:fail
exit /b 1

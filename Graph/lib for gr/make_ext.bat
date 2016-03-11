@if not "%1" == ""  goto START
@ECHO    /****************************************************************/
@ECHO    /**                      make_ext                              **/
@ECHO    /**                                                            **/
@ECHO    /** make_ext  creates the Dislin extension disjava.dll for     **/
@ECHO    /**           for java and Windows 64-bit.                     **/
@ECHO    /**                                                            **/
@ECHO    /** Command:  make_ext java                                    **/
@ECHO    /**                                                            **/
@ECHO    /** The Dislin library disvc_d.lib for Visual C++ 64-bit is    **/
@ECHO    /** required.                                                  **/
@ECHO    /****************************************************************/
@goto ENDE
:START
@if %1 == java goto M0
@ECHO Bad Parameter
@goto ENDE
:M0
cl /c /Ic:\opt64\jdk17\include /Ic:\opt64\jdk17\include\win32 disjava.c
link -dll -out:disjava.dll disjava.obj ..\disvc_d.lib user32.lib gdi32.lib
:ENDE

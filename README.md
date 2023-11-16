# VTK - The Visualization Toolkit

## build on Windows use mingw64 Qt

```bash
> mkdir build & cd build
> cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0
>
> cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0 -DQT_PATH=D:\devtools\Qt\Qt5.12.12\5.12.12\mingw73_64 -DModule_vtkGROUPQT:BOOL=TRUE -DModule_vtkGUISUPPORTQT:BOOL=TRUE -DModule_vtkGUISUPPORTQTOPENGL:BOOL=TRUE -DModule_vtkGUISUPPORTQTSQL:BOOL=TRUE 
-DModule_vtkGUISUPPORTQTWEBKIT:BOOL=TRUE -DModule_vtkRENDERINGQT:BOOL=TRUE -DModule_vtkVIEWSQT:BOOL=TRUE 
> mingw32-make.exe -j12
```
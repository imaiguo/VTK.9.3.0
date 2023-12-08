# VTK - The Visualization Toolkit

可以选择依赖Qt 5/6

```bash
> -DVTK_GROUP_ENABLE_Qt=YES
> -DVTK_QT_VERSION=5
> -DQt5_DIR=D:\devtools\Qt\Qt5.12.12\5.12.12\mingw73_64
```


## build on Windows 

```bash
>
> mkdir build & cd build
> cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0 -DVTK_GROUP_ENABLE_Qt=YES
> cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0 -DVTK_GROUP_ENABLE_Qt=YES
> mingw32-make.exe -j12
>
```

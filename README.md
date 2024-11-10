
# VTK - The Visualization Toolkit

可以选择依赖Qt5或者Qt6

```bash
> -DVTK_GROUP_ENABLE_Qt=YES
> -DVTK_QT_VERSION=5
> -DQt5_DIR=D:\devtools\Qt\Qt5.12.12\5.12.12\mingw73_64
> -DQt5_DIR=D:\devtools\Qt\qteverywhere.5.15.2
```

## build on MSVC 

### 编译Release

```bash
> git checkout msvc
> cmd
> "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
> cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0 -DVTK_GROUP_ENABLE_Qt=YES -DVTK_QT_VERSION=5 -DQt5_DIR=D:\devtools\Qt\qteverywhere.5.15.2\lib\cmake\Qt5
> cd build & ninja & ninja install
>
```

### 编译Debug
```bash
> cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0 -DVTK_GROUP_ENABLE_Qt=YES -DVTK_QT_VERSION=5 -DQt5_DIR=D:\devtools\Qt\qteverywhere.5.15.2\lib\cmake\Qt5
>
```

## build on mingw 

```bash
>
> mkdir build & cd build
> cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0 -DVTK_GROUP_ENABLE_Qt=YES
> cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=D:\devtools\VTK.9.3.0 -DVTK_GROUP_ENABLE_Qt=YES
> mingw32-make.exe -j12
>
```

## 运行环境设置

```bash
> set Path=D:\devtools\VTK.9.3.0\bin;%Path%;
```

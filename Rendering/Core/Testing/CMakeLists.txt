vtk_module_test_data(
  Data/GreenCircle.png
  Data/SainteHelens.dem
  Data/SampleStructGrid.vtk
  Data/Tango/TangoIcons.png
  Data/alphachannel.png
  Data/beach.jpg
  Data/can.ex2
  Data/chombo3d/,REGEX:.*
  Data/combq.bin
  Data/combxyz.bin
  Data/earth.ppm
  Data/mb_with_pieces/,REGEX:.*\.vtu
  Data/mb_with_pieces.vtm
  Data/quadraticTetra01.vtu
  Data/textureRGBA.png)

if (NOT vtk_testing_cxx_disabled)
  add_subdirectory(Cxx)
endif ()

if (VTK_WRAP_PYTHON)
  vtk_module_test_data(
    Data/Viewpoint/cow.g
    Data/Viewpoint/cow.obj
    Data/beach.tif
    Data/blow.vtk
    Data/headsq/,REGEX:.*
    Data/masonry.bmp)

  add_subdirectory(Python)
endif ()

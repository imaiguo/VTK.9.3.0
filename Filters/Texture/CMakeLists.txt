set(classes
  vtkImplicitTextureCoords
  vtkScalarsToTextureFilter
  vtkTextureMapToCylinder
  vtkTextureMapToPlane
  vtkTextureMapToSphere
  vtkThresholdTextureCoords
  vtkTransformTextureCoords
  vtkTriangularTCoords)

vtk_module_add_module(VTK::FiltersTexture
  CLASSES ${classes})
vtk_add_test_mangling(VTK::FiltersTexture)

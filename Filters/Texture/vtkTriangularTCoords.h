// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkTriangularTCoords
 * @brief   2D texture coordinates based for triangles.
 *
 * vtkTriangularTCoords is a filter that generates texture coordinates
 * for triangles. Texture coordinates for each triangle are:
 * (0,0), (1,0) and (.5,sqrt(3)/2). This filter assumes that the triangle
 * texture map is symmetric about the center of the triangle. Thus the order
 * Of the texture coordinates is not important. The procedural texture
 * in vtkTriangularTexture is designed with this symmetry. For more information
 * see the paper "Opacity-modulating Triangular Textures for Irregular
 * Surfaces,"  by Penny Rheingans, IEEE Visualization '96, pp. 219-225.
 * @sa
 * vtkTriangularTexture vtkThresholdPoints vtkTextureMapToPlane
 * vtkTextureMapToSphere vtkTextureMapToCylinder
 */

#ifndef vtkTriangularTCoords_h
#define vtkTriangularTCoords_h

#include "vtkFiltersTextureModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

VTK_ABI_NAMESPACE_BEGIN
class VTKFILTERSTEXTURE_EXPORT vtkTriangularTCoords : public vtkPolyDataAlgorithm
{
public:
  static vtkTriangularTCoords* New();
  vtkTypeMacro(vtkTriangularTCoords, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkTriangularTCoords() = default;
  ~vtkTriangularTCoords() override = default;

  // Usual data generation method
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkTriangularTCoords(const vtkTriangularTCoords&) = delete;
  void operator=(const vtkTriangularTCoords&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

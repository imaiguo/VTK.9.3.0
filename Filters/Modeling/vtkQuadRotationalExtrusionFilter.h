// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkQuadRotationalExtrusionFilter
 * @brief   sweep polygonal data creating "skirt" from free edges and lines, and lines from vertices
 *
 * vtkQuadRotationalExtrusionFilter is a modeling filter. It takes polygonal
 * data as input and generates polygonal data on output. The input dataset
 * is swept around the z-axis to create new polygonal primitives. These
 * primitives form a "skirt" or swept surface. For example, sweeping a
 * line results in a cylindrical shell, and sweeping a circle creates a
 * torus.
 *
 * There are a number of control parameters for this filter. You can
 * control whether the sweep of a 2D object (i.e., polygon or triangle
 * strip) is capped with the generating geometry via the "Capping" instance
 * variable. Also, you can control the angle of rotation, and whether
 * translation along the z-axis is performed along with the rotation.
 * (Translation is useful for creating "springs".) You also can adjust
 * the radius of the generating geometry using the "DeltaRotation" instance
 * variable.
 *
 * The skirt is generated by locating certain topological features. Free
 * edges (edges of polygons or triangle strips only used by one polygon or
 * triangle strips) generate surfaces. This is true also of lines or
 * polylines. Vertices generate lines.
 *
 * This filter can be used to model axisymmetric objects like cylinders,
 * bottles, and wine glasses; or translational/rotational symmetric objects
 * like springs or corkscrews.
 *
 * @warning
 * If the object sweeps 360 degrees, radius does not vary, and the object
 * does not translate, capping is not performed. This is because the cap
 * is unnecessary.
 *
 * @warning
 * Some polygonal objects have no free edges (e.g., sphere). When swept,
 * this will result in two separate surfaces if capping is on, or no surface
 * if capping is off.
 *
 * @sa
 * vtkLinearExtrusionFilter vtkRotationalExtrusionFilter
 *
 * @par Thanks:
 * This class was initially developed by Daniel Aguilera, CEA/DIF
 * Ported and modified by Philippe Pebay, Kitware, 2011
 */

#ifndef vtkQuadRotationalExtrusionFilter_h
#define vtkQuadRotationalExtrusionFilter_h

#include "vtkFiltersModelingModule.h" // For export macro
#include "vtkMultiBlockDataSetAlgorithm.h"
#include <map> // STL vector need for per block angles

VTK_ABI_NAMESPACE_BEGIN
class vtkPoints;
class vtkPointData;

class VTKFILTERSMODELING_EXPORT vtkQuadRotationalExtrusionFilter
  : public vtkMultiBlockDataSetAlgorithm
{
public:
  vtkTypeMacro(vtkQuadRotationalExtrusionFilter, vtkMultiBlockDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Create object with capping on, angle of 360 degrees, resolution = 12, and
   * no translation along z-axis.
   * vector (0,0,1), and point (0,0,0).
   */
  static vtkQuadRotationalExtrusionFilter* New();

  enum RotationAxis
  {
    USE_X = 0,
    USE_Y = 1,
    USE_Z = 2
  };

  ///@{
  /**
   * Set the axis of rotation to use. It is set by default to Z.
   */
  vtkSetClampMacro(Axis, int, 0, 2);
  vtkGetMacro(Axis, int);
  void SetAxisToX() { this->SetAxis(USE_X); }
  void SetAxisToY() { this->SetAxis(USE_Y); }
  void SetAxisToZ() { this->SetAxis(USE_Z); }
  ///@}

  ///@{
  /**
   * Set/Get resolution of sweep operation. Resolution controls the number
   * of intermediate node points.
   */
  vtkSetClampMacro(Resolution, int, 1, VTK_INT_MAX);
  vtkGetMacro(Resolution, int);
  ///@}

  ///@{
  /**
   * Turn on/off the capping of the skirt.
   */
  vtkSetMacro(Capping, vtkTypeBool);
  vtkGetMacro(Capping, vtkTypeBool);
  vtkBooleanMacro(Capping, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/Get angle of rotation.
   */
  vtkSetMacro(DefaultAngle, double);
  vtkGetMacro(DefaultAngle, double);
  ///@}

  ///@{
  /**
   * Set/Get angles of rotation for each block in a composite data set.
   */
  void RemoveAllPerBlockAngles();
  void AddPerBlockAngle(vtkIdType blockId, double angle);
  ///@}

  ///@{
  /**
   * Set/Get total amount of translation along the z-axis.
   */
  vtkSetMacro(Translation, double);
  vtkGetMacro(Translation, double);
  ///@}

  ///@{
  /**
   * Set/Get change in radius during sweep process.
   */
  vtkSetMacro(DeltaRadius, double);
  vtkGetMacro(DeltaRadius, double);
  ///@}

protected:
  vtkQuadRotationalExtrusionFilter();
  ~vtkQuadRotationalExtrusionFilter() override = default;

  int FillInputPortInformation(int, vtkInformation*) override;
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  int RotateAroundAxis(double, vtkIdType, vtkPoints*, vtkPoints*, vtkPointData*, vtkPointData*);
  int Axis;
  int Resolution;
  vtkTypeBool Capping;
  double DefaultAngle;
  double Translation;
  double DeltaRadius;

  std::map<vtkIdType, double> PerBlockAngles;

private:
  vtkQuadRotationalExtrusionFilter(const vtkQuadRotationalExtrusionFilter&) = delete;
  void operator=(const vtkQuadRotationalExtrusionFilter&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

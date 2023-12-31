// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkBSplineTransform
 * @brief   a cubic b-spline deformation transformation
 *
 * vtkBSplineTransform computes a cubic b-spline transformation from a
 * grid of b-spline coefficients.
 * @warning
 * The inverse grid transform is calculated using an iterative method,
 * and is several times more expensive than the forward transform.
 * @sa
 * vtkGeneralTransform vtkTransformToGrid vtkImageBSplineCoefficients
 * @par Thanks:
 * This class was written by David Gobbi at the Seaman Family MR Research
 * Centre, Foothills Medical Centre, Calgary, Alberta.
 * DG Gobbi and YP Starreveld,
 * "Uniform B-Splines for the VTK Imaging Pipeline,"
 * VTK Journal, 2011,
 * http://hdl.handle.net/10380/3252
 */

#ifndef vtkBSplineTransform_h
#define vtkBSplineTransform_h

#include "vtkFiltersHybridModule.h" // For export macro
#include "vtkWarpTransform.h"

VTK_ABI_NAMESPACE_BEGIN
class vtkAlgorithmOutput;
class vtkBSplineTransformConnectionHolder;
class vtkImageData;

#define VTK_BSPLINE_EDGE 0
#define VTK_BSPLINE_ZERO 1
#define VTK_BSPLINE_ZERO_AT_BORDER 2

class VTKFILTERSHYBRID_EXPORT vtkBSplineTransform : public vtkWarpTransform
{
public:
  static vtkBSplineTransform* New();
  vtkTypeMacro(vtkBSplineTransform, vtkWarpTransform);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Set/Get the coefficient grid for the b-spline transform.
   * The vtkBSplineTransform class will never modify the data.
   * Note that SetCoefficientData() does not setup a pipeline
   * connection whereas SetCoefficientConnection does.
   */
  virtual void SetCoefficientConnection(vtkAlgorithmOutput*);
  virtual void SetCoefficientData(vtkImageData*);
  virtual vtkImageData* GetCoefficientData();
  ///@}

  ///@{
  /**
   * Set/Get a scale to apply to the transformation.
   */
  vtkSetMacro(DisplacementScale, double);
  vtkGetMacro(DisplacementScale, double);
  ///@}

  ///@{
  /**
   * Set/Get the border mode, to alter behavior at the edge of the grid.
   * The Edge mode allows the displacement to converge to the edge
   * coefficient past the boundary, which is similar to the behavior of
   * the vtkGridTransform. The Zero mode allows the displacement to
   * smoothly converge to zero two node-spacings past the boundary,
   * which is useful when you want to create a localized transform.
   * The ZeroAtBorder mode sacrifices smoothness to further localize
   * the transform to just one node-spacing past the boundary.
   */
  vtkSetClampMacro(BorderMode, int, VTK_BSPLINE_EDGE, VTK_BSPLINE_ZERO_AT_BORDER);
  void SetBorderModeToEdge() { this->SetBorderMode(VTK_BSPLINE_EDGE); }
  void SetBorderModeToZero() { this->SetBorderMode(VTK_BSPLINE_ZERO); }
  void SetBorderModeToZeroAtBorder() { this->SetBorderMode(VTK_BSPLINE_ZERO_AT_BORDER); }
  vtkGetMacro(BorderMode, int);
  const char* GetBorderModeAsString();
  ///@}

  /**
   * Make another transform of the same type.
   */
  vtkAbstractTransform* MakeTransform() override;

  /**
   * Get the MTime.
   */
  vtkMTimeType GetMTime() override;

protected:
  vtkBSplineTransform();
  ~vtkBSplineTransform() override;

  /**
   * Update the displacement grid.
   */
  void InternalUpdate() override;

  /**
   * Copy this transform from another of the same type.
   */
  void InternalDeepCopy(vtkAbstractTransform* transform) override;

  ///@{
  /**
   * Internal functions for calculating the transformation.
   */
  void ForwardTransformPoint(const float in[3], float out[3]) override;
  void ForwardTransformPoint(const double in[3], double out[3]) override;
  ///@}

  void ForwardTransformDerivative(const float in[3], float out[3], float derivative[3][3]) override;
  void ForwardTransformDerivative(
    const double in[3], double out[3], double derivative[3][3]) override;

  void InverseTransformPoint(const float in[3], float out[3]) override;
  void InverseTransformPoint(const double in[3], double out[3]) override;

  void InverseTransformDerivative(const float in[3], float out[3], float derivative[3][3]) override;
  void InverseTransformDerivative(
    const double in[3], double out[3], double derivative[3][3]) override;

  void (*CalculateSpline)(const double point[3], double displacement[3], double derivatives[3][3],
    void* gridPtr, int inExt[6], vtkIdType inInc[3], int borderMode);

  double DisplacementScale;
  int BorderMode;

  void* GridPointer;
  double GridSpacing[3];
  double GridOrigin[3];
  int GridExtent[6];
  vtkIdType GridIncrements[3];

private:
  vtkBSplineTransform(const vtkBSplineTransform&) = delete;
  void operator=(const vtkBSplineTransform&) = delete;

  vtkBSplineTransformConnectionHolder* ConnectionHolder;
};

VTK_ABI_NAMESPACE_END
#endif

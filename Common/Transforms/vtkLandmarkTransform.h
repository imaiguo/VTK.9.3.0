// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkLandmarkTransform
 * @brief   a linear transform specified by two corresponding point sets
 *
 * A vtkLandmarkTransform is defined by two sets of landmarks, the
 * transform computed gives the best fit mapping one onto the other, in a
 * least squares sense. The indices are taken to correspond, so point 1
 * in the first set will get mapped close to point 1 in the second set,
 * etc. Call SetSourceLandmarks and SetTargetLandmarks to specify the two
 * sets of landmarks, ensure they have the same number of points.
 * @warning
 * Whenever you add, subtract, or set points you must call Modified()
 * on the vtkPoints object, or the transformation might not update.
 * @sa
 * vtkLinearTransform
 */

#ifndef vtkLandmarkTransform_h
#define vtkLandmarkTransform_h

#include "vtkCommonTransformsModule.h" // For export macro
#include "vtkLinearTransform.h"

#define VTK_LANDMARK_RIGIDBODY 6
#define VTK_LANDMARK_SIMILARITY 7
#define VTK_LANDMARK_AFFINE 12

VTK_ABI_NAMESPACE_BEGIN
class VTKCOMMONTRANSFORMS_EXPORT vtkLandmarkTransform : public vtkLinearTransform
{
public:
  static vtkLandmarkTransform* New();

  vtkTypeMacro(vtkLandmarkTransform, vtkLinearTransform);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Specify the source and target landmark sets. The two sets must have
   * the same number of points.  If you add or change points in these objects,
   * you must call Modified() on them or the transformation might not update.
   */
  void SetSourceLandmarks(vtkPoints* source);
  void SetTargetLandmarks(vtkPoints* target);
  vtkGetObjectMacro(SourceLandmarks, vtkPoints);
  vtkGetObjectMacro(TargetLandmarks, vtkPoints);
  ///@}

  ///@{
  /**
   * Set the number of degrees of freedom to constrain the solution to.
   * Rigidbody (VTK_LANDMARK_RIGIDBODY): rotation and translation only.
   * Similarity (VTK_LANDMARK_SIMILARITY): rotation, translation and
   * isotropic scaling.
   * Affine (VTK_LANDMARK_AFFINE): collinearity is preserved.
   * Ratios of distances along a line are preserved.
   * The default is similarity.
   */
  vtkSetMacro(Mode, int);
  void SetModeToRigidBody() { this->SetMode(VTK_LANDMARK_RIGIDBODY); }
  void SetModeToSimilarity() { this->SetMode(VTK_LANDMARK_SIMILARITY); }
  void SetModeToAffine() { this->SetMode(VTK_LANDMARK_AFFINE); }
  ///@}

  ///@{
  /**
   * Get the current transformation mode.
   */
  vtkGetMacro(Mode, int);
  const char* GetModeAsString();
  ///@}

  /**
   * Invert the transformation.  This is done by switching the
   * source and target landmarks.
   */
  void Inverse() override;

  /**
   * Get the MTime.
   */
  vtkMTimeType GetMTime() override;

  /**
   * Make another transform of the same type.
   */
  vtkAbstractTransform* MakeTransform() override;

protected:
  vtkLandmarkTransform();
  ~vtkLandmarkTransform() override;

  // Update the matrix from the quaternion.
  void InternalUpdate() override;

  /**
   * This method does no type checking, use DeepCopy instead.
   */
  void InternalDeepCopy(vtkAbstractTransform* transform) override;

  vtkPoints* SourceLandmarks;
  vtkPoints* TargetLandmarks;

  int Mode;

private:
  vtkLandmarkTransform(const vtkLandmarkTransform&) = delete;
  void operator=(const vtkLandmarkTransform&) = delete;
};

inline const char* vtkLandmarkTransform::GetModeAsString()
{
  switch (this->Mode)
  {
    case VTK_LANDMARK_RIGIDBODY:
      return "RigidBody";
    case VTK_LANDMARK_SIMILARITY:
      return "Similarity";
    case VTK_LANDMARK_AFFINE:
      return "Affine";
    default:
      return "Unrecognized";
  }
}

VTK_ABI_NAMESPACE_END
#endif

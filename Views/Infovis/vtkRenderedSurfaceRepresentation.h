// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
/**
 * @class   vtkRenderedSurfaceRepresentation
 * @brief   Displays a geometric dataset as a surface.
 *
 *
 * vtkRenderedSurfaceRepresentation is used to show a geometric dataset in a view.
 * The representation uses a vtkGeometryFilter to convert the dataset to
 * polygonal data (e.g. volumetric data is converted to its external surface).
 * The representation may then be added to vtkRenderView.
 */

#ifndef vtkRenderedSurfaceRepresentation_h
#define vtkRenderedSurfaceRepresentation_h

#include "vtkRenderedRepresentation.h"
#include "vtkViewsInfovisModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class vtkActor;
class vtkAlgorithmOutput;
class vtkApplyColors;
class vtkDataObject;
class vtkGeometryFilter;
class vtkPolyDataMapper;
class vtkRenderView;
class vtkScalarsToColors;
class vtkSelection;
class vtkTransformFilter;
class vtkView;

class VTKVIEWSINFOVIS_EXPORT vtkRenderedSurfaceRepresentation : public vtkRenderedRepresentation
{
public:
  static vtkRenderedSurfaceRepresentation* New();
  vtkTypeMacro(vtkRenderedSurfaceRepresentation, vtkRenderedRepresentation);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Sets the color array name
   */
  virtual void SetCellColorArrayName(const char* arrayName);
  virtual const char* GetCellColorArrayName() { return this->GetCellColorArrayNameInternal(); }

  /**
   * Apply a theme to this representation.
   */
  void ApplyViewTheme(vtkViewTheme* theme) override;

protected:
  vtkRenderedSurfaceRepresentation();
  ~vtkRenderedSurfaceRepresentation() override;

  /**
   * Sets the input pipeline connection to this representation.
   */
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  /**
   * Performs per-render operations.
   */
  void PrepareForRendering(vtkRenderView* view) override;

  /**
   * Adds the representation to the view.  This is called from
   * vtkView::AddRepresentation().
   */
  bool AddToView(vtkView* view) override;

  /**
   * Removes the representation to the view.  This is called from
   * vtkView::RemoveRepresentation().
   */
  bool RemoveFromView(vtkView* view) override;

  /**
   * Convert the selection to a type appropriate for sharing with other
   * representations through vtkAnnotationLink.
   * If the selection cannot be applied to this representation, returns nullptr.
   */
  vtkSelection* ConvertSelection(vtkView* view, vtkSelection* selection) override;

  ///@{
  /**
   * Internal pipeline objects.
   */
  vtkTransformFilter* TransformFilter;
  vtkApplyColors* ApplyColors;
  vtkGeometryFilter* GeometryFilter;
  vtkPolyDataMapper* Mapper;
  vtkActor* Actor;
  ///@}

  vtkGetStringMacro(CellColorArrayNameInternal);
  vtkSetStringMacro(CellColorArrayNameInternal);
  char* CellColorArrayNameInternal;

private:
  vtkRenderedSurfaceRepresentation(const vtkRenderedSurfaceRepresentation&) = delete;
  void operator=(const vtkRenderedSurfaceRepresentation&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

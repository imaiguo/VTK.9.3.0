// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkApproximatingSubdivisionFilter
 * @brief   generate a subdivision surface using an Approximating Scheme
 *
 * vtkApproximatingSubdivisionFilter is an abstract class that defines
 * the protocol for Approximating subdivision surface filters.
 *
 * @par Thanks:
 * This work was supported by PHS Research Grant No. 1 P41 RR13218-01
 * from the National Center for Research Resources.
 */

#ifndef vtkApproximatingSubdivisionFilter_h
#define vtkApproximatingSubdivisionFilter_h

#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkSubdivisionFilter.h"

VTK_ABI_NAMESPACE_BEGIN
class vtkCellArray;
class vtkCellData;
class vtkIdList;
class vtkIntArray;
class vtkPoints;
class vtkPointData;

class VTKFILTERSGENERAL_EXPORT vtkApproximatingSubdivisionFilter : public vtkSubdivisionFilter
{
public:
  vtkTypeMacro(vtkApproximatingSubdivisionFilter, vtkSubdivisionFilter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkApproximatingSubdivisionFilter();
  ~vtkApproximatingSubdivisionFilter() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  virtual int GenerateSubdivisionPoints(
    vtkPolyData* inputDS, vtkIntArray* edgeData, vtkPoints* outputPts, vtkPointData* outputPD) = 0;
  void GenerateSubdivisionCells(
    vtkPolyData* inputDS, vtkIntArray* edgeData, vtkCellArray* outputPolys, vtkCellData* outputCD);
  int FindEdge(vtkPolyData* mesh, vtkIdType cellId, vtkIdType p1, vtkIdType p2,
    vtkIntArray* edgeData, vtkIdList* cellIds);
  vtkIdType InterpolatePosition(
    vtkPoints* inputPts, vtkPoints* outputPts, vtkIdList* stencil, double* weights);

private:
  vtkApproximatingSubdivisionFilter(const vtkApproximatingSubdivisionFilter&) = delete;
  void operator=(const vtkApproximatingSubdivisionFilter&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

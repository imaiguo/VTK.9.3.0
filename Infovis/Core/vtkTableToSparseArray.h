// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

/**
 * @class   vtkTableToSparseArray
 * @brief   converts a vtkTable into a sparse array.
 *
 *
 * Converts a vtkTable into a sparse array.  Use AddCoordinateColumn() to
 * designate one-to-many table columns that contain coordinates for each
 * array value, and SetValueColumn() to designate the table column that
 * contains array values.
 *
 * Thus, the number of dimensions in the output array will equal the number
 * of calls to AddCoordinateColumn().
 *
 * The coordinate columns will also be used to populate dimension labels
 * in the output array.
 *
 * By default, the extent of the output array will be set to the range
 * [0, largest coordinate + 1) along each dimension.  In some situations
 * you may prefer to set the extents explicitly, using the
 * SetOutputExtents() method.  This is useful when the output array should
 * be larger than its largest coordinates, or when working with partitioned
 * data.
 *
 * @par Thanks:
 * Developed by Timothy M. Shead (tshead@sandia.gov) at Sandia National Laboratories.
 */

#ifndef vtkTableToSparseArray_h
#define vtkTableToSparseArray_h

#include "vtkArrayDataAlgorithm.h"
#include "vtkInfovisCoreModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class vtkArrayExtents;

class VTKINFOVISCORE_EXPORT vtkTableToSparseArray : public vtkArrayDataAlgorithm
{
public:
  static vtkTableToSparseArray* New();
  vtkTypeMacro(vtkTableToSparseArray, vtkArrayDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Specify the set of input table columns that will be mapped to coordinates
   * in the output sparse array.
   */
  void ClearCoordinateColumns();
  void AddCoordinateColumn(const char* name);
  ///@}

  ///@{
  /**
   * Specify the input table column that will be mapped to values in the output array.
   */
  void SetValueColumn(const char* name);
  const char* GetValueColumn();
  ///@}

  ///@{
  /**
   * Explicitly specify the extents of the output array.
   */
  void ClearOutputExtents();
  void SetOutputExtents(const vtkArrayExtents& extents);
  ///@}

protected:
  vtkTableToSparseArray();
  ~vtkTableToSparseArray() override;

  int FillInputPortInformation(int, vtkInformation*) override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkTableToSparseArray(const vtkTableToSparseArray&) = delete;
  void operator=(const vtkTableToSparseArray&) = delete;

  class implementation;
  implementation* const Implementation;
};

VTK_ABI_NAMESPACE_END
#endif

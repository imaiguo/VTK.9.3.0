// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkBridgePointIteratorOnDataSet
 * @brief   Implementation of vtkGenericPointIterator.
 *
 * It iterates over the points of a dataset (can be corner points of cells or
 * isolated points)
 * @sa
 * vtkGenericPointIterator, vtkBridgeDataSet
 */

#ifndef vtkBridgePointIteratorOnDataSet_h
#define vtkBridgePointIteratorOnDataSet_h

#include "vtkBridgeExport.h" //for module export macro
#include "vtkGenericPointIterator.h"

VTK_ABI_NAMESPACE_BEGIN
class vtkBridgeDataSet;

class VTKTESTINGGENERICBRIDGE_EXPORT vtkBridgePointIteratorOnDataSet
  : public vtkGenericPointIterator
{
public:
  static vtkBridgePointIteratorOnDataSet* New();
  vtkTypeMacro(vtkBridgePointIteratorOnDataSet, vtkGenericPointIterator);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Move iterator to first position if any (loop initialization).
   */
  void Begin() override;

  /**
   * Is there no point at iterator position? (exit condition).
   */
  vtkTypeBool IsAtEnd() override;

  /**
   * Move iterator to next position. (loop progression).
   * \pre not_off: !IsAtEnd()
   */
  void Next() override;

  /**
   * Point at iterator position.
   * \pre not_off: !IsAtEnd()
   * \post result_exists: result!=0
   */
  double* GetPosition() override;

  /**
   * Point at iterator position.
   * \pre not_off: !IsAtEnd()
   * \pre x_exists: x!=0
   */
  void GetPosition(double x[3]) override;

  /**
   * Unique identifier for the point, could be non-contiguous
   * \pre not_off: !IsAtEnd()
   */
  vtkIdType GetId() override;

  /**
   * Used internally by vtkBridgeDataSet.
   * Iterate over points of `ds'.
   * \pre ds_exists: ds!=0
   */
  void InitWithDataSet(vtkBridgeDataSet* ds);

protected:
  /**
   * Default constructor.
   */
  vtkBridgePointIteratorOnDataSet();

  /**
   * Destructor.
   */
  ~vtkBridgePointIteratorOnDataSet() override;

  vtkBridgeDataSet* DataSet; // the structure on which the object iterates.
  vtkIdType Id;              // the id at current position.
  int Size;                  // size of the structure.

private:
  vtkBridgePointIteratorOnDataSet(const vtkBridgePointIteratorOnDataSet&) = delete;
  void operator=(const vtkBridgePointIteratorOnDataSet&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

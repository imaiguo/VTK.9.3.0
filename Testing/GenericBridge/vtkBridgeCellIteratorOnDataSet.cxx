// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// .NAME vtkBridgeCellIteratorOnDataSet - Iterate over cells of a dataset.
// .SECTION See Also
// vtkBridgeCellIterator, vtkBridgeDataSet, vtkBridgeCellIteratorStrategy

#include "vtkBridgeCellIteratorOnDataSet.h"

#include <cassert>

#include "vtkBridgeCell.h"
#include "vtkBridgeDataSet.h"
#include "vtkCell.h"
#include "vtkDataSet.h"
#include "vtkObjectFactory.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkBridgeCellIteratorOnDataSet);

//------------------------------------------------------------------------------
void vtkBridgeCellIteratorOnDataSet::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
vtkBridgeCellIteratorOnDataSet::vtkBridgeCellIteratorOnDataSet()
{
  this->DataSet = nullptr;
  this->Cell = vtkBridgeCell::New();
  this->Id = 0;
  this->Size = 0;
  //  this->DebugOn();
}

//------------------------------------------------------------------------------
vtkBridgeCellIteratorOnDataSet::~vtkBridgeCellIteratorOnDataSet()
{
  if (this->DataSet != nullptr)
  {
    this->DataSet->Delete();
    this->DataSet = nullptr;
  }
  this->Cell->Delete();
  this->Cell = nullptr;
}

//------------------------------------------------------------------------------
// Description:
// Move iterator to first position if any (loop initialization).
void vtkBridgeCellIteratorOnDataSet::Begin()
{
  this->Id = -1;
  this->Next(); // skip cells of other dimensions
}

//------------------------------------------------------------------------------
// Description:
// Is there no cell at iterator position? (exit condition).
vtkTypeBool vtkBridgeCellIteratorOnDataSet::IsAtEnd()
{
  return (this->Id >= this->Size);
}

//------------------------------------------------------------------------------
// Description:
// Cell at current position
// \pre not_at_end: !IsAtEnd()
// \pre c_exists: c!=0
// THREAD SAFE
void vtkBridgeCellIteratorOnDataSet::GetCell(vtkGenericAdaptorCell* c)
{
  assert("pre: not_at_end" && !IsAtEnd());
  assert("pre: c_exists" && c != nullptr);

  vtkBridgeCell* c2 = static_cast<vtkBridgeCell*>(c);
  c2->Init(this->DataSet, this->Id);
}

//------------------------------------------------------------------------------
// Description:
// Cell at current position.
// NOT THREAD SAFE
// \pre not_at_end: !IsAtEnd()
// \post result_exits: result!=0
vtkGenericAdaptorCell* vtkBridgeCellIteratorOnDataSet::GetCell()
{
  assert("pre: not_at_end" && !IsAtEnd());

  this->Cell->Init(this->DataSet, this->Id);
  vtkGenericAdaptorCell* result = this->Cell;

  assert("post: result_exits" && result != nullptr);
  return result;
}

//------------------------------------------------------------------------------
// Description:
// Move iterator to next position. (loop progression).
// \pre not_at_end: !IsAtEnd()
void vtkBridgeCellIteratorOnDataSet::Next()
{
  assert("pre: not_off" && !IsAtEnd());

  vtkIdType size = this->Size;
  vtkCell* c;
  int found;

  this->Id++;

  if (this->Dim >= 0) // skip cells of other dimensions than this->Dim
  {
    found = 0;
    while ((this->Id < size) && (!found))
    {
      c = this->DataSet->Implementation->GetCell(this->Id);
      found = c->GetCellDimension() == this->Dim;
      this->Id++;
    }
    if (found)
    {
      this->Id--;
    }
  }
}

//------------------------------------------------------------------------------
// Description:
// Used internally by vtkBridgeDataSet.
// Iterate over cells of `ds' of some dimension `dim'.
// \pre ds_exists: ds!=0
// \pre valid_dim_range: (dim>=-1) && (dim<=3)
void vtkBridgeCellIteratorOnDataSet::InitWithDataSet(vtkBridgeDataSet* ds, int dim)
{
  assert("pre: ds_exists" && ds != nullptr);
  assert("pre: valid_dim_range" && (dim >= -1) && (dim <= 3));

  this->Dim = dim;
  vtkSetObjectBodyMacro(DataSet, vtkBridgeDataSet, ds);
  this->Size = ds->GetNumberOfCells();
  this->Id = this->Size; // at end
}
VTK_ABI_NAMESPACE_END

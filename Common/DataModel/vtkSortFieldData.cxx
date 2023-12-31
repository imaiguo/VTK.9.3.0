// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkSortFieldData.h"

#include "vtkAbstractArray.h"
#include "vtkFieldData.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"

//------------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkSortFieldData);

//------------------------------------------------------------------------------
vtkSortFieldData::vtkSortFieldData() = default;

//------------------------------------------------------------------------------
vtkSortFieldData::~vtkSortFieldData() = default;

//------------------------------------------------------------------------------
// Using vtkSortDataArray, it's easy to loop over all of the arrays in the
// field data and sort them. Initially we just need to generate the sort
// indices which are then applied to each array in turn.
vtkIdType* vtkSortFieldData::Sort(
  vtkFieldData* fd, const char* arrayName, int k, int retIndices, int dir)
{
  // Verify the input
  if (fd == nullptr || arrayName == nullptr)
  {
    vtkGenericWarningMacro("SortFieldData needs valid input");
    return nullptr;
  }
  int pos;
  vtkAbstractArray* array = fd->GetAbstractArray(arrayName, pos);
  if (pos < 0)
  {
    vtkGenericWarningMacro("Sorting array not found.");
    return nullptr;
  }
  int numComp = array->GetNumberOfComponents();
  if (k < 0 || k >= numComp)
  {
    vtkGenericWarningMacro("Cannot sort by column "
      << k << " since the array only has columns 0 through " << (numComp - 1));
    return nullptr;
  }
  vtkIdType numKeys = array->GetNumberOfTuples();
  if (numKeys <= 0)
  {
    return nullptr;
  }

  // Create and initialize the sorting indices
  vtkIdType* idx = vtkSortDataArray::InitializeSortIndices(numKeys);

  // Sort and generate the sorting indices
  void* dataIn = array->GetVoidPointer(0);
  int dataType = array->GetDataType();
  vtkSortDataArray::GenerateSortIndices(dataType, dataIn, numKeys, numComp, k, idx);

  // Now loop over all arrays in the field data. Those that are the
  // same length as the sorting indices are processed. Otherwise they
  // are skipped and remain unchanged.
  int nc, numArrays = fd->GetNumberOfArrays();
  for (int arrayNum = 0; arrayNum < numArrays; ++arrayNum)
  {
    array = fd->GetAbstractArray(arrayNum);
    if (array != nullptr && array->GetNumberOfTuples() == numKeys)
    { // process the array
      dataIn = array->GetVoidPointer(0);
      dataType = array->GetDataType();
      nc = array->GetNumberOfComponents();
      vtkSortDataArray::ShuffleArray(idx, dataType, numKeys, nc, array, dataIn, dir);
    }
  }

  // Clean up
  if (retIndices)
  {
    return idx;
  }
  else
  {
    delete[] idx;
    return nullptr;
  }
}

//------------------------------------------------------------------------------
void vtkSortFieldData::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

// vtkSortFieldData methods -------------------------------------------------------
VTK_ABI_NAMESPACE_END

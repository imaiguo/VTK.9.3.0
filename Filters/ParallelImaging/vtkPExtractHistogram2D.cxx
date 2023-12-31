// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2009 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
#include "vtkPExtractHistogram2D.h"

#include "vtkDataArray.h"
#include "vtkDataSetAttributes.h"
#include "vtkIdList.h"
#include "vtkIdTypeArray.h"
#include "vtkImageData.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiProcessController.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkTable.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkPExtractHistogram2D);
vtkCxxSetObjectMacro(vtkPExtractHistogram2D, Controller, vtkMultiProcessController);
//------------------------------------------------------------------------------
vtkPExtractHistogram2D::vtkPExtractHistogram2D()
{
  this->Controller = nullptr;
  this->SetController(vtkMultiProcessController::GetGlobalController());
}
//------------------------------------------------------------------------------
vtkPExtractHistogram2D::~vtkPExtractHistogram2D()
{
  this->SetController(nullptr);
}
//------------------------------------------------------------------------------
void vtkPExtractHistogram2D::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Controller: " << this->Controller << endl;
}
//------------------------------------------------------------------------------
void vtkPExtractHistogram2D::Learn(
  vtkTable* inData, vtkTable* inParameters, vtkMultiBlockDataSet* outMeta)
{
  vtkTable* primaryTab = vtkTable::SafeDownCast(outMeta->GetBlock(0));
  if (!primaryTab)
  {
    return;
  }

  vtkImageData* outImage =
    vtkImageData::SafeDownCast(this->GetOutputDataObject(vtkPExtractHistogram2D::HISTOGRAM_IMAGE));

  // have all of the nodes compute their histograms
  this->Superclass::Learn(inData, inParameters, outMeta);

  if (!this->Controller || this->Controller->GetNumberOfProcesses() <= 1)
  {
    // Nothing to do for single process.
    return;
  }

  // Now we need to collect and reduce data from all nodes on the root.
  vtkCommunicator* comm = this->Controller->GetCommunicator();
  if (!comm)
  {
    vtkErrorMacro("vtkCommunicator is needed.");
    return;
  }

  int myid = this->Controller->GetLocalProcessId();

  vtkImageData* reducedOutImage = vtkImageData::New();
  reducedOutImage->DeepCopy(outImage);

  vtkDataArray* myArray = outImage->GetPointData()->GetScalars();
  vtkDataArray* recvArray = reducedOutImage->GetPointData()->GetScalars();

  // this sums up all of the images and distributes them to
  // every node
  if (!comm->AllReduce(myArray, recvArray, vtkCommunicator::SUM_OP))
  {
    vtkErrorMacro(<< myid << ": Reduce failed!");
    reducedOutImage->Delete();
    return;
  }

  outImage->DeepCopy(reducedOutImage);

  // update the maximum bin count
  for (int i = 0; i < recvArray->GetNumberOfTuples(); i++)
  {
    if (this->MaximumBinCount < recvArray->GetTuple1(i))
      this->MaximumBinCount = (long unsigned)recvArray->GetTuple1(i);
  }

  reducedOutImage->Delete();

  primaryTab->Initialize();
  primaryTab->AddColumn(outImage->GetPointData()->GetScalars());
}

int vtkPExtractHistogram2D::ComputeBinExtents(
  vtkDataSetAttributes* rowData, vtkDataArray* col1, vtkDataArray* col2)
{
  if (!this->Controller || this->Controller->GetNumberOfProcesses() <= 1 ||
    this->UseCustomHistogramExtents)
  {
    // Nothing extra to do for single process.
    return this->Superclass::ComputeBinExtents(rowData, col1, col2);
  }

  vtkCommunicator* comm = this->Controller->GetCommunicator();
  if (!comm)
  {
    vtkErrorMacro("vtkCommunicator is needed.");
    return false;
  }

  // have everyone compute their own bin extents
  double myRange[4] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, VTK_DOUBLE_MIN };
  double allRange[4] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, VTK_DOUBLE_MIN };
  if (this->Superclass::ComputeBinExtents(rowData, col1, col2))
  {
    double* r = this->GetHistogramExtents();
    myRange[0] = r[0];
    myRange[1] = r[1];
    myRange[2] = r[2];
    myRange[3] = r[3];
  }

  int myid = this->Controller->GetLocalProcessId();
  double* r = this->GetHistogramExtents();
  if (!comm->AllReduce(myRange, allRange, 1, vtkCommunicator::MIN_OP) ||
    !comm->AllReduce(myRange + 1, allRange + 1, 1, vtkCommunicator::MAX_OP) ||
    !comm->AllReduce(myRange + 2, allRange + 2, 1, vtkCommunicator::MIN_OP) ||
    !comm->AllReduce(myRange + 3, allRange + 3, 1, vtkCommunicator::MAX_OP))
  {
    vtkErrorMacro(<< myid << ": Reduce failed!");
    return 0;
  }

  r[0] = allRange[0];
  r[1] = allRange[1];
  r[2] = allRange[2];
  r[3] = allRange[3];
  return 1;
}
VTK_ABI_NAMESPACE_END

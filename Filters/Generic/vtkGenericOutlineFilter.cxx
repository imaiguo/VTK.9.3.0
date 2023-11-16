// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkGenericOutlineFilter.h"

#include "vtkGenericDataSet.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkOutlineSource.h"
#include "vtkPolyData.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkGenericOutlineFilter);

//------------------------------------------------------------------------------
void vtkGenericOutlineFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "OutlineSource:\n";
  this->OutlineSource->PrintSelf(os, indent.GetNextIndent());
}

//------------------------------------------------------------------------------
vtkGenericOutlineFilter::vtkGenericOutlineFilter()
{
  this->OutlineSource = vtkOutlineSource::New();
  this->OutlineSource->SetContainerAlgorithm(this);
}

//------------------------------------------------------------------------------
vtkGenericOutlineFilter::~vtkGenericOutlineFilter()
{
  this->OutlineSource->Delete();
}

//------------------------------------------------------------------------------
int vtkGenericOutlineFilter::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkGenericDataSet* input =
    vtkGenericDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  //  vtkPolyData *output = this->GetOutput();

  vtkDebugMacro(<< "Creating dataset outline");

  //
  // Let OutlineSource do all the work
  //

  this->OutlineSource->SetBounds(input->GetBounds());
  this->OutlineSource->Update();

  output->CopyStructure(this->OutlineSource->GetOutput());
  this->CheckAbort();
  return 1;
}

//------------------------------------------------------------------------------
int vtkGenericOutlineFilter::RequestInformation(
  vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // get the info objects
  //  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  //  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  vtkDebugMacro(<< "Creating dataset outline");

  //
  // Let OutlineSource do all the work
  //

  int result = this->Superclass::RequestInformation(request, inputVector, outputVector);

  this->OutlineSource->UpdateInformation();

  return result;
}

//------------------------------------------------------------------------------
int vtkGenericOutlineFilter::FillInputPortInformation(int port, vtkInformation* info)
{
  if (!this->Superclass::FillInputPortInformation(port, info))
  {
    return 0;
  }
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkGenericDataSet");
  return 1;
}
VTK_ABI_NAMESPACE_END

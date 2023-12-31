// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include "vtkAreaLayout.h"

#include "vtkAdjacentVertexIterator.h"
#include "vtkAreaLayoutStrategy.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkSmartPointer.h"
#include "vtkTree.h"
#include "vtkTreeDFSIterator.h"
#include "vtkTreeFieldAggregator.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkAreaLayout);
vtkCxxSetObjectMacro(vtkAreaLayout, LayoutStrategy, vtkAreaLayoutStrategy);

vtkAreaLayout::vtkAreaLayout()
{
  this->AreaArrayName = nullptr;
  this->LayoutStrategy = nullptr;
  this->SetAreaArrayName("area");
  this->EdgeRoutingPoints = true;
  this->SetSizeArrayName("size");
  this->SetNumberOfOutputPorts(2);
}

vtkAreaLayout::~vtkAreaLayout()
{
  this->SetAreaArrayName(nullptr);
  if (this->LayoutStrategy)
  {
    this->LayoutStrategy->Delete();
  }
}

int vtkAreaLayout::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  if (this->LayoutStrategy == nullptr)
  {
    vtkErrorMacro(<< "Layout strategy must be non-null.");
    return 0;
  }
  if (this->AreaArrayName == nullptr)
  {
    vtkErrorMacro(<< "Sector array name must be non-null.");
    return 0;
  }
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkInformation* outEdgeRoutingInfo = outputVector->GetInformationObject(1);

  // Storing the inputTree and outputTree handles
  vtkTree* inputTree = vtkTree::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkTree* outputTree = vtkTree::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkTree* outputEdgeRoutingTree =
    vtkTree::SafeDownCast(outEdgeRoutingInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Copy the input into the output
  outputTree->ShallowCopy(inputTree);
  outputEdgeRoutingTree->ShallowCopy(inputTree);

  // Add the 4-tuple array that will store the min,max xy coords
  vtkFloatArray* coordsArray = vtkFloatArray::New();
  coordsArray->SetName(this->AreaArrayName);
  coordsArray->SetNumberOfComponents(4);
  coordsArray->SetNumberOfTuples(outputTree->GetNumberOfVertices());
  vtkDataSetAttributes* data = outputTree->GetVertexData();
  data->AddArray(coordsArray);
  coordsArray->Delete();

  if (!this->EdgeRoutingPoints)
  {
    outputEdgeRoutingTree = nullptr;
  }

  vtkSmartPointer<vtkDataArray> sizeArray = this->GetInputArrayToProcess(0, inputTree);
  if (!sizeArray)
  {
    vtkSmartPointer<vtkTreeFieldAggregator> agg = vtkSmartPointer<vtkTreeFieldAggregator>::New();
    vtkSmartPointer<vtkTree> t = vtkSmartPointer<vtkTree>::New();
    t->ShallowCopy(outputTree);
    agg->SetInputData(t);
    agg->SetField("size");
    agg->SetLeafVertexUnitSize(true);
    agg->Update();
    sizeArray = agg->GetOutput()->GetVertexData()->GetArray("size");
  }

  // Okay now layout the tree :)
  this->LayoutStrategy->Layout(outputTree, coordsArray, sizeArray);
  this->LayoutStrategy->LayoutEdgePoints(outputTree, coordsArray, sizeArray, outputEdgeRoutingTree);

  return 1;
}

void vtkAreaLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "AreaArrayName: " << (this->AreaArrayName ? this->AreaArrayName : "(none)")
     << endl;
  os << indent << "EdgeRoutingPoints: " << this->EdgeRoutingPoints << endl;
  os << indent << "LayoutStrategy: " << (this->LayoutStrategy ? "" : "(none)") << endl;
  if (this->LayoutStrategy)
  {
    this->LayoutStrategy->PrintSelf(os, indent.GetNextIndent());
  }
}

vtkIdType vtkAreaLayout::FindVertex(float pnt[2])
{
  // Do we have an output?
  vtkTree* otree = this->GetOutput();
  if (!otree)
  {
    vtkErrorMacro(<< "Could not get output tree.");
    return -1;
  }

  // Get the four tuple array for the points
  vtkDataArray* array = otree->GetVertexData()->GetArray(this->AreaArrayName);
  if (!array)
  {
    return -1;
  }

  if (otree->GetNumberOfVertices() == 0)
  {
    return -1;
  }

  return this->LayoutStrategy->FindVertex(otree, array, pnt);
}

void vtkAreaLayout::GetBoundingArea(vtkIdType id, float* sinfo)
{
  // Do we have an output?
  vtkTree* otree = this->GetOutput();
  if (!otree)
  {
    vtkErrorMacro(<< "Could not get output tree.");
    return;
  }

  // Get the four tuple array for the points
  vtkDataArray* array = otree->GetVertexData()->GetArray(this->AreaArrayName);
  if (!array)
  {
    return;
  }

  vtkFloatArray* sectorInfo = vtkArrayDownCast<vtkFloatArray>(array);
  sectorInfo->GetTypedTuple(id, sinfo);
}

vtkMTimeType vtkAreaLayout::GetMTime()
{
  vtkMTimeType mTime = this->Superclass::GetMTime();
  vtkMTimeType time;

  if (this->LayoutStrategy != nullptr)
  {
    time = this->LayoutStrategy->GetMTime();
    mTime = (time > mTime ? time : mTime);
  }
  return mTime;
}
VTK_ABI_NAMESPACE_END

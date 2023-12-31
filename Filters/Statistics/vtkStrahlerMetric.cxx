// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include "vtkStrahlerMetric.h"

#include "vtkDataSetAttributes.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkOutEdgeIterator.h"
#include "vtkSmartPointer.h"

//------------------------------------------------------------------------------

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkStrahlerMetric);

vtkStrahlerMetric::vtkStrahlerMetric()
{
  this->MaxStrahler = 0;
  this->Normalize = 0;
  this->MetricArrayName = nullptr;
  this->SetMetricArrayName("Strahler");
}

vtkStrahlerMetric::~vtkStrahlerMetric()
{
  this->SetMetricArrayName(nullptr);
}

//------------------------------------------------------------------------------

float vtkStrahlerMetric::CalculateStrahler(vtkIdType root, vtkFloatArray* metric, vtkTree* tree)
{
  float strahler, maxStrahler;
  bool same;
  vtkSmartPointer<vtkOutEdgeIterator> children = vtkSmartPointer<vtkOutEdgeIterator>::New();

  vtkIdType nrChildren = tree->GetNumberOfChildren(root);

  std::vector<float> childStrahler(nrChildren);

  // A leaf node has a Strahler value of 1.
  if (nrChildren == 0)
  {
    strahler = 1.0;
  }
  else
  {
    // Non-leaf node: find the Strahler values of the children.
    tree->GetOutEdges(root, children);
    for (vtkIdType i = 0; i < nrChildren; i++)
    {
      childStrahler[i] = this->CalculateStrahler(children->Next().Target, metric, tree);
    }
    // Determine if the children have the same strahler values
    same = true;
    maxStrahler = childStrahler[0];
    for (vtkIdType j = 1; j < nrChildren; j++)
    {
      same = same && (maxStrahler == childStrahler[j]);
      if (maxStrahler < childStrahler[j])
      {
        maxStrahler = childStrahler[j];
      }
    }
    // Calculate the strahler value for this node
    strahler = (same) ? maxStrahler + nrChildren - 1 : maxStrahler + nrChildren - 2;
  }
  // Record the strahler value within the array.
  metric->InsertValue(root, strahler);
  if (strahler > this->MaxStrahler)
  {
    this->MaxStrahler = strahler;
  }
  return strahler;
}

int vtkStrahlerMetric::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  vtkDebugMacro(<< "StrahlerMetric executing.");

  // get the input and output
  vtkTree* input = vtkTree::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkTree* output = vtkTree::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // The output model should share the data of the input.
  output->ShallowCopy(input);

  // Create a new array to hold the metric.
  vtkFloatArray* metric = vtkFloatArray::New();
  metric->SetName(this->MetricArrayName);
  metric->SetNumberOfValues(input->GetNumberOfVertices());

  this->MaxStrahler = 1.0;

  this->CalculateStrahler(input->GetRoot(), metric, input);

  if (this->Normalize)
  {
    for (vtkIdType i = 0; i < input->GetNumberOfVertices(); i++)
    {
      metric->SetValue(i, metric->GetValue(i) / this->MaxStrahler);
    }
  }

  output->GetVertexData()->AddArray(metric);
  metric->Delete();

  vtkDebugMacro(<< "StrahlerMetric done.");
  return 1;
}

void vtkStrahlerMetric::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Normalize: " << this->Normalize << endl;
  os << indent << "MaxStrahler: " << this->MaxStrahler << endl;
  os << indent << "MetricArrayName: " << (this->MetricArrayName ? this->MetricArrayName : "(none)")
     << endl;
}
VTK_ABI_NAMESPACE_END

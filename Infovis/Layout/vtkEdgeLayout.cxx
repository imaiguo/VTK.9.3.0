// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright (c) Sandia Corporation
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkEdgeLayout.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkEdgeLayoutStrategy.h"
#include "vtkEventForwarderCommand.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkEdgeLayout);

//------------------------------------------------------------------------------

vtkEdgeLayout::vtkEdgeLayout()
{
  this->LayoutStrategy = nullptr;
  this->InternalGraph = nullptr;

  this->ObserverTag = 0;
  this->EventForwarder = vtkEventForwarderCommand::New();
  this->EventForwarder->SetTarget(this);
}

//------------------------------------------------------------------------------

vtkEdgeLayout::~vtkEdgeLayout()
{
  if (this->LayoutStrategy)
  {
    this->LayoutStrategy->Delete();
  }
  if (this->InternalGraph)
  {
    this->InternalGraph->Delete();
  }
  this->EventForwarder->Delete();
}

//------------------------------------------------------------------------------

void vtkEdgeLayout::SetLayoutStrategy(vtkEdgeLayoutStrategy* strategy)
{
  // This method is a cut and paste of vtkCxxSetObjectMacro
  // except for the call to SetEdge in the middle :)
  if (strategy != this->LayoutStrategy)
  {
    vtkEdgeLayoutStrategy* tmp = this->LayoutStrategy;
    this->LayoutStrategy = strategy;
    if (this->LayoutStrategy != nullptr)
    {
      this->LayoutStrategy->Register(this);
      this->ObserverTag =
        this->LayoutStrategy->AddObserver(vtkCommand::ProgressEvent, this->EventForwarder);
      if (this->InternalGraph)
      {
        // Set the graph in the layout strategy
        this->LayoutStrategy->SetGraph(this->InternalGraph);
      }
    }
    if (tmp != nullptr)
    {
      tmp->RemoveObserver(this->ObserverTag);
      tmp->UnRegister(this);
    }
    this->Modified();
  }
}

//------------------------------------------------------------------------------

vtkMTimeType vtkEdgeLayout::GetMTime()
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

//------------------------------------------------------------------------------

int vtkEdgeLayout::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  if (this->LayoutStrategy == nullptr)
  {
    vtkErrorMacro(<< "Layout strategy must be non-null.");
    return 0;
  }

  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkGraph* input = vtkGraph::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkGraph* output = vtkGraph::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (this->InternalGraph)
  {
    this->InternalGraph->Delete();
  }

  this->InternalGraph = input->NewInstance();
  // The strategy object is going to modify the Points member so
  // we'll replace that with a deep copy.  For everything else a
  // shallow copy is sufficient.
  this->InternalGraph->ShallowCopy(input);

  // Copy the edge layout points.
  this->InternalGraph->DeepCopyEdgePoints(input);

  // Give the layout strategy a pointer to the input.  We set it to
  // nullptr first to force the layout algorithm to re-initialize
  // itself.  This is necessary in case the input is the same data
  // object with a newer mtime.
  this->LayoutStrategy->SetGraph(nullptr);
  this->LayoutStrategy->SetGraph(this->InternalGraph);

  // No matter whether the input is new or not, the layout strategy
  // needs to do its thing.  It modifies its input
  // (this->InternalGraph) so we can just use that as the output.
  this->LayoutStrategy->Layout();
  output->ShallowCopy(this->InternalGraph);

  return 1;
}

//------------------------------------------------------------------------------

void vtkEdgeLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "LayoutStrategy: " << (this->LayoutStrategy ? "" : "(none)") << endl;
  if (this->LayoutStrategy)
  {
    this->LayoutStrategy->PrintSelf(os, indent.GetNextIndent());
  }
  os << indent << "InternalGraph: " << (this->InternalGraph ? "" : "(none)") << endl;
  if (this->InternalGraph)
  {
    this->InternalGraph->PrintSelf(os, indent.GetNextIndent());
  }
}
VTK_ABI_NAMESPACE_END

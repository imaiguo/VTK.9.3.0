// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkButtonRepresentation.h"
#include "vtkCommand.h"
#include "vtkObjectFactory.h"

//------------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
vtkButtonRepresentation::vtkButtonRepresentation()
{
  this->NumberOfStates = 0;
  this->State = 0;
  this->HighlightState = vtkButtonRepresentation::HighlightNormal;
}

//------------------------------------------------------------------------------
vtkButtonRepresentation::~vtkButtonRepresentation() = default;

//------------------------------------------------------------------------------
// Implement the modulo behavior in this method
void vtkButtonRepresentation::SetState(int state)
{
  if (this->NumberOfStates < 1)
  {
    return;
  }

  int remain = state % this->NumberOfStates;
  if (remain < 0)
  {
    remain += this->NumberOfStates;
  }
  state = remain;

  // Modify if necessary
  if (state != this->State)
  {
    this->State = state;
    this->Modified();
  }
}

//------------------------------------------------------------------------------
void vtkButtonRepresentation::NextState()
{
  this->SetState(this->State + 1);
}

//------------------------------------------------------------------------------
void vtkButtonRepresentation::PreviousState()
{
  this->SetState(this->State - 1);
}

//------------------------------------------------------------------------------
void vtkButtonRepresentation::Highlight(int state)
{
  int newState;
  if (state == vtkButtonRepresentation::HighlightNormal)
  {
    newState = vtkButtonRepresentation::HighlightNormal;
  }
  else if (state == vtkButtonRepresentation::HighlightHovering)
  {
    newState = vtkButtonRepresentation::HighlightHovering;
  }
  else // if ( state == vtkButtonRepresentation::HighlightSelecting )
  {
    newState = vtkButtonRepresentation::HighlightSelecting;
  }

  if (newState != this->HighlightState)
  {
    this->HighlightState = newState;
    this->InvokeEvent(vtkCommand::HighlightEvent, &(this->HighlightState));
    this->Modified();
  }
}

//------------------------------------------------------------------------------
void vtkButtonRepresentation::ShallowCopy(vtkProp* prop)
{
  vtkButtonRepresentation* rep = vtkButtonRepresentation::SafeDownCast(prop);

  if (rep)
  {
    this->NumberOfStates = rep->NumberOfStates;
    this->State = rep->State;
    this->HighlightState = rep->HighlightState;
  }

  this->Superclass::ShallowCopy(prop);
}

//------------------------------------------------------------------------------
void vtkButtonRepresentation::PrintSelf(ostream& os, vtkIndent indent)
{
  // Superclass typedef defined in vtkTypeMacro() found in vtkSetGet.h
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Number Of States: " << this->NumberOfStates << "\n";
  os << indent << "State: " << this->State << "\n";
  os << indent << "Highlight State: " << this->HighlightState << "\n";
}
VTK_ABI_NAMESPACE_END

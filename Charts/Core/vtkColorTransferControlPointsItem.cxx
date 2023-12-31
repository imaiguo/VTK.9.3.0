// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkColorTransferControlPointsItem.h"
#include "vtkBrush.h"
#include "vtkCallbackCommand.h"
#include "vtkColorTransferFunction.h"
#include "vtkContext2D.h"
#include "vtkContextScene.h"
#include "vtkIdTypeArray.h"
#include "vtkObjectFactory.h"
#include "vtkPen.h"
#include "vtkPoints2D.h"

// to handle mouse.GetButton
#include "vtkContextMouseEvent.h"

#include <algorithm>
#include <cassert>
#include <limits>

//------------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkColorTransferControlPointsItem);

//------------------------------------------------------------------------------
vtkColorTransferControlPointsItem::~vtkColorTransferControlPointsItem()
{
  if (this->ColorTransferFunction)
  {
    this->ColorTransferFunction->RemoveObserver(this->Callback);
    this->ColorTransferFunction->Delete();
    this->ColorTransferFunction = nullptr;
  }
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "ColorTransferFunction: ";
  if (this->ColorTransferFunction)
  {
    os << endl;
    this->ColorTransferFunction->PrintSelf(os, indent.GetNextIndent());
  }
  else
  {
    os << "(none)" << endl;
  }
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::emitEvent(unsigned long event, void* params)
{
  if (this->ColorTransferFunction)
  {
    this->ColorTransferFunction->InvokeEvent(event, params);
  }
}

//------------------------------------------------------------------------------
vtkMTimeType vtkColorTransferControlPointsItem::GetControlPointsMTime()
{
  if (this->ColorTransferFunction)
  {
    return this->ColorTransferFunction->GetMTime();
  }
  return this->GetMTime();
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::SetColorTransferFunction(vtkColorTransferFunction* t)
{
  if (t == this->ColorTransferFunction)
  {
    return;
  }
  if (this->ColorTransferFunction)
  {
    this->ColorTransferFunction->RemoveObserver(this->Callback);
  }
  vtkSetObjectBodyMacro(ColorTransferFunction, vtkColorTransferFunction, t);
  if (this->ColorTransferFunction)
  {
    this->ColorTransferFunction->AddObserver(vtkCommand::StartEvent, this->Callback);
    this->ColorTransferFunction->AddObserver(vtkCommand::ModifiedEvent, this->Callback);
    this->ColorTransferFunction->AddObserver(vtkCommand::EndEvent, this->Callback);
  }
  this->ResetBounds();
  this->ComputePoints();
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::DrawPoint(vtkContext2D* painter, vtkIdType index)
{
  assert(index != -1);
  if (this->ColorFill)
  {
    double xrgbms[6];
    this->ColorTransferFunction->GetNodeValue(index, xrgbms);
    painter->GetBrush()->SetColorF(xrgbms[1], xrgbms[2], xrgbms[3], 0.55);
  }
  this->vtkControlPointsItem::DrawPoint(painter, index);
}

//------------------------------------------------------------------------------
vtkIdType vtkColorTransferControlPointsItem::GetNumberOfPoints() const
{
  return this->ColorTransferFunction
    ? static_cast<vtkIdType>(this->ColorTransferFunction->GetSize())
    : 0;
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::GetControlPoint(vtkIdType index, double* pos) const
{
  double xrgbms[6];
  vtkColorTransferFunction* thisTF =
    const_cast<vtkColorTransferFunction*>(this->ColorTransferFunction);

  if (thisTF)
  {
    thisTF->GetNodeValue(index, xrgbms);
    pos[0] = xrgbms[0];
    pos[1] = 0.5;
    pos[2] = xrgbms[4];
    pos[3] = xrgbms[5];
  }
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::SetControlPoint(vtkIdType index, double* newPos)
{
  double xrgbms[6];
  this->ColorTransferFunction->GetNodeValue(index, xrgbms);
  if (newPos[0] != xrgbms[0] || newPos[2] != xrgbms[1] || newPos[3] != xrgbms[2])
  {
    xrgbms[0] = newPos[0];
    xrgbms[4] = newPos[2];
    xrgbms[5] = newPos[3];
    this->StartChanges();
    this->ColorTransferFunction->SetNodeValue(index, xrgbms);
    this->EndChanges();
  }
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::EditPoint(float tX, float tY)
{
  if (!this->ColorTransferFunction)
  {
    return;
  }

  this->StartChanges();

  double xrgbms[6];
  this->ColorTransferFunction->GetNodeValue(this->CurrentPoint, xrgbms);
  xrgbms[4] += tX;
  xrgbms[5] += tY;
  this->ColorTransferFunction->SetNodeValue(this->CurrentPoint, xrgbms);
  if (this->CurrentPoint > 0)
  {
    this->ColorTransferFunction->GetNodeValue(this->CurrentPoint - 1, xrgbms);
    xrgbms[4] += tX;
    xrgbms[5] += tY;
    this->ColorTransferFunction->SetNodeValue(this->CurrentPoint - 1, xrgbms);
  }

  this->EndChanges();
}

//------------------------------------------------------------------------------
vtkIdType vtkColorTransferControlPointsItem::AddPoint(double* newPos)
{
  if (!this->ColorTransferFunction)
  {
    return -1;
  }

  this->StartChanges();

  double posX = newPos[0];
  double rgb[3] = { 0., 0., 0. };
  this->ColorTransferFunction->GetColor(posX, rgb);
  vtkIdType addedPoint = this->ColorTransferFunction->AddRGBPoint(posX, rgb[0], rgb[1], rgb[2]);
  this->vtkControlPointsItem::AddPointId(addedPoint);

  this->EndChanges();
  return addedPoint;
}

//------------------------------------------------------------------------------
vtkIdType vtkColorTransferControlPointsItem::RemovePoint(double* currentPoint)
{
  if (!this->ColorTransferFunction ||
    !this->IsPointRemovable(this->GetControlPointId(currentPoint)))
  {
    return -1;
  }

  this->StartChanges();

#ifndef NDEBUG
  vtkIdType expectedPoint =
#endif
    this->vtkControlPointsItem::RemovePoint(currentPoint);
  vtkIdType removedPoint = this->ColorTransferFunction->RemovePoint(currentPoint[0]);
  assert(removedPoint == expectedPoint);

  this->EndChanges();
  return removedPoint;
}

//------------------------------------------------------------------------------
void vtkColorTransferControlPointsItem::ComputeBounds(double* bounds)
{
  if (this->ColorTransferFunction)
  {
    this->ColorTransferFunction->GetRange(bounds);
    bounds[2] = 0.5;
    bounds[3] = 0.5;

    this->TransformDataToScreen(bounds[0], bounds[2], bounds[0], bounds[2]);
    this->TransformDataToScreen(bounds[1], bounds[3], bounds[1], bounds[3]);
  }
  else
  {
    this->Superclass::ComputeBounds(bounds);
  }
}
VTK_ABI_NAMESPACE_END

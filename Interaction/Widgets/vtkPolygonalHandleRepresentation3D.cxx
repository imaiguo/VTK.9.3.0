// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkPolygonalHandleRepresentation3D.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkCoordinate.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkPointPlacer.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkPolygonalHandleRepresentation3D);

//------------------------------------------------------------------------------
vtkPolygonalHandleRepresentation3D::vtkPolygonalHandleRepresentation3D()
{
  this->Offset[0] = this->Offset[1] = this->Offset[2] = 0.0;

  this->Actor = vtkActor::New();
  this->Actor->SetMapper(this->Mapper);
  this->Actor->SetProperty(this->Property);
  this->HandlePicker->AddPickList(this->Actor);
}

//------------------------------------------------------------------------------
void vtkPolygonalHandleRepresentation3D::SetWorldPosition(double p[3])
{
  if (!this->Renderer || !this->PointPlacer || this->PointPlacer->ValidateWorldPosition(p))
  {
    this->HandleTransformMatrix->SetElement(0, 3, p[0] - this->Offset[0]);
    this->HandleTransformMatrix->SetElement(1, 3, p[1] - this->Offset[1]);
    this->HandleTransformMatrix->SetElement(2, 3, p[2] - this->Offset[2]);

    this->WorldPosition->SetValue(this->HandleTransformMatrix->GetElement(0, 3),
      this->HandleTransformMatrix->GetElement(1, 3), this->HandleTransformMatrix->GetElement(2, 3));

    this->WorldPositionTime.Modified();
  }
}

//------------------------------------------------------------------------------
void vtkPolygonalHandleRepresentation3D::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Offset: (" << this->Offset[0] << "," << this->Offset[1] << ")\n";
}
VTK_ABI_NAMESPACE_END

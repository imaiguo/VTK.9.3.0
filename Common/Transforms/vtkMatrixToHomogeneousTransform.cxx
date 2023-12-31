// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkMatrixToHomogeneousTransform.h"

#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkMatrixToHomogeneousTransform);
vtkCxxSetObjectMacro(vtkMatrixToHomogeneousTransform, Input, vtkMatrix4x4);

//------------------------------------------------------------------------------
vtkMatrixToHomogeneousTransform::vtkMatrixToHomogeneousTransform()
{
  this->Input = nullptr;
  this->InverseFlag = 0;
}

//------------------------------------------------------------------------------
vtkMatrixToHomogeneousTransform::~vtkMatrixToHomogeneousTransform()
{
  this->SetInput(nullptr);
}

//------------------------------------------------------------------------------
void vtkMatrixToHomogeneousTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Update();

  this->Superclass::PrintSelf(os, indent);
  os << indent << "Input: " << this->Input << "\n";
  os << indent << "InverseFlag: " << this->InverseFlag << "\n";
}

//------------------------------------------------------------------------------
void vtkMatrixToHomogeneousTransform::Inverse()
{
  this->InverseFlag = !this->InverseFlag;
  this->Modified();
}

//------------------------------------------------------------------------------
void vtkMatrixToHomogeneousTransform::InternalUpdate()
{
  if (this->Input)
  {
    this->Matrix->DeepCopy(this->Input);
    if (this->InverseFlag)
    {
      this->Matrix->Invert();
    }
  }
  else
  {
    this->Matrix->Identity();
  }
}

//------------------------------------------------------------------------------
void vtkMatrixToHomogeneousTransform::InternalDeepCopy(vtkAbstractTransform* gtrans)
{
  vtkMatrixToHomogeneousTransform* transform =
    static_cast<vtkMatrixToHomogeneousTransform*>(gtrans);

  this->SetInput(transform->Input);

  if (this->InverseFlag != transform->InverseFlag)
  {
    this->Inverse();
  }
}

//------------------------------------------------------------------------------
vtkAbstractTransform* vtkMatrixToHomogeneousTransform::MakeTransform()
{
  return vtkMatrixToHomogeneousTransform::New();
}

//------------------------------------------------------------------------------
// Get the MTime
vtkMTimeType vtkMatrixToHomogeneousTransform::GetMTime()
{
  vtkMTimeType mtime = this->vtkHomogeneousTransform::GetMTime();

  if (this->Input)
  {
    vtkMTimeType matrixMTime = this->Input->GetMTime();
    if (matrixMTime > mtime)
    {
      return matrixMTime;
    }
  }
  return mtime;
}
VTK_ABI_NAMESPACE_END

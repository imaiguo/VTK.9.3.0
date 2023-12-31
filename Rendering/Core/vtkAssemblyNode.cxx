// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkAssemblyNode.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkProp.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkAssemblyNode);

//------------------------------------------------------------------------------
vtkAssemblyNode::vtkAssemblyNode()
{
  this->ViewProp = nullptr;
  this->Matrix = nullptr;
}

//------------------------------------------------------------------------------
vtkAssemblyNode::~vtkAssemblyNode()
{
  if (this->Matrix)
  {
    this->Matrix->Delete();
    this->Matrix = nullptr;
  }
}

//------------------------------------------------------------------------------
// Don't do reference counting
void vtkAssemblyNode::SetViewProp(vtkProp* prop)
{
  this->ViewProp = prop;
}

//------------------------------------------------------------------------------
void vtkAssemblyNode::SetMatrix(vtkMatrix4x4* matrix)
{
  // delete previous
  if (this->Matrix)
  {
    this->Matrix->Delete();
    this->Matrix = nullptr;
  }
  // return if nullptr matrix specified
  if (!matrix)
  {
    return;
  }

  // else create a copy of the matrix
  vtkMatrix4x4* newMatrix = vtkMatrix4x4::New();
  newMatrix->DeepCopy(matrix);
  this->Matrix = newMatrix;
}

//------------------------------------------------------------------------------
vtkMTimeType vtkAssemblyNode::GetMTime()
{
  vtkMTimeType propMTime = 0;
  vtkMTimeType matrixMTime = 0;

  if (this->ViewProp)
  {
    propMTime = this->ViewProp->GetMTime();
  }
  if (this->Matrix)
  {
    matrixMTime = this->Matrix->GetMTime();
  }

  return (propMTime > matrixMTime ? propMTime : matrixMTime);
}

//------------------------------------------------------------------------------
void vtkAssemblyNode::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  if (this->ViewProp)
  {
    os << indent << "ViewProp: " << this->ViewProp << "\n";
  }
  else
  {
    os << indent << "ViewProp: (none)\n";
  }

  if (this->Matrix)
  {
    os << indent << "Matrix: " << this->Matrix << "\n";
  }
  else
  {
    os << indent << "Matrix: (none)\n";
  }
}
VTK_ABI_NAMESPACE_END

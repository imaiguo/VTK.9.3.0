// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkAbstractPropPicker.h"

#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkAssembly.h"
#include "vtkAssemblyNode.h"
#include "vtkAssemblyPath.h"
#include "vtkObjectFactory.h"
#include "vtkPropAssembly.h"
#include "vtkVolume.h"

VTK_ABI_NAMESPACE_BEGIN
vtkCxxSetObjectMacro(vtkAbstractPropPicker, Path, vtkAssemblyPath);

vtkAbstractPropPicker::vtkAbstractPropPicker()
{
  this->Path = nullptr;
}

vtkAbstractPropPicker::~vtkAbstractPropPicker()
{
  if (this->Path)
  {
    this->Path->Delete();
  }
}

// set up for a pick
void vtkAbstractPropPicker::Initialize()
{
  this->vtkAbstractPicker::Initialize();
  if (this->Path)
  {
    this->Path->Delete();
    this->Path = nullptr;
  }
}

//------------------------------------------------------------------------------
vtkProp* vtkAbstractPropPicker::GetViewProp()
{
  if (this->Path != nullptr)
  {
    return this->Path->GetFirstNode()->GetViewProp();
  }
  else
  {
    return nullptr;
  }
}

vtkProp3D* vtkAbstractPropPicker::GetProp3D()
{
  if (this->Path != nullptr)
  {
    vtkProp* prop = this->Path->GetFirstNode()->GetViewProp();
    return vtkProp3D::SafeDownCast(prop);
  }
  else
  {
    return nullptr;
  }
}

vtkActor* vtkAbstractPropPicker::GetActor()
{
  if (this->Path != nullptr)
  {
    vtkProp* prop = this->Path->GetFirstNode()->GetViewProp();
    return vtkActor::SafeDownCast(prop);
  }
  else
  {
    return nullptr;
  }
}

vtkActor2D* vtkAbstractPropPicker::GetActor2D()
{
  if (this->Path != nullptr)
  {
    vtkProp* prop = this->Path->GetFirstNode()->GetViewProp();
    return vtkActor2D::SafeDownCast(prop);
  }
  else
  {
    return nullptr;
  }
}

vtkVolume* vtkAbstractPropPicker::GetVolume()
{
  if (this->Path != nullptr)
  {
    vtkProp* prop = this->Path->GetFirstNode()->GetViewProp();
    return vtkVolume::SafeDownCast(prop);
  }
  else
  {
    return nullptr;
  }
}

vtkAssembly* vtkAbstractPropPicker::GetAssembly()
{
  if (this->Path != nullptr)
  {
    vtkProp* prop = this->Path->GetFirstNode()->GetViewProp();
    return vtkAssembly::SafeDownCast(prop);
  }
  else
  {
    return nullptr;
  }
}

vtkPropAssembly* vtkAbstractPropPicker::GetPropAssembly()
{
  if (this->Path != nullptr)
  {
    vtkProp* prop = this->Path->GetFirstNode()->GetViewProp();
    return vtkPropAssembly::SafeDownCast(prop);
  }
  else
  {
    return nullptr;
  }
}

void vtkAbstractPropPicker::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  if (this->Path)
  {
    os << indent << "Path: " << this->Path << endl;
  }
  else
  {
    os << indent << "Path: (none)" << endl;
  }
}
VTK_ABI_NAMESPACE_END

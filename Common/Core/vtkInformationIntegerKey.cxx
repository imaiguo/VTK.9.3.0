// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkInformationIntegerKey.h"

#include "vtkInformation.h"

//------------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
vtkInformationIntegerKey::vtkInformationIntegerKey(const char* name, const char* location)
  : vtkInformationKey(name, location)
{
  vtkCommonInformationKeyManager::Register(this);
}

//------------------------------------------------------------------------------
vtkInformationIntegerKey::~vtkInformationIntegerKey() = default;

//------------------------------------------------------------------------------
void vtkInformationIntegerKey::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
class vtkInformationIntegerValue : public vtkObjectBase
{
public:
  vtkBaseTypeMacro(vtkInformationIntegerValue, vtkObjectBase);
  int Value;
};

//------------------------------------------------------------------------------
void vtkInformationIntegerKey::Set(vtkInformation* info, int value)
{
  if (vtkInformationIntegerValue* oldv =
        static_cast<vtkInformationIntegerValue*>(this->GetAsObjectBase(info)))
  {
    if (oldv->Value != value)
    {
      // Replace the existing value.
      oldv->Value = value;
      // Since this sets a value without call SetAsObjectBase(),
      // the info has to be modified here (instead of
      // vtkInformation::SetAsObjectBase()
      info->Modified(this);
    }
  }
  else
  {
    // Allocate a new value.
    vtkInformationIntegerValue* v = new vtkInformationIntegerValue;
    v->InitializeObjectBase();
    v->Value = value;
    this->SetAsObjectBase(info, v);
    v->Delete();
  }
}

//------------------------------------------------------------------------------
int vtkInformationIntegerKey::Get(vtkInformation* info)
{
  vtkInformationIntegerValue* v =
    static_cast<vtkInformationIntegerValue*>(this->GetAsObjectBase(info));
  return v ? v->Value : 0;
}

//------------------------------------------------------------------------------
void vtkInformationIntegerKey::ShallowCopy(vtkInformation* from, vtkInformation* to)
{
  if (this->Has(from))
  {
    this->Set(to, this->Get(from));
  }
  else
  {
    this->SetAsObjectBase(to, nullptr); // doesn't exist in from, so remove the key
  }
}

//------------------------------------------------------------------------------
void vtkInformationIntegerKey::Print(ostream& os, vtkInformation* info)
{
  // Print the value.
  if (this->Has(info))
  {
    os << this->Get(info);
  }
}

//------------------------------------------------------------------------------
int* vtkInformationIntegerKey::GetWatchAddress(vtkInformation* info)
{
  if (vtkInformationIntegerValue* v =
        static_cast<vtkInformationIntegerValue*>(this->GetAsObjectBase(info)))
  {
    return &v->Value;
  }
  return nullptr;
}
VTK_ABI_NAMESPACE_END

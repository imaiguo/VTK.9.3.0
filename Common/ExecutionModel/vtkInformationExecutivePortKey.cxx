// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkInformationExecutivePortKey.h"

#include "vtkExecutive.h"
#include "vtkInformation.h"
#include "vtkSmartPointer.h"

//------------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
vtkInformationExecutivePortKey::vtkInformationExecutivePortKey(
  const char* name, const char* location)
  : vtkInformationKey(name, location)
{
  vtkFilteringInformationKeyManager::Register(this);
}

//------------------------------------------------------------------------------
vtkInformationExecutivePortKey::~vtkInformationExecutivePortKey() = default;

//------------------------------------------------------------------------------
void vtkInformationExecutivePortKey::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
class vtkInformationExecutivePortValue : public vtkObjectBase
{
public:
  vtkBaseTypeMacro(vtkInformationExecutivePortValue, vtkObjectBase);
  vtkSmartPointer<vtkExecutive> Executive;
  int Port;
};

//------------------------------------------------------------------------------
void vtkInformationExecutivePortKey::Set(vtkInformation* info, vtkExecutive* executive, int port)
{
  if (executive)
  {
    if (vtkInformationExecutivePortValue* oldv =
          static_cast<vtkInformationExecutivePortValue*>(this->GetAsObjectBase(info)))
    {
      // Replace the existing value.
      oldv->Executive = executive;
      oldv->Port = port;
      // Since this sets a value without call SetAsObjectBase(),
      // the info has to be modified here (instead of
      // vtkInformation::SetAsObjectBase()
      info->Modified();
    }
    else
    {
      // Allocate a new value.
      vtkInformationExecutivePortValue* v = new vtkInformationExecutivePortValue;
      v->InitializeObjectBase();
      v->Executive = executive;
      v->Port = port;
      this->SetAsObjectBase(info, v);
      v->Delete();
    }
  }
  else
  {
    this->SetAsObjectBase(info, nullptr);
  }
}

void vtkInformationExecutivePortKey::Get(vtkInformation* info, vtkExecutive*& executive, int& port)
{
  if (vtkInformationExecutivePortValue* v =
        static_cast<vtkInformationExecutivePortValue*>(this->GetAsObjectBase(info)))
  {
    executive = v->Executive;
    port = v->Port;
    return;
  }

  executive = nullptr;
  port = 0;
}

//------------------------------------------------------------------------------
vtkExecutive* vtkInformationExecutivePortKey::GetExecutive(vtkInformation* info)
{
  if (vtkInformationExecutivePortValue* v =
        static_cast<vtkInformationExecutivePortValue*>(this->GetAsObjectBase(info)))
  {
    return v->Executive;
  }
  return nullptr;
}

//------------------------------------------------------------------------------
int vtkInformationExecutivePortKey::GetPort(vtkInformation* info)
{
  vtkInformationExecutivePortValue* v =
    static_cast<vtkInformationExecutivePortValue*>(this->GetAsObjectBase(info));
  return v ? v->Port : 0;
}

//------------------------------------------------------------------------------
void vtkInformationExecutivePortKey::ShallowCopy(vtkInformation* from, vtkInformation* to)
{
  this->Set(to, this->GetExecutive(from), this->GetPort(from));
}

//------------------------------------------------------------------------------
void vtkInformationExecutivePortKey::Print(ostream& os, vtkInformation* info)
{
  // Print the value.
  if (this->Has(info))
  {
    vtkExecutive* executive = this->GetExecutive(info);
    int port = this->GetPort(info);
    if (executive)
    {
      os << executive->GetClassName() << "(" << executive << ") port " << port;
    }
    else
    {
      os << "(nullptr) port " << port;
    }
  }
}

//------------------------------------------------------------------------------
void vtkInformationExecutivePortKey::Report(vtkInformation* info, vtkGarbageCollector* collector)
{
  if (vtkInformationExecutivePortValue* v =
        static_cast<vtkInformationExecutivePortValue*>(this->GetAsObjectBase(info)))
  {
    v->Executive.Report(collector, this->GetName());
  }
}
VTK_ABI_NAMESPACE_END

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include "vtkAnnotation.h"
#include "vtkInformation.h"
#include "vtkInformationDataObjectKey.h"
#include "vtkInformationDoubleKey.h"
#include "vtkInformationDoubleVectorKey.h"
#include "vtkInformationIntegerKey.h"
#include "vtkInformationStringKey.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkSelection.h"
#include "vtkSmartPointer.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkAnnotation);

vtkCxxSetObjectMacro(vtkAnnotation, Selection, vtkSelection);

vtkInformationKeyMacro(vtkAnnotation, LABEL, String);
vtkInformationKeyRestrictedMacro(vtkAnnotation, COLOR, DoubleVector, 3);
vtkInformationKeyMacro(vtkAnnotation, OPACITY, Double);
vtkInformationKeyMacro(vtkAnnotation, ICON_INDEX, Integer);
vtkInformationKeyMacro(vtkAnnotation, ENABLE, Integer);
vtkInformationKeyMacro(vtkAnnotation, HIDE, Integer);
vtkInformationKeyMacro(vtkAnnotation, DATA, DataObject);

vtkAnnotation::vtkAnnotation()
{
  this->Selection = nullptr;
}

vtkAnnotation::~vtkAnnotation()
{
  if (this->Selection)
  {
    this->Selection->Delete();
  }
}

void vtkAnnotation::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Selection: ";
  if (this->Selection)
  {
    os << "\n";
    this->Selection->PrintSelf(os, indent.GetNextIndent());
  }
  else
  {
    os << "(none)\n";
  }
}

void vtkAnnotation::Initialize()
{
  this->Superclass::Initialize();
}

void vtkAnnotation::ShallowCopy(vtkDataObject* other)
{
  this->Superclass::ShallowCopy(other);
  vtkAnnotation* obj = vtkAnnotation::SafeDownCast(other);
  if (!obj)
  {
    return;
  }
  this->SetSelection(obj->GetSelection());

  vtkInformation* info = this->GetInformation();
  vtkInformation* otherInfo = obj->GetInformation();
  if (otherInfo->Has(vtkAnnotation::ENABLE()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::ENABLE());
  }
  if (otherInfo->Has(vtkAnnotation::HIDE()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::HIDE());
  }
  if (otherInfo->Has(vtkAnnotation::LABEL()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::LABEL());
  }
  if (otherInfo->Has(vtkAnnotation::COLOR()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::COLOR());
  }
  if (otherInfo->Has(vtkAnnotation::OPACITY()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::OPACITY());
  }
  if (otherInfo->Has(vtkAnnotation::DATA()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::DATA());
  }
  if (otherInfo->Has(vtkAnnotation::ICON_INDEX()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::ICON_INDEX());
  }
}

void vtkAnnotation::DeepCopy(vtkDataObject* other)
{
  this->Superclass::DeepCopy(other);
  vtkAnnotation* obj = vtkAnnotation::SafeDownCast(other);
  if (!obj)
  {
    return;
  }
  vtkSmartPointer<vtkSelection> sel = vtkSmartPointer<vtkSelection>::New();
  sel->DeepCopy(obj->GetSelection());
  this->SetSelection(sel);

  vtkInformation* info = this->GetInformation();
  vtkInformation* otherInfo = obj->GetInformation();
  if (otherInfo->Has(vtkAnnotation::ENABLE()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::ENABLE());
  }
  if (otherInfo->Has(vtkAnnotation::HIDE()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::HIDE());
  }
  if (otherInfo->Has(vtkAnnotation::LABEL()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::LABEL());
  }
  if (otherInfo->Has(vtkAnnotation::COLOR()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::COLOR());
  }
  if (otherInfo->Has(vtkAnnotation::OPACITY()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::OPACITY());
  }
  if (otherInfo->Has(vtkAnnotation::DATA()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::DATA());
  }
  if (otherInfo->Has(vtkAnnotation::ICON_INDEX()))
  {
    info->CopyEntry(otherInfo, vtkAnnotation::ICON_INDEX());
  }
}

vtkMTimeType vtkAnnotation::GetMTime()
{
  vtkMTimeType mtime = this->Superclass::GetMTime();
  if (this->Selection)
  {
    vtkMTimeType stime = this->Selection->GetMTime();
    if (stime > mtime)
    {
      mtime = stime;
    }
  }
  return mtime;
}

vtkAnnotation* vtkAnnotation::GetData(vtkInformation* info)
{
  return info ? vtkAnnotation::SafeDownCast(info->Get(DATA_OBJECT())) : nullptr;
}

vtkAnnotation* vtkAnnotation::GetData(vtkInformationVector* v, int i)
{
  return vtkAnnotation::GetData(v->GetInformationObject(i));
}
VTK_ABI_NAMESPACE_END

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkLightingMapPass.h"

#include "vtkClearRGBPass.h"
#include "vtkInformation.h"
#include "vtkInformationIntegerKey.h"
#include "vtkObjectFactory.h"
#include "vtkProp.h"
#include "vtkRenderState.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

#include <cassert>

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkLightingMapPass);

vtkInformationKeyMacro(vtkLightingMapPass, RENDER_LUMINANCE, Integer);
vtkInformationKeyMacro(vtkLightingMapPass, RENDER_NORMALS, Integer);

//------------------------------------------------------------------------------
vtkLightingMapPass::vtkLightingMapPass()
{
  this->RenderType = LUMINANCE;
}

//------------------------------------------------------------------------------
vtkLightingMapPass::~vtkLightingMapPass() = default;

//------------------------------------------------------------------------------
void vtkLightingMapPass::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
// Description:
// Perform rendering according to a render state \p s.
// \pre s_exists: s!=0
void vtkLightingMapPass::Render(const vtkRenderState* s)
{
  assert("pre: s_exists" && s != nullptr);

  // Render filtered geometry according to our keys
  this->NumberOfRenderedProps = 0;

  this->ClearLights(s->GetRenderer());
  this->UpdateLightGeometry(s->GetRenderer());
  this->UpdateLights(s->GetRenderer());

  this->RenderOpaqueGeometry(s);
}

//------------------------------------------------------------------------------
// Description:
// Opaque pass with key checking.
// \pre s_exists: s!=0
void vtkLightingMapPass::RenderOpaqueGeometry(const vtkRenderState* s)
{
  assert("pre: s_exists" && s != nullptr);

  // Clear the RGB buffer first
  vtkSmartPointer<vtkClearRGBPass> clear = vtkSmartPointer<vtkClearRGBPass>::New();
  clear->Render(s);

  int c = s->GetPropArrayCount();
  int i = 0;
  while (i < c)
  {
    vtkProp* p = s->GetPropArray()[i];
    vtkSmartPointer<vtkInformation> keys = p->GetPropertyKeys();
    if (!keys)
    {
      keys.TakeReference(vtkInformation::New());
    }
    switch (this->GetRenderType())
    {
      case LUMINANCE:
        keys->Set(vtkLightingMapPass::RENDER_LUMINANCE(), 1);
        break;
      case NORMALS:
        keys->Set(vtkLightingMapPass::RENDER_NORMALS(), 1);
        break;
    }
    p->SetPropertyKeys(keys);
    int rendered = p->RenderOpaqueGeometry(s->GetRenderer());
    this->NumberOfRenderedProps += rendered;
    ++i;
  }

  // Remove keys
  i = 0;
  while (i < c)
  {
    vtkProp* p = s->GetPropArray()[i];
    vtkInformation* keys = p->GetPropertyKeys();
    switch (this->GetRenderType())
    {
      case LUMINANCE:
        keys->Remove(vtkLightingMapPass::RENDER_LUMINANCE());
        break;
      case NORMALS:
        keys->Remove(vtkLightingMapPass::RENDER_NORMALS());
        break;
    }
    p->SetPropertyKeys(keys);
    ++i;
  }
}
VTK_ABI_NAMESPACE_END

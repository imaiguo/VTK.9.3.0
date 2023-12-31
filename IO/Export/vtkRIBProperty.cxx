// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkRIBProperty.h"
#include "vtkObjectFactory.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkRIBProperty);

vtkRIBProperty::vtkRIBProperty()
{
  this->Declarations = nullptr;
  this->SurfaceShaderParameters = nullptr;
  this->DisplacementShaderParameters = nullptr;
  this->SurfaceShader = new char[strlen("plastic") + 1];
  strcpy(this->SurfaceShader, "plastic");
  this->DisplacementShader = nullptr;
  this->SurfaceShaderUsesDefaultParameters = true;

  // create a vtkProperty that can be rendered
  this->Property = vtkProperty::New();
}

vtkRIBProperty::~vtkRIBProperty()
{
  delete[] this->SurfaceShader;
  delete[] this->DisplacementShader;
  delete[] this->Declarations;

  if (this->Property)
  {
    this->Property->Delete();
  }

  delete[] this->SurfaceShaderParameters;
  delete[] this->DisplacementShaderParameters;
}

void vtkRIBProperty::Render(vtkActor* anActor, vtkRenderer* ren)
{
  // Copy this property's ivars into the property to be rendered
  this->Property->DeepCopy(this);

  // Render the property
  this->Property->Render(anActor, ren);
}

void vtkRIBProperty::SetVariable(const char* variable, const char* value)
{
  delete[] this->Declarations;

  // format of line is: Declare "variable" "type"\n
  size_t length = strlen("Declare ") + strlen(variable) + strlen(value) + 8;
  this->Declarations = new char[length];

  snprintf(this->Declarations, length, "Declare \"%s\" \"%s\"\n", variable, value);
  this->Modified();
}

void vtkRIBProperty::AddVariable(const char* variable, const char* value)
{
  if (this->Declarations == nullptr)
  {
    this->SetVariable(variable, value);
  }
  else
  {
    size_t length = strlen("Declare ") + strlen(variable) + strlen(value) + 8;
    char* newVariable = new char[length];

    snprintf(newVariable, length, "Declare \"%s\" \"%s\"\n", variable, value);
    char* oldDeclarations = this->Declarations;

    this->Declarations = new char[strlen(oldDeclarations) + strlen(newVariable) + 1];
    strcpy(this->Declarations, oldDeclarations);
    strcat(this->Declarations, newVariable);
    delete[] oldDeclarations;
    delete[] newVariable;
    this->Modified();
  }
}

void vtkRIBProperty::SetParameter(const char* parameter, const char* value)
{
  vtkWarningMacro(
    "vtkRIBProperty::SetParameter is deprecated. Using SetSurfaceShaderParameter instead.");
  this->SetSurfaceShaderParameter(parameter, value);
}

void vtkRIBProperty::SetSurfaceShaderParameter(const char* parameter, const char* value)
{
  delete[] this->SurfaceShaderParameters;

  // format of line is: "parameter" "value"
  size_t length = strlen(parameter) + strlen(value) + 7;
  this->SurfaceShaderParameters = new char[length];

  snprintf(this->SurfaceShaderParameters, length, " \"%s\" [%s]", parameter, value);
  this->Modified();
}

void vtkRIBProperty::SetDisplacementShaderParameter(const char* parameter, const char* value)
{
  delete[] this->DisplacementShaderParameters;

  // format of line is: "parameter" "value"
  size_t length = strlen(parameter) + strlen(value) + 7;
  this->DisplacementShaderParameters = new char[length];

  snprintf(this->DisplacementShaderParameters, length, " \"%s\" [%s]", parameter, value);
  this->Modified();
}

void vtkRIBProperty::AddParameter(const char* parameter, const char* value)
{
  vtkWarningMacro(
    "vtkRIBProperty::AddParameter is deprecated. Using AddSurfaceShaderParameter instead.");
  this->AddSurfaceShaderParameter(parameter, value);
}

void vtkRIBProperty::AddSurfaceShaderParameter(
  const char* SurfaceShaderParameter, const char* value)
{
  if (this->SurfaceShaderParameters == nullptr)
  {
    this->SetSurfaceShaderParameter(SurfaceShaderParameter, value);
  }
  else
  {
    size_t length = strlen(SurfaceShaderParameter) + strlen(value) + 7;
    char* newSurfaceShaderParameter = new char[length];

    snprintf(newSurfaceShaderParameter, length, " \"%s\" [%s]", SurfaceShaderParameter, value);
    char* oldSurfaceShaderParameters = this->SurfaceShaderParameters;

    this->SurfaceShaderParameters =
      new char[strlen(oldSurfaceShaderParameters) + strlen(newSurfaceShaderParameter) + 1];
    strcpy(this->SurfaceShaderParameters, oldSurfaceShaderParameters);
    strcat(this->SurfaceShaderParameters, newSurfaceShaderParameter);
    delete[] oldSurfaceShaderParameters;
    delete[] newSurfaceShaderParameter;
    this->Modified();
  }
}

void vtkRIBProperty::AddDisplacementShaderParameter(
  const char* DisplacementShaderParameter, const char* value)
{
  if (this->DisplacementShaderParameters == nullptr)
  {
    this->SetDisplacementShaderParameter(DisplacementShaderParameter, value);
  }
  else
  {
    size_t length = strlen(DisplacementShaderParameter) + strlen(value) + 7;
    char* newDisplacementShaderParameter = new char[length];

    snprintf(
      newDisplacementShaderParameter, length, " \"%s\" [%s]", DisplacementShaderParameter, value);
    char* oldDisplacementShaderParameters = this->DisplacementShaderParameters;

    this->DisplacementShaderParameters = new char[strlen(oldDisplacementShaderParameters) +
      strlen(newDisplacementShaderParameter) + 1];
    strcpy(this->DisplacementShaderParameters, oldDisplacementShaderParameters);
    strcat(this->DisplacementShaderParameters, newDisplacementShaderParameter);
    delete[] oldDisplacementShaderParameters;
    delete[] newDisplacementShaderParameter;
    this->Modified();
  }
}

char* vtkRIBProperty::GetParameters()
{
  vtkWarningMacro(
    "vtkRIBProperty::GetParameters is deprecated. Using GetSurfaceShaderParameter instead.");
  return this->GetSurfaceShaderParameters();
}

char* vtkRIBProperty::GetSurfaceShaderParameters()
{
  return this->SurfaceShaderParameters;
}

char* vtkRIBProperty::GetDisplacementShaderParameters()
{
  return this->DisplacementShaderParameters;
}

char* vtkRIBProperty::GetDeclarations()
{
  return this->Declarations;
}

void vtkRIBProperty::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  if (this->SurfaceShader)
  {
    os << indent << "SurfaceShader: " << this->SurfaceShader << "\n";
  }
  else
  {
    os << indent << "SurfaceShader: (none)\n";
  }
  if (this->DisplacementShader)
  {
    os << indent << "DisplacementShader: " << this->DisplacementShader << "\n";
  }
  else
  {
    os << indent << "DisplacementShader: (none)\n";
  }
  if (this->Declarations)
  {
    os << indent << "Declarations: " << this->Declarations;
  }
  else
  {
    os << indent << "Declarations: (none)\n";
  }
  if (this->SurfaceShaderParameters)
  {
    os << indent << "SurfaceShaderParameters: " << this->SurfaceShaderParameters;
  }
  else
  {
    os << indent << "SurfaceShaderParameters: (none)\n";
  }
  if (this->DisplacementShaderParameters)
  {
    os << indent << "DisplacementShaderParameters: " << this->DisplacementShaderParameters;
  }
  else
  {
    os << indent << "DisplacementShaderParameters: (none)\n";
  }
  os << indent
     << "SurfaceShaderUsesDefaultParameters: " << this->GetSurfaceShaderUsesDefaultParameters()
     << std::endl;
}
VTK_ABI_NAMESPACE_END

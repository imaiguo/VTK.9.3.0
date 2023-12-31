// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
#include "vtkDistanceToCamera.h"

#include "vtkCamera.h"
#include "vtkDoubleArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkDistanceToCamera);

vtkDistanceToCamera::vtkDistanceToCamera()
{
  this->Renderer = nullptr;
  this->ScreenSize = 5.0;
  this->Scaling = false;
  this->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "scale");
  this->LastRendererSize[0] = 0;
  this->LastRendererSize[1] = 0;
  this->LastCameraPosition[0] = 0.0;
  this->LastCameraPosition[1] = 0.0;
  this->LastCameraPosition[2] = 0.0;
  this->LastCameraFocalPoint[0] = 0.0;
  this->LastCameraFocalPoint[1] = 0.0;
  this->LastCameraFocalPoint[2] = 0.0;
  this->LastCameraViewUp[0] = 0.0;
  this->LastCameraViewUp[1] = 0.0;
  this->LastCameraViewUp[2] = 0.0;
  this->LastCameraParallelScale = 0.0;
  this->DistanceArrayName = nullptr;
  this->SetDistanceArrayName("DistanceToCamera");
}

vtkDistanceToCamera::~vtkDistanceToCamera()
{
  delete[] this->DistanceArrayName;
}

void vtkDistanceToCamera::SetRenderer(vtkRenderer* ren)
{
  if (ren != this->Renderer)
  {
    this->Renderer = ren;
    this->Modified();
  }
}

vtkMTimeType vtkDistanceToCamera::GetMTime()
{
  // Check for minimal changes
  if (this->Renderer)
  {
    const int* sz = this->Renderer->GetSize();
    if (this->LastRendererSize[0] != sz[0] || this->LastRendererSize[1] != sz[1])
    {
      this->LastRendererSize[0] = sz[0];
      this->LastRendererSize[1] = sz[1];
      this->Modified();
    }
    vtkCamera* cam = this->Renderer->GetActiveCamera();
    if (cam)
    {
      double* pos = cam->GetPosition();
      if (this->LastCameraPosition[0] != pos[0] || this->LastCameraPosition[1] != pos[1] ||
        this->LastCameraPosition[2] != pos[2])
      {
        this->LastCameraPosition[0] = pos[0];
        this->LastCameraPosition[1] = pos[1];
        this->LastCameraPosition[2] = pos[2];
        this->Modified();
      }
      double* fp = cam->GetFocalPoint();
      if (this->LastCameraFocalPoint[0] != fp[0] || this->LastCameraFocalPoint[1] != fp[1] ||
        this->LastCameraFocalPoint[2] != fp[2])
      {
        this->LastCameraFocalPoint[0] = fp[0];
        this->LastCameraFocalPoint[1] = fp[1];
        this->LastCameraFocalPoint[2] = fp[2];
        this->Modified();
      }
      double* up = cam->GetViewUp();
      if (this->LastCameraViewUp[0] != up[0] || this->LastCameraViewUp[1] != up[1] ||
        this->LastCameraViewUp[2] != up[2])
      {
        this->LastCameraViewUp[0] = up[0];
        this->LastCameraViewUp[1] = up[1];
        this->LastCameraViewUp[2] = up[2];
        this->Modified();
      }
      double scale = cam->GetParallelScale();
      if (this->LastCameraParallelScale != scale)
      {
        this->LastCameraParallelScale = scale;
        this->Modified();
      }
    }
  }
  return this->Superclass::GetMTime();
}

int vtkDistanceToCamera::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkPointSet* input = vtkPointSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPointSet* output = vtkPointSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (input->GetNumberOfPoints() == 0)
  {
    return 1;
  }

  if (!this->Renderer)
  {
    vtkErrorMacro("Renderer must be non-nullptr");
    return 0;
  }

  if (!this->DistanceArrayName || strlen(this->DistanceArrayName) == 0)
  {
    vtkErrorMacro("The name of the distance array must be specified");
    return 0;
  }

  vtkCamera* camera = this->Renderer->GetActiveCamera();
  double* pos = camera->GetPosition();

  vtkDataArray* scaleArr = nullptr;
  if (this->Scaling)
  {
    scaleArr = this->GetInputArrayToProcess(0, inputVector);
    if (!scaleArr)
    {
      vtkErrorMacro("Scaling array not found.");
      return 0;
    }
    if (scaleArr->GetNumberOfComponents() > 1)
    {
      vtkErrorMacro("Scaling array has more than one component.");
      return 0;
    }
  }

  output->ShallowCopy(input);
  vtkIdType numPoints = input->GetNumberOfPoints();
  vtkSmartPointer<vtkDoubleArray> distArr = vtkSmartPointer<vtkDoubleArray>::New();
  distArr->SetName(this->DistanceArrayName);
  distArr->SetNumberOfTuples(numPoints);
  output->GetPointData()->AddArray(distArr);
  if (camera->GetParallelProjection())
  {
    double size = 1;
    if (this->Renderer->GetSize()[1] > 0)
    {
      size = 2.0 * (camera->GetParallelScale() / this->Renderer->GetSize()[1]) * this->ScreenSize;
    }
    if (scaleArr)
    {
      double tuple[1];
      for (vtkIdType i = 0; i < numPoints; ++i)
      {
        scaleArr->GetTuple(i, tuple);
        distArr->SetValue(i, size * tuple[0]);
      }
    }
    else
    {
      for (vtkIdType i = 0; i < numPoints; ++i)
      {
        distArr->SetValue(i, size);
      }
    }
  }
  else
  {
    double factor = 1;
    if (this->Renderer->GetSize()[1] > 0)
    {
      factor = 2.0 * this->ScreenSize *
        tan(vtkMath::RadiansFromDegrees(camera->GetViewAngle() / 2.0)) /
        this->Renderer->GetSize()[1];
    }
    if (scaleArr)
    {
      double tuple[1];
      for (vtkIdType i = 0; i < numPoints; ++i)
      {
        double dist = sqrt(vtkMath::Distance2BetweenPoints(input->GetPoint(i), pos));
        double size = factor * dist;
        scaleArr->GetTuple(i, tuple);
        distArr->SetValue(i, size * tuple[0]);
      }
    }
    else
    {
      for (vtkIdType i = 0; i < numPoints; ++i)
      {
        double dist = sqrt(vtkMath::Distance2BetweenPoints(input->GetPoint(i), pos));
        double size = factor * dist;
        distArr->SetValue(i, size);
      }
    }
  }

  return 1;
}

void vtkDistanceToCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Renderer: ";
  if (this->Renderer)
  {
    os << "\n";
    this->Renderer->PrintSelf(os, indent.GetNextIndent());
  }
  else
  {
    os << "(none)";
  }
  os << indent << "ScreenSize: " << this->ScreenSize << endl;
  os << indent << "Scaling: " << (this->Scaling ? "on" : "off") << endl;
}
VTK_ABI_NAMESPACE_END

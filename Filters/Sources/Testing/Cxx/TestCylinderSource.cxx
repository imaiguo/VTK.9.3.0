// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include <vtkCylinderSource.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkSmartPointer.h>

int TestCylinderSource(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  vtkSmartPointer<vtkMinimalStandardRandomSequence> randomSequence =
    vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
  randomSequence->SetSeed(1);

  vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
  cylinderSource->SetResolution(8);
  cylinderSource->CappingOn();

  cylinderSource->SetOutputPointsPrecision(vtkAlgorithm::SINGLE_PRECISION);

  double center[3];
  for (unsigned int i = 0; i < 3; ++i)
  {
    randomSequence->Next();
    center[i] = randomSequence->GetValue();
  }
  cylinderSource->SetCenter(center);

  randomSequence->Next();
  double height = randomSequence->GetValue();
  cylinderSource->SetHeight(height);

  randomSequence->Next();
  double radius = randomSequence->GetValue();
  cylinderSource->SetRadius(radius);

  cylinderSource->Update();

  vtkSmartPointer<vtkPolyData> polyData = cylinderSource->GetOutput();
  vtkSmartPointer<vtkPoints> points = polyData->GetPoints();

  if (points->GetDataType() != VTK_FLOAT)
  {
    return EXIT_FAILURE;
  }

  cylinderSource->SetOutputPointsPrecision(vtkAlgorithm::DOUBLE_PRECISION);

  for (unsigned int i = 0; i < 3; ++i)
  {
    randomSequence->Next();
    center[i] = randomSequence->GetValue();
  }
  cylinderSource->SetCenter(center);

  randomSequence->Next();
  height = randomSequence->GetValue();
  cylinderSource->SetHeight(height);

  randomSequence->Next();
  radius = randomSequence->GetValue();
  cylinderSource->SetRadius(radius);

  cylinderSource->Update();

  polyData = cylinderSource->GetOutput();
  points = polyData->GetPoints();

  if (points->GetDataType() != VTK_DOUBLE)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include <vtkMinimalStandardRandomSequence.h>
#include <vtkPlaneSource.h>
#include <vtkSmartPointer.h>

int TestPlaneSource(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  vtkSmartPointer<vtkMinimalStandardRandomSequence> randomSequence =
    vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
  randomSequence->SetSeed(1);

  vtkSmartPointer<vtkPlaneSource> planeSource = vtkSmartPointer<vtkPlaneSource>::New();
  planeSource->SetXResolution(8);
  planeSource->SetYResolution(8);

  planeSource->SetOutputPointsPrecision(vtkAlgorithm::SINGLE_PRECISION);

  double center[3];
  for (unsigned int i = 0; i < 3; ++i)
  {
    randomSequence->Next();
    center[i] = randomSequence->GetValue();
  }
  planeSource->SetCenter(center);

  double normal[3];
  for (unsigned int i = 0; i < 3; ++i)
  {
    randomSequence->Next();
    normal[i] = randomSequence->GetValue();
  }
  planeSource->SetNormal(normal);

  planeSource->Update();

  vtkSmartPointer<vtkPolyData> polyData = planeSource->GetOutput();
  vtkSmartPointer<vtkPoints> points = polyData->GetPoints();

  if (points->GetDataType() != VTK_FLOAT)
  {
    return EXIT_FAILURE;
  }

  planeSource->SetOutputPointsPrecision(vtkAlgorithm::DOUBLE_PRECISION);

  for (unsigned int i = 0; i < 3; ++i)
  {
    randomSequence->Next();
    center[i] = randomSequence->GetValue();
  }
  planeSource->SetCenter(center);

  for (unsigned int i = 0; i < 3; ++i)
  {
    randomSequence->Next();
    normal[i] = randomSequence->GetValue();
  }
  planeSource->SetNormal(normal);

  planeSource->Update();

  polyData = planeSource->GetOutput();
  points = polyData->GetPoints();

  if (points->GetDataType() != VTK_DOUBLE)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkBiQuadraticQuad.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkMathUtilities.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkProbeFilter.h"
#include "vtkUnstructuredGrid.h"

//------------------------------------------------------------------------------
int TestBiQuadraticQuad(int, char*[])
{
  vtkNew<vtkPoints> points;
  points->InsertNextPoint(0.0, 0.0, 0.0);
  points->InsertNextPoint(1.0, 0.0, 0.0);
  points->InsertNextPoint(1.0, 1.0, 0.0);
  points->InsertNextPoint(0.0, 1.0, 0.0);
  points->InsertNextPoint(0.5, 0.0, 0.0);
  points->InsertNextPoint(1.0, 0.5, 0.0);
  points->InsertNextPoint(0.5, 1.0, 0.0);
  points->InsertNextPoint(0.0, 0.5, 0.0);
  points->InsertNextPoint(0.5, 0.5, 0.0);

  vtkNew<vtkBiQuadraticQuad> quad;
  for (int i = 0; i < 9; ++i)
  {
    quad->GetPointIds()->SetId(i, i);
  }

  vtkNew<vtkCellArray> cellArray;
  cellArray->InsertNextCell(quad);

  vtkNew<vtkDoubleArray> uArray;
  uArray->SetName("u");
  uArray->SetNumberOfComponents(1);
  uArray->SetNumberOfTuples(9);
  // set u(x, y) = x
  for (int i = 0; i < 9; i++)
  {
    uArray->SetValue(i, points->GetPoint(i)[0]);
  }

  vtkNew<vtkUnstructuredGrid> grid;
  grid->SetPoints(points);
  grid->SetCells(VTK_BIQUADRATIC_QUAD, cellArray);
  grid->GetPointData()->SetScalars(uArray);

  double probeX = 2.0 / 3.0;
  double probeY = 0.25;
  vtkNew<vtkPoints> probePoints;
  probePoints->InsertNextPoint(probeX, probeY, 0.0);
  vtkNew<vtkPolyData> probePolyData;
  probePolyData->SetPoints(probePoints);

  vtkNew<vtkProbeFilter> prober;
  prober->SetSourceData(grid);
  prober->SetInputData(probePolyData);
  prober->Update();

  vtkDataArray* data = prober->GetOutput()->GetPointData()->GetScalars();
  vtkDoubleArray* doubleData = vtkArrayDownCast<vtkDoubleArray>(data);

  double interpolated(0.0);
  if (doubleData)
  {
    interpolated = doubleData->GetComponent(0, 0);
  }
  else
  {
    cout << "Failed to downcast prober scalars." << endl;
  }
  if (!vtkMathUtilities::FuzzyCompare(interpolated, probeX, 1.0e-6))
  {
    cout << "Interpolated value of " << interpolated << " with probe value " << probeX
         << " difference of " << (interpolated - probeX) << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

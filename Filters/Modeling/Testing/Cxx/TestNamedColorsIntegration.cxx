// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include <vtkNamedColors.h>

#include "vtkRegressionTestImage.h"
#include <vtkActor.h>
#include <vtkAlgorithm.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkConeSource.h>
#include <vtkElevationFilter.h>
#include <vtkLookupTable.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

// Create a cone, contour it using the banded contour filter and
// color it with the primary additive and subtractive colors.
int TestNamedColorsIntegration(int argc, char* argv[])
{
  vtkSmartPointer<vtkNamedColors> namedColors = vtkSmartPointer<vtkNamedColors>::New();
  // namedColors->PrintSelf(std::cout,vtkIndent(2));

  // Create a cone
  vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
  coneSource->SetCenter(0.0, 0.0, 0.0);
  coneSource->SetRadius(5.0);
  coneSource->SetHeight(10);
  coneSource->SetDirection(0, 1, 0);
  coneSource->Update();

  double bounds[6];
  coneSource->GetOutput()->GetBounds(bounds);

  vtkSmartPointer<vtkElevationFilter> elevation = vtkSmartPointer<vtkElevationFilter>::New();
  elevation->SetInputConnection(coneSource->GetOutputPort());
  elevation->SetLowPoint(0, bounds[2], 0);
  elevation->SetHighPoint(0, bounds[3], 0);

  vtkSmartPointer<vtkBandedPolyDataContourFilter> bcf =
    vtkSmartPointer<vtkBandedPolyDataContourFilter>::New();
  bcf->SetInputConnection(elevation->GetOutputPort());
  bcf->SetScalarModeToValue();
  bcf->GenerateContourEdgesOn();
  bcf->GenerateValues(7, elevation->GetScalarRange());

  // Build a simple lookup table of
  // primary additive and subtractive colors.
  vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
  lut->SetNumberOfTableValues(7);
  double rgba[4];
  // Test setting and getting colors here.
  namedColors->GetColor("Red", rgba);
  namedColors->SetColor("My Red", rgba);
  namedColors->GetColor("My Red", rgba);
  lut->SetTableValue(0, rgba);
  namedColors->GetColor("DarkGreen", rgba);
  lut->SetTableValue(1, rgba);
  // Alternatively we can use tuple methods here:
  lut->SetTableValue(2, namedColors->GetColor4d("Blue").GetData());
  lut->SetTableValue(3, namedColors->GetColor4d("Cyan").GetData());
  lut->SetTableValue(4, namedColors->GetColor4d("Magenta").GetData());
  lut->SetTableValue(5, namedColors->GetColor4d("Yellow").GetData());
  lut->SetTableValue(6, namedColors->GetColor4d("White").GetData());
  lut->SetTableRange(elevation->GetScalarRange());
  lut->Build();

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(bcf->GetOutputPort());
  mapper->SetScalarRange(elevation->GetScalarRange());
  mapper->SetLookupTable(lut);
  mapper->SetScalarModeToUseCellData();

  vtkSmartPointer<vtkPolyDataMapper> contourLineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  contourLineMapper->SetInputData(bcf->GetContourEdgesOutput());
  contourLineMapper->SetScalarRange(elevation->GetScalarRange());
  contourLineMapper->SetResolveCoincidentTopologyToPolygonOffset();

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkActor> contourLineActor = vtkSmartPointer<vtkActor>::New();
  contourLineActor->SetMapper(contourLineMapper);
  contourLineActor->GetProperty()->SetColor(namedColors->GetColor3d("Black").GetData());

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->AddActor(contourLineActor);
  renderer->SetBackground(namedColors->GetColor3d("SteelBlue").GetData());
  renderWindow->Render();

  int retVal = vtkRegressionTestImage(renderWindow);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    renderWindowInteractor->Start();
  }

  return !retVal;
}

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProgrammableGlyphFilter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

void CalcGlyph(void* arg)
{

  vtkProgrammableGlyphFilter* glyphFilter = (vtkProgrammableGlyphFilter*)arg;

  if (!glyphFilter)
  {
    std::cerr << "glyphFilter is not valid!" << std::endl;
  }
  double pointCoords[3];
  glyphFilter->GetPoint(pointCoords);

  std::cout << "Calling CalcGlyph for point " << glyphFilter->GetPointId() << std::endl;
  std::cout << "Point coords are: " << pointCoords[0] << " " << pointCoords[1] << " "
            << pointCoords[2] << std::endl;

  if (glyphFilter->GetPointId() == 0)
  {
    // Normal use case: should produce cone
    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetCenter(pointCoords);
    glyphFilter->SetSourceConnection(coneSource->GetOutputPort());
  }
  else if (glyphFilter->GetPointId() == 1)
  {
    // nullptr SourceConnection, valid SourceData: should produce cube
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->SetCenter(pointCoords);
    cubeSource->Update();
    glyphFilter->SetSourceConnection(nullptr);
    glyphFilter->SetSourceData(cubeSource->GetOutput());
  }
  else if (glyphFilter->GetPointId() == 2)
  {
    // Normal use case: should produce sphere
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(pointCoords);
    glyphFilter->SetSourceConnection(sphereSource->GetOutputPort());
  }
  else
  {
    // nullptr SourceConnection and nullptr SourceData: should produce nothing
    glyphFilter->SetSourceConnection(nullptr);
    glyphFilter->SetSourceData(nullptr);
  }
}

int TestProgrammableGlyph(int, char*[])
{
  // Create points
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(0, 0, 0);
  points->InsertNextPoint(5, 0, 0);
  points->InsertNextPoint(10, 0, 0);
  points->InsertNextPoint(15, 0, 0);

  // Combine into a polydata
  vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);

  vtkSmartPointer<vtkProgrammableGlyphFilter> glyphFilter =
    vtkSmartPointer<vtkProgrammableGlyphFilter>::New();
  glyphFilter->SetInputData(polydata);
  glyphFilter->SetGlyphMethod(CalcGlyph, glyphFilter);
  // need a default glyph, but this should not be used
  vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
  coneSource->Update();
  glyphFilter->SetSourceData(coneSource->GetOutput());

  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(glyphFilter->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(.2, .3, .4);

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

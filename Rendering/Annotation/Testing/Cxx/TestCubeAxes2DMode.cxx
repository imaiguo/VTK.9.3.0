// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// .SECTION Thanks
// This test was written by Philippe Pebay, Kitware SAS 2011

#include "vtkActor.h"
#include "vtkAxisActor.h"
#include "vtkCamera.h"
#include "vtkCoordinate.h"
#include "vtkCubeAxesActor.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkProperty2D.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"
#include "vtkTextProperty.h"

int TestCubeAxes2DMode(int argc, char* argv[])
{
  // Create plane source
  vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
  plane->SetXResolution(10);
  plane->SetYResolution(10);

  // Create plane mapper
  vtkSmartPointer<vtkPolyDataMapper> planeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  planeMapper->SetInputConnection(plane->GetOutputPort());
  vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();

  // Create plane actor
  vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
  planeActor->SetMapper(planeMapper);
  planeActor->GetProperty()->SetColor(.5, .5, .5);

  // Create edge mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> edgeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  edgeMapper->SetInputConnection(plane->GetOutputPort());
  // move the lines behind the axes a tad
  edgeMapper->SetRelativeCoincidentTopologyLineOffsetParameters(0, 2);

  // Create edge actor
  vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
  edgeActor->SetMapper(edgeMapper);
  edgeActor->GetProperty()->SetColor(.0, .0, .0);
  edgeActor->GetProperty()->SetRepresentationToWireframe();

  // Create renderer
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(1., 1., 1.);
  renderer->GetActiveCamera()->SetFocalPoint(.0, .0, .0);
  renderer->GetActiveCamera()->SetPosition(.0, .0, 2.5);

  // Create cube axes actor
  vtkSmartPointer<vtkCubeAxesActor> axes = vtkSmartPointer<vtkCubeAxesActor>::New();
  axes->SetCamera(renderer->GetActiveCamera());
  axes->SetBounds(-.5, .5, -.5, .5, 0., 0.);
  axes->SetCornerOffset(.0);
  axes->SetXAxisVisibility(true);
  axes->SetYAxisVisibility(true);
  axes->SetZAxisVisibility(false);
  axes->SetUse2DMode(true);

  // Deactivate LOD for all axes
  axes->SetEnableDistanceLOD(false);
  axes->SetEnableViewAngleLOD(false);

  // Use red color for X axis
  axes->GetXAxesLinesProperty()->SetColor(1., 0., 0.);
  axes->GetTitleTextProperty(0)->SetColor(1., 0., 0.);
  axes->GetLabelTextProperty(0)->SetColor(1., 0., 0.);

  // Use green color for Y axis
  axes->GetYAxesLinesProperty()->SetColor(0., 1., 0.);
  axes->GetTitleTextProperty(1)->SetColor(0., 1., 0.);
  axes->GetLabelTextProperty(1)->SetColor(0., 1., 0.);

  // Add all actors to renderer
  renderer->AddActor(planeActor);
  renderer->AddActor(edgeActor);
  renderer->AddActor(axes);

  // Create render window and interactor
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(800, 600);
  renderWindow->SetMultiSamples(0);
  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  // Render and possibly interact
  renderWindow->Render();
  int retVal = vtkRegressionTestImage(renderWindow);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    interactor->Start();
  }

  return !retVal;
}

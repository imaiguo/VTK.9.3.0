// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkBYUReader.h"
#include "vtkCamera.h"
#include "vtkCubeAxesActor.h"
#include "vtkLODActor.h"
#include "vtkLight.h"
#include "vtkNew.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTestUtilities.h"
#include "vtkTextProperty.h"

//------------------------------------------------------------------------------
int TestCubeAxesSticky(int argc, char* argv[])
{
  vtkNew<vtkBYUReader> fohe;
  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/teapot.g");
  fohe->SetGeometryFileName(fname);
  delete[] fname;

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection(fohe->GetOutputPort());

  vtkNew<vtkPolyDataMapper> foheMapper;
  foheMapper->SetInputConnection(normals->GetOutputPort());

  vtkNew<vtkLODActor> foheActor;
  foheActor->SetMapper(foheMapper);
  foheActor->GetProperty()->SetDiffuseColor(0.7, 0.3, 0.0);

  vtkNew<vtkOutlineFilter> outline;
  outline->SetInputConnection(normals->GetOutputPort());

  vtkNew<vtkPolyDataMapper> mapOutline;
  mapOutline->SetInputConnection(outline->GetOutputPort());

  vtkNew<vtkActor> outlineActor;
  outlineActor->SetMapper(mapOutline);
  outlineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);

  vtkNew<vtkCamera> camera;
  camera->SetClippingRange(1.0, 100.0);
  double xShift = -5;
  double yShift = -1;
  double zShift = 1;
  camera->SetFocalPoint(0.9 + xShift, 1.0 + yShift, 0.0 + zShift);
  camera->SetPosition(8.63 + xShift, 6.0 + yShift, 3.77 + zShift);

  vtkNew<vtkLight> light;
  light->SetFocalPoint(0.21406, 1.5, 0.0);
  light->SetPosition(8.3761, 4.94858, 4.12505);

  vtkNew<vtkRenderer> ren2;
  ren2->SetActiveCamera(camera);
  ren2->AddLight(light);

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetMultiSamples(0);
  renWin->AddRenderer(ren2);
  renWin->SetWindowName("Cube Axes");
  renWin->SetSize(800, 400);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  ren2->AddViewProp(foheActor);
  ren2->AddViewProp(outlineActor);
  ren2->SetBackground(0.1, 0.2, 0.4);

  normals->Update();

  vtkNew<vtkCubeAxesActor> axes;
  axes->SetBounds(normals->GetOutput()->GetBounds());
  axes->SetXAxisRange(20, 300);
  axes->SetYAxisRange(-.01, .01);
  axes->SetCamera(ren2->GetActiveCamera());
  axes->SetXLabelFormat("%6.1f");
  axes->SetYLabelFormat("%6.1f");
  axes->SetZLabelFormat("%6.1f");
  axes->SetScreenSize(15.);
  axes->SetFlyModeToClosestTriad();
  axes->SetCornerOffset(.0);
  axes->SetStickyAxes(true);
  axes->SetCenterStickyAxes(false);

  // Use red color for X axis
  axes->GetXAxesLinesProperty()->SetColor(1., 0., 0.);
  axes->GetTitleTextProperty(0)->SetColor(1., 0., 0.);
  axes->GetLabelTextProperty(0)->SetColor(.8, 0., 0.);

  // Use green color for Y axis
  axes->GetYAxesLinesProperty()->SetColor(0., 1., 0.);
  axes->GetTitleTextProperty(1)->SetColor(0., 1., 0.);
  axes->GetLabelTextProperty(1)->SetColor(0., .8, 0.);

  ren2->AddViewProp(axes);
  renWin->Render();

  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  return !retVal;
}

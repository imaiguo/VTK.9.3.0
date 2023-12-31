// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkSmartPointer.h"

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkElevationFilter.h"
#include "vtkNew.h"
#include "vtkOSPRayPass.h"
#include "vtkOSPRayRendererNode.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty2D.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkScalarBarActor.h"
#include "vtkScalarsToColors.h"
#include "vtkSphereSource.h"
#include "vtkTextProperty.h"

#include "vtkTestUtilities.h"

int TestOSPRayScalarBar(int argc, char* argv[])
{
  vtkNew<vtkSphereSource> sphere;
  sphere->SetThetaResolution(8);
  sphere->SetPhiResolution(8);
  vtkNew<vtkElevationFilter> elev;
  elev->SetInputConnection(sphere->GetOutputPort(0));

  vtkNew<vtkPolyDataMapper> sphereMapper;
  sphereMapper->SetInputConnection(elev->GetOutputPort(0));
  vtkNew<vtkActor> sphereActor;
  sphereActor->SetMapper(sphereMapper);

  // Create the RenderWindow, Renderer and all Actors
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);

  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  vtkSmartPointer<vtkScalarBarActor> scalarBar1 = vtkSmartPointer<vtkScalarBarActor>::New();
  vtkScalarsToColors* lut = sphereMapper->GetLookupTable();
  lut->SetAnnotation(0.0, "Zed");
  lut->SetAnnotation(1.0, "Uno");
  lut->SetAnnotation(0.1, "$\\frac{1}{10}$");
  lut->SetAnnotation(0.125, "$\\frac{1}{8}$");
  lut->SetAnnotation(0.5, "Half");
  scalarBar1->SetTitle("Density");
  scalarBar1->SetLookupTable(lut);
  scalarBar1->DrawAnnotationsOn();
  scalarBar1->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
  scalarBar1->GetPositionCoordinate()->SetValue(.6, .05);
  scalarBar1->SetWidth(0.15);
  scalarBar1->SetHeight(0.5);
  scalarBar1->SetTextPositionToPrecedeScalarBar();
  scalarBar1->GetTitleTextProperty()->SetColor(0., 0., 1.);
  scalarBar1->GetLabelTextProperty()->SetColor(0., 0., 1.);
  scalarBar1->SetDrawFrame(1);
  scalarBar1->GetFrameProperty()->SetColor(0., 0., 0.);
  scalarBar1->SetDrawBackground(1);
  scalarBar1->GetBackgroundProperty()->SetColor(1., 1., 1.);

  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(sphereActor);
  ren1->AddActor(scalarBar1);
  ren1->GradientBackgroundOn();
  ren1->SetBackground(.5, .5, .5);
  ren1->SetBackground2(.0, .0, .0);

  // render the image
  renWin->SetWindowName("VTK - Scalar Bar options");
  renWin->SetSize(600, 500);
  renWin->SetMultiSamples(0);
  vtkSmartPointer<vtkOSPRayPass> ospray = vtkSmartPointer<vtkOSPRayPass>::New();
  ren1->SetPass(ospray);

  for (int i = 0; i < argc; ++i)
  {
    if (!strcmp(argv[i], "--OptiX"))
    {
      vtkOSPRayRendererNode::SetRendererType("optix pathtracer", ren1);
      break;
    }
  }

  renWin->Render();

  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  return !retVal;
}

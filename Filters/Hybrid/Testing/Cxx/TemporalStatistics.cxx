// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include "vtkCamera.h"
#include "vtkCompositeDataGeometryFilter.h"
#include "vtkCompositeDataPipeline.h"
#include "vtkPolyDataMapper.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTemporalFractal.h"
#include "vtkTemporalStatistics.h"

#include "vtkSmartPointer.h"
#define VTK_CREATE(type, name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//------------------------------------------------------------------------------
static void ShowResult(vtkRenderer* renderer, vtkAlgorithmOutput* input, const char* arrayName);

//------------------------------------------------------------------------------
int TemporalStatistics(int argc, char* argv[])
{
  // We have to use a composite pipeline to handle these composite data
  // structures.
  VTK_CREATE(vtkCompositeDataPipeline, prototype);
  vtkAlgorithm::SetDefaultExecutivePrototype(prototype);

  // create temporal fractals
  VTK_CREATE(vtkTemporalFractal, source);
  source->SetMaximumLevel(3);
  source->DiscreteTimeStepsOn();
  // source->GenerateRectilinearGridsOn();
  source->AdaptiveSubdivisionOff();

  VTK_CREATE(vtkTemporalStatistics, statistics);
  statistics->SetInputConnection(source->GetOutputPort());

  // Convert the hierarchical information into render-able polydata.
  VTK_CREATE(vtkCompositeDataGeometryFilter, geometry);
  geometry->SetInputConnection(statistics->GetOutputPort());

  VTK_CREATE(vtkRenderWindow, renWin);
  VTK_CREATE(vtkRenderWindowInteractor, iren);

  VTK_CREATE(vtkRenderer, avgRenderer);
  avgRenderer->SetViewport(0.0, 0.5, 0.5, 1.0);
  ShowResult(avgRenderer, geometry->GetOutputPort(), "Fractal Volume Fraction_average");
  renWin->AddRenderer(avgRenderer);

  VTK_CREATE(vtkRenderer, minRenderer);
  minRenderer->SetViewport(0.5, 0.5, 1.0, 1.0);
  ShowResult(minRenderer, geometry->GetOutputPort(), "Fractal Volume Fraction_minimum");
  renWin->AddRenderer(minRenderer);

  VTK_CREATE(vtkRenderer, maxRenderer);
  maxRenderer->SetViewport(0.0, 0.0, 0.5, 0.5);
  ShowResult(maxRenderer, geometry->GetOutputPort(), "Fractal Volume Fraction_maximum");
  renWin->AddRenderer(maxRenderer);

  VTK_CREATE(vtkRenderer, stddevRenderer);
  stddevRenderer->SetViewport(0.5, 0.0, 1.0, 0.5);
  ShowResult(stddevRenderer, geometry->GetOutputPort(), "Fractal Volume Fraction_stddev");
  renWin->AddRenderer(stddevRenderer);

  renWin->SetSize(450, 400);
  iren->SetRenderWindow(renWin);
  renWin->Render();

  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  vtkAlgorithm::SetDefaultExecutivePrototype(nullptr);
  return !retVal;
}

//------------------------------------------------------------------------------
static void ShowResult(vtkRenderer* renderer, vtkAlgorithmOutput* input, const char* arrayName)
{
  // Set up rendering classes
  VTK_CREATE(vtkPolyDataMapper, mapper);
  mapper->SetInputConnection(input);
  mapper->SetScalarModeToUseCellFieldData();
  mapper->SelectColorArray(arrayName);

  VTK_CREATE(vtkActor, actor);
  actor->SetMapper(mapper);

  renderer->AddActor(actor);
  renderer->SetBackground(0.5, 0.5, 0.5);
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Zoom(1.5);
}

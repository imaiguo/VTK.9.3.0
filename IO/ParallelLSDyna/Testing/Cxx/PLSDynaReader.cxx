// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2009 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCompositeDataGeometryFilter.h"
#include "vtkCompositeRenderManager.h"
#include "vtkLookupTable.h"
#include "vtkMPIController.h"
#include "vtkPLSDynaReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTestUtilities.h"

#include "vtkNew.h"
#include "vtkSmartPointer.h"

struct TestArgs
{
  int* retval;
  int argc;
  char** argv;
};

//=============================================================================
void PLSDynaReader(vtkMultiProcessController* controller, void* _args)
{
  TestArgs* args = reinterpret_cast<TestArgs*>(_args);
  int argc = args->argc;
  char** argv = args->argv;
  *(args->retval) = 1;

  // Set up reader.
  vtkNew<vtkPLSDynaReader> reader;

  char* meshFileName =
    vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/LSDyna/hemi.draw/hemi_draw.d3plot");
  reader->SetFileName(meshFileName);
  delete[] meshFileName;

  // Extract geometry that we can render.
  vtkNew<vtkCompositeDataGeometryFilter> geometry;
  geometry->SetInputConnection(reader->GetOutputPort());

  // Set up rendering stuff.
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(geometry->GetOutputPort());
  mapper->SetScalarModeToUsePointFieldData();

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkCompositeRenderManager> prm;

  vtkSmartPointer<vtkRenderer> renderer;
  renderer.TakeReference(prm->MakeRenderer());
  renderer->AddActor(actor);

  vtkSmartPointer<vtkRenderWindow> renwin;
  renwin.TakeReference(prm->MakeRenderWindow());
  renwin->SetSize(300, 300);
  renwin->SetPosition(0, 200 * controller->GetLocalProcessId());
  renwin->AddRenderer(renderer);

  prm->SetRenderWindow(renwin);
  prm->SetController(controller);
  prm->InitializePieces();
  prm->InitializeOffScreen(); // Mesa GL only

  if (controller->GetLocalProcessId() == 0)
  {
    renwin->Render();

    // Do the test comparison.
    int retval = vtkRegressionTestImage(renwin);
    if (retval == vtkRegressionTester::DO_INTERACTOR)
    {
      vtkNew<vtkRenderWindowInteractor> iren;
      iren->SetRenderWindow(renwin);
      iren->Initialize();
      iren->Start();
      retval = vtkRegressionTester::PASSED;
    }

    *(args->retval) = (retval == vtkRegressionTester::PASSED) ? 0 : 1;

    prm->StopServices();
  }
  else // not root node
  {
    prm->StartServices();
  }

  controller->Broadcast(args->retval, 1, 0);
}

//=============================================================================
int PLSDynaReader(int argc, char* argv[])
{
  int retval = 1;

  vtkNew<vtkMPIController> controller;
  controller->Initialize(&argc, &argv);

  vtkMultiProcessController::SetGlobalController(controller);

  TestArgs args;
  args.retval = &retval;
  args.argc = argc;
  args.argv = argv;

  controller->SetSingleMethod(PLSDynaReader, &args);
  controller->SingleMethodExecute();

  controller->Finalize();

  return retval;
}

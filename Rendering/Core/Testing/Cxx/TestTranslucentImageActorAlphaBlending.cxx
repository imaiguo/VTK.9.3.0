// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// This test covers rendering of translucent image actor with alpha blending.
//
// The command line arguments are:
// -I        => run in interactive mode; unless this is used, the program will
//              not allow interaction and exit

#include "vtkRegressionTestImage.h"
#include "vtkTestUtilities.h"

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkImageActor.h"
#include "vtkImageMapper3D.h"
#include "vtkPNGReader.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

int TestTranslucentImageActorAlphaBlending(int argc, char* argv[])
{
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  iren->SetRenderWindow(renWin);
  renWin->Delete();

  vtkRenderer* renderer = vtkRenderer::New();
  renWin->AddRenderer(renderer);
  renderer->Delete();

  vtkImageActor* ia = vtkImageActor::New();
  renderer->AddActor(ia);
  ia->Delete();

  vtkPNGReader* pnmReader = vtkPNGReader::New();
  ia->GetMapper()->SetInputConnection(pnmReader->GetOutputPort());
  pnmReader->Delete();

  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/alphachannel.png");

  pnmReader->SetFileName(fname);
  delete[] fname;

  renderer->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(400, 400);

  renWin->Render();
  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }
  iren->Delete();

  return !retVal;
}

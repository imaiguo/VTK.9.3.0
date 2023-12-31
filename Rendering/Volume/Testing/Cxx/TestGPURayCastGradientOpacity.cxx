// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// This code volume renders the torso dataset and tests the gradient opacity
// function support for volume mappers

#include "vtkCamera.h"
#include "vtkColorTransferFunction.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkMetaImageReader.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTestUtilities.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"

int TestGPURayCastGradientOpacity(int argc, char* argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetSize(400, 401);
  renWin->SetMultiSamples(0);

  vtkNew<vtkRenderer> ren1;
  ren1->SetViewport(0.0, 0.0, 0.5, 1.0);
  renWin->AddRenderer(ren1);
  vtkNew<vtkRenderer> ren2;
  ren2->SetViewport(0.5, 0.0, 1.0, 1.0);
  renWin->AddRenderer(ren2);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  vtkNew<vtkInteractorStyleTrackballCamera> style;
  iren->SetInteractorStyle(style);

  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/HeadMRVolume.mhd");

  vtkNew<vtkMetaImageReader> reader;
  reader->SetFileName(fname);
  reader->Update();
  delete[] fname;

  vtkNew<vtkGPUVolumeRayCastMapper> mapper1;
  mapper1->SetInputConnection(reader->GetOutputPort());
  vtkNew<vtkGPUVolumeRayCastMapper> mapper2;
  mapper2->SetInputConnection(reader->GetOutputPort());

  vtkNew<vtkColorTransferFunction> ctf;
  ctf->AddHSVPoint(1.0, 0.095, 0.33, 0.82);
  ctf->AddHSVPoint(53.3, 0.04, 0.7, 0.63);
  ctf->AddHSVPoint(256, 0.095, 0.33, 0.82);

  vtkNew<vtkPiecewiseFunction> pwf;
  pwf->AddPoint(0.0, 0.0);
  pwf->AddPoint(4.48, 0.0);
  pwf->AddPoint(43.116, 1.0);
  pwf->AddPoint(641.0, 1.0);

  vtkNew<vtkPiecewiseFunction> gf;
  gf->AddPoint(10, 0.0);
  gf->AddPoint(70, 1.0);

  vtkNew<vtkVolumeProperty> volumeProperty1;
  volumeProperty1->SetScalarOpacity(pwf);
  volumeProperty1->SetColor(ctf);
  volumeProperty1->SetDisableGradientOpacity(1);
  volumeProperty1->ShadeOn();

  vtkNew<vtkVolume> volume1;
  volume1->SetMapper(mapper1);
  volume1->SetProperty(volumeProperty1);
  ren1->AddVolume(volume1);
  volume1->RotateX(-20);
  ren1->ResetCamera();
  ren1->GetActiveCamera()->Zoom(2.2);

  vtkNew<vtkVolumeProperty> volumeProperty2;
  volumeProperty2->SetScalarOpacity(pwf);
  volumeProperty2->SetColor(ctf);
  volumeProperty2->SetGradientOpacity(gf);
  volumeProperty2->SetDisableGradientOpacity(0);
  volumeProperty2->ShadeOn();

  vtkNew<vtkVolume> volume2;
  volume2->SetMapper(mapper2);
  volume2->SetProperty(volumeProperty2);
  volume2->RotateX(-20);
  ren2->AddVolume(volume2);
  ren2->ResetCamera();
  ren2->GetActiveCamera()->Zoom(2.2);

  renWin->Render();

  int retVal = vtkTesting::Test(argc, argv, renWin, 90);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  return !((retVal == vtkTesting::PASSED) || (retVal == vtkTesting::DO_INTERACTOR));
}

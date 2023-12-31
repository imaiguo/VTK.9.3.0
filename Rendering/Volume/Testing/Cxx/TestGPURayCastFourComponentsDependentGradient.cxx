// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

// Description
// This is a test for volume rendering using the GPU ray cast
// mapper of a dataset with four components treating them as
// dependent and applying a gradient opacity function

#include "vtkCamera.h"
#include "vtkColorTransferFunction.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTestUtilities.h"
#include "vtkTesting.h"
#include "vtkVolumeProperty.h"
#include "vtkXMLImageDataReader.h"

int TestGPURayCastFourComponentsDependentGradient(int argc, char* argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;

  char* cfname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/vase_4comp.vti");

  vtkNew<vtkXMLImageDataReader> reader;
  reader->SetFileName(cfname);
  delete[] cfname;

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetSize(301, 300); // Intentional NPOT size
  renWin->SetMultiSamples(0);

  vtkNew<vtkRenderer> ren;
  renWin->AddRenderer(ren);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  renWin->Render();

  // Volume render the dataset
  vtkNew<vtkGPUVolumeRayCastMapper> mapper;
  mapper->AutoAdjustSampleDistancesOff();
  mapper->SetSampleDistance(0.5);
  mapper->SetInputConnection(reader->GetOutputPort());

  // Opacity transfer function
  vtkNew<vtkPiecewiseFunction> pf;
  pf->AddPoint(0, 0);
  pf->AddPoint(255, 1);

  // Gradient opacity transfer function
  vtkNew<vtkPiecewiseFunction> pf1;
  pf1->AddPoint(30, 0);
  pf1->AddPoint(255, 1);

  // Volume property with independent components OFF
  vtkNew<vtkVolumeProperty> property;
  property->IndependentComponentsOff();
  property->SetScalarOpacity(pf);
  property->SetGradientOpacity(pf1);

  vtkNew<vtkVolume> volume;
  volume->SetMapper(mapper);
  volume->SetProperty(property);
  ren->AddVolume(volume);

  ren->ResetCamera();
  renWin->Render();

  iren->Initialize();

  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  return !retVal;
}

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// This test volume tests whether updating the volume MTime updates the ,
// geometry in the volume mapper.

#include <vtkColorTransferFunction.h>
#include <vtkContourValues.h>
#include <vtkDataArray.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNew.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRegressionTestImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTestUtilities.h>
#include <vtkTesting.h>
#include <vtkVolumeProperty.h>
#include <vtkXMLImageDataReader.h>

int TestGPURayCastVolumeDepthPass(int argc, char* argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;

  double scalarRange[2];

  vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;

  vtkNew<vtkXMLImageDataReader> reader;
  char* volumeFile = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/vase_1comp.vti");
  reader->SetFileName(volumeFile);
  delete[] volumeFile;

  // This is the feature we are testing
  volumeMapper->UseDepthPassOn();
  volumeMapper->GetDepthPassContourValues()->SetValue(0, 50.0);

  // Set other parameters
  volumeMapper->SetInputConnection(reader->GetOutputPort());
  volumeMapper->GetInput()->GetScalarRange(scalarRange);
  volumeMapper->SetSampleDistance(0.1);
  volumeMapper->SetAutoAdjustSampleDistances(0);
  volumeMapper->SetBlendModeToComposite();

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetMultiSamples(0);
  renWin->SetSize(400, 400);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);
  vtkNew<vtkInteractorStyleTrackballCamera> style;
  iren->SetInteractorStyle(style);

  renWin->Render(); // make sure we have an OpenGL context.

  vtkNew<vtkRenderer> ren;
  ren->SetBackground(0.2, 0.2, 0.5);
  renWin->AddRenderer(ren);

  vtkNew<vtkPiecewiseFunction> scalarOpacity;
  scalarOpacity->AddPoint(50, 0.0);
  scalarOpacity->AddPoint(75, 1.0);

  vtkNew<vtkVolumeProperty> volumeProperty;
  volumeProperty->ShadeOn();
  volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
  volumeProperty->SetScalarOpacity(scalarOpacity);

  vtkNew<vtkColorTransferFunction> colorTransferFunction;
  colorTransferFunction->RemoveAllPoints();
  colorTransferFunction->AddRGBPoint(scalarRange[0], 0.6, 0.4, 0.1);
  volumeProperty->SetColor(colorTransferFunction);

  vtkNew<vtkVolume> volume;
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  ren->AddVolume(volume);
  ren->ResetCamera();

  int valid = volumeMapper->IsRenderSupported(renWin, volumeProperty);

  int retVal;
  if (valid)
  {
    renWin->Render();

    iren->Initialize();

    retVal = vtkRegressionTestImage(renWin);
    if (retVal == vtkRegressionTester::DO_INTERACTOR)
    {
      iren->Start();
    }

    return !retVal;
  }
  else
  {
    retVal = vtkTesting::PASSED;
    cout << "Required extensions not supported" << endl;
  }

  return !retVal;
}

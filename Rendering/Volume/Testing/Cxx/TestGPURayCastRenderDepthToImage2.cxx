// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

// Description
// Test the GPU volume mapper low level API to render depth buffer to texture

#include "vtkActor.h"
#include "vtkColorTransferFunction.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapToColors.h"
#include "vtkImageMapper3D.h"
#include "vtkLookupTable.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRTAnalyticSource.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTestUtilities.h"
#include "vtkVolumeProperty.h"

int TestGPURayCastRenderDepthToImage2(int argc, char* argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;

  vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;
  vtkNew<vtkRTAnalyticSource> waveletSource;
  volumeMapper->SetInputConnection(waveletSource->GetOutputPort());
  volumeMapper->RenderToImageOn();
  volumeMapper->SetClampDepthToBackface(1);

  vtkNew<vtkColorTransferFunction> colorFunction;
  colorFunction->AddRGBPoint(37.35310363769531, 0.231373, 0.298039, 0.752941);
  colorFunction->AddRGBPoint(157.0909652709961, 0.865003, 0.865003, 0.865003);
  colorFunction->AddRGBPoint(276.8288269042969, 0.705882, 0.0156863, 0.14902);

  float dataRange[2];
  dataRange[0] = 37.3;
  dataRange[1] = 276.8;
  float halfSpread = (dataRange[1] - dataRange[0]) / 2.0;
  float center = dataRange[0] + halfSpread;

  vtkNew<vtkPiecewiseFunction> scalarOpacity;
  scalarOpacity->RemoveAllPoints();
  scalarOpacity->AddPoint(center, 0.0);
  scalarOpacity->AddPoint(dataRange[1], 0.4);

  vtkNew<vtkVolumeProperty> volumeProperty;
  volumeProperty->ShadeOn();
  volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
  volumeProperty->SetColor(colorFunction);
  volumeProperty->SetScalarOpacity(scalarOpacity);

  // Setup volume actor
  vtkNew<vtkVolume> volume;
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  // Testing prefers image comparison with small images
  vtkNew<vtkRenderWindow> renWin;

  // Intentional odd and NPOT  width/height
  renWin->SetSize(401, 399);

  vtkNew<vtkRenderer> ren;
  renWin->AddRenderer(ren);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  ren->AddVolume(volume);
  ren->ResetCamera();
  renWin->Render();

  vtkNew<vtkImageData> im;

  // Get color texture as image
  volumeMapper->GetColorImage(im);

  // Get depth texture as image
  volumeMapper->GetDepthImage(im);

  // Create a grayscale lookup table
  vtkNew<vtkLookupTable> lut;
  lut->SetRange(0.0, 1.0);
  lut->SetValueRange(0.0, 1.0);
  lut->SetSaturationRange(0.0, 0.0);
  lut->SetRampToLinear();
  lut->Build();

  // Map the pixel values of the image with the lookup table
  vtkNew<vtkImageMapToColors> imageMap;
  imageMap->SetInputData(im);
  imageMap->SetLookupTable(lut);

  // Render the image in the scene
  vtkNew<vtkImageActor> ia;
  ia->GetMapper()->SetInputConnection(imageMap->GetOutputPort());
  ren->AddActor(ia);
  ren->RemoveVolume(volume);
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

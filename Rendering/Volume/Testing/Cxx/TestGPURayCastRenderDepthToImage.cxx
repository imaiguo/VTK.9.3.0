// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

// Description
// Test the GPU volume mapper low level API to render depth buffer to texture

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkColorTransferFunction.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapToColors.h"
#include "vtkImageMapper3D.h"
#include "vtkLookupTable.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTestUtilities.h"
#include "vtkVolume16Reader.h"
#include "vtkVolumeProperty.h"

int TestGPURayCastRenderDepthToImage(int argc, char* argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;

  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/headsq/quarter");

  vtkNew<vtkVolume16Reader> reader;
  reader->SetDataDimensions(64, 64);
  reader->SetDataByteOrderToLittleEndian();
  reader->SetImageRange(1, 93);
  reader->SetDataSpacing(3.2, 3.2, 1.5);
  reader->SetFilePrefix(fname);
  reader->SetDataMask(0x7fff);

  delete[] fname;

  vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;
  volumeMapper->SetInputConnection(reader->GetOutputPort());
  volumeMapper->RenderToImageOn();

  vtkNew<vtkColorTransferFunction> colorFunction;
  colorFunction->AddRGBPoint(900.0, 198 / 255.0, 134 / 255.0, 66 / 255.0);

  vtkNew<vtkPiecewiseFunction> scalarOpacity;
  scalarOpacity->AddPoint(0, 0.0);
  scalarOpacity->AddPoint(70, 0.0);
  scalarOpacity->AddPoint(449, 0.0);
  scalarOpacity->AddPoint(900, 0.15);
  scalarOpacity->AddPoint(1120, 0.25);
  scalarOpacity->AddPoint(1404, 0.35);
  scalarOpacity->AddPoint(4095, 0.5);

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
  ren->GetActiveCamera()->Azimuth(90);
  ren->GetActiveCamera()->Roll(90);
  ren->GetActiveCamera()->Azimuth(-90);
  ren->ResetCamera();
  ren->GetActiveCamera()->Zoom(1.8);
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
  ren->GetActiveCamera()->SetPosition(0, 0, -1);
  ren->GetActiveCamera()->SetFocalPoint(0, 0, 1);
  ren->GetActiveCamera()->SetViewUp(0, 1, 0);
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

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// This is a test to make sure graphics resources are properly released and
// re-allocated when the context changes

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkCommand.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include <vtkPolyDataMapper.h>
#include <vtkRegressionTestImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTestUtilities.h>
#include <vtkTimerLog.h>
#include <vtkVolumeProperty.h>
#include <vtkXMLImageDataReader.h>

int TestGPURayCastReleaseResources(int argc, char* argv[])
{
  double scalarRange[2];

  vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;

  vtkNew<vtkXMLImageDataReader> reader;
  const char* volumeFile = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/vase_1comp.vti");

  reader->SetFileName(volumeFile);
  reader->Update();
  volumeMapper->SetInputConnection(reader->GetOutputPort());

  delete[] volumeFile;

  volumeMapper->GetInput()->GetScalarRange(scalarRange);
  volumeMapper->SetBlendModeToComposite();

  // Testing prefers image comparison with small images
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  // Intentional odd and NPOT  width/height
  renWin->SetSize(500, 401);

  vtkNew<vtkRenderer> ren;
  renWin->AddRenderer(ren);

  vtkNew<vtkPiecewiseFunction> scalarOpacity;
  scalarOpacity->AddPoint(scalarRange[0], 0.0);
  scalarOpacity->AddPoint(scalarRange[1], 1.0);

  vtkNew<vtkVolumeProperty> volumeProperty;
  volumeProperty->ShadeOff();
  volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
  volumeProperty->SetScalarOpacity(scalarOpacity);

  vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
    volumeProperty->GetRGBTransferFunction(0);
  colorTransferFunction->RemoveAllPoints();
  colorTransferFunction->AddRGBPoint(scalarRange[0], 0.1, 0.5, 1.0);
  colorTransferFunction->AddRGBPoint(scalarRange[1], 1.0, 0.5, 0.1);

  // Setup volume actor
  vtkNew<vtkVolume> volume;
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  ren->AddViewProp(volume);
  ren->GetActiveCamera()->Azimuth(-40);
  ren->ResetCamera();
  renWin->Render();

  // Delete the old render window to release graphics resources
  renWin->Delete();

  vtkNew<vtkRenderWindow> renWin2;
  renWin2->SetSize(300, 401);
  renWin2->AddRenderer(ren);
  renWin2->Render();

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin2);

  iren->Initialize();

  int retVal = vtkRegressionTestImage(renWin2);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  return !retVal;
}

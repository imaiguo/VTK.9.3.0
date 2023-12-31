// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkGL2PSExporter.h"
#include "vtkRegressionTestImage.h"
#include "vtkTestUtilities.h"

#include "vtkCamera.h"
#include "vtkColorTransferFunction.h"
#include "vtkCone.h"
#include "vtkCubeAxesActor2D.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkImageShiftScale.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPointData.h"
#include "vtkProperty.h"
#include "vtkProperty2D.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSampleFunction.h"
#include "vtkSmartPointer.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkTestingInteractor.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"

#include <string>

int TestGL2PSExporterVolumeRaster(int, char*[])
{
  vtkNew<vtkCone> coneFunction;
  vtkNew<vtkSampleFunction> coneSample;
  coneSample->SetImplicitFunction(coneFunction);
  coneSample->SetOutputScalarTypeToFloat();
  coneSample->SetSampleDimensions(127, 127, 127);
  coneSample->SetModelBounds(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  coneSample->SetCapping(false);
  coneSample->SetComputeNormals(false);
  coneSample->SetScalarArrayName("volume");
  coneSample->Update();

  vtkNew<vtkImageShiftScale> coneShift;
  double range[2];
  coneSample->GetOutput()->GetPointData()->GetScalars("volume")->GetRange(range);
  coneShift->SetInputConnection(coneSample->GetOutputPort());
  coneShift->SetShift(-range[0]);
  double mag = range[1] - range[0];
  if (mag == 0.0)
  {
    mag = 1.0;
  }
  coneShift->SetScale(255.0 / mag);
  coneShift->SetOutputScalarTypeToUnsignedChar();
  coneShift->Update();

  vtkNew<vtkSmartVolumeMapper> coneMapper;
  coneMapper->SetInputConnection(coneShift->GetOutputPort());
  coneMapper->SetBlendModeToComposite();
  coneMapper->SetRequestedRenderModeToRayCast();

  vtkNew<vtkVolumeProperty> volProp;
  volProp->ShadeOff();
  volProp->SetInterpolationTypeToLinear();

  vtkNew<vtkPiecewiseFunction> opacity;
  opacity->AddPoint(0.0, 0.9);
  opacity->AddPoint(20.0, 0.9);
  opacity->AddPoint(40.0, 0.3);
  opacity->AddPoint(90.0, 0.8);
  opacity->AddPoint(100.1, 0.0);
  opacity->AddPoint(255.0, 0.0);
  volProp->SetScalarOpacity(opacity);

  vtkNew<vtkColorTransferFunction> color;
  color->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
  color->AddRGBPoint(20.0, 0.0, 1.0, 1.0);
  color->AddRGBPoint(40.0, 0.5, 0.0, 1.0);
  color->AddRGBPoint(80.0, 1.0, 0.2, 0.3);
  color->AddRGBPoint(255.0, 1.0, 1.0, 1.0);
  volProp->SetColor(color);

  vtkNew<vtkVolume> coneVolume;
  coneVolume->SetMapper(coneMapper);
  coneVolume->SetProperty(volProp);

  vtkNew<vtkCubeAxesActor2D> axes;
  axes->SetInputConnection(coneShift->GetOutputPort());
  axes->SetFontFactor(2.0);
  axes->SetCornerOffset(0.0);
  axes->GetProperty()->SetColor(0.0, 0.0, 0.0);

  vtkNew<vtkRenderer> ren;
  axes->SetCamera(ren->GetActiveCamera());
  ren->AddActor(coneVolume);
  ren->AddActor(axes);
  ren->SetBackground(0.2, 0.3, 0.5);

  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  vtkSmartPointer<vtkCamera> camera = ren->GetActiveCamera();
  ren->ResetCamera();
  camera->Azimuth(30);

  renWin->SetSize(500, 500);
  renWin->Render();

  vtkNew<vtkGL2PSExporter> exp;
  exp->SetRenderWindow(renWin);
  exp->SetFileFormatToPS();
  exp->CompressOff();
  exp->SetSortToBSP();
  exp->DrawBackgroundOn();
  exp->Write3DPropsAsRasterImageOn();

  std::string fileprefix =
    vtkTestingInteractor::TempDirectory + std::string("/TestGL2PSExporterVolumeRaster");

  exp->SetFilePrefix(fileprefix.c_str());
  exp->Write();

  exp->SetFileFormatToPDF();
  exp->Write();

  renWin->SetMultiSamples(0);
  renWin->GetInteractor()->Initialize();
  renWin->GetInteractor()->Start();

  return EXIT_SUCCESS;
}

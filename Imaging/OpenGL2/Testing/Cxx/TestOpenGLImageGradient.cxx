// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkNew.h"

#include "vtkCamera.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkImageProperty.h"
#include "vtkImageReader2.h"
#include "vtkImageSlice.h"
#include "vtkImageSliceMapper.h"
#include "vtkInteractorStyleImage.h"
#include "vtkOpenGLImageGradient.h"
#include "vtkPointData.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "vtkRegressionTestImage.h"
#include "vtkTestUtilities.h"

int TestOpenGLImageGradient(int argc, char* argv[])
{
  vtkNew<vtkRenderWindowInteractor> iren;
  vtkNew<vtkInteractorStyleImage> style;
  style->SetInteractionModeToImageSlicing();
  vtkNew<vtkRenderWindow> renWin;
  iren->SetRenderWindow(renWin);
  iren->SetInteractorStyle(style);

  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/headsq/quarter");

  vtkNew<vtkImageReader2> reader;
  reader->SetDataByteOrderToLittleEndian();
  reader->SetDataExtent(0, 63, 0, 63, 1, 93);
  reader->SetDataSpacing(3.2, 3.2, 1.5);
  reader->SetFilePrefix(fname);

  delete[] fname;

  vtkNew<vtkOpenGLImageGradient> filter;
  //  vtkNew<vtkImageGradient> filter;
  filter->SetInputConnection(reader->GetOutputPort());
  filter->Update();
  // double *rnger = filter->GetOutput()->GetPointData()->GetScalars()->GetRange();

  vtkNew<vtkImageSliceMapper> imageMapper;
  imageMapper->SetInputConnection(filter->GetOutputPort());
  imageMapper->SetOrientation(2);
  imageMapper->SliceAtFocalPointOn();

  vtkNew<vtkImageSlice> image;
  image->SetMapper(imageMapper);

  double range[2] = { -100, 100 };

  image->GetProperty()->SetColorWindow(range[1] - range[0]);
  image->GetProperty()->SetColorLevel(0.5 * (range[0] + range[1]));
  image->GetProperty()->SetInterpolationTypeToNearest();

  vtkNew<vtkRenderer> renderer;
  renderer->AddViewProp(image);
  renderer->SetBackground(0.2, 0.3, 0.4);
  renWin->AddRenderer(renderer);

  const double* bounds = imageMapper->GetBounds();
  double point[3];
  point[0] = 0.5 * (bounds[0] + bounds[1]);
  point[1] = 0.5 * (bounds[2] + bounds[3]);
  point[2] = 0.5 * (bounds[4] + bounds[5]);

  vtkCamera* camera = renderer->GetActiveCamera();
  camera->SetFocalPoint(point);
  point[imageMapper->GetOrientation()] += 500.0;
  camera->SetPosition(point);
  if (imageMapper->GetOrientation() == 2)
  {
    camera->SetViewUp(0.0, 1.0, 0.0);
  }
  else
  {
    camera->SetViewUp(0.0, 0.0, -1.0);
  }
  camera->ParallelProjectionOn();
  camera->SetParallelScale(0.8 * 128);

  renWin->SetSize(512, 512);
  iren->Initialize();
  renWin->Render();

  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  return !retVal;
}

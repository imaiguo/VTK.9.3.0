// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// Test the Border variable on ImageResliceMapper
//
// The command line arguments are:
// -I        => run in interactive mode

#include "vtkRegressionTestImage.h"
#include "vtkTestUtilities.h"

#include "vtkCamera.h"
#include "vtkImageClip.h"
#include "vtkImageData.h"
#include "vtkImageProperty.h"
#include "vtkImageResliceMapper.h"
#include "vtkImageSlice.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTIFFReader.h"

int TestImageResliceMapperBackground(int argc, char* argv[])
{
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
  vtkInteractorStyle* style = vtkInteractorStyleImage::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  iren->SetRenderWindow(renWin);
  iren->SetInteractorStyle(style);
  renWin->Delete();
  style->Delete();

  vtkTIFFReader* reader = vtkTIFFReader::New();

  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/beach.tif");

  reader->SetFileName(fname);
  delete[] fname;

  vtkImageClip* clip = vtkImageClip::New();
  clip->SetInputConnection(reader->GetOutputPort());
  clip->SetOutputWholeExtent(100, 107, 100, 107, 0, 0);

  for (int i = 0; i < 4; i++)
  {
    vtkRenderer* renderer = vtkRenderer::New();
    vtkCamera* camera = renderer->GetActiveCamera();
    renderer->SetBackground(0.1, 0.2, 0.4);
    renderer->SetViewport(0.5 * (i & 1), 0.25 * (i & 2), 0.5 + 0.5 * (i & 1), 0.5 + 0.25 * (i & 2));
    renWin->AddRenderer(renderer);
    renderer->Delete();

    vtkImageResliceMapper* imageMapper = vtkImageResliceMapper::New();
    imageMapper->SetInputConnection(clip->GetOutputPort());

    const double* bounds = imageMapper->GetBounds();
    double point[3];
    point[0] = 0.5 * (bounds[0] + bounds[1]);
    point[1] = 0.5 * (bounds[2] + bounds[3]);
    point[2] = 0.5 * (bounds[4] + bounds[5]);

    camera->SetFocalPoint(point);
    point[2] += 500.0;
    camera->SetPosition(point);
    camera->ParallelProjectionOn();
    camera->SetParallelScale(5.0);

    vtkImageSlice* image = vtkImageSlice::New();
    image->SetMapper(imageMapper);
    imageMapper->Delete();
    renderer->AddViewProp(image);
    image->GetMapper()->BackgroundOn();
    image->GetMapper()->SliceFacesCameraOn();

    if ((i & 1))
    {
      imageMapper->ResampleToScreenPixelsOff();
    }
    if ((i & 2))
    {
      imageMapper->SeparateWindowLevelOperationOn();
    }

    image->GetProperty()->SetColorWindow(255.0);
    image->GetProperty()->SetColorLevel(127.5);

    image->Delete();
  }

  renWin->SetSize(400, 400);

  renWin->Render();
  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }
  iren->Delete();

  clip->Delete();
  reader->Delete();

  return !retVal;
}

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2004 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

// Simple test of vtkFastSplatter

#include "vtkFastSplatter.h"
#include "vtkImageData.h"
#include "vtkImageShiftScale.h"
#include "vtkImageViewer2.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "vtkSmartPointer.h"
#define VTK_CREATE(type, name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include <cmath>

const int SPLAT_IMAGE_SIZE = 100;

int FastSplatter(int, char*[])
{
  // For the purposes of this example we'll build the splat image by
  // hand.

  VTK_CREATE(vtkImageData, SplatImage);
  SplatImage->SetDimensions(SPLAT_IMAGE_SIZE, SPLAT_IMAGE_SIZE, 1);
  SplatImage->AllocateScalars(VTK_FLOAT, 1);

  for (int i = 0; i < SPLAT_IMAGE_SIZE; ++i)
  {
    for (int j = 0; j < SPLAT_IMAGE_SIZE; ++j)
    {
      double xCoord = 1 - fabs((i - SPLAT_IMAGE_SIZE / 2) / (SPLAT_IMAGE_SIZE / 2.0));
      double yCoord = 1 - fabs((j - SPLAT_IMAGE_SIZE / 2) / (SPLAT_IMAGE_SIZE / 2.0));

      SplatImage->SetScalarComponentFromDouble(i, j, 0, 0, xCoord * yCoord);
    }
  }

  VTK_CREATE(vtkPolyData, SplatPoints);
  VTK_CREATE(vtkPoints, Points);

  Points->SetNumberOfPoints(5);
  double point[3];

  point[0] = 0;
  point[1] = 0;
  point[2] = 0;
  Points->SetPoint(0, point);

  point[0] = 1;
  point[1] = 1;
  point[2] = 0;
  Points->SetPoint(1, point);

  point[0] = -1;
  point[1] = 1;
  point[2] = 0;
  Points->SetPoint(2, point);

  point[0] = 1;
  point[1] = -1;
  point[2] = 0;
  Points->SetPoint(3, point);

  point[0] = -1;
  point[1] = -1;
  point[2] = 0;
  Points->SetPoint(4, point);

  SplatPoints->SetPoints(Points);

  VTK_CREATE(vtkFastSplatter, splatter);
  splatter->SetInputData(SplatPoints);
  splatter->SetOutputDimensions(2 * SPLAT_IMAGE_SIZE, 2 * SPLAT_IMAGE_SIZE, 1);
  splatter->SetInputData(1, SplatImage);

  // The image viewers and writers are only happy with unsigned char
  // images.  This will convert the floats into that format.
  VTK_CREATE(vtkImageShiftScale, resultScale);
  resultScale->SetOutputScalarTypeToUnsignedChar();
  resultScale->SetShift(0);
  resultScale->SetScale(255);
  resultScale->SetInputConnection(splatter->GetOutputPort());

  splatter->Update();
  resultScale->Update();

  // Set up a viewer for the image.  vtkImageViewer and
  // vtkImageViewer2 are convenient wrappers around vtkActor2D,
  // vtkImageMapper, vtkRenderer, and vtkRenderWindow.  All you need
  // to supply is the interactor and hooray, Bob's your uncle.
  VTK_CREATE(vtkImageViewer2, ImageViewer);
  ImageViewer->SetInputConnection(resultScale->GetOutputPort());
  ImageViewer->SetColorLevel(127);
  ImageViewer->SetColorWindow(255);

  VTK_CREATE(vtkRenderWindowInteractor, iren);
  ImageViewer->SetupInteractor(iren);

  ImageViewer->Render();
  ImageViewer->GetRenderer()->ResetCamera();

  iren->Initialize();
  ImageViewer->Render();
  iren->Start();

  return EXIT_SUCCESS;
}

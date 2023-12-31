// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
#include "vtkSmartPointer.h"
#include "vtkWordCloud.h"

#include <vtkCamera.h>
#include <vtkImageViewer2.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <iostream>

int TestWordCloud(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << "filename" << std::endl;
    return EXIT_FAILURE;
  }
  vtkWordCloud::OffsetDistributionContainer offset;
  offset[0] = 0;
  offset[1] = 0;
  auto wordCloud = vtkSmartPointer<vtkWordCloud>::New();
  wordCloud->SetFileName(argv[1]);
  wordCloud->SetOffsetDistribution(offset);
  wordCloud->SetFontFileName(argv[2]);
  wordCloud->AddOrientation(0.0);
  wordCloud->AddOrientation(90.0);
  wordCloud->Update();
  std::cout << "File" << argv[1] << std::endl;
  std::cout << "Font" << argv[2] << std::endl;
  std::cout << "Kept Words: " << wordCloud->GetKeptWords().size() << std::endl;
  std::cout << "Stopped Words: " << wordCloud->GetStoppedWords().size() << std::endl;
  std::cout << "Skipped Words: " << wordCloud->GetSkippedWords().size() << std::endl;

  // Display the final image
  auto colors = vtkSmartPointer<vtkNamedColors>::New();
  auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

  auto imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
  imageViewer->SetInputData(wordCloud->GetOutput());
  imageViewer->SetupInteractor(interactor);
  imageViewer->GetRenderer()->SetBackground(colors->GetColor3d("Wheat").GetData());
  imageViewer->SetSize(wordCloud->GetSizes()[0], wordCloud->GetSizes()[1]);
  imageViewer->GetRenderer()->ResetCamera();

  // Zoom in a bit
  vtkCamera* camera = imageViewer->GetRenderer()->GetActiveCamera();
  camera->ParallelProjectionOn();
  camera->SetParallelScale(wordCloud->GetAdjustedSizes()[0] * .4);

  imageViewer->GetRenderWindow()->Render();
  interactor->Start();

  return EXIT_SUCCESS;
}

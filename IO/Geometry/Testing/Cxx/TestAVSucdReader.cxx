// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include <vtkAVSucdReader.h>
#include <vtkDataSetMapper.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>

#include <vtkRegressionTestImage.h>
#include <vtkTestUtilities.h>

int TestAVSucdReader(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Required parameters: <filename>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename = argv[1];

  vtkNew<vtkAVSucdReader> reader;
  reader->SetFileName(filename.c_str());
  reader->Update();
  reader->Print(std::cout);
  reader->GetOutput()->Print(std::cout);

  vtkUnstructuredGrid* grid = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput());
  grid->GetPointData()->SetActiveScalars("temperature");

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(reader->GetOutput());
  mapper->ScalarVisibilityOn();
  mapper->SetScalarRange(grid->GetPointData()->GetScalars()->GetRange());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->EdgeVisibilityOn();

  vtkNew<vtkRenderer> ren;
  ren->AddActor(actor);
  ren->SetBackground(0, 0, 0);

  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren);
  renWin->SetSize(300, 300);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  renWin->Render();
  int r = vtkRegressionTestImage(renWin);
  if (r == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }
  return !r;
}

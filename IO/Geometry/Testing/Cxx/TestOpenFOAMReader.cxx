// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkOpenFOAMReader.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

#include <vtkRegressionTestImage.h>
#include <vtkTestUtilities.h>

int TestOpenFOAMReader(int argc, char* argv[])
{
  // Read file name.
  char* filename =
    vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/OpenFOAM/cavity/cavity.foam");

  // Read the file
  vtkSmartPointer<vtkOpenFOAMReader> reader = vtkSmartPointer<vtkOpenFOAMReader>::New();
  reader->SetFileName(filename);
  delete[] filename;
  reader->Update();
  reader->SetTimeValue(.5);
  //  reader->CreateCellToPointOn();
  reader->ReadZonesOn();
  reader->Update();
  reader->Print(std::cout);
  reader->GetOutput()->Print(std::cout);
  reader->GetOutput()->GetBlock(0)->Print(std::cout);

  // 1) Default array settings
  int numberOfCellArrays = reader->GetNumberOfCellArrays();
  std::cout << "----- Default array settings" << std::endl;
  for (int i = 0; i < numberOfCellArrays; ++i)
  {
    const char* name = reader->GetCellArrayName(i);
    std::cout << "  Cell Array: " << i << " is named " << name << " and is "
              << (reader->GetCellArrayStatus(name) ? "Enabled" : "Disabled") << std::endl;
  }

  int numberOfPointArrays = reader->GetNumberOfPointArrays();
  std::cout << "----- Default array settings" << std::endl;
  for (int i = 0; i < numberOfPointArrays; ++i)
  {
    const char* name = reader->GetPointArrayName(i);
    std::cout << "  Point Array: " << i << " is named " << name << " and is "
              << (reader->GetPointArrayStatus(name) ? "Enabled" : "Disabled") << std::endl;
  }

  int numberOfLagrangianArrays = reader->GetNumberOfLagrangianArrays();
  std::cout << "----- Default array settings" << std::endl;
  for (int i = 0; i < numberOfLagrangianArrays; ++i)
  {
    const char* name = reader->GetLagrangianArrayName(i);
    std::cout << "  Lagrangian Array: " << i << " is named " << name << " and is "
              << (reader->GetLagrangianArrayStatus(name) ? "Enabled" : "Disabled") << std::endl;
  }

  int numberOfPatchArrays = reader->GetNumberOfPatchArrays();
  std::cout << "----- Default array settings" << std::endl;
  for (int i = 0; i < numberOfPatchArrays; ++i)
  {
    const char* name = reader->GetPatchArrayName(i);
    std::cout << "  Patch Array: " << i << " is named " << name << " and is "
              << (reader->GetPatchArrayStatus(name) ? "Enabled" : "Disabled") << std::endl;
  }

  vtkUnstructuredGrid* block0 = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput()->GetBlock(0));
  block0->GetCellData()->SetActiveScalars("p");
  std::cout << "Scalar range: " << block0->GetCellData()->GetScalars()->GetRange()[0] << ", "
            << block0->GetCellData()->GetScalars()->GetRange()[1] << std::endl;

  // Visualize
  vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputData(block0);
  mapper->SetScalarRange(block0->GetScalarRange());

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(.2, .4, .6);

  renderWindow->Render();

  int retVal = vtkRegressionTestImage(renderWindow);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    renderWindowInteractor->Start();
  }

  return !retVal;
}

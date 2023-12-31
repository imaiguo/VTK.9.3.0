// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkConeSource.h>
#include <vtkDoubleArray.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProjectedTetrahedraMapper.h>
#include <vtkProp3D.h>
#include <vtkProperty.h>
#include <vtkRectilinearGrid.h>
#include <vtkRectilinearGridToTetrahedra.h>
#include <vtkRegressionTestImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTesting.h>
#include <vtkTransform.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVolumeProperty.h>
#include <vtkWindowToImageFilter.h>

// Description
// Tests off-screen rendering of vtkProjectedTetrahedra.

// Creates a cube volume
vtkSmartPointer<vtkVolume> CubeVolume_TetrahedraOffscreen(double r, double g, double b)
{
  // Create the coordinates
  vtkNew<vtkDoubleArray> xArray;
  xArray->InsertNextValue(0.0);
  xArray->InsertNextValue(1.0);
  vtkNew<vtkDoubleArray> yArray;
  yArray->InsertNextValue(0.0);
  yArray->InsertNextValue(1.0);
  vtkNew<vtkDoubleArray> zArray;
  zArray->InsertNextValue(0.0);
  zArray->InsertNextValue(1.0);

  // Create the RectilinearGrid
  vtkNew<vtkRectilinearGrid> grid;
  grid->SetDimensions(2, 2, 2);
  grid->SetXCoordinates(xArray);
  grid->SetYCoordinates(yArray);
  grid->SetZCoordinates(zArray);

  // Obtain an UnstructuredGrid made of tetrahedras
  vtkNew<vtkRectilinearGridToTetrahedra> rectilinearGridToTetrahedra;
  rectilinearGridToTetrahedra->SetInputData(grid);
  rectilinearGridToTetrahedra->Update();

  vtkSmartPointer<vtkUnstructuredGrid> ugrid = rectilinearGridToTetrahedra->GetOutput();

  // Add scalars to the grid
  vtkNew<vtkDoubleArray> scalars;
  for (int i = 0; i < 8; i++)
  {
    scalars->InsertNextValue(0);
  }
  ugrid->GetPointData()->SetScalars(scalars);

  // Volume Rendering Mapper
  vtkNew<vtkProjectedTetrahedraMapper> mapper;
  mapper->SetInputData(ugrid);
  mapper->Update();

  // Create the volume
  vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper(mapper);

  // Apply a ColorTransferFunction to the volume
  vtkNew<vtkColorTransferFunction> colorTransferFunction;
  colorTransferFunction->AddRGBPoint(0.0, r, g, b);
  volume->GetProperty()->SetColor(colorTransferFunction);

  return volume;
}

// Creates a cone actor
vtkSmartPointer<vtkActor> ConeActor_TetrahedraOffscreen(double r, double g, double b)
{
  // Simple cone mapper
  vtkNew<vtkPolyDataMapper> mapper;
  vtkNew<vtkConeSource> coneSource;
  coneSource->SetCenter(0.0, 0.0, 0.0);
  mapper->SetInputConnection(coneSource->GetOutputPort());

  // Create the actor
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->GetProperty()->SetColor(r, g, b);
  actor->SetMapper(mapper);

  return actor;
}

int TestProjectedTetrahedraOffscreen(int argc, char* argv[])
{
  // Create the props

  // The red cube volume
  vtkSmartPointer<vtkProp3D> volume1 = CubeVolume_TetrahedraOffscreen(1, 0, 0);

  // The blue cube volume
  vtkSmartPointer<vtkProp3D> volume2 = CubeVolume_TetrahedraOffscreen(0, 0, 1);

  // The red cone actor
  vtkSmartPointer<vtkProp3D> actor1 = ConeActor_TetrahedraOffscreen(1, 0, 0);

  // Translate the blue props by (2,2)
  vtkNew<vtkTransform> transform;
  transform->Translate(2, 2, 0);
  volume2->SetUserTransform(transform);

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->SetMultiSamples(0);
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(300, 300);

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Render dummy scene on-screen
  renderWindow->SetOffScreenRendering(false);
  renderer->SetBackground(1, 1, 1);
  renderer->AddVolume(volume1);
  renderer->AddVolume(volume2);
  renderWindow->Render();

  renderer->RemoveVolume(volume1);
  renderer->RemoveVolume(volume2);

  // Render off-screen and grab the rendered image
  renderWindow->SetOffScreenRendering(true);
  renderer->SetBackground(0.4, 0.8, 0.4);
  renderer->AddVolume(volume2);
  renderer->AddActor(actor1);
  renderWindow->Render();
  renderer->ResetCamera();

  vtkNew<vtkWindowToImageFilter> win2image;
  win2image->SetInput(renderWindow);
  win2image->Update();
  vtkImageData* offScreenImage = win2image->GetOutput();

  renderer->RemoveVolume(volume2);
  renderer->RemoveActor(actor1);

  renderWindow->SetOffScreenRendering(false);
  renderWindow->Finalize();
  renderWindow->Start();

  // Render on-screen a texture map of the off-screen rendered image
  vtkNew<vtkImageActor> ia;
  ia->GetMapper()->SetInputData(offScreenImage);
  renderer->AddActor(ia);
  renderer->SetBackground(0, 0, 0);

  renderer->GetActiveCamera()->SetPosition(0, 0, -1);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 1);
  renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
  renderer->ResetCamera();
  renderWindow->Render();

  int retVal = vtkTesting::Test(argc, argv, renderWindow, 20);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    renderWindowInteractor->Start();
  }

  return !retVal;
}

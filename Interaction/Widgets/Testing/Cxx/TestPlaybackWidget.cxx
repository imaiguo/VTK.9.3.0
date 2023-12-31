// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
//
// This example tests the vtkPlaybackWidget.

// First include the required header files for the VTK classes we are using.
#include "vtkSmartPointer.h"

#include "vtkActor.h"
#include "vtkCommand.h"
#include "vtkInteractorEventRecorder.h"
#include "vtkPlaybackRepresentation.h"
#include "vtkPlaybackWidget.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"

class vtkSubclassPlaybackRepresentation : public vtkPlaybackRepresentation
{
public:
  static vtkSubclassPlaybackRepresentation* New();
  void Play() override { std::cout << "play\n"; }
  void Stop() override { std::cout << "stop\n"; }
  void ForwardOneFrame() override { std::cout << "forward one frame\n"; }
  void BackwardOneFrame() override { std::cout << "backward one frame\n"; }
  void JumpToBeginning() override { std::cout << "jump to beginning\n"; }
  void JumpToEnd() override { std::cout << "jump to end\n"; }
};

vtkStandardNewMacro(vtkSubclassPlaybackRepresentation);

int TestPlaybackWidget(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  // Create the RenderWindow, Renderer and both Actors
  //
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);

  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  // Create a test pipeline
  //
  vtkSmartPointer<vtkSphereSource> ss = vtkSmartPointer<vtkSphereSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(ss->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // Create the widget
  vtkSmartPointer<vtkSubclassPlaybackRepresentation> rep =
    vtkSmartPointer<vtkSubclassPlaybackRepresentation>::New();

  vtkSmartPointer<vtkPlaybackWidget> widget = vtkSmartPointer<vtkPlaybackWidget>::New();
  widget->SetInteractor(iren);
  widget->SetRepresentation(rep);

  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(actor);
  ren1->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(300, 300);

  // record events
  vtkSmartPointer<vtkInteractorEventRecorder> recorder =
    vtkSmartPointer<vtkInteractorEventRecorder>::New();
  recorder->SetInteractor(iren);
  recorder->SetFileName("c:/record.log");
  //  recorder->Record();
  //  recorder->ReadFromInputStringOn();
  //  recorder->SetInputString(eventLog);

  // render the image
  //
  iren->Initialize();
  renWin->Render();
  widget->On();
  //  recorder->Play();

  // Remove the observers so we can go interactive. Without this the "-I"
  // testing option fails.
  recorder->Off();

  iren->Start();

  return EXIT_SUCCESS;
}

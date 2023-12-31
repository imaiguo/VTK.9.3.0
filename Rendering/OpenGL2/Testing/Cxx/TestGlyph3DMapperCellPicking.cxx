// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

// This tests vtkVisibleCellSelector, vtkExtractSelectedFrustum,
// vtkRenderedAreaPicker, and vtkInteractorStyleRubberBandPick.
//
// The command line arguments are:
// -I        => run in interactive mode; unless this is used, the program will
//              not allow interaction and exit

#include "vtkRegressionTestImage.h"
#include "vtkTestUtilities.h"

#include "vtkActor.h"
#include "vtkBitArray.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkElevationFilter.h"
#include "vtkGlyph3DMapper.h"
#include "vtkHardwareSelector.h"
#include "vtkIdTypeArray.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleRubberBandPick.h"
#include "vtkPlaneSource.h"
#include "vtkPointData.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderedAreaPicker.h"
#include "vtkRenderer.h"
#include "vtkSelection.h"
#include "vtkSelectionNode.h"
#include "vtkSphereSource.h"
#include <cassert>

static vtkRenderer* renderer = nullptr;

class MyEndPickCommand : public vtkCommand
{
public:
  MyEndPickCommand()
  {
    this->Renderer = nullptr; // no reference counting
    this->Mask = nullptr;     // no reference counting
    this->DataSet = nullptr;
  }

  ~MyEndPickCommand() override = default;

  void Execute(vtkObject* vtkNotUsed(caller), unsigned long vtkNotUsed(eventId),
    void* vtkNotUsed(callData)) override
  {
    assert("pre: renderer_exists" && this->Renderer != nullptr);

    vtkHardwareSelector* sel = vtkHardwareSelector::New();
    sel->SetFieldAssociation(vtkDataObject::FIELD_ASSOCIATION_CELLS);
    sel->SetRenderer(renderer);

    double x0 = renderer->GetPickX1();
    double y0 = renderer->GetPickY1();
    double x1 = renderer->GetPickX2();
    double y1 = renderer->GetPickY2();
    sel->SetArea(static_cast<unsigned int>(x0), static_cast<unsigned int>(y0),
      static_cast<unsigned int>(x1), static_cast<unsigned int>(y1));

    vtkSelection* res = sel->Select();

#if 1
    cerr << "x0 " << x0 << " y0 " << y0 << "\t";
    cerr << "x1 " << x1 << " y1 " << y1 << endl;
    res->Print(cout);
#endif

    // Reset the mask to false.
    vtkIdType numPoints = this->Mask->GetNumberOfTuples();
    for (vtkIdType i = 0; i < numPoints; i++)
    {
      this->Mask->SetValue(i, false);
    }

    vtkSelectionNode* glyphids = res->GetNode(0);
    if (glyphids != nullptr)
    {
      vtkAbstractArray* abs = glyphids->GetSelectionList();
      if (abs == nullptr)
      {
        cout << "abs is null" << endl;
      }
      vtkIdTypeArray* ids = vtkArrayDownCast<vtkIdTypeArray>(abs);
      if (ids == nullptr)
      {
        cout << "ids is null" << endl;
      }
      else
      {
        // modify mask array with selection.
        vtkIdType numSelPoints = ids->GetNumberOfTuples();
        for (vtkIdType i = 0; i < numSelPoints; i++)
        {
          vtkIdType value = ids->GetValue(i);
          if (value >= 0 && value < numPoints)
          {
            cout << "Turn On: " << value << endl;
            this->Mask->SetValue(value, true);
          }
          else
          {
            cout << "Ignoring: " << value << endl;
          }
        }
      }
    }
    this->DataSet->Modified();

    sel->Delete();
    res->Delete();
  }

  void SetRenderer(vtkRenderer* r) { this->Renderer = r; }

  vtkRenderer* GetRenderer() const { return this->Renderer; }

  void SetMask(vtkBitArray* m) { this->Mask = m; }
  void SetDataSet(vtkDataSet* ds) { this->DataSet = ds; }

protected:
  vtkRenderer* Renderer;
  vtkBitArray* Mask;
  vtkDataSet* DataSet;
};

int TestGlyph3DMapperCellPicking(int argc, char* argv[])
{
  int res = 1;
  vtkPlaneSource* plane = vtkPlaneSource::New();
  plane->SetResolution(res, res);
  vtkElevationFilter* colors = vtkElevationFilter::New();
  colors->SetInputConnection(plane->GetOutputPort());
  plane->Delete();
  colors->SetLowPoint(-1, -1, -1);
  colors->SetHighPoint(0.5, 0.5, 0.5);

  vtkSphereSource* squad = vtkSphereSource::New();
  squad->SetPhiResolution(4);
  squad->SetThetaResolution(6);

  vtkGlyph3DMapper* glypher = vtkGlyph3DMapper::New();
  glypher->SetInputConnection(colors->GetOutputPort());
  colors->Delete();
  glypher->SetScaleFactor(1.5);
  glypher->SetSourceConnection(squad->GetOutputPort());
  squad->Delete();

  // selection is performed on actor1
  vtkActor* glyphActor1 = vtkActor::New();
  glyphActor1->SetMapper(glypher);
  glypher->Delete();
  glyphActor1->PickableOn();

  // result of selection is on actor2
  vtkActor* glyphActor2 = vtkActor::New();
  glyphActor2->PickableOff();
  colors->Update(); // make sure output is valid.
  vtkDataSet* selection = colors->GetOutput()->NewInstance();
  selection->ShallowCopy(colors->GetOutput());

  vtkBitArray* selectionMask = vtkBitArray::New();
  selectionMask->SetName("mask");
  selectionMask->SetNumberOfComponents(1);
  selectionMask->SetNumberOfTuples(selection->GetNumberOfPoints());
  // Initially, everything is selected
  vtkIdType i = 0;
  vtkIdType c = selectionMask->GetNumberOfTuples();
  while (i < c)
  {
    selectionMask->SetValue(i, true);
    ++i;
  }
  selection->GetPointData()->AddArray(selectionMask);
  selectionMask->Delete();

  vtkGlyph3DMapper* glypher2 = vtkGlyph3DMapper::New();
  //  glypher->SetNestedDisplayLists(0);
  glypher2->SetMasking(true);
  glypher2->SetMaskArray("mask");

  glypher2->SetInputData(selection);
  glypher2->SetScaleFactor(1.5);
  glypher2->SetSourceConnection(squad->GetOutputPort());
  glyphActor2->SetMapper(glypher2);
  glypher2->Delete();

  // Standard rendering classes
  renderer = vtkRenderer::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  renWin->AddRenderer(renderer);
  renWin->SetMultiSamples(0);
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  // set up the view
  renderer->SetBackground(0.2, 0.2, 0.2);
  renWin->SetSize(600, 300);

  // use the rubber band pick interactor style
  vtkRenderWindowInteractor* rwi = renWin->GetInteractor();
  vtkInteractorStyleRubberBandPick* rbp = vtkInteractorStyleRubberBandPick::New();
  rwi->SetInteractorStyle(rbp);

  vtkRenderedAreaPicker* areaPicker = vtkRenderedAreaPicker::New();
  rwi->SetPicker(areaPicker);

  renderer->AddActor(glyphActor1);
  renderer->AddActor(glyphActor2);
  glyphActor2->SetPosition(2, 0, 0);
  glyphActor1->Delete();
  glyphActor2->Delete();

  // pass pick events to the VisibleGlyphSelector
  MyEndPickCommand* cbc = new MyEndPickCommand;
  cbc->SetRenderer(renderer);
  cbc->SetMask(selectionMask);
  cbc->SetDataSet(selection);
  rwi->AddObserver(vtkCommand::EndPickEvent, cbc);
  cbc->Delete();

  ////////////////////////////////////////////////////////////

  // run the test

  renderer->ResetCamera();
  renderer->GetActiveCamera()->Zoom(2.0);

  renWin->Render();
  // areaPicker->AreaPick(0, 0, 241, 160, renderer);
  areaPicker->AreaPick(233, 120, 241, 160, renderer);
  cbc->Execute(nullptr, 0, nullptr);
  renWin->Render();

  int retVal = vtkRegressionTestImage(renWin);
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  // Cleanup
  renderer->Delete();
  renWin->Delete();
  iren->Delete();
  rbp->Delete();
  areaPicker->Delete();
  selection->Delete();
  return !retVal;
}

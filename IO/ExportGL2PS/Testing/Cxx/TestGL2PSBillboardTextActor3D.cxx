// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

// Note: The output of this test will be different on OpenGL2 -- all text will
// be rendered atop the polygonal geometry. This is because we do not yet
// export 3D geometry as vector primitives in OpenGL2, but rather rasterize it
// into the background.
//
// OpenGL1 should mix the text with the geometry.

#include "vtkBillboardTextActor3D.h"

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkGL2PSExporter.h"
#include "vtkNew.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTestingInteractor.h"
#include "vtkTextProperty.h"
#include "vtkUnsignedCharArray.h"

#include <sstream>
#include <vector>

namespace vtkTestGL2PSBillboardTextActor3D
{
void setupBillboardTextActor3D(vtkBillboardTextActor3D* actor, vtkPolyData* anchor)
{
  vtkTextProperty* p = actor->GetTextProperty();
  std::ostringstream label;
  label << "TProp Angle: " << p->GetOrientation() << "\n"
        << "HAlign: " << p->GetJustificationAsString() << "\n"
        << "VAlign: " << p->GetVerticalJustificationAsString();
  actor->SetInput(label.str().c_str());

  // Add the anchor point:
  double* pos = actor->GetPosition();
  double* col = p->GetColor();
  vtkIdType ptId = anchor->GetPoints()->InsertNextPoint(pos[0], pos[1], pos[2]);
  anchor->GetVerts()->InsertNextCell(1, &ptId);
  anchor->GetCellData()->GetScalars()->InsertNextTuple4(
    col[0] * 255, col[1] * 255, col[2] * 255, 255);
}

void setupGrid(vtkPolyData* grid)
{
  double marks[4] = { 0., 200., 400., 600. };
  double thickness = 200.;

  vtkNew<vtkPoints> points;
  grid->SetPoints(points);
  for (int x_i = 0; x_i < 4; ++x_i)
  {
    for (int y_i = 0; y_i < 4; ++y_i)
    {
      points->InsertNextPoint(marks[x_i], marks[y_i], -thickness / 2.);
      points->InsertNextPoint(marks[x_i], marks[y_i], +thickness / 2.);
    }
  }

  std::vector<vtkIdType> quads;

  for (vtkIdType col = 0; col < 4; ++col)
  {
    for (int row = 0; row < 3; ++row)
    {
      // Along y:
      vtkIdType base = 8 * col + 2 * row;
      quads.push_back(base + 0);
      quads.push_back(base + 1);
      quads.push_back(base + 3);
      quads.push_back(base + 2);
    }
  }

  vtkNew<vtkCellArray> cellArray;
  grid->SetPolys(cellArray);
  for (size_t i = 0; i < quads.size(); i += 4)
  {
    grid->InsertNextCell(VTK_QUAD, 4, &quads[i]);
  }
}
} // end namespace vtkTestGL2PSBillboardTextActor3D

//------------------------------------------------------------------------------
int TestGL2PSBillboardTextActor3D(int, char*[])
{
  using namespace vtkTestGL2PSBillboardTextActor3D;
  vtkNew<vtkRenderer> ren;
  ren->UseDepthPeelingOn();

  int width = 600;
  int height = 600;
  int x[3] = { 100, 300, 500 };
  int y[3] = { 100, 300, 500 };

  // Render the anchor points to check alignment:
  vtkNew<vtkPolyData> anchors;
  vtkNew<vtkPoints> points;
  anchors->SetPoints(points);
  vtkNew<vtkCellArray> verts;
  anchors->SetVerts(verts);
  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetNumberOfComponents(4);
  anchors->GetCellData()->SetScalars(colors);

  for (size_t row = 0; row < 3; ++row)
  {
    for (size_t col = 0; col < 3; ++col)
    {
      vtkNew<vtkBillboardTextActor3D> actor;
      switch (row)
      {
        case 0:
          actor->GetTextProperty()->SetJustificationToRight();
          break;
        case 1:
          actor->GetTextProperty()->SetJustificationToCentered();
          break;
        case 2:
          actor->GetTextProperty()->SetJustificationToLeft();
          break;
      }
      switch (col)
      {
        case 0:
          actor->GetTextProperty()->SetVerticalJustificationToBottom();
          break;
        case 1:
          actor->GetTextProperty()->SetVerticalJustificationToCentered();
          break;
        case 2:
          actor->GetTextProperty()->SetVerticalJustificationToTop();
          break;
      }
      actor->GetTextProperty()->SetFontSize(20);
      actor->GetTextProperty()->SetOrientation(45.0 * (3 * row + col));
      actor->GetTextProperty()->SetColor(0.75, .2 + col * .26, .2 + row * .26);
      actor->GetTextProperty()->SetBackgroundColor(0., 1. - col * .26, 1. - row * .26);
      actor->GetTextProperty()->SetBackgroundOpacity(0.85);
      actor->SetPosition(x[col], y[row], 0.);
      setupBillboardTextActor3D(actor, anchors);
      ren->AddActor(actor);
    }
  }

  vtkNew<vtkPolyDataMapper> anchorMapper;
  anchorMapper->SetInputData(anchors);
  vtkNew<vtkActor> anchorActor;
  anchorActor->SetMapper(anchorMapper);
  anchorActor->GetProperty()->SetPointSize(5);
  ren->AddActor(anchorActor);

  // Add some various 'empty' actors to make sure there are no surprises:
  vtkNew<vtkBillboardTextActor3D> nullInputActor;
  nullInputActor->SetInput(nullptr);
  ren->AddActor(nullInputActor);

  vtkNew<vtkBillboardTextActor3D> emptyInputActor;
  emptyInputActor->SetInput("");
  ren->AddActor(emptyInputActor);

  vtkNew<vtkBillboardTextActor3D> spaceActor;
  spaceActor->SetInput(" ");
  ren->AddActor(spaceActor);

  vtkNew<vtkBillboardTextActor3D> tabActor;
  tabActor->SetInput("\t");
  ren->AddActor(tabActor);

  vtkNew<vtkBillboardTextActor3D> newlineActor;
  newlineActor->SetInput("\n");
  ren->AddActor(newlineActor);

  vtkNew<vtkPolyData> grid;
  setupGrid(grid);
  vtkNew<vtkPolyDataMapper> gridMapper;
  gridMapper->SetInputData(grid);
  vtkNew<vtkActor> gridActor;
  gridActor->GetProperty()->SetRepresentationToSurface();
  gridActor->GetProperty()->SetColor(0.6, 0.6, 0.6);
  gridActor->SetMapper(gridMapper);
  ren->AddActor(gridActor);

  vtkNew<vtkRenderWindow> win;
  win->AddRenderer(ren);
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(win);

  ren->SetBackground(0.0, 0.0, 0.0);
  ren->GetActiveCamera()->SetPosition(width / 2, height / 2, 1400);
  ren->GetActiveCamera()->SetFocalPoint(width / 2, height / 2, 0);
  ren->GetActiveCamera()->SetViewUp(0, 1, 0);
  ren->GetActiveCamera()->Roll(45.);
  ren->GetActiveCamera()->Elevation(45.);
  ren->ResetCameraClippingRange();
  win->SetSize(width, height);
  win->Render();

  vtkNew<vtkGL2PSExporter> exp;
  exp->SetRenderWindow(win);
  exp->SetFileFormatToPS();
  exp->CompressOff();
  exp->SetSortToSimple();
  exp->DrawBackgroundOn();
  exp->Write3DPropsAsRasterImageOff();
  exp->TextAsPathOn();

  std::string fileprefix =
    vtkTestingInteractor::TempDirectory + std::string("/TestGL2PSBillboardTextActor3D");

  exp->SetFilePrefix(fileprefix.c_str());
  exp->Write();

  // Finally render the scene and compare the image to a reference image
  win->SetMultiSamples(0);
  win->GetInteractor()->Initialize();
  win->GetInteractor()->Start();

  return EXIT_SUCCESS;
}

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// This example demonstrates how to implement a vtkGenericDataSet
// (here vtkBridgeDataSet) and to use vtkGenericDataSetTessellator filter on
// it.
//
// The command line arguments are:
// -I        => run in interactive mode; unless this is used, the program will
//              not allow interaction and exit
// -D <path> => path to the data; the data should be in <path>/Data/

#include "vtkActor.h"
#include "vtkArrowSource.h"
#include "vtkAttributesErrorMetric.h"
#include "vtkBridgeDataSet.h"
#include "vtkDebugLeaks.h"
#include "vtkGenericCellTessellator.h"
#include "vtkGenericGeometryFilter.h"
#include "vtkGenericGlyph3DFilter.h"
#include "vtkGenericSubdivisionErrorMetric.h"
#include "vtkGeometricErrorMetric.h"
#include "vtkLookupTable.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSimpleCellTessellator.h"
#include "vtkTestUtilities.h"
#include "vtkUnstructuredGrid.h"
#include "vtkXMLUnstructuredGridReader.h"
#include <cassert>

int TestGenericGlyph3DFilter(int argc, char* argv[])
{
  // Standard rendering classes
  vtkRenderer* renderer = vtkRenderer::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  renWin->AddRenderer(renderer);
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  // Load the mesh geometry and data from a file
  vtkXMLUnstructuredGridReader* reader = vtkXMLUnstructuredGridReader::New();
  char* cfname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/quadraticTetra01.vtu");
  reader->SetFileName(cfname);
  delete[] cfname;

  // Force reading
  reader->Update();

  // Initialize the bridge
  vtkBridgeDataSet* ds = vtkBridgeDataSet::New();
  ds->SetDataSet(reader->GetOutput());
  reader->Delete();

  // Set the error metric thresholds:
  // 1. for the geometric error metric
  vtkGeometricErrorMetric* geometricError = vtkGeometricErrorMetric::New();
  geometricError->SetRelativeGeometricTolerance(0.1, ds);

  ds->GetTessellator()->GetErrorMetrics()->AddItem(geometricError);
  geometricError->Delete();

  // 2. for the attribute error metric
  vtkAttributesErrorMetric* attributesError = vtkAttributesErrorMetric::New();
  attributesError->SetAttributeTolerance(0.01);

  ds->GetTessellator()->GetErrorMetrics()->AddItem(attributesError);
  attributesError->Delete();
  cout << "input unstructured grid: " << ds << endl;

  static_cast<vtkSimpleCellTessellator*>(ds->GetTessellator())->SetMaxSubdivisionLevel(10);

  vtkIndent indent;
  ds->PrintSelf(cout, indent);

  // Create the filter
  vtkArrowSource* arrow = vtkArrowSource::New();
  vtkGenericGlyph3DFilter* glyph = vtkGenericGlyph3DFilter::New();
  glyph->SetInputData(ds);
  glyph->SetInputConnection(1, arrow->GetOutputPort());
  glyph->SetScaling(1);
  glyph->SetScaleModeToScaleByScalar();
  glyph->SelectInputScalars("scalars");
  glyph->SetColorModeToColorByScale();

  vtkPolyDataMapper* glyphMapper = vtkPolyDataMapper::New();
  glyphMapper->SetInputConnection(glyph->GetOutputPort());
  vtkActor* glyphActor = vtkActor::New();
  glyphActor->SetMapper(glyphMapper);
  renderer->AddActor(glyphActor);

  // Create the filter
  vtkGenericGeometryFilter* geom = vtkGenericGeometryFilter::New();
  geom->SetInputData(ds);

  geom->Update(); // So that we can call GetRange() on the scalars

  assert(geom->GetOutput() != nullptr);

  // This creates a blue to red lut.
  vtkLookupTable* lut = vtkLookupTable::New();
  lut->SetHueRange(0.667, 0.0);

  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
  mapper->SetLookupTable(lut);
  mapper->SetInputConnection(geom->GetOutputPort());

  if (geom->GetOutput()->GetPointData() != nullptr)
  {
    if (geom->GetOutput()->GetPointData()->GetScalars() != nullptr)
    {
      mapper->SetScalarRange(geom->GetOutput()->GetPointData()->GetScalars()->GetRange());
    }
  }

  vtkActor* actor = vtkActor::New();
  actor->SetMapper(mapper);
  renderer->AddActor(actor);

  // Standard testing code.
  renderer->SetBackground(0.5, 0.5, 0.5);
  renWin->SetSize(300, 300);
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
  mapper->Delete();
  actor->Delete();
  geom->Delete();
  ds->Delete();
  lut->Delete();
  glyphActor->Delete();
  glyphMapper->Delete();
  glyph->Delete();
  arrow->Delete();

  return !retVal;
}

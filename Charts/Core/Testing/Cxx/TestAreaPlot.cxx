// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkAxis.h"
#include "vtkBrush.h"
#include "vtkCharArray.h"
#include "vtkChartXY.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkFloatArray.h"
#include "vtkNew.h"
#include "vtkPlot.h"
#include "vtkPlotArea.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkTable.h"

#include <algorithm>
//------------------------------------------------------------------------------
int TestAreaPlot(int, char*[])
{
  // Set up a 2D scene, add an XY chart to it
  vtkNew<vtkContextView> view;
  view->GetRenderWindow()->SetSize(400, 300);
  vtkNew<vtkChartXY> chart;
  view->GetScene()->AddItem(chart);

  // Create a table with some points in it...
  vtkNew<vtkTable> table;
  vtkNew<vtkFloatArray> arrX;
  arrX->SetName("X Axis");
  table->AddColumn(arrX);
  vtkNew<vtkFloatArray> arrC;
  arrC->SetName("Cosine");
  table->AddColumn(arrC);
  vtkNew<vtkFloatArray> arrS;
  arrS->SetName("Sine");
  table->AddColumn(arrS);
  vtkNew<vtkFloatArray> arrS2;
  arrS2->SetName("Sine2");
  table->AddColumn(arrS2);
  vtkNew<vtkFloatArray> arrS3;
  arrS3->SetName("Sine3");
  table->AddColumn(arrS3);
  vtkNew<vtkFloatArray> arr1;
  arr1->SetName("One");
  table->AddColumn(arr1);

  vtkNew<vtkCharArray> validMask;
  validMask->SetName("ValidMask");
  table->AddColumn(validMask);

  // Test charting with a few more points...
  int numPoints = 69;
  float inc = 7.5 / (numPoints - 1);
  table->SetNumberOfRows(numPoints);
  for (int i = 0; i < numPoints; ++i)
  {
    table->SetValue(i, 0, i * inc + 0.01);
    table->SetValue(i, 1, cos(i * inc) + 0.01);
    table->SetValue(i, 2, sin(i * inc) + 0.01);
    table->SetValue(i, 3, sin(i * inc) + 0.5);
    table->SetValue(i, 4, sin(i * inc) * sin(i * inc) + 0.01);
    table->SetValue(i, 5, 1.0);

    validMask->SetValue(i, (i > 30 && i < 40) ? 0 : 1);
  }

  // Add multiple line plots, setting the colors etc
  vtkPlotArea* area = vtkPlotArea::SafeDownCast(chart->AddPlot(vtkChart::AREA));
  area->SetInputData(table);
  area->SetInputArray(0, "X Axis");
  area->SetInputArray(1, "Sine");
  area->SetInputArray(2, "Sine2");
  area->SetValidPointMaskName("ValidMask");
  area->GetBrush()->SetColorF(0.5, 0.5, 0.5, 0.5);
  chart->GetAxis(vtkAxis::LEFT)->SetLogScale(true);

  // Render the scene and compare the image to a reference image
  view->GetRenderWindow()->SetMultiSamples(0);
  view->GetInteractor()->Initialize();
  view->GetInteractor()->Start();

  return EXIT_SUCCESS;
}

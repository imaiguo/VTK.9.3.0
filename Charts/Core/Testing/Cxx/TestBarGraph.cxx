// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkChartXY.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkIntArray.h"
#include "vtkNew.h"
#include "vtkPlot.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTable.h"

// Monthly circulation data
static int data_2008[] = { 10822, 10941, 9979, 10370, 9460, 11228, 15093, 12231, 10160, 9816, 9384,
  7892 };
static int data_2009[] = { 9058, 9474, 9979, 9408, 8900, 11569, 14688, 12231, 10294, 9585, 8957,
  8590 };
static int data_2010[] = { 9058, 10941, 9979, 10270, 8900, 11228, 14688, 12231, 10160, 9585, 9384,
  8590 };

//------------------------------------------------------------------------------
int TestBarGraph(int, char*[])
{
  // Set up a 2D scene, add an XY chart to it
  vtkNew<vtkContextView> view;
  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
  view->GetRenderWindow()->SetSize(400, 300);
  vtkNew<vtkChartXY> chart;
  view->GetScene()->AddItem(chart);

  // Create a table with some points in it...
  vtkNew<vtkTable> table;

  vtkNew<vtkIntArray> arrMonth;
  arrMonth->SetName("Month");
  table->AddColumn(arrMonth);

  vtkNew<vtkIntArray> arr2008;
  arr2008->SetName("2008");
  table->AddColumn(arr2008);

  vtkNew<vtkIntArray> arr2009;
  arr2009->SetName("2009");
  table->AddColumn(arr2009);

  vtkNew<vtkIntArray> arr2010;
  arr2010->SetName("2010");
  table->AddColumn(arr2010);

  table->SetNumberOfRows(12);
  for (int i = 0; i < 12; i++)
  {
    table->SetValue(i, 0, i + 1);
    table->SetValue(i, 1, data_2008[i]);
    table->SetValue(i, 2, data_2009[i]);
    table->SetValue(i, 3, data_2010[i]);
  }

  // Add multiple bar plots, setting the colors etc
  vtkPlot* plot = nullptr;

  plot = chart->AddPlot(vtkChart::BAR);
  plot->SetInputData(table, 0, 1);
  plot->SetColor(0, 255, 0, 255);

  plot = chart->AddPlot(vtkChart::BAR);
  plot->SetInputData(table, 0, 2);
  plot->SetColor(255, 0, 0, 255);

  plot = chart->AddPlot(vtkChart::BAR);
  plot->SetInputData(table, 0, 3);
  plot->SetColor(0, 0, 255, 255);

  // Finally render the scene and compare the image to a reference image
  view->GetRenderWindow()->SetMultiSamples(0);
  view->GetInteractor()->Initialize();
  view->GetInteractor()->Start();

  return EXIT_SUCCESS;
}

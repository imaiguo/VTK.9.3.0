// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkAxis.h"
#include "vtkChartXYZ.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkFloatArray.h"
#include "vtkPlotPoints3D.h"

#include "vtkCallbackCommand.h"
#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTable.h"

// Need a timer so that we can animate, and then take a snapshot!
namespace
{
double angle = 0;

void ProcessEvents(vtkObject* caller, unsigned long, void* clientData, void* callerData)
{
  vtkChartXYZ* chart = reinterpret_cast<vtkChartXYZ*>(clientData);
  vtkRenderWindowInteractor* interactor = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
  angle += 2;
  chart->SetAngle(angle);
  interactor->Render();
  if (angle >= 90)
  {
    int timerId = *reinterpret_cast<int*>(callerData);
    interactor->DestroyTimer(timerId);
  }
}
} // End of anonymous namespace.

int TestChartXYZ(int, char*[])
{
  // Now the chart
  vtkNew<vtkChartXYZ> chart;
  chart->SetAutoRotate(true);
  chart->SetFitToScene(false);
  chart->SetDecorateAxes(false);
  vtkNew<vtkContextView> view;
  view->GetRenderWindow()->SetSize(400, 300);
  view->GetScene()->AddItem(chart);
  vtkNew<vtkChartXYZ> chart2;
  chart2->SetAutoRotate(true);
  chart2->SetFitToScene(false);
  chart->SetDecorateAxes(false);
  view->GetScene()->AddItem(chart2);

  chart->SetGeometry(vtkRectf(75.0, 20.0, 250, 260));
  chart2->SetGeometry(vtkRectf(75.0, 20.0, 250, 260));

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
  // Test charting with a few more points...
  int numPoints = 69;
  float inc = 7.5 / (numPoints - 1);
  table->SetNumberOfRows(numPoints);
  for (int i = 0; i < numPoints; ++i)
  {
    table->SetValue(i, 0, i * inc);
    table->SetValue(i, 1, cos(i * inc) + 0.0);
    table->SetValue(i, 2, sin(i * inc) + 0.0);
  }

  // chart->SetAroundX(true);
  // Add the three dimensions we are interested in visualizing.
  vtkNew<vtkPlotPoints3D> plot;
  plot->SetInputData(table, "X Axis", "Sine", "Cosine");
  chart->AddPlot(plot);
  const vtkColor4ub axisColor(20, 200, 30);
  chart->SetAxisColor(axisColor);
  chart->GetAxis(0)->SetUnscaledRange(-0.1, 7.6);
  chart->GetAxis(1)->SetUnscaledRange(-1.1, 1.1);
  chart->GetAxis(2)->SetUnscaledRange(-1.1, 1.1);
  chart->RecalculateTransform();

  // We want a duplicate, that does not move.
  vtkNew<vtkPlotPoints3D> plot2;
  plot2->SetInputData(table, "X Axis", "Sine", "Cosine");
  chart2->AddPlot(plot2);

  chart2->GetAxis(0)->SetUnscaledRange(-0.1, 7.6);
  chart2->GetAxis(1)->SetUnscaledRange(-1.1, 1.1);
  chart2->GetAxis(2)->SetUnscaledRange(-1.1, 1.1);
  chart2->RecalculateTransform();

  view->GetRenderWindow()->SetMultiSamples(0);
  view->GetInteractor()->Initialize();

  // Set up the timer, and be sure to incrememt the angle.
  vtkNew<vtkCallbackCommand> callback;
  callback->SetClientData(chart);
  callback->SetCallback(::ProcessEvents);
  view->GetInteractor()->AddObserver(vtkCommand::TimerEvent, callback, 0);
  view->GetInteractor()->CreateRepeatingTimer(1000 / 25);

  view->GetInteractor()->Start();

  return EXIT_SUCCESS;
}

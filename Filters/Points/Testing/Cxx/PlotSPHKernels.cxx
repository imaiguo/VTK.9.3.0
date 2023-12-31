// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// Plot the SPH kernel functions and derivatives.

#include "vtkSPHCubicKernel.h"
#include "vtkSPHQuarticKernel.h"
#include "vtkSPHQuinticKernel.h"
#include "vtkSmartPointer.h"
#include "vtkWendlandQuinticKernel.h"

#include <vtkAxis.h>
#include <vtkChartXY.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkFloatArray.h>
#include <vtkPen.h>
#include <vtkPlot.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTable.h>

//------------------------------------------------------------------------------
// Helper function
template <class T>
void AddKernelToPlot(vtkSmartPointer<T> kernel, vtkSmartPointer<vtkChartXY> chart,
  const std::string& description, vtkSmartPointer<vtkTable> table, unsigned char rgb[3])
{
  double r, width = 3.5;
  int res = 100;
  double inc = width / static_cast<double>(res);
  double fVal, dVal;

  int numCols = table->GetNumberOfColumns();
  numCols = (numCols < 1 ? 1 : numCols);

  vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
  arrX->SetName("X Axis");
  arrX->SetNumberOfValues(res);
  table->AddColumn(arrX);

  vtkSmartPointer<vtkFloatArray> arrC = vtkSmartPointer<vtkFloatArray>::New();
  arrC->SetName(description.c_str());
  arrC->SetNumberOfValues(res);
  table->AddColumn(arrC);

  std::string deriv = description + "_deriv";
  vtkSmartPointer<vtkFloatArray> arrS = vtkSmartPointer<vtkFloatArray>::New();
  arrS->SetName(deriv.c_str());
  arrS->SetNumberOfValues(res);
  table->AddColumn(arrS);

  // Fill in the table with function values
  table->SetNumberOfRows(res);
  for (int i = 0; i < res; ++i)
  {
    r = static_cast<double>(i) * inc;
    fVal = kernel->GetNormFactor() * kernel->ComputeFunctionWeight(r);
    dVal = kernel->GetNormFactor() * kernel->ComputeDerivWeight(r);

    table->SetValue(i, 0, r);
    table->SetValue(i, numCols, fVal);
    table->SetValue(i, numCols + 1, dVal);
  }

  vtkPlot* line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, numCols);
  line->SetColor(rgb[0], rgb[1], rgb[2], 255);
  line->SetWidth(1.0);

  line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, numCols + 1);
  line->SetColor(rgb[0], rgb[1], rgb[2], 255);
  line->SetWidth(1.0);
}

//------------------------------------------------------------------------------
int PlotSPHKernels(int, char*[])
{
  // Set up the view
  vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
  view->GetRenderWindow()->SetSize(400, 300);
  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);

  // Add multiple line plots, setting the colors etc
  vtkSmartPointer<vtkChartXY> chart = vtkSmartPointer<vtkChartXY>::New();
  chart->SetTitle("SPH Kernels");
  chart->SetShowLegend(true);
  view->GetScene()->AddItem(chart);

  // Create a table which will contain the plots
  vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

  unsigned char rgb[3];
  // We will approach the kernel from beyond the cutoff distance and plot the
  // function and derivative values.

  // Cubic SPH Kernel
  vtkSmartPointer<vtkSPHCubicKernel> cubic = vtkSmartPointer<vtkSPHCubicKernel>::New();
  cubic->SetDimension(2);
  cubic->SetSpatialStep(1.0);
  cubic->Initialize(nullptr, nullptr, nullptr);
  rgb[0] = 255;
  rgb[1] = 0;
  rgb[2] = 0;
  AddKernelToPlot<vtkSPHCubicKernel>(cubic, chart, "Cubic", table, rgb);

  // Quartic Kernel
  vtkSmartPointer<vtkSPHQuarticKernel> quartic = vtkSmartPointer<vtkSPHQuarticKernel>::New();
  quartic->SetDimension(2);
  quartic->SetSpatialStep(1.0);
  quartic->Initialize(nullptr, nullptr, nullptr);
  rgb[0] = 0;
  rgb[1] = 255;
  rgb[2] = 0;
  AddKernelToPlot<vtkSPHQuarticKernel>(quartic, chart, "Quartic", table, rgb);

  // Quintic Kernel
  vtkSmartPointer<vtkSPHQuinticKernel> quintic = vtkSmartPointer<vtkSPHQuinticKernel>::New();
  quintic->SetDimension(2);
  quintic->SetSpatialStep(1.0);
  quintic->Initialize(nullptr, nullptr, nullptr);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 255;
  AddKernelToPlot<vtkSPHQuinticKernel>(quintic, chart, "Quintic", table, rgb);

  // Wendland C2 (quintic) Kernel
  vtkSmartPointer<vtkWendlandQuinticKernel> wendland =
    vtkSmartPointer<vtkWendlandQuinticKernel>::New();
  wendland->SetDimension(2);
  wendland->SetSpatialStep(1.0);
  wendland->Initialize(nullptr, nullptr, nullptr);
  rgb[0] = 255;
  rgb[1] = 0;
  rgb[2] = 255;
  AddKernelToPlot<vtkWendlandQuinticKernel>(wendland, chart, "Wendland", table, rgb);

  vtkAxis* left = chart->GetAxis(vtkAxis::LEFT);
  vtkAxis* bottom = chart->GetAxis(vtkAxis::BOTTOM);
  left->SetTitle("Kernel Value");
  bottom->SetTitle("r/h");

  // Start interactor
  view->GetInteractor()->Initialize();
  view->GetInteractor()->Start();

  return EXIT_SUCCESS;
}

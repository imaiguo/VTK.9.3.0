// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkCategoryLegend.h"

#include "vtkColorSeries.h"
#include "vtkLookupTable.h"
#include "vtkNew.h"
#include "vtkVariantArray.h"

#include "vtkContextScene.h"
#include "vtkContextTransform.h"
#include "vtkContextView.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "vtkRegressionTestImage.h"

//------------------------------------------------------------------------------
int TestCategoryLegend(int argc, char* argv[])
{
  vtkNew<vtkVariantArray> values;
  values->InsertNextValue(vtkVariant("a"));
  values->InsertNextValue(vtkVariant("b"));
  values->InsertNextValue(vtkVariant("c"));

  vtkNew<vtkLookupTable> lut;
  for (int i = 0; i < values->GetNumberOfTuples(); ++i)
  {
    lut->SetAnnotation(values->GetValue(i), values->GetValue(i).ToString());
  }

  vtkNew<vtkColorSeries> colorSeries;
  colorSeries->SetColorScheme(vtkColorSeries::BREWER_QUALITATIVE_SET3);
  colorSeries->BuildLookupTable(lut);

  vtkNew<vtkCategoryLegend> legend;
  legend->SetScalarsToColors(lut);
  legend->SetValues(values);
  legend->SetTitle("legend");

  vtkNew<vtkContextTransform> trans;
  trans->SetInteractive(true);
  trans->AddItem(legend);
  trans->Translate(180, 70);

  vtkNew<vtkContextView> contextView;
  contextView->GetScene()->AddItem(trans);
  contextView->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
  contextView->GetRenderWindow()->SetSize(300, 200);
  contextView->GetRenderWindow()->SetMultiSamples(0);
  contextView->GetRenderWindow()->Render();

  int retVal = vtkRegressionTestImage(contextView->GetRenderWindow());
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
  {
    contextView->GetRenderWindow()->Render();
    contextView->GetInteractor()->Start();
    retVal = vtkRegressionTester::PASSED;
  }
  return !retVal;
}

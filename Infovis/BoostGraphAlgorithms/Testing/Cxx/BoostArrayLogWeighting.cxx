// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include <vtkArrayData.h>
#include <vtkArrayPrint.h>
#include <vtkBoostLogWeighting.h>
#include <vtkDiagonalMatrixSource.h>
#include <vtkSmartPointer.h>
#include <vtkTypedArray.h>

#include <cmath>
#include <iostream>
#include <stdexcept>

#define test_expression(expression)                                                                \
  {                                                                                                \
    if (!(expression))                                                                             \
      throw std::runtime_error("Expression failed: " #expression);                                 \
  }

static bool close_enough(const double lhs, const double rhs)
{
  return fabs(lhs - rhs) < 1.0e-12;
}

int BoostArrayLogWeighting(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  try
  {
    vtkSmartPointer<vtkDiagonalMatrixSource> source =
      vtkSmartPointer<vtkDiagonalMatrixSource>::New();
    source->SetExtents(3);
    source->SetArrayType(vtkDiagonalMatrixSource::SPARSE);
    source->SetSuperDiagonal(1.0);
    source->SetDiagonal(3.0);
    source->SetSubDiagonal(7.0);

    cout << std::fixed << setprecision(1);
    cout << "sparse diagonal source:\n";
    source->Update();
    vtkPrintMatrixFormat(
      cout, vtkTypedArray<double>::SafeDownCast(source->GetOutput()->GetArray(0)));

    vtkSmartPointer<vtkBoostLogWeighting> log_weighting =
      vtkSmartPointer<vtkBoostLogWeighting>::New();
    log_weighting->AddInputConnection(source->GetOutputPort());

    log_weighting->Update();
    vtkTypedArray<double>* weighted =
      vtkTypedArray<double>::SafeDownCast(log_weighting->GetOutput()->GetArray(0));
    cout << std::fixed << setprecision(17);
    cout << "sparse weighted:\n";
    vtkPrintMatrixFormat(cout, weighted);

    test_expression(weighted);
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(0, 0)), 1.38629436111989057));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(1, 0)), 2.07944154167983575));
    test_expression(close_enough(weighted->GetValue(vtkArrayCoordinates(2, 0)), 0.0));

    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(0, 1)), 0.69314718055994529));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(1, 1)), 1.38629436111989057));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(2, 1)), 2.07944154167983575));

    test_expression(close_enough(weighted->GetValue(vtkArrayCoordinates(0, 2)), 0.0));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(1, 2)), 0.69314718055994529));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(2, 2)), 1.38629436111989057));

    source->SetArrayType(vtkDiagonalMatrixSource::DENSE);
    cout << std::fixed << setprecision(1);
    cout << "dense diagonal source:\n";
    source->Update();
    vtkPrintMatrixFormat(
      cout, vtkTypedArray<double>::SafeDownCast(source->GetOutput()->GetArray(0)));

    log_weighting->Update();
    weighted = vtkTypedArray<double>::SafeDownCast(log_weighting->GetOutput()->GetArray(0));
    cout << std::fixed << setprecision(17);
    cout << "dense weighted:\n";
    vtkPrintMatrixFormat(cout, weighted);

    test_expression(weighted);
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(0, 0)), 1.38629436111989057));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(1, 0)), 2.07944154167983575));
    test_expression(close_enough(weighted->GetValue(vtkArrayCoordinates(2, 0)), 0.0));

    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(0, 1)), 0.69314718055994529));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(1, 1)), 1.38629436111989057));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(2, 1)), 2.07944154167983575));

    test_expression(close_enough(weighted->GetValue(vtkArrayCoordinates(0, 2)), 0.0));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(1, 2)), 0.69314718055994529));
    test_expression(
      close_enough(weighted->GetValue(vtkArrayCoordinates(2, 2)), 1.38629436111989057));

    return 0;
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
    return 1;
  }
}

// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include <vtkArrayData.h>
#include <vtkArrayPrint.h>
#include <vtkBoostRandomSparseArraySource.h>
#include <vtkSmartPointer.h>
#include <vtkSparseArray.h>

#include <cmath>
#include <iostream>
#include <stdexcept>

#define test_expression(expression)                                                                \
  do                                                                                               \
  {                                                                                                \
    if (!(expression))                                                                             \
      throw std::runtime_error("Expression failed: " #expression);                                 \
  } while (false)

static bool close_enough(const double lhs, const double rhs)
{
  return fabs(lhs - rhs) < 1.0e-12;
}

int BoostArrayRandomSparseArraySource(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  try
  {
    vtkSmartPointer<vtkBoostRandomSparseArraySource> source =
      vtkSmartPointer<vtkBoostRandomSparseArraySource>::New();
    source->SetExtents(vtkArrayExtents(2, 2, 2));
    source->SetElementProbabilitySeed(123);
    source->SetElementProbability(0.65);
    source->SetElementValueSeed(456);
    source->SetMinValue(-1.0);
    source->SetMaxValue(1.0);
    source->Update();

    vtkSparseArray<double>* const sparse_array =
      vtkSparseArray<double>::SafeDownCast(source->GetOutput()->GetArray(0));

    cout << "sparse random source:\n";

    cout << setprecision(17);
    vtkPrintCoordinateFormat(cout, sparse_array);

    test_expression(sparse_array);
    test_expression(sparse_array->GetValue(vtkArrayCoordinates(0, 0, 0)) == 0.0);
    test_expression(sparse_array->GetValue(vtkArrayCoordinates(0, 0, 1)) == 0.0);
    test_expression(
      close_enough(sparse_array->GetValue(vtkArrayCoordinates(0, 1, 0)), -0.67386643076315522));
    test_expression(
      close_enough(sparse_array->GetValue(vtkArrayCoordinates(0, 1, 1)), -0.69691725401207805));
    test_expression(
      close_enough(sparse_array->GetValue(vtkArrayCoordinates(1, 0, 0)), 0.56728651095181704));
    test_expression(sparse_array->GetValue(vtkArrayCoordinates(1, 0, 1)) == 0.0);
    test_expression(
      close_enough(sparse_array->GetValue(vtkArrayCoordinates(1, 1, 0)), 0.61704678647220135));
    test_expression(sparse_array->GetValue(vtkArrayCoordinates(1, 1, 1)) == 0.0);

    return 0;
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
    return 1;
  }
}

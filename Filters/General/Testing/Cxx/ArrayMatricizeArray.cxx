// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-NVIDIA-USGov

#include <vtkArrayData.h>
#include <vtkArrayPrint.h>
#include <vtkMatricizeArray.h>
#include <vtkSmartPointer.h>
#include <vtkSparseArray.h>

#include <iostream>
#include <stdexcept>

#define test_expression(expression)                                                                \
  do                                                                                               \
  {                                                                                                \
    if (!(expression))                                                                             \
      throw std::runtime_error("Expression failed: " #expression);                                 \
  } while (false)

int ArrayMatricizeArray(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  try
  {
    // Create an array ...
    vtkSmartPointer<vtkSparseArray<double>> array = vtkSmartPointer<vtkSparseArray<double>>::New();
    array->Resize(vtkArrayExtents(2, 2, 2));

    double value = 0;
    const vtkArrayExtents extents = array->GetExtents();
    for (int i = extents[0].GetBegin(); i != extents[0].GetEnd(); ++i)
    {
      for (int j = extents[1].GetBegin(); j != extents[1].GetEnd(); ++j)
      {
        for (int k = extents[2].GetBegin(); k != extents[2].GetEnd(); ++k)
        {
          array->AddValue(vtkArrayCoordinates(i, j, k), value++);
        }
      }
    }

    std::cout << "array source:\n";
    vtkPrintCoordinateFormat(std::cout, array.GetPointer());

    // Create an array data object to hold it ...
    vtkSmartPointer<vtkArrayData> array_data = vtkSmartPointer<vtkArrayData>::New();
    array_data->AddArray(array);

    // Matricize it ...
    vtkSmartPointer<vtkMatricizeArray> matricize = vtkSmartPointer<vtkMatricizeArray>::New();
    matricize->SetInputData(array_data);
    matricize->SetSliceDimension(0);
    matricize->Update();

    vtkSparseArray<double>* const matricized_array = vtkSparseArray<double>::SafeDownCast(
      matricize->GetOutput()->GetArray(static_cast<vtkIdType>(0)));
    test_expression(matricized_array);

    std::cout << "matricize output:\n";
    vtkPrintCoordinateFormat(std::cout, matricized_array);

    test_expression(matricized_array->GetValue(vtkArrayCoordinates(0, 0)) == 0);
    test_expression(matricized_array->GetValue(vtkArrayCoordinates(0, 1)) == 1);
    test_expression(matricized_array->GetValue(vtkArrayCoordinates(0, 2)) == 2);
    test_expression(matricized_array->GetValue(vtkArrayCoordinates(0, 3)) == 3);
    test_expression(matricized_array->GetValue(vtkArrayCoordinates(1, 0)) == 4);
    test_expression(matricized_array->GetValue(vtkArrayCoordinates(1, 1)) == 5);
    test_expression(matricized_array->GetValue(vtkArrayCoordinates(1, 2)) == 6);
    test_expression(matricized_array->GetValue(vtkArrayCoordinates(1, 3)) == 7);

    return EXIT_SUCCESS;
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}

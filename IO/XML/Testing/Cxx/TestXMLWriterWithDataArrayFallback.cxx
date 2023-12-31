// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// .NAME Test of vtkXMLWriter with data array dispatch fallback
// .SECTION Description
//

#include "vtkImageData.h"
#include "vtkIntArray.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkTestDataArray.h"
#include "vtkTestUtilities.h"
#include "vtkXMLImageDataReader.h"
#include "vtkXMLImageDataWriter.h"

#include <string>

int TestXMLWriterWithDataArrayFallback(int argc, char* argv[])
{
  char* temp_dir_c =
    vtkTestUtilities::GetArgOrEnvOrDefault("-T", argc, argv, "VTK_TEMP_DIR", "Testing/Temporary");
  std::string temp_dir = std::string(temp_dir_c);
  delete[] temp_dir_c;

  if (temp_dir.empty())
  {
    cerr << "Could not determine temporary directory." << endl;
    return EXIT_FAILURE;
  }

  std::string filename = temp_dir + "/testXMLWriterWithDataArrayFallback.vti";

  {
    vtkNew<vtkImageData> imageData;
    imageData->SetDimensions(2, 3, 1);

    vtkNew<vtkTestDataArray<vtkIntArray>> data;
    data->SetName("test_data");
    data->SetNumberOfTuples(6);
    for (vtkIdType i = 0; i < 6; i++)
    {
      data->SetValue(i, static_cast<int>(i));
    }

    imageData->GetPointData()->AddArray(data);

    vtkNew<vtkXMLImageDataWriter> writer;
    writer->SetFileName(filename.c_str());
    writer->SetInputData(imageData);
    writer->Write();
  }

  {
    vtkNew<vtkXMLImageDataReader> reader;
    reader->SetFileName(filename.c_str());
    reader->Update();

    vtkImageData* imageData = reader->GetOutput();
    vtkIntArray* data = vtkIntArray::SafeDownCast(imageData->GetPointData()->GetArray("test_data"));

    if (!data || data->GetNumberOfTuples() != 6)
    {
      cerr << "Could not read data array." << endl;
      return EXIT_FAILURE;
    }

    for (vtkIdType i = 0; i < data->GetNumberOfTuples(); i++)
    {
      if (data->GetValue(i) != i)
      {
        cerr << "Incorrect value from data array." << endl;
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}

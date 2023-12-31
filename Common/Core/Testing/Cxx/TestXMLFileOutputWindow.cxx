// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkXMLFileOutputWindow.h"

#include "vtkSmartPointer.h"
#include "vtksys/FStream.hxx"
#include <string>

int TestXMLFileOutputWindow(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " outputFilename" << std::endl;
    return EXIT_FAILURE;
  }
  std::string sample("Test string: &\"'<>");

  {
    vtkSmartPointer<vtkXMLFileOutputWindow> ofw = vtkSmartPointer<vtkXMLFileOutputWindow>::New();
    ofw->SetInstance(ofw);
    ofw->FlushOn();

    // Use the default filename
    ofw->DisplayTag(sample.c_str());
    ofw->DisplayText(sample.c_str());
    ofw->DisplayErrorText(sample.c_str());
    ofw->DisplayWarningText(sample.c_str());
    ofw->DisplayGenericWarningText(sample.c_str());
    ofw->DisplayDebugText(sample.c_str());

    // Check nullptr strings
    ofw->DisplayTag(nullptr);
    ofw->DisplayText(nullptr);
    ofw->DisplayErrorText(nullptr);
    ofw->DisplayWarningText(nullptr);
    ofw->DisplayGenericWarningText(nullptr);
    ofw->DisplayDebugText(nullptr);
  }

  // Append to default
  {
    vtkSmartPointer<vtkXMLFileOutputWindow> ofw2 = vtkSmartPointer<vtkXMLFileOutputWindow>::New();
    ofw2->SetInstance(ofw2);
    ofw2->AppendOn();
    ofw2->DisplayText("Appended");
  }

  // Change the file name
  {
    vtkSmartPointer<vtkXMLFileOutputWindow> ofw3 = vtkSmartPointer<vtkXMLFileOutputWindow>::New();
    ofw3->SetInstance(ofw3);
    ofw3->SetFileName(argv[1]);
    ofw3->DisplayTag(sample.c_str());
    ofw3->DisplayText(sample.c_str());
    ofw3->DisplayErrorText(sample.c_str());
    ofw3->DisplayWarningText(sample.c_str());
    ofw3->DisplayGenericWarningText(sample.c_str());
    ofw3->DisplayDebugText(sample.c_str());
    ofw3->AppendOn();
    ofw3->DisplayText("Appended");
  }

  // Now, compare the default and specified files
  // Read the default XML file
  vtksys::ifstream dfin("vtkMessageLog.xml");
  std::string def((std::istreambuf_iterator<char>(dfin)), std::istreambuf_iterator<char>());

  if (dfin.fail())
  {
    std::cout << argv[0] << ": Cannot open vtkMessageLog.xml" << std::endl;
    return EXIT_FAILURE;
  }

  vtksys::ifstream sfin(argv[1]);
  std::string specified((std::istreambuf_iterator<char>(sfin)), std::istreambuf_iterator<char>());

  if (sfin.fail())
  {
    std::cout << argv[0] << ": Cannot open " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  if (def != specified)
  {
    std::cout << "The string in the default file ***********" << std::endl
              << def << std::endl
              << "does not match the string in the specified file  ***********" << std::endl
              << specified << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

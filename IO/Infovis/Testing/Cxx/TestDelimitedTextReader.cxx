// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#include <vtkDelimitedTextReader.h>
#include <vtkIOStream.h>
#include <vtkStringArray.h>
#include <vtkTable.h>
#include <vtkTestUtilities.h>

#define NUM_TEST_FILES 5

int TestDelimitedTextReader(int argc, char* argv[])
{

  const char* testOneFNames[NUM_TEST_FILES] = { "Data/delimited.txt", "Data/delimited.txt",
    "Data/delimitedUTF16.txt", "Data/delimitedUTF16LE.txt", "Data/delimitedUTF16BE.txt" };

  const char* testTwoFNames[NUM_TEST_FILES] = { "Data/delimited2.txt", "Data/delimited2.txt",
    "Data/delimited2UTF16.txt", "Data/delimited2UTF16LE.txt", "Data/delimited2UTF16BE.txt" };

  const char* UnicodeCharacterSet[NUM_TEST_FILES] = { "ASCII", "UTF-8", "UTF-16", "UTF-16LE",
    "UTF-16BE" };

  for (int index = 0; index < NUM_TEST_FILES; index++)
  {
    char* filename = vtkTestUtilities::ExpandDataFileName(argc, argv, testOneFNames[index]);

    vtkDelimitedTextReader* reader = vtkDelimitedTextReader::New();

    if (!strcmp(UnicodeCharacterSet[index], "ASCII"))
    {
      reader->SetFieldDelimiterCharacters(":");
      reader->SetStringDelimiter('"');
    }
    else
    {
      reader->SetUTF8FieldDelimiters(":");
      reader->SetUTF8StringDelimiters("\"");
      reader->SetUnicodeCharacterSet(UnicodeCharacterSet[index]);
    }

    reader->SetUseStringDelimiter(true);
    reader->SetFileName(filename);
    reader->SetHaveHeaders(false);
    reader->Update();

    vtkTable* table = reader->GetOutput();
    cout << "### Test 1: colon delimiter, no headers, do not merge consecutive delimiters" << endl;
    cout << "Printing reader info..." << endl;
    reader->Print(cout);
    table->Dump();

    if (table->GetNumberOfRows() != 6)
    {
      cout << "ERROR: Wrong number of rows." << endl;
      return 1;
    }
    if (table->GetNumberOfColumns() != 4)
    {
      cout << "ERROR: Wrong number of columns." << endl;
      return 1;
    }

    reader->Delete();
    delete[] filename;

    // Test 2: make sure the MergeConsecutiveDelimiters thing works
    reader = vtkDelimitedTextReader::New();

    if (!strcmp(UnicodeCharacterSet[index], "ASCII"))
    {
      reader->SetFieldDelimiterCharacters(",");
    }
    else
    {
      reader->SetUnicodeFieldDelimiters(",");
      reader->SetUnicodeCharacterSet(UnicodeCharacterSet[index]);
    }

    filename = vtkTestUtilities::ExpandDataFileName(argc, argv, testTwoFNames[index]);

    reader->MergeConsecutiveDelimitersOn();
    reader->SetHaveHeaders(true);
    reader->SetFileName(filename);
    reader->Update();
    table = reader->GetOutput();
    cout << endl << "### Test 2: comma delimiter, headers, merge consecutive delimiters" << endl;
    cout << "Printing reader info..." << endl;
    reader->Print(cout);
    table->Dump();

    if (table->GetNumberOfRows() != 1)
    {
      cout << "ERROR: Wrong number of rows." << endl;
      return 1;
    }
    if (table->GetNumberOfColumns() != 9)
    {
      cout << "ERROR: Wrong number of columns." << endl;
      return 1;
    }

    delete[] filename;
    reader->Delete();
  }

  return 0;
}

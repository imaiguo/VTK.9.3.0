// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// .NAME TestFieldDataSerialization.cxx -- Test for vtkFieldDataSerializer
//
// .SECTION Description
//  Simple tests for serialization/de-serialization of field data.

#include "vtkDoubleArray.h"
#include "vtkFieldData.h"
#include "vtkFieldDataSerializer.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMathUtilities.h"
#include "vtkMultiProcessStream.h"
#include "vtkPointData.h"
#include "vtkStringArray.h"

#include <cassert>
#include <sstream>

//------------------------------------------------------------------------------
vtkPointData* GetEmptyField()
{
  vtkPointData* field = vtkPointData::New();
  return (field);
}

//------------------------------------------------------------------------------
vtkIntArray* GetSampleIntArray(const int numTuples, const int numComp)
{
  assert("pre: numTuples > 0" && (numTuples > 0));
  assert("pre: numComp > 0" && (numComp > 0));

  vtkIntArray* array = vtkIntArray::New();
  array->SetNumberOfComponents(numComp);
  array->SetNumberOfTuples(numTuples);

  std::ostringstream oss;
  oss << "SampleIntArray-" << numComp;
  array->SetName(oss.str().c_str());

  int* ptr = static_cast<int*>(array->GetVoidPointer(0));
  int idx = 0;
  for (int i = 0; i < numTuples; ++i)
  {
    for (int j = 0; j < numComp; ++j)
    {
      ptr[i * numComp + j] = idx;
      ++idx;
    } // END for all components
  }   // END for all tuples

  return (array);
}

//------------------------------------------------------------------------------
vtkDoubleArray* GetSampleDoubleArray(const int numTuples, const int numComp)
{
  assert("pre: numTuples > 0" && (numTuples > 0));
  assert("pre: numComp > 0" && (numComp > 0));

  vtkDoubleArray* array = vtkDoubleArray::New();
  array->SetNumberOfComponents(numComp);
  array->SetNumberOfTuples(numTuples);

  std::ostringstream oss;
  oss << "SampleDoubleArray-" << numComp;
  array->SetName(oss.str().c_str());

  double* ptr = static_cast<double*>(array->GetVoidPointer(0));
  double val = 0.5;
  for (int i = 0; i < numTuples; ++i)
  {
    for (int j = 0; j < numComp; ++j)
    {
      ptr[i * numComp + j] = val;
      ++val;
    } // END for all components
  }   // END for all tuples

  return (array);
}

//------------------------------------------------------------------------------
vtkFloatArray* GetSampleFloatArray(const int numTuples, const int numComp)
{
  assert("pre: numTuples > 0" && (numTuples > 0));
  assert("pre: numComp > 0" && (numComp > 0));

  vtkFloatArray* array = vtkFloatArray::New();
  array->SetNumberOfComponents(numComp);
  array->SetNumberOfTuples(numTuples);

  std::ostringstream oss;
  oss << "SampleFloatArray-" << numComp;
  array->SetName(oss.str().c_str());

  float* ptr = static_cast<float*>(array->GetVoidPointer(0));
  float val = 0.5;
  for (int i = 0; i < numTuples; ++i)
  {
    for (int j = 0; j < numComp; ++j)
    {
      ptr[i * numComp + j] = val;
      ++val;
    } // END for all components
  }   // END for all tuples

  return (array);
}

//------------------------------------------------------------------------------
vtkPointData* GetSamplePointData(const int numTuples)
{
  assert("pre: numTuples > 0" && (numTuples > 0));

  // STEP 0: Get int field
  vtkPointData* field = vtkPointData::New();
  vtkIntArray* intDataArray = GetSampleIntArray(numTuples, 1);
  field->AddArray(intDataArray);
  intDataArray->Delete();

  // STEP 1: Get double field
  vtkDoubleArray* doubleDataArray = GetSampleDoubleArray(numTuples, 3);
  field->AddArray(doubleDataArray);
  doubleDataArray->Delete();

  // STEP 2: Get float field
  vtkFloatArray* floatDataArray = GetSampleFloatArray(numTuples, 2);
  field->AddArray(floatDataArray);
  floatDataArray->Delete();

  return (field);
}

//------------------------------------------------------------------------------
bool AreArraysEqual(vtkDataArray* A1, vtkDataArray* A2)
{
  assert("pre: array 1 is nullptr!" && (A1 != nullptr));
  assert("pre: array 2 is nullptr!" && (A2 != nullptr));

  if (A1->GetDataType() != A2->GetDataType())
  {
    cerr << "ERROR: array datatype mismatch!\n";
    return false;
  }

  if (strcmp(A1->GetName(), A2->GetName()) != 0)
  {
    cerr << "ERROR: array name mismatch!\n";
    cerr << "A1: " << A1->GetName() << endl;
    cerr << "A2: " << A2->GetName() << endl;
    return false;
  }

  if (A1->GetNumberOfTuples() != A2->GetNumberOfTuples())
  {
    cerr << "ERROR: number of tuples mismatch for array ";
    cerr << A1->GetName() << endl;
    return false;
  }

  if (A1->GetNumberOfComponents() != A2->GetNumberOfComponents())
  {
    cerr << "ERROR: number of tuples mismatch for array ";
    cerr << A1->GetName() << endl;
    return false;
  }

  int M = A1->GetNumberOfTuples();
  int N = A1->GetNumberOfComponents();

  switch (A1->GetDataType())
  {
    case VTK_FLOAT:
    {
      float* a1 = static_cast<float*>(A1->GetVoidPointer(0));
      float* a2 = static_cast<float*>(A2->GetVoidPointer(0));
      for (int i = 0; i < M; ++i)
      {
        for (int j = 0; j < N; ++j)
        {
          int idx = i * N + j;
          if (!vtkMathUtilities::FuzzyCompare(a1[idx], a2[idx]))
          {
            cerr << "INFO:" << a1[idx] << " != " << a2[idx] << endl;
            cerr << "ERROR: float array mismatch!\n";
            return false;
          } // END if not equal
        }   // END for all N
      }     // END for all M
    }
    break;
    case VTK_DOUBLE:
    {
      double* a1 = static_cast<double*>(A1->GetVoidPointer(0));
      double* a2 = static_cast<double*>(A2->GetVoidPointer(0));
      for (int i = 0; i < M; ++i)
      {
        for (int j = 0; j < N; ++j)
        {
          int idx = i * N + j;
          if (!vtkMathUtilities::FuzzyCompare(a1[idx], a2[idx]))
          {
            cerr << "INFO:" << a1[idx] << " != " << a2[idx] << endl;
            cerr << "ERROR: float array mismatch!\n";
            return false;
          } // END if not equal
        }   // END for all N
      }     // END for all M
    }
    break;
    case VTK_INT:
    {
      int* a1 = static_cast<int*>(A1->GetVoidPointer(0));
      int* a2 = static_cast<int*>(A2->GetVoidPointer(0));
      for (int i = 0; i < M; ++i)
      {
        for (int j = 0; j < N; ++j)
        {
          int idx = i * N + j;
          if (a1[idx] != a2[idx])
          {
            cerr << "INFO:" << a1[idx] << " != " << a2[idx] << endl;
            cerr << "ERROR: int array mismatch!\n";
            return false;
          }
        } // END for N
      }   // END for all M
    }
    break;
    default:
      cerr << "ERROR: unhandled case! Code should not reach here!\n";
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool AreFieldsEqual(vtkFieldData* F1, vtkFieldData* F2)
{
  assert("pre: field 1 is nullptr!" && (F1 != nullptr));
  assert("pre: field 2 is nullptr!" && (F2 != nullptr));

  if (F1->GetNumberOfArrays() != F2->GetNumberOfArrays())
  {
    cerr << "ERROR: number of arrays mismatch between fields!\n";
    return false;
  }

  bool status = true;
  for (int array = 0; array < F1->GetNumberOfArrays(); ++array)
  {
    vtkDataArray* a1 = F1->GetArray(array);
    vtkDataArray* a2 = F2->GetArray(array);
    if (!AreArraysEqual(a1, a2))
    {
      cerr << "ERROR: array " << a1->GetName() << " and " << a2->GetName();
      cerr << " do not match!\n";
      status = false;
    }
    else
    {
      cout << "INFO: " << a1->GetName() << " fields are equal!\n";
      cout.flush();
    }
  } // END for all arrays
  return (status);
}

//------------------------------------------------------------------------------
int TestFieldDataMetaData()
{
  int rc = 0;

  // STEP 0: Construct the field data
  vtkPointData* field = GetSamplePointData(5);
  assert("pre: field is nullptr!" && (field != nullptr));

  // STEP 1: Serialize the field data in a bytestream
  vtkMultiProcessStream bytestream;
  vtkFieldDataSerializer::SerializeMetaData(field, bytestream);

  // STEP 2: De-serialize the metadata
  vtkStringArray* namesArray = vtkStringArray::New();
  vtkIntArray* datatypesArray = vtkIntArray::New();
  vtkIntArray* dimensionsArray = vtkIntArray::New();

  vtkFieldDataSerializer::DeserializeMetaData(
    bytestream, namesArray, datatypesArray, dimensionsArray);

  vtkIdType NumberOfArrays = namesArray->GetNumberOfValues();
  std::string* names = static_cast<std::string*>(namesArray->GetVoidPointer(0));
  int* datatypes = static_cast<int*>(datatypesArray->GetVoidPointer(0));
  int* dimensions = static_cast<int*>(dimensionsArray->GetVoidPointer(0));

  // STEP 3: Check deserialized data with expected values
  if (NumberOfArrays != field->GetNumberOfArrays())
  {
    ++rc;
    cerr << "ERROR: NumberOfArrays=" << NumberOfArrays
         << " expected val=" << field->GetNumberOfArrays() << "\n";
  }
  assert("pre: names arrays is nullptr" && (names != nullptr));
  assert("pre: datatypes is nullptr" && (datatypes != nullptr));
  assert("pre: dimensions is nullptr" && (dimensions != nullptr));

  for (int i = 0; i < NumberOfArrays; ++i)
  {
    vtkDataArray* dataArray = field->GetArray(i);
    if (dataArray->GetName() != names[i])
    {
      rc++;
      cerr << "ERROR: Array name mismatch!\n";
    }
    if (dataArray->GetDataType() != datatypes[i])
    {
      rc++;
      cerr << "ERROR: Array data type mismatch!\n";
    }
    if (dataArray->GetNumberOfTuples() != dimensions[i * 2])
    {
      rc++;
      cerr << "ERROR: Array number of tuples mismatch!\n";
    }
    if (dataArray->GetNumberOfComponents() != dimensions[i * 2 + 1])
    {
      rc++;
      cerr << "ERROR: Array number of components mismatch!\n";
    }
  } // END for all arrays

  // STEP 4: Clean up memory
  namesArray->Delete();
  datatypesArray->Delete();
  dimensionsArray->Delete();
  field->Delete();

  return (rc);
}

//------------------------------------------------------------------------------
int TestFieldData()
{
  int rc = 0;

  vtkPointData* field = GetSamplePointData(5);
  assert("pre: field is nullptr!" && (field != nullptr));

  vtkMultiProcessStream bytestream;
  vtkFieldDataSerializer::Serialize(field, bytestream);
  if (bytestream.Empty())
  {
    cerr << "ERROR: failed to serialize field data, bytestream is empty!\n";
    rc++;
    return (rc);
  }

  vtkPointData* field2 = vtkPointData::New();
  vtkFieldDataSerializer::Deserialize(bytestream, field2);
  if (!AreFieldsEqual(field, field2))
  {
    cerr << "ERROR: fields are not equal!\n";
    rc++;
    return (rc);
  }
  else
  {
    cout << "Fields are equal!\n";
    cout.flush();
  }

  field->Delete();
  field2->Delete();
  return (rc);
}

//------------------------------------------------------------------------------
int TestFieldDataSerialization(int argc, char* argv[])
{
  // Resolve compiler warnings about unused vars
  static_cast<void>(argc);
  static_cast<void>(argv);

  int rc = 0;
  rc += TestFieldData();

  cout << "Testing metadata serialization...";
  rc += TestFieldDataMetaData();
  cout << "[DONE]\n";
  return (rc);
}

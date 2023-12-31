// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright (c) 2006 Atamai, Inc.
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkMNIObjectWriter.h"

#include "vtkObjectFactory.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkErrorCode.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkIntArray.h"
#include "vtkLookupTable.h"
#include "vtkMapper.h"
#include "vtkMath.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkProperty.h"
#include "vtkUnsignedCharArray.h"
#include "vtksys/FStream.hxx"

#include <cctype>
#include <cmath>

#if !defined(_WIN32) || defined(__CYGWIN__)
#include <unistd.h> /* unlink */
#else
#include <io.h> /* unlink */
#endif

//------------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkMNIObjectWriter);

vtkCxxSetObjectMacro(vtkMNIObjectWriter, Property, vtkProperty);
vtkCxxSetObjectMacro(vtkMNIObjectWriter, Mapper, vtkMapper);
vtkCxxSetObjectMacro(vtkMNIObjectWriter, LookupTable, vtkLookupTable);

//------------------------------------------------------------------------------
vtkMNIObjectWriter::vtkMNIObjectWriter()
{
  this->Property = nullptr;
  this->Mapper = nullptr;
  this->LookupTable = nullptr;

  this->FileName = nullptr;

  // Whether file is binary or ASCII
  this->FileType = VTK_ASCII;

  this->OutputStream = nullptr;
}

//------------------------------------------------------------------------------
vtkMNIObjectWriter::~vtkMNIObjectWriter()
{
  if (this->Property)
  {
    this->Property->Delete();
  }
  if (this->Mapper)
  {
    this->Mapper->Delete();
  }
  if (this->LookupTable)
  {
    this->LookupTable->Delete();
  }

  delete[] this->FileName;
}

//------------------------------------------------------------------------------
void vtkMNIObjectWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Property: " << this->Property << "\n";
  os << indent << "Mapper: " << this->Mapper << "\n";
  os << indent << "LookupTable: " << this->LookupTable << "\n";
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteObjectType(int objType)
{
  if (this->FileType == VTK_ASCII)
  {
    this->OutputStream->put(char(objType));
  }
  else
  {
    this->OutputStream->put(char(tolower(objType)));
  }

  return 1;
}

//------------------------------------------------------------------------------
// Write floating-point values into a vtkFloatArray.
int vtkMNIObjectWriter::WriteValues(vtkDataArray* array)
{
  int dataType = array->GetDataType();
  void* data = array->GetVoidPointer(0);

  vtkIdType numTuples = array->GetNumberOfTuples();
  vtkIdType numComponents = array->GetNumberOfComponents();
  vtkIdType n = numTuples * numComponents;

  if (this->FileType == VTK_ASCII && dataType == VTK_UNSIGNED_CHAR)
  {
    vtkIdType m = numComponents;
    for (vtkIdType i = 0; i < n; i += m)
    {
      unsigned char* cdata = static_cast<unsigned char*>(data) + i;
      ostream& os = *this->OutputStream;

      double r = cdata[0] / 255.0;
      double g = r;
      double b = r;
      double a = 1.0;

      if (m > 2)
      {
        g = cdata[1] / 255.0;
        b = cdata[2] / 255.0;
      }
      if (m == 2 || m == 4)
      {
        a = cdata[m - 1] / 255.0;
      }

      os << " " << r << " " << g << " " << b << " " << a;

      if (this->WriteNewline() == 0)
      {
        return 0;
      }
    }
  }
  else if (this->FileType == VTK_ASCII)
  {
    vtkIdType valuesPerLine = 8;
    if (numComponents > 1 && numComponents < 8)
    {
      valuesPerLine = numComponents;
    }

    vtkIdType i = 0;
    while (i < n)
    {
      for (vtkIdType j = 0; j < valuesPerLine && i < n; i++, j++)
      {
        ostream& os = *this->OutputStream;

        switch (dataType)
        {
          case VTK_FLOAT:
          {
            float* fdata = static_cast<float*>(data);
            double val = fdata[i];
            os << " " << val;
          }
          break;
          case VTK_DOUBLE:
          {
            double* ddata = static_cast<double*>(data);
            double val = ddata[i];
            os << " " << val;
          }
          break;
          case VTK_INT:
          {
            int* idata = static_cast<int*>(data);
            int val = idata[i];
            os << " " << val;
          }
          break;
        }
      }
      if (this->WriteNewline() == 0)
      {
        return 0;
      }
    }
  }
  else
  {
    // machine-order endianness and data size
    if (dataType == VTK_UNSIGNED_CHAR)
    {
      // colors need to be swapped to ABGR order for binary
      char* odata = static_cast<char*>(data);
      vtkIdType m = numComponents;
      for (vtkIdType i = 0; i < n && this->OutputStream->good(); i += m)
      {
        char cdata[4];
        if (m > 2)
        {
          cdata[3] = odata[0];
          cdata[2] = odata[1];
          cdata[1] = odata[2];
        }
        else
        {
          cdata[3] = odata[0];
          cdata[2] = odata[0];
          cdata[1] = odata[0];
        }
        if (m == 2 || m == 4)
        {
          cdata[0] = odata[m - 1];
        }
        else
        {
          cdata[0] = static_cast<char>(255);
        }

        this->OutputStream->write(cdata, 4);

        odata += m;
      }
    }
    else if (dataType == VTK_DOUBLE)
    {
      // doubles must be converted to float
      double* ddata = (double*)(data);
      for (vtkIdType i = 0; i < n && this->OutputStream->good(); i++)
      {
        float fval = ddata[i];
        this->OutputStream->write((char*)(&fval), sizeof(float));
      }
    }
    else
    {
      this->OutputStream->write((char*)(data), n * array->GetDataTypeSize());
    }

    if (this->OutputStream->fail())
    {
      this->SetErrorCode(vtkErrorCode::OutOfDiskSpaceError);
      return 0;
    }
  }

  return 1;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteIdValue(vtkIdType value)
{
  // The .obj files use 32-bit integers exclusively
  int ival = static_cast<int>(value);

  if (this->FileType == VTK_ASCII)
  {
    *this->OutputStream << " " << ival;
  }
  else
  {
    // machine-order endianness and data size
    this->OutputStream->write((char*)(&ival), sizeof(int));
  }

  return 1;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteNewline()
{
  if (this->FileType == VTK_ASCII)
  {
    *this->OutputStream << "\n";

    this->OutputStream->flush();
    if (this->OutputStream->fail())
    {
      this->SetErrorCode(vtkErrorCode::OutOfDiskSpaceError);
      return 0;
    }
  }

  return 1;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteProperty(vtkProperty* property)
{
  float properties[5];

  properties[0] = 0.0;
  properties[1] = 1.0;
  properties[2] = 0.0;
  properties[3] = 1.0;
  properties[4] = 1.0;

  if (property)
  {
    properties[0] = property->GetAmbient();
    properties[1] = property->GetDiffuse();
    properties[2] = property->GetSpecular();
    properties[3] = property->GetSpecularPower();
    properties[4] = property->GetOpacity();
  }

  if (this->FileType == VTK_ASCII)
  {
    for (int i = 0; i < 5; i++)
    {
      *this->OutputStream << " " << properties[i];
    }
  }
  else
  {
    // machine-order endianness and data size
    this->OutputStream->write((char*)(properties), 5 * sizeof(float));
  }

  return 1;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteLineThickness(vtkProperty* property)
{
  float width = 1;

  if (property)
  {
    width = property->GetLineWidth();
  }

  if (this->FileType == VTK_ASCII)
  {
    *this->OutputStream << " " << width;
  }
  else
  {
    // machine-order endianness and data size
    this->OutputStream->write((char*)(&width), sizeof(float));
  }

  return 1;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WritePoints(vtkPolyData* data)
{
  return this->WriteValues(data->GetPoints()->GetData());
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteNormals(vtkPolyData* data)
{
  vtkDataArray* normals = data->GetPointData()->GetNormals();
  vtkFloatArray* newNormals = nullptr;

  // Calculate normals if necessary
  if (!normals)
  {
    // Normals will be calculated according to BIC conventions,
    // which weighs the polygon normals by the interior angle.

    vtkPoints* points = data->GetPoints();
    vtkIdType numPoints = points->GetNumberOfPoints();
    vtkCellArray* polyArray = data->GetPolys();
    vtkCellArray* stripArray = data->GetStrips();
    vtkIdType numPolys = data->GetNumberOfPolys();
    vtkIdType numCells = numPolys + data->GetNumberOfStrips();

    newNormals = vtkFloatArray::New();
    newNormals->SetNumberOfComponents(3);
    newNormals->SetNumberOfTuples(numPoints);

    for (vtkIdType ii = 0; ii < numPoints; ii++)
    {
      float* normal = newNormals->GetPointer(3 * ii);
      normal[0] = 0.0;
      normal[1] = 0.0;
      normal[2] = 0.0;
    }

    vtkIdType polyCellId = 0;
    vtkIdType stripCellId = 0;
    for (vtkIdType i = 0; i < numCells; i++)
    {
      const vtkIdType* pointIds;
      vtkIdType numIds;
      vtkIdType numFaces = 1;

      if (i < numPolys)
      {
        polyArray->GetCellAtId(polyCellId, numIds, pointIds);
        ++polyCellId;
      }
      else
      {
        stripArray->GetCellAtId(stripCellId, numIds, pointIds);
        ++stripCellId;
        numFaces = numIds - 2;
        numIds = 3;
      }

      for (vtkIdType j = 0; j < numFaces; j++)
      {
        double faceNormal[3];
        vtkPolygon::ComputeNormal(points, numIds, pointIds, faceNormal);

        // For strips, reverse normal of every other triangle
        if ((j & 1) == 1)
        {
          faceNormal[0] = -faceNormal[0];
          faceNormal[1] = -faceNormal[1];
          faceNormal[2] = -faceNormal[2];
        }

        // Extra stuff is for calculating the angle
        double vec1[3];
        double vec2[3];
        double p1[3];
        double p2[3];

        points->GetPoint(pointIds[numIds - 1], p1);
        points->GetPoint(pointIds[0], p2);
        vec2[0] = p2[0] - p1[0];
        vec2[1] = p2[1] - p1[1];
        vec2[2] = p2[2] - p1[2];
        vtkMath::Normalize(vec2);

        // Go through all points in the face
        for (vtkIdType k = 0; k < numIds; k++)
        {
          vec1[0] = -vec2[0];
          vec1[1] = -vec2[1];
          vec1[2] = -vec2[2];

          p1[0] = p2[0];
          p1[1] = p2[1];
          p1[2] = p2[2];

          points->GetPoint(pointIds[(k + 1) % numIds], p2);

          vec2[0] = p2[0] - p1[0];
          vec2[1] = p2[1] - p1[1];
          vec2[2] = p2[2] - p1[2];
          vtkMath::Normalize(vec2);

          // Use dotprod to calculate angle between vectors
          double angle = 0.0;
          double dotprod = vtkMath::Dot(vec1, vec2);
          if (dotprod >= 1.0)
          {
            angle = 0.0;
          }
          else if (dotprod <= -1.0)
          {
            angle = vtkMath::Pi();
          }
          else
          {
            angle = acos(dotprod);
          }

          // Accumulate contributions of each polygon to the point normals
          float* normal = newNormals->GetPointer(3 * pointIds[k]);
          normal[0] += angle * faceNormal[0];
          normal[1] += angle * faceNormal[1];
          normal[2] += angle * faceNormal[2];
        }

        // Go to next triangle in strip
        pointIds++;
      }
    }

    // Normalize the normals
    for (vtkIdType jj = 0; jj < numPoints; jj++)
    {
      float* normal = newNormals->GetPointer(3 * jj);
      vtkMath::Normalize(normal);
    }

    normals = newNormals;
  }

  int status = this->WriteValues(normals);

  if (newNormals)
  {
    newNormals->Delete();
  }

  return status;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteColors(vtkProperty* property, vtkMapper* mapper, vtkPolyData* data)
{
  vtkUnsignedCharArray* newScalars = nullptr;
  vtkDataArray* scalars = data->GetPointData()->GetScalars();
  vtkIdType colorType = 2;

  if (scalars == nullptr)
  {
    scalars = data->GetCellData()->GetScalars();
    colorType = 1;
  }

  if (this->Mapper)
  {
    int cellFlag = 0;
    scalars = nullptr;

    // Get color scalars according to Mapper rules
    if (mapper->GetScalarVisibility())
    {
      scalars = vtkAbstractMapper::GetScalars(data, mapper->GetScalarMode(),
        mapper->GetArrayAccessMode(), mapper->GetArrayId(), mapper->GetArrayName(), cellFlag);
    }

    // Cell or point scalars?
    colorType = 2;
    if (cellFlag)
    {
      colorType = 1;
    }

    // Cannot use cell scalars for triangle strips
    if (cellFlag == 1 && data->GetStrips() && data->GetStrips()->GetNumberOfCells() != 0)
    {
      scalars = nullptr;
    }

    if (scalars)
    {
      int arrayComponent = mapper->GetArrayComponent();
      if (scalars->GetNumberOfComponents() <= arrayComponent)
      {
        arrayComponent = 0;
      }

      vtkScalarsToColors* lookupTable = scalars->GetLookupTable();
      if (lookupTable == nullptr)
      {
        lookupTable = mapper->GetLookupTable();
        lookupTable->Build();
      }

      if (!mapper->GetUseLookupTableScalarRange())
      {
        lookupTable->SetRange(mapper->GetScalarRange());
      }

      newScalars = lookupTable->MapScalars(scalars, mapper->GetColorMode(), arrayComponent);
      scalars = newScalars;
    }
  }
  else if (scalars != nullptr)
  {
    if (this->LookupTable)
    {
      newScalars = this->LookupTable->MapScalars(scalars, VTK_COLOR_MODE_MAP_SCALARS, -1);
      scalars = newScalars;
    }
    else if (scalars->GetDataType() != VTK_UNSIGNED_CHAR)
    {
      scalars = nullptr;
    }
  }

  if (scalars == nullptr)
  {
    colorType = 0;

    newScalars = vtkUnsignedCharArray::New();
    newScalars->SetNumberOfComponents(4);
    newScalars->SetNumberOfTuples(1);

    unsigned char rgba[4];

    if (property)
    {
      double color[3];
      property->GetColor(color);
      double opacity = property->GetOpacity();

      rgba[0] = static_cast<unsigned char>(color[0] * 255);
      rgba[1] = static_cast<unsigned char>(color[1] * 255);
      rgba[2] = static_cast<unsigned char>(color[2] * 255);
      rgba[3] = static_cast<unsigned char>(opacity * 255);
    }
    else
    {
      rgba[0] = 255;
      rgba[1] = 255;
      rgba[2] = 255;
      rgba[3] = 255;
    }

    newScalars->SetTypedTuple(0, rgba);
    scalars = newScalars;
  }

  int status = this->WriteIdValue(colorType);

  if (status != 0)
  {
    status = this->WriteValues(scalars);
  }

  if (newScalars)
  {
    newScalars->Delete();
  }

  return status;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteCells(vtkPolyData* data, int cellType)
{
  vtkCellArray* cellArray = nullptr;
  if (cellType == VTK_POLYGON)
  {
    cellArray = data->GetPolys();
  }
  else if (cellType == VTK_POLY_LINE)
  {
    cellArray = data->GetLines();
  }
  else
  {
    return 0;
  }

  vtkIntArray* endIndices = vtkIntArray::New();
  vtkIntArray* cellIndices = vtkIntArray::New();

  if (cellArray)
  {
    vtkIdType numCells = cellArray->GetNumberOfCells();
    vtkIdType numCellIndices = cellArray->GetNumberOfConnectivityIds();

    endIndices->Allocate(numCells);
    cellIndices->Allocate(numCellIndices);

    vtkIdType endIndex = 0;
    for (vtkIdType i = 0; i < numCells; i++)
    {
      const vtkIdType* pointIds;
      vtkIdType numIds;
      cellArray->GetCellAtId(i, numIds, pointIds);

      endIndex += numIds;
      endIndices->InsertNextValue(endIndex);
      for (vtkIdType j = 0; j < numIds; j++)
      {
        cellIndices->InsertNextValue(pointIds[j]);
      }
    }
  }

  // Convert triangle strips to triangles
  if (cellType == VTK_POLYGON && data->GetNumberOfStrips() != 0)
  {
    cellArray = data->GetStrips();
    vtkIdType numCells = cellArray->GetNumberOfCells();

    vtkIdType endIndex = 0;
    if (endIndices->GetMaxId() >= 0)
    {
      endIndex = endIndices->GetValue(endIndices->GetMaxId());
    }
    for (vtkIdType i = 0; i < numCells; i++)
    {
      const vtkIdType* pointIds;
      vtkIdType numIds;
      cellArray->GetCellAtId(i, numIds, pointIds);

      int inc1 = 2;
      int inc2 = 1;
      for (vtkIdType j = 2; j < numIds; j++)
      {
        endIndex += 3;
        endIndices->InsertNextValue(endIndex);

        cellIndices->InsertNextValue(pointIds[j - inc1]);
        cellIndices->InsertNextValue(pointIds[j - inc2]);
        cellIndices->InsertNextValue(pointIds[j]);

        // reverse the order each time around
        int tmp = inc1;
        inc1 = inc2;
        inc2 = tmp;
      }
    }
  }

  // Write the cell end indices
  int status = this->WriteValues(endIndices);

  // Write the newline
  if (status != 0)
  {
    status = this->WriteNewline();
  }

  // Write the cell point indices
  if (status != 0)
  {
    status = this->WriteValues(cellIndices);
  }

  endIndices->Delete();
  cellIndices->Delete();

  return status;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WritePolygonObject(vtkPolyData* output)
{
  // Write the surface property
  if (this->WriteProperty(this->Property) == 0)
  {
    return 0;
  }

  // Write the number of points
  if (this->WriteIdValue(output->GetNumberOfPoints()) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the points
  if (this->WritePoints(output) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the normals
  if (this->WriteNormals(output) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the number of items
  vtkIdType numPolys = output->GetNumberOfPolys();
  vtkIdType numStrips = output->GetNumberOfStrips();
  if (numStrips > 0)
  {
    numPolys += (output->GetStrips()->GetNumberOfConnectivityIds() - 2 * numStrips);
  }
  if (this->WriteIdValue(numPolys) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the colors
  if (this->WriteColors(this->Property, this->Mapper, output) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the cells
  if (this->WriteCells(output, VTK_POLYGON) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  return 1;
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::WriteLineObject(vtkPolyData* output)
{
  // Write the surface property
  if (this->WriteLineThickness(this->Property) == 0)
  {
    return 0;
  }

  // Write the number of points
  if (this->WriteIdValue(output->GetNumberOfPoints()) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the points
  if (this->WritePoints(output) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the number of items
  if (this->WriteIdValue(output->GetNumberOfLines()) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the colors
  if (this->WriteColors(this->Property, this->Mapper, output) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  // Write the cells
  if (this->WriteCells(output, VTK_POLY_LINE) == 0)
  {
    return 0;
  }

  // Write a newline
  if (this->WriteNewline() == 0)
  {
    return 0;
  }

  return 1;
}

//------------------------------------------------------------------------------
void vtkMNIObjectWriter::WriteData()
{
  vtkPolyData* input = this->GetInput();
  int objType = 0;

  vtkIdType npolys = input->GetNumberOfPolys();
  vtkIdType nstrips = input->GetNumberOfStrips();
  vtkIdType nlines = input->GetNumberOfLines();
  vtkIdType nverts = input->GetNumberOfVerts();

  if (nverts != 0)
  {
    vtkErrorMacro("Unable to write vertices.");
    return;
  }

  if ((npolys != 0 || nstrips != 0) && nlines != 0)
  {
    vtkErrorMacro("Unable to write a data set with multiple cell types.");
    return;
  }

  if (npolys != 0 || nstrips != 0)
  {
    objType = 'P';
  }
  else if (nlines != 0)
  {
    objType = 'L';
  }

  // Open the file
  this->OutputStream = this->OpenFile();

  if (!this->OutputStream)
  {
    return;
  }

  // Write the type character
  this->WriteObjectType(objType);

  // Write the object
  switch (objType)
  {
    case 'P':
      this->WritePolygonObject(input);
      break;
    case 'L':
      this->WriteLineObject(input);
      break;
  }

  // Close the file
  this->CloseFile(this->OutputStream);

  // Delete the file if an error occurred
  if (this->ErrorCode == vtkErrorCode::OutOfDiskSpaceError)
  {
    vtkErrorMacro("Ran out of disk space; deleting file: " << this->FileName);
    unlink(this->FileName);
  }
}

//------------------------------------------------------------------------------
vtkPolyData* vtkMNIObjectWriter::GetInput()
{
  return vtkPolyData::SafeDownCast(this->Superclass::GetInput());
}

//------------------------------------------------------------------------------
vtkPolyData* vtkMNIObjectWriter::GetInput(int port)
{
  return vtkPolyData::SafeDownCast(this->Superclass::GetInput(port));
}

//------------------------------------------------------------------------------
int vtkMNIObjectWriter::FillInputPortInformation(int, vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  return 1;
}

//------------------------------------------------------------------------------
ostream* vtkMNIObjectWriter::OpenFile()
{
  ostream* fptr;

  if (!this->FileName)
  {
    vtkErrorMacro(<< "No FileName specified! Can't write!");
    this->SetErrorCode(vtkErrorCode::NoFileNameError);
    return nullptr;
  }

  vtkDebugMacro(<< "Opening file for writing...");

  if (this->FileType == VTK_ASCII)
  {
    fptr = new vtksys::ofstream(this->FileName, ios::out);
  }
  else
  {
#ifdef _WIN32
    fptr = new vtksys::ofstream(this->FileName, ios::out | ios::binary);
#else
    fptr = new vtksys::ofstream(this->FileName, ios::out);
#endif
  }

  if (fptr->fail())
  {
    vtkErrorMacro(<< "Unable to open file: " << this->FileName);
    this->SetErrorCode(vtkErrorCode::CannotOpenFileError);
    delete fptr;
    return nullptr;
  }

  return fptr;
}

//------------------------------------------------------------------------------
void vtkMNIObjectWriter::CloseFile(ostream* fp)
{
  vtkDebugMacro(<< "Closing file\n");

  delete fp;
}
VTK_ABI_NAMESPACE_END

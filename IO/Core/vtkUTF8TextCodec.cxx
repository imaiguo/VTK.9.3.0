// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2010 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
#include "vtkUTF8TextCodec.h"

#include "vtkObjectFactory.h"
#include "vtkTextCodecFactory.h"

#include <vtk_utf8.h>

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkUTF8TextCodec);

void vtkUTF8TextCodec::ToUnicode(istream& InputStream, vtkTextCodec::OutputIterator& Output)
{
  try
  {
    vtkTextCodec::ToUnicode(InputStream, Output);
  }
  catch (std::string& ef)
  {
    if (ef == "End of Input")
    {
      return; // we just completed the sequence...
    }
    else
    {
      throw;
    }
  }
}

vtkTypeUInt32 vtkUTF8TextCodec::NextUTF32CodePoint(istream& inputStream)
{
  istream::char_type c[5];
  c[4] = '\0';

  unsigned int getSize = 0;
  c[getSize] = inputStream.get();
  if (inputStream.fail())
  {
    throw(std::string("End of Input"));
  }

  getSize = utf8::internal::sequence_length(c);

  if (0 == getSize)
    throw(std::string("Not enough space"));

  for (unsigned int i = 1; i < getSize; ++i)
  {
    c[i] = inputStream.get();
    if (inputStream.fail())
      throw(std::string("Not enough space"));
  }

  istream::char_type* c1 = c;

  const vtkTypeUInt32 code_point = utf8::next(c1, &c[getSize]);

  return code_point;
}

vtkUTF8TextCodec::vtkUTF8TextCodec() = default;

vtkUTF8TextCodec::~vtkUTF8TextCodec() = default;

void vtkUTF8TextCodec::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "vtkUTF8TextCodec (" << this << ") \n";
  indent = indent.GetNextIndent();
  this->Superclass::PrintSelf(os, indent.GetNextIndent());
}
VTK_ABI_NAMESPACE_END

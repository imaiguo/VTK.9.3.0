// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkInformationInternals
 * @brief   internal structure for vtkInformation
 *
 * vtkInformationInternals is used in internal implementation of
 * vtkInformation. This should only be accessed by friends
 * and sub-classes of that class.
 */

#ifndef vtkInformationInternals_h
#define vtkInformationInternals_h

#include "vtkInformationKey.h"
#include "vtkObjectBase.h"

#include <cstdint>
#define VTK_INFORMATION_USE_HASH_MAP
#ifdef VTK_INFORMATION_USE_HASH_MAP
#include <unordered_map>
#else
#include <map>
#endif

//----------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
class vtkInformationInternals
{
public:
  typedef vtkInformationKey* KeyType;
  typedef vtkObjectBase* DataType;
#ifdef VTK_INFORMATION_USE_HASH_MAP
  struct HashFun
  {
    size_t operator()(KeyType key) const
    {
      return reinterpret_cast<uintptr_t>(key) / sizeof(vtkInformationKey);
    }
  };
  typedef std::unordered_map<KeyType, DataType, HashFun> MapType;
#else
  typedef std::map<KeyType, DataType> MapType;
#endif
  MapType Map;

#ifdef VTK_INFORMATION_USE_HASH_MAP
  vtkInformationInternals()
    : Map(33)
  {
  }
#endif

  ~vtkInformationInternals()
  {
    for (MapType::iterator i = this->Map.begin(); i != this->Map.end(); ++i)
    {
      if (vtkObjectBase* value = i->second)
      {
        value->UnRegister(nullptr);
      }
    }
  }

#ifdef VTK_INFORMATION_USE_HASH_MAP
private:
  vtkInformationInternals(vtkInformationInternals const&) = delete;
#endif
};

#undef VTK_INFORMATION_USE_HASH_MAP

VTK_ABI_NAMESPACE_END
#endif
// VTK-HeaderTest-Exclude: vtkInformationInternals.h

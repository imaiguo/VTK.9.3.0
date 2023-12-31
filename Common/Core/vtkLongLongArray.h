// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkLongLongArray
 * @brief   dynamic, self-adjusting array of long long
 *
 * vtkLongLongArray is an array of values of type long long.
 * It provides methods for insertion and retrieval of values and will
 * automatically resize itself to hold new data.
 *
 * This class should not be used directly, as it only exists on systems
 * where the long long type is defined.  If you need a 64 bit integer
 * data array, use vtkTypeInt64Array instead.
 */

#ifndef vtkLongLongArray_h
#define vtkLongLongArray_h

#include "vtkAOSDataArrayTemplate.h" // Real Superclass
#include "vtkCommonCoreModule.h"     // For export macro
#include "vtkDataArray.h"

// Fake the superclass for the wrappers.
#ifndef __VTK_WRAP__
#define vtkDataArray vtkAOSDataArrayTemplate<long long>
#endif
VTK_ABI_NAMESPACE_BEGIN
class VTKCOMMONCORE_EXPORT vtkLongLongArray : public vtkDataArray
{
public:
  vtkTypeMacro(vtkLongLongArray, vtkDataArray);
#ifndef __VTK_WRAP__
#undef vtkDataArray
#endif
  static vtkLongLongArray* New();
  static vtkLongLongArray* ExtendedNew();
  void PrintSelf(ostream& os, vtkIndent indent) override;

  // This macro expands to the set of method declarations that
  // make up the interface of vtkAOSDataArrayTemplate, which is ignored
  // by the wrappers.
#if defined(__VTK_WRAP__) || defined(__WRAP_GCCXML__)
  vtkCreateWrappedArrayInterface(long long);
#endif

  /**
   * A faster alternative to SafeDownCast for downcasting vtkAbstractArrays.
   */
  static vtkLongLongArray* FastDownCast(vtkAbstractArray* source)
  {
    return static_cast<vtkLongLongArray*>(Superclass::FastDownCast(source));
  }

  /**
   * Get the minimum data value in its native type.
   */
  static long long GetDataTypeValueMin() { return VTK_LONG_LONG_MIN; }

  /**
   * Get the maximum data value in its native type.
   */
  static long long GetDataTypeValueMax() { return VTK_LONG_LONG_MAX; }

protected:
  vtkLongLongArray();
  ~vtkLongLongArray() override;

private:
  typedef vtkAOSDataArrayTemplate<long long> RealSuperclass;

  vtkLongLongArray(const vtkLongLongArray&) = delete;
  void operator=(const vtkLongLongArray&) = delete;
};

// Define vtkArrayDownCast implementation:
vtkArrayDownCast_FastCastMacro(vtkLongLongArray);

VTK_ABI_NAMESPACE_END
#endif

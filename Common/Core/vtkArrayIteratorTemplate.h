// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkArrayIteratorTemplate
 * @brief   Implementation template for a array
 * iterator.
 *
 *
 * This is implementation template for a array iterator. It only works
 * with arrays that have a contiguous internal storage of values (as in
 * vtkDataArray, vtkStringArray).
 */

#ifndef vtkArrayIteratorTemplate_h
#define vtkArrayIteratorTemplate_h

#include "vtkArrayIterator.h"
#include "vtkCommonCoreModule.h" // For export macro

#include "vtkCompiler.h"  // for VTK_USE_EXTERN_TEMPLATE
#include "vtkStdString.h" // For template instantiation
#include "vtkVariant.h"   // For template instantiation

VTK_ABI_NAMESPACE_BEGIN
template <class T>
class VTKCOMMONCORE_EXPORT vtkArrayIteratorTemplate : public vtkArrayIterator
{
public:
  static vtkArrayIteratorTemplate<T>* New();
  vtkTemplateTypeMacro(vtkArrayIteratorTemplate<T>, vtkArrayIterator);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Set the array this iterator will iterate over.
   * After Initialize() has been called, the iterator is valid
   * so long as the Array has not been modified
   * (except using the iterator itself).
   * If the array is modified, the iterator must be re-initialized.
   */
  void Initialize(vtkAbstractArray* array) override;

  /**
   * Get the array.
   */
  vtkAbstractArray* GetArray() { return this->Array; }

  /**
   * Must be called only after Initialize.
   */
  T* GetTuple(vtkIdType id);

  /**
   * Must be called only after Initialize.
   */
  T& GetValue(vtkIdType id) { return this->Pointer[id]; }

  /**
   * Sets the value at the index. This does not verify if the index is
   * valid.  The caller must ensure that id is less than the maximum
   * number of values.
   */
  void SetValue(vtkIdType id, T value) { this->Pointer[id] = value; }

  /**
   * Must be called only after Initialize.
   */
  vtkIdType GetNumberOfTuples() const;

  /**
   * Must be called only after Initialize.
   */
  vtkIdType GetNumberOfValues() const;

  /**
   * Must be called only after Initialize.
   */
  int GetNumberOfComponents() const;

  /**
   * Get the data type from the underlying array.
   */
  int GetDataType() const override;

  /**
   * Get the data type size from the underlying array.
   */
  int GetDataTypeSize() const;

  /**
   * This is the data type for the value.
   */
  typedef T ValueType;

protected:
  vtkArrayIteratorTemplate();
  ~vtkArrayIteratorTemplate() override;

  T* Pointer;

private:
  vtkArrayIteratorTemplate(const vtkArrayIteratorTemplate&) = delete;
  void operator=(const vtkArrayIteratorTemplate&) = delete;

  void SetArray(vtkAbstractArray*);
  vtkAbstractArray* Array;
};

// GetValue
template<>
inline vtkStdString& vtkArrayIteratorTemplate<vtkStdString>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline double& vtkArrayIteratorTemplate<double>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline float& vtkArrayIteratorTemplate<float>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline long long& vtkArrayIteratorTemplate<long long>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline unsigned long long& vtkArrayIteratorTemplate<unsigned long long>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline long& vtkArrayIteratorTemplate<long>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline unsigned long& vtkArrayIteratorTemplate<unsigned long>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline int& vtkArrayIteratorTemplate<int>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline unsigned int& vtkArrayIteratorTemplate<unsigned int>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline short& vtkArrayIteratorTemplate<short>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline unsigned short& vtkArrayIteratorTemplate<unsigned short>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline unsigned char& vtkArrayIteratorTemplate<unsigned char>::GetValue(vtkIdType id) { return this->Pointer[id]; }

template<>
inline char& vtkArrayIteratorTemplate<char>::GetValue(vtkIdType id) { return this->Pointer[id]; }

// GetArray

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<int>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<unsigned int>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<short>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<unsigned short>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<char>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<unsigned char>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<signed char>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<double>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<float>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<long>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<long long>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<unsigned long long>::GetArray() { return this->Array; }

template<>
inline vtkAbstractArray* vtkArrayIteratorTemplate<unsigned long>::GetArray() { return this->Array; }

#ifdef VTK_USE_EXTERN_TEMPLATE
#ifndef vtkArrayIteratorTemplateInstantiate_cxx
#ifdef _MSC_VER
#pragma warning(push)
// The following is needed when the vtkArrayIteratorTemplate is declared
// dllexport and is used from another class in vtkCommonCore
#pragma warning(disable : 4910) // extern and dllexport incompatible
#endif
vtkInstantiateTemplateMacro(extern template class VTKCOMMONCORE_EXPORT vtkArrayIteratorTemplate);
extern template class VTKCOMMONCORE_EXPORT vtkArrayIteratorTemplate<vtkStdString>;
extern template class VTKCOMMONCORE_EXPORT vtkArrayIteratorTemplate<vtkVariant>;
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
#endif // VTK_USE_EXTERN_TEMPLATE

VTK_ABI_NAMESPACE_END
#endif

// VTK-HeaderTest-Exclude: vtkArrayIteratorTemplate.h

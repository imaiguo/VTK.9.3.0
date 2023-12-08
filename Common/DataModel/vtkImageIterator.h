// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkImageIterator
 * @brief   a simple image iterator
 *
 * This is a simple image iterator that can be used to iterate over an
 * image. This should be used internally by Filter writers.
 *
 * @sa
 * vtkImageData vtkImageProgressIterator
 */

#ifndef vtkImageIterator_h
#define vtkImageIterator_h

#include "vtkCommonDataModelModule.h" // For export macro
#include "vtkSystemIncludes.h"
VTK_ABI_NAMESPACE_BEGIN
class vtkImageData;

template <class DType>
class VTKCOMMONDATAMODEL_EXPORT vtkImageIterator
{
public:
  typedef DType* SpanIterator;

  /**
   * Default empty constructor, useful only when creating an array of iterators
   * You need to call Initialize afterward
   */
  vtkImageIterator();

  /**
   * Create an image iterator for a given image data and a given extent
   */
  vtkImageIterator(vtkImageData* id, int* ext);

  /**
   * Initialize the image iterator for a given image data, and given extent
   */
  void Initialize(vtkImageData* id, int* ext);

  /**
   * Move the iterator to the next span
   */
  void NextSpan();

  /**
   * Return an iterator (pointer) for the span
   */
  SpanIterator BeginSpan() { return this->Pointer; }

  /**
   * Return an iterator (pointer) for the end of the span
   */
  SpanIterator EndSpan() { return this->SpanEndPointer; }

  /**
   * Test if the end of the extent has been reached
   */
  vtkTypeBool IsAtEnd() { return (this->Pointer >= this->EndPointer); }

protected:
  DType* Pointer;
  DType* SpanEndPointer;
  DType* SliceEndPointer;
  DType* EndPointer;
  vtkIdType Increments[3];
  vtkIdType ContinuousIncrements[3];
};

// IsAtEnd
template<>
inline vtkTypeBool vtkImageIterator<float>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<double>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<char>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<signed char>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<unsigned char>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<short>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<unsigned short>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<int>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<unsigned int>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<long>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<unsigned long>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<long long>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

template<>
inline vtkTypeBool vtkImageIterator<unsigned long long>::IsAtEnd() { return (this->Pointer >= this->EndPointer); }

//BeginSpan
template<>
inline long* vtkImageIterator<long>::BeginSpan() { return this->Pointer; }

template<>
inline long long* vtkImageIterator<long long>::BeginSpan() { return this->Pointer; }

template<>
inline unsigned long* vtkImageIterator<unsigned long>::BeginSpan() { return this->Pointer; }

template<>
inline unsigned long long* vtkImageIterator<unsigned long long>::BeginSpan() { return this->Pointer; }

template<>
inline int* vtkImageIterator<int>::BeginSpan() { return this->Pointer; }

template<>
inline unsigned int* vtkImageIterator<unsigned int>::BeginSpan() { return this->Pointer; }

template<>
inline short* vtkImageIterator<short>::BeginSpan() { return this->Pointer; }

template<>
inline unsigned short* vtkImageIterator<unsigned short>::BeginSpan() { return this->Pointer; }

template<>
inline char* vtkImageIterator<char>::BeginSpan() { return this->Pointer; }

template<>
inline signed char* vtkImageIterator<signed char>::BeginSpan() { return this->Pointer; }

template<>
inline unsigned char* vtkImageIterator<unsigned char>::BeginSpan() { return this->Pointer; }

template<>
inline double* vtkImageIterator<double>::BeginSpan() { return this->Pointer; }

template<>
inline float* vtkImageIterator<float>::BeginSpan() { return this->Pointer; }



// EndSpan
template<>
inline long* vtkImageIterator<long>::EndSpan() { return this->SpanEndPointer; }

template<>
inline long long* vtkImageIterator<long long>::EndSpan() { return this->SpanEndPointer; }

template<>
inline unsigned long* vtkImageIterator<unsigned long>::EndSpan() { return this->SpanEndPointer; }

template<>
inline unsigned long long* vtkImageIterator<unsigned long long>::EndSpan() { return this->SpanEndPointer; }

template<>
inline int* vtkImageIterator<int>::EndSpan() { return this->SpanEndPointer; }

template<>
inline unsigned int* vtkImageIterator<unsigned int>::EndSpan() { return this->SpanEndPointer; }

template<>
inline short* vtkImageIterator<short>::EndSpan() { return this->SpanEndPointer; }

template<>
inline unsigned short* vtkImageIterator<unsigned short>::EndSpan() { return this->SpanEndPointer; }

template<>
inline char* vtkImageIterator<char>::EndSpan() { return this->SpanEndPointer; }

template<>
inline signed char* vtkImageIterator<signed char>::EndSpan() { return this->SpanEndPointer; }

template<>
inline unsigned char* vtkImageIterator<unsigned char>::EndSpan() { return this->SpanEndPointer; }

template<>
inline double* vtkImageIterator<double>::EndSpan() { return this->SpanEndPointer; }

template<>
inline float* vtkImageIterator<float>::EndSpan() { return this->SpanEndPointer; }

#ifndef vtkImageIterator_cxx
#ifdef _MSC_VER
#pragma warning(push)
// The following is needed when the vtkImageIterator is declared
// dllexport and is used from another class in vtkCommonCore
#pragma warning(disable : 4910) // extern and dllexport incompatible
#endif
vtkExternTemplateMacro(extern template class VTKCOMMONDATAMODEL_EXPORT vtkImageIterator);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif

VTK_ABI_NAMESPACE_END
#endif
// VTK-HeaderTest-Exclude: vtkImageIterator.h

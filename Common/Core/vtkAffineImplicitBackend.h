// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// Funded by CEA, DAM, DIF, F-91297 Arpajon, France
#ifndef vtkAffineImplicitBackend_h
#define vtkAffineImplicitBackend_h

#include "vtkCommonCoreModule.h"

/**
 * \struct vtkAffineImplicitBackend
 * \brief A utility structure serving as a backend for affine (as a function of the index) implicit
 * arrays
 *
 * This structure can be classified as a closure and can be called using syntax similar to a
 * function call.
 *
 * At construction it takes two parameters: the slope of the map and the intercept. It returns a
 * value calculated as:
 *
 *   value = slope * index + intercept
 *
 * An example of potential usage in a vtkImplicitArray
 * ```
 * double slope = some_number;
 * double intercept = some_other_number;
 * vtkNew<vtkImplicitArray<vtkAffineImplicitBackend<double>>> affineArray;
 * affineArray->SetBackend(std::make_shared<vtkAffineImplicitBackend<double>>(slope, intercept));
 * affineArray->SetNumberOfTuples(however_many_you_want);
 * affineArray->SetNumberOfComponents(whatever_youd_like);
 * double value = affineArray->GetTypedComponent(index_in_tuple_range, index_in_component_range);
 * ```
 */
VTK_ABI_NAMESPACE_BEGIN
template <typename ValueType>
struct VTKCOMMONCORE_EXPORT vtkAffineImplicitBackend final
{
  /**
   * A non-trivially constructible constructor
   *
   * \param slope the slope of the affine function
   * \param intercept the intercept value at the origin (i.e. the value at 0)
   */
  vtkAffineImplicitBackend(ValueType slope, ValueType intercept);

  /**
   * The main call method for the backend
   *
   * \param index the index at which one wished to evaluate the backend
   * \return the affinely computed value
   */
  ValueType operator()(int index) const;

  /**
   * The slope of the affine function on the indeces
   */
  ValueType Slope;
  /**
   * The value of the affine function at index 0
   */
  ValueType Intercept;
};

template <>
inline vtkAffineImplicitBackend<double>::vtkAffineImplicitBackend(double slope, double intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<char>::vtkAffineImplicitBackend(char slope, char intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<float>::vtkAffineImplicitBackend(float slope, float intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<int>::vtkAffineImplicitBackend(int slope, int intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<long>::vtkAffineImplicitBackend(long slope, long intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<long long>::vtkAffineImplicitBackend(long long slope, long long intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<short>::vtkAffineImplicitBackend(short slope, short intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<signed char>::vtkAffineImplicitBackend(signed char slope, signed char intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<unsigned char>::vtkAffineImplicitBackend(unsigned char slope, unsigned char intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<unsigned int>::vtkAffineImplicitBackend(unsigned int slope, unsigned int intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<unsigned long>::vtkAffineImplicitBackend(unsigned long slope, unsigned long intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<unsigned long long>::vtkAffineImplicitBackend(unsigned long long slope, unsigned long long intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

template <>
inline vtkAffineImplicitBackend<unsigned short>::vtkAffineImplicitBackend(unsigned short slope, unsigned short intercept)
  : Slope(slope)
  , Intercept(intercept)
{
}

VTK_ABI_NAMESPACE_END

#endif // vtkAffineImplicitBackend_h

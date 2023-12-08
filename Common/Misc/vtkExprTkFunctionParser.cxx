// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkExprTkFunctionParser.h"
#include "vtkObjectFactory.h"

#include <algorithm>
#include <cctype>
#include <random>
#include <sstream>

// exprtk macros
#define exprtk_disable_string_capabilities
#define exprtk_disable_rtl_io_file
#define exprtk_disable_caseinsensitivity
#include "vtk_exprtk.h"
#include "vtksys/SystemTools.hxx"

using ExprTkResultType = exprtk::results_context<double>::type_store_t::store_type;

/**
 * Implementation of vtkExprTkTools
 */
VTK_ABI_NAMESPACE_BEGIN
struct vtkExprTkTools
{
  exprtk::symbol_table<double> SymbolTable;
  exprtk::expression<double> Expression;
  exprtk::parser<double> Parser;
};

/**
 * Implementation of the magnitude function
 */
template <typename T>
class mag : public exprtk::igeneric_function<T>
{
public:
  typedef typename exprtk::igeneric_function<T> igfun_t;
  typedef typename igfun_t::parameter_list_t parameter_list_t;
  typedef typename igfun_t::generic_type generic_type;
  typedef typename generic_type::scalar_view scalar_t;
  typedef typename generic_type::vector_view vector_t;

  using exprtk::igeneric_function<T>::operator();

  mag()
    : exprtk::igeneric_function<T>("V|VTT")
  /*
     Overloads:
     0. V   - x(vector)
     1. VTT - x(vector), r0, r1
  */
  {
  }

  inline T operator()(const std::size_t& ps_index, parameter_list_t parameters) override
  {
    const vector_t x(parameters[0]);

    std::size_t r0 = 0;
    std::size_t r1 = x.size() - 1;

    if ((1 == ps_index) &&
      !exprtk::rtl::vecops::helper::load_vector_range<T>::process(parameters, r0, r1, 2, 3, 0))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }
    else if (exprtk::rtl::vecops::helper::invalid_range(x, r0, r1))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }

    T result = T(0);

    for (std::size_t i = r0; i <= r1; ++i)
    {
      result += (x[i] * x[i]);
    }
    result = std::sqrt(result);

    return result;
  }
};

/**
 * Implementation of the x element of cross product function
 */
template <typename T>
class crossX : public exprtk::igeneric_function<T>
{
public:
  typedef typename exprtk::igeneric_function<T> igfun_t;
  typedef typename igfun_t::parameter_list_t parameter_list_t;
  typedef typename igfun_t::generic_type generic_type;
  typedef typename generic_type::scalar_view scalar_t;
  typedef typename generic_type::vector_view vector_t;

  using exprtk::igeneric_function<T>::operator();

  crossX()
    : exprtk::igeneric_function<T>("VV|VVTT")
  /*
     Overloads:
     0. VV   - x(vector), y(vector)
     1. VVTT - x(vector), y(vector), r0, r1
  */
  {
  }

  inline T operator()(const std::size_t& ps_index, parameter_list_t parameters) override
  {
    const vector_t x(parameters[0]);
    const vector_t y(parameters[1]);

    std::size_t r0 = 0;
    std::size_t r1 = std::min(x.size(), y.size()) - 1;

    if ((1 == ps_index) &&
      !exprtk::rtl::vecops::helper::load_vector_range<T>::process(parameters, r0, r1, 2, 3, 0))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }
    else if (exprtk::rtl::vecops::helper::invalid_range(y, r0, r1))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }

    T result = x[1] * y[2] - x[2] * y[1];

    return result;
  }
};

/**
 * Implementation of the y element of cross product function
 */
template <typename T>
class crossY : public exprtk::igeneric_function<T>
{
public:
  typedef typename exprtk::igeneric_function<T> igfun_t;
  typedef typename igfun_t::parameter_list_t parameter_list_t;
  typedef typename igfun_t::generic_type generic_type;
  typedef typename generic_type::scalar_view scalar_t;
  typedef typename generic_type::vector_view vector_t;

  using exprtk::igeneric_function<T>::operator();

  crossY()
    : exprtk::igeneric_function<T>("VV|VVTT")
  /*
     Overloads:
     0. VV   - x(vector), y(vector)
     1. VVTT - x(vector), y(vector), r0, r1
  */
  {
  }

  inline T operator()(const std::size_t& ps_index, parameter_list_t parameters) override
  {
    const vector_t x(parameters[0]);
    const vector_t y(parameters[1]);

    std::size_t r0 = 0;
    std::size_t r1 = std::min(x.size(), y.size()) - 1;

    if ((1 == ps_index) &&
      !exprtk::rtl::vecops::helper::load_vector_range<T>::process(parameters, r0, r1, 2, 3, 0))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }
    else if (exprtk::rtl::vecops::helper::invalid_range(y, r0, r1))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }

    T result = x[2] * y[0] - x[0] * y[2];

    return result;
  }
};

/**
 * Implementation of the z element of cross product function
 */
template <typename T>
class crossZ : public exprtk::igeneric_function<T>
{
public:
  typedef typename exprtk::igeneric_function<T> igfun_t;
  typedef typename igfun_t::parameter_list_t parameter_list_t;
  typedef typename igfun_t::generic_type generic_type;
  typedef typename generic_type::scalar_view scalar_t;
  typedef typename generic_type::vector_view vector_t;

  using exprtk::igeneric_function<T>::operator();

  crossZ()
    : exprtk::igeneric_function<T>("VV|VVTT")
  /*
     Overloads:
     0. VV   - x(vector), y(vector)
     1. VVTT - x(vector), y(vector), r0, r1
  */
  {
  }

  inline T operator()(const std::size_t& ps_index, parameter_list_t parameters) override
  {
    const vector_t x(parameters[0]);
    const vector_t y(parameters[1]);

    std::size_t r0 = 0;
    std::size_t r1 = std::min(x.size(), y.size()) - 1;

    if ((1 == ps_index) &&
      !exprtk::rtl::vecops::helper::load_vector_range<T>::process(parameters, r0, r1, 2, 3, 0))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }
    else if (exprtk::rtl::vecops::helper::invalid_range(y, r0, r1))
    {
      return std::numeric_limits<T>::quiet_NaN();
    }

    T result = x[0] * y[1] - x[1] * y[0];

    return result;
  }
};

namespace
{
/**
 * Implementation of sign function.
 */
inline double sign(double v)
{
  if (v == 0.)
  {
    return 0.;
  }
  else if (std::signbit(v))
  {
    return -1.;
  }
  else
  {
    return 1.;
  }
}

// compile-time declaration of needed function/variables/vectors/packages
// these are useful to minimize the construction cost, especially when
// multiple instances of this class are instantiated
exprtk::rtl::vecops::package<double> vectorOperationsPackage;
std::vector<double> iHat = { 1, 0, 0 };
std::vector<double> jHat = { 0, 1, 0 };
std::vector<double> kHat = { 0, 0, 1 };
mag<double> magnitude;
crossX<double> crossXProduct;
crossY<double> crossYProduct;
crossZ<double> crossZProduct;

// the value that is returned as a result if there is an error
double vtkParserErrorResult = std::numeric_limits<double>::quiet_NaN();
double vtkParserVectorErrorResult[3] = { vtkParserErrorResult, vtkParserErrorResult,
  vtkParserErrorResult };

//------------------------------------------------------------------------------
std::string RemoveSpacesFrom(std::string str)
{
  str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
  return str;
}

//------------------------------------------------------------------------------
bool HasEnding(const std::string& fullString, const std::string& ending)
{
  if (fullString.size() >= ending.size())
  {
    return (fullString.compare(fullString.size() - ending.size(), ending.size(), ending) == 0);
  }
  else
  {
    return false;
  }
}

//------------------------------------------------------------------------------
std::string GenerateRandomAlphabeticString(unsigned int len)
{
  static constexpr auto chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz";
  auto rng = std::default_random_engine(std::random_device{}());
  auto dist = std::uniform_int_distribution<int>(0, static_cast<int>(std::strlen(chars) - 1));
  auto result = std::string(len, '\0');
  std::generate_n(begin(result), len, [&]() { return chars[dist(rng)]; });

  return result;
}

//------------------------------------------------------------------------------
std::string GenerateUniqueVariableName(
  const std::vector<std::string>& variableNames, const std::string& variableName)
{
  std::string sanitizedName = vtkExprTkFunctionParser::SanitizeName(variableName.c_str());
  do
  {
    sanitizedName += GenerateRandomAlphabeticString(5);
  } while (
    std::find(variableNames.begin(), variableNames.end(), sanitizedName) != variableNames.end());

  return sanitizedName;
}
}

vtkStandardNewMacro(vtkExprTkFunctionParser);

//------------------------------------------------------------------------------
vtkExprTkFunctionParser::vtkExprTkFunctionParser()
{
  this->ParseMTime.Modified();
  this->FunctionMTime.Modified();

  this->ReplaceInvalidValues = 0;
  this->ReplacementValue = 0.0;

  this->ExprTkTools = new vtkExprTkTools;
  // add vector support
  this->ExprTkTools->SymbolTable.add_package(vectorOperationsPackage);
  // add unit vectors
  this->ExprTkTools->SymbolTable.add_vector("iHat", iHat);
  this->ExprTkTools->SymbolTable.add_vector("jHat", jHat);
  this->ExprTkTools->SymbolTable.add_vector("kHat", kHat);
  // add ln and sign
  this->ExprTkTools->SymbolTable.add_function("ln", std::log);
  this->ExprTkTools->SymbolTable.add_function("sign", sign);
  // add magnitude function
  this->ExprTkTools->SymbolTable.add_function("mag", magnitude);
  // add functions which are used to implement cross product
  this->ExprTkTools->SymbolTable.add_function("crossX", crossXProduct);
  this->ExprTkTools->SymbolTable.add_function("crossY", crossYProduct);
  this->ExprTkTools->SymbolTable.add_function("crossZ", crossZProduct);
  // register symbol table
  this->ExprTkTools->Expression.register_symbol_table(this->ExprTkTools->SymbolTable);
  // enable the collection of variables, which will be used in UpdateNeededVariables
  this->ExprTkTools->Parser.dec().collect_variables() = true;
}

//------------------------------------------------------------------------------
vtkExprTkFunctionParser::~vtkExprTkFunctionParser()
{
  this->RemoveAllVariables();
  delete this->ExprTkTools;
}

VTK_ABI_NAMESPACE_END

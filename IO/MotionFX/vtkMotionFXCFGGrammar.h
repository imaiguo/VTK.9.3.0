// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#ifndef vtkMotionFXCFGGrammar_h
#define vtkMotionFXCFGGrammar_h

// Internal header used by vtkMotionFXCFGReader.
// We define the various grammars here rather than clobbering the
// vtkMotionFXCFGReader.cxx.

#include <vtk_pegtl.h>

// for debugging
// clang-format off
#include VTK_PEGTL(pegtl/contrib/tracer.hpp)
// clang-format on

namespace MotionFX
{
using namespace tao::pegtl;

//-----------------------------------------------------------------------------
// lets define some common rules here.
namespace Common
{
VTK_ABI_NAMESPACE_BEGIN
struct Sign : sor<one<'+'>, one<'-'>>
{
};
struct Exponent : seq<sor<one<'e'>, one<'E'>>, opt<Sign>, plus<digit>>
{
};
struct Number
  : seq<opt<Sign>,
      sor<seq<plus<digit>, one<'.'>, star<digit>>, seq<one<'.'>, plus<digit>>, plus<digit>>,
      opt<Exponent>>
{
};

// delimiter for columns in files such as the position files
// this can be ',' separated by optional spaces or just spaces
struct Delimiter : sor<seq<star<space>, one<','>, star<space>>, plus<space>>
{
};
VTK_ABI_NAMESPACE_END
} // namespace Common

//-----------------------------------------------------------------------------
// rules for parsing a position file in legacy format, also called old rot.vel.
// format.
namespace LegacyPositionFile
{
VTK_ABI_NAMESPACE_BEGIN
using namespace Common;

// format: time CoMx CoMy CoMz Fx Fy Fz
struct Row
  : seq<star<space>, Number, Delimiter, Number, Delimiter, Number, Delimiter, Number, Delimiter,
      Number, Delimiter, Number, Delimiter, Number, star<space>>
{
};

struct Grammar : star<Row>
{
};
VTK_ABI_NAMESPACE_END
} // namespace LegacyPositionFile

//-----------------------------------------------------------------------------
// rules for parsing a position file in orientations formation.
namespace OrientationsPositionFile
{
VTK_ABI_NAMESPACE_BEGIN
using namespace Common;

// format: time CoMx CoMy CoMz cosX cosY cosZ Orientation (radians)
struct Row
  : seq<star<space>, Number, Delimiter, Number, Delimiter, Number, Delimiter, Number, Delimiter,
      Number, Delimiter, Number, Delimiter, Number, Delimiter, Number, star<space>>
{
};

struct Grammar : star<Row>
{
};
VTK_ABI_NAMESPACE_END
} // namespace OrientationsPositionFile

//-----------------------------------------------------------------------------
// rules for parsing a universal transform file
namespace UniversalTransformRow
{
VTK_ABI_NAMESPACE_BEGIN
using namespace Common;

// format: time
//         trnvecx trnvecy trnvecz
//         rotcntrx rotcntry rotcntrz
//         quat0 quat1 quat2 quat3
//         scalex scaley scalez
struct Row
  : seq<star<space>, Number, Delimiter, Number, Delimiter, Number, Delimiter, Number, Delimiter,
      Number, Delimiter, Number, Delimiter, Number, Delimiter, Number, Delimiter, Number, Delimiter,
      Number, Delimiter, Number, Delimiter, Number, Delimiter, Number, Delimiter, Number,
      star<space>>
{
};

struct Grammar : star<Row>
{
};
VTK_ABI_NAMESPACE_END
} // namespace UniversalTransformRow

//-----------------------------------------------------------------------------
// rules to parse CFG file.
namespace CFG
{
VTK_ABI_NAMESPACE_BEGIN
using namespace Common;

// Rule that matches a Comment. Consume everything on the line following a ';'
struct Comment : seq<string<';'>, until<eolf>>
{
};

struct WS_Required : sor<Comment, eol, plus<space>>
{
};
struct WS : star<WS_Required>
{
};

struct Value : plus<not_one<';', '}', '\r', '\n'>>
{
};

struct ParameterName : identifier
{
};
struct Statement : seq<ParameterName, WS_Required, Value>
{
};
struct StatementOther : seq<ParameterName, WS_Required, plus<not_one<'}', '{', ';'>>>
{
};

struct Motion : seq<TAO_PEGTL_STRING("motion"), WS, one<'{'>, WS, list<Statement, WS>, WS, one<'}'>>
{
};
struct Motions : seq<TAO_PEGTL_STRING("motions"), WS, one<'{'>, WS, list<Motion, WS>, WS, one<'}'>>
{
};

struct OtherNonNested : seq<identifier, WS, one<'{'>, WS, list<StatementOther, WS>, WS, one<'}'>>
{
};

struct OtherNested
  : seq<identifier, WS, one<'{'>, WS, list<sor<OtherNonNested, StatementOther>, WS>, WS, one<'}'>>
{
};

struct Lines : sor<Comment, space, Motions, OtherNonNested, OtherNested>
{
};

struct Grammar : star<Lines>
{
};

VTK_ABI_NAMESPACE_END
} // namespace CFG
} // namespace MotionFX

#endif
// VTK-HeaderTest-Exclude: vtkMotionFXCFGGrammar.h

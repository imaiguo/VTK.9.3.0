// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
/**
 * @class   vtkExtractSelectedGraph
 * @brief   return a subgraph of a vtkGraph
 *
 *
 * The first input is a vtkGraph to take a subgraph from.
 * The second input (optional) is a vtkSelection containing selected
 * indices. The third input (optional) is a vtkAnnotationsLayers whose
 * annotations contain selected specifying selected indices.
 * The vtkSelection may have FIELD_TYPE set to POINTS (a vertex selection)
 * or CELLS (an edge selection).  A vertex selection preserves all edges
 * that connect selected vertices.  An edge selection preserves all vertices
 * that are adjacent to at least one selected edge.  Alternately, you may
 * indicate that an edge selection should maintain the full set of vertices,
 * by turning RemoveIsolatedVertices off.
 */

#ifndef vtkExtractSelectedGraph_h
#define vtkExtractSelectedGraph_h

#include "vtkGraphAlgorithm.h"
#include "vtkInfovisCoreModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class vtkSelection;
class vtkDataSet;

class VTKINFOVISCORE_EXPORT vtkExtractSelectedGraph : public vtkGraphAlgorithm
{
public:
  static vtkExtractSelectedGraph* New();
  vtkTypeMacro(vtkExtractSelectedGraph, vtkGraphAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * A convenience method for setting the second input (i.e. the selection).
   */
  void SetSelectionConnection(vtkAlgorithmOutput* in);

  /**
   * A convenience method for setting the third input (i.e. the annotation layers).
   */
  void SetAnnotationLayersConnection(vtkAlgorithmOutput* in);

  ///@{
  /**
   * If set, removes vertices with no adjacent edges in an edge selection.
   * A vertex selection ignores this flag and always returns the full set
   * of selected vertices.  Default is on.
   */
  vtkSetMacro(RemoveIsolatedVertices, bool);
  vtkGetMacro(RemoveIsolatedVertices, bool);
  vtkBooleanMacro(RemoveIsolatedVertices, bool);
  ///@}

  /**
   * Specify the first vtkGraph input and the second vtkSelection input.
   */
  int FillInputPortInformation(int port, vtkInformation* info) override;

protected:
  vtkExtractSelectedGraph();
  ~vtkExtractSelectedGraph() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  int RequestDataObject(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  bool RemoveIsolatedVertices;

private:
  vtkExtractSelectedGraph(const vtkExtractSelectedGraph&) = delete;
  void operator=(const vtkExtractSelectedGraph&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

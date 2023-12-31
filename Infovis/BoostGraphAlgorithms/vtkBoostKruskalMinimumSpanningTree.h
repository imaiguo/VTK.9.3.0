// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
/**
 * @class   vtkBoostKruskalMinimumSpanningTree
 * @brief   Constructs a minimum spanning
 *    tree from a graph and the weighting array
 *
 *
 *
 * This vtk class uses the Boost Kruskal Minimum Spanning Tree
 * generic algorithm to perform a minimum spanning tree creation given
 * a weighting value for each of the edges in the input graph.
 *
 * @sa
 * vtkGraph vtkBoostGraphAdapter
 */

#ifndef vtkBoostKruskalMinimumSpanningTree_h
#define vtkBoostKruskalMinimumSpanningTree_h

#include "vtkInfovisBoostGraphAlgorithmsModule.h" // For export macro
#include "vtkVariant.h"                           // For variant type

#include "vtkSelectionAlgorithm.h"

VTK_ABI_NAMESPACE_BEGIN
class VTKINFOVISBOOSTGRAPHALGORITHMS_EXPORT vtkBoostKruskalMinimumSpanningTree
  : public vtkSelectionAlgorithm
{
public:
  static vtkBoostKruskalMinimumSpanningTree* New();
  vtkTypeMacro(vtkBoostKruskalMinimumSpanningTree, vtkSelectionAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Set the name of the edge-weight input array, which must name an
   * array that is part of the edge data of the input graph and
   * contains numeric data. If the edge-weight array is not of type
   * vtkDoubleArray, the array will be copied into a temporary
   * vtkDoubleArray.
   */
  vtkSetStringMacro(EdgeWeightArrayName);
  ///@}

  ///@{
  /**
   * Set the output selection type. The default is to use the
   * the set of minimum spanning tree edges "MINIMUM_SPANNING_TREE_EDGES". No
   * other options are defined.
   */
  vtkSetStringMacro(OutputSelectionType);
  ///@}

  ///@{
  /**
   * Whether to negate the edge weights. By negating the edge
   * weights this algorithm will give you the 'maximal' spanning
   * tree (i.e. the algorithm will try to create a spanning tree
   * with the highest weighted edges). Defaulted to Off.
   * FIXME: put a real definition in...
   */
  void SetNegateEdgeWeights(bool value);
  vtkGetMacro(NegateEdgeWeights, bool);
  vtkBooleanMacro(NegateEdgeWeights, bool);
  ///@}

protected:
  vtkBoostKruskalMinimumSpanningTree();
  ~vtkBoostKruskalMinimumSpanningTree() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  int FillInputPortInformation(int port, vtkInformation* info) override;

  int FillOutputPortInformation(int port, vtkInformation* info) override;

private:
  char* EdgeWeightArrayName;
  char* OutputSelectionType;
  bool NegateEdgeWeights;
  float EdgeWeightMultiplier;

  vtkBoostKruskalMinimumSpanningTree(const vtkBoostKruskalMinimumSpanningTree&) = delete;
  void operator=(const vtkBoostKruskalMinimumSpanningTree&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

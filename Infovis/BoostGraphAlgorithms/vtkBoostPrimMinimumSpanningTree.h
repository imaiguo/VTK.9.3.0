// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
/**
 * @class   vtkBoostPrimMinimumSpanningTree
 * @brief   Constructs a minimum spanning
 *    tree from a graph, start node, and the weighting array
 *
 *
 *
 * This vtk class uses the Boost Prim Minimum Spanning Tree
 * generic algorithm to perform a minimum spanning tree creation given
 * a weighting value for each of the edges in the input graph and a
 * a starting node for the tree.
 * A couple of caveats to be noted with the Prim implementation versus the
 * Kruskal implementation:
 *   1. The negate edge weights function cannot be utilized to obtain a
 * 'maximal' spanning tree (an exception is thrown when negated edge weights
 * exist), and
 *   2. the Boost implementation of the Prim algorithm returns a vertex
 * predecessor map which results in some ambiguity about which edge from
 * the original graph should be utilized if parallel edges between nodes
 * exist; therefore, the current VTK implementation does not copy the edge
 * data from the graph to the new tree.
 *
 * @sa
 * vtkGraph vtkBoostGraphAdapter
 */

#ifndef vtkBoostPrimMinimumSpanningTree_h
#define vtkBoostPrimMinimumSpanningTree_h

#include "vtkInfovisBoostGraphAlgorithmsModule.h" // For export macro
#include "vtkStdString.h"                         // For string type
#include "vtkVariant.h"                           // For variant type

#include "vtkTreeAlgorithm.h"

VTK_ABI_NAMESPACE_BEGIN
class VTKINFOVISBOOSTGRAPHALGORITHMS_EXPORT vtkBoostPrimMinimumSpanningTree
  : public vtkTreeAlgorithm
{
public:
  static vtkBoostPrimMinimumSpanningTree* New();
  vtkTypeMacro(vtkBoostPrimMinimumSpanningTree, vtkTreeAlgorithm);
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

  /**
   * Set the index (into the vertex array) of the
   * minimum spanning tree 'origin' vertex.
   */
  void SetOriginVertex(vtkIdType index);

  /**
   * Set the minimum spanning tree 'origin' vertex.
   * This method is basically the same as above
   * but allows the application to simply specify
   * an array name and value, instead of having to
   * know the specific index of the vertex.
   */
  void SetOriginVertex(vtkStdString arrayName, vtkVariant value);

  ///@{
  /**
   * Stores the graph vertex ids for the tree vertices in an array
   * named "GraphVertexId".  Default is off.
   */
  vtkSetMacro(CreateGraphVertexIdArray, bool);
  vtkGetMacro(CreateGraphVertexIdArray, bool);
  vtkBooleanMacro(CreateGraphVertexIdArray, bool);
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
  vtkBoostPrimMinimumSpanningTree();
  ~vtkBoostPrimMinimumSpanningTree() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  int FillInputPortInformation(int port, vtkInformation* info) override;

private:
  char* EdgeWeightArrayName;
  vtkIdType OriginVertexIndex;
  vtkVariant OriginValue;
  bool CreateGraphVertexIdArray;
  bool ArrayNameSet;
  char* ArrayName;
  bool NegateEdgeWeights;
  float EdgeWeightMultiplier;

  ///@{
  /**
   * Using the convenience function internally
   */
  vtkSetStringMacro(ArrayName);
  ///@}

  /**
   * This method is basically a helper function to find
   * the index of a specific value within a specific array
   */
  vtkIdType GetVertexIndex(vtkAbstractArray* abstract, vtkVariant value);

  vtkBoostPrimMinimumSpanningTree(const vtkBoostPrimMinimumSpanningTree&) = delete;
  void operator=(const vtkBoostPrimMinimumSpanningTree&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

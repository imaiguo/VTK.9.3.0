// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

/**
 * @class   vtkExpandSelectedGraph
 * @brief   expands a selection set of a vtkGraph
 *
 *
 * The first input is a vtkSelection containing the selected vertices.
 * The second input is a vtkGraph.
 * This filter 'grows' the selection set in one of the following ways
 * 1) SetBFSDistance controls how many 'hops' the selection is grown
 *    from each seed point in the selection set (defaults to 1)
 * 2) IncludeShortestPaths controls whether this filter tries to
 *    'connect' the vertices in the selection set by computing the
 *    shortest path between the vertices (if such a path exists)
 * Note: IncludeShortestPaths is currently non-functional
 */

#ifndef vtkExpandSelectedGraph_h
#define vtkExpandSelectedGraph_h

#include "vtkInfovisCoreModule.h" // For export macro
#include "vtkSelectionAlgorithm.h"

VTK_ABI_NAMESPACE_BEGIN
class vtkGraph;
class vtkIdTypeArray;

class VTKINFOVISCORE_EXPORT vtkExpandSelectedGraph : public vtkSelectionAlgorithm
{
public:
  static vtkExpandSelectedGraph* New();
  vtkTypeMacro(vtkExpandSelectedGraph, vtkSelectionAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * A convenience method for setting the second input (i.e. the graph).
   */
  void SetGraphConnection(vtkAlgorithmOutput* in);

  /**
   * Specify the first vtkSelection input and the second vtkGraph input.
   */
  int FillInputPortInformation(int port, vtkInformation* info) override;

  ///@{
  /**
   * Set/Get BFSDistance which controls how many 'hops' the selection
   * is grown from each seed point in the selection set (defaults to 1)
   */
  vtkSetMacro(BFSDistance, int);
  vtkGetMacro(BFSDistance, int);
  ///@}

  ///@{
  /**
   * Set/Get IncludeShortestPaths controls whether this filter tries to
   * 'connect' the vertices in the selection set by computing the
   * shortest path between the vertices (if such a path exists)
   * Note: IncludeShortestPaths is currently non-functional
   */
  vtkSetMacro(IncludeShortestPaths, bool);
  vtkGetMacro(IncludeShortestPaths, bool);
  vtkBooleanMacro(IncludeShortestPaths, bool);
  ///@}

  ///@{
  /**
   * Set/Get the vertex domain to use in the expansion.
   */
  vtkSetStringMacro(Domain);
  vtkGetStringMacro(Domain);
  ///@}

  ///@{
  /**
   * Whether or not to use the domain when deciding to add a vertex to the
   * expansion. Defaults to false.
   */
  vtkSetMacro(UseDomain, bool);
  vtkGetMacro(UseDomain, bool);
  vtkBooleanMacro(UseDomain, bool);
  ///@}

protected:
  vtkExpandSelectedGraph();
  ~vtkExpandSelectedGraph() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  void Expand(vtkIdTypeArray*, vtkGraph*);

  int BFSDistance;
  bool IncludeShortestPaths;
  char* Domain;
  bool UseDomain;

private:
  vtkExpandSelectedGraph(const vtkExpandSelectedGraph&) = delete;
  void operator=(const vtkExpandSelectedGraph&) = delete;

  void BFSExpandSelection(vtkIdTypeArray* selection, vtkGraph* graph);
};

VTK_ABI_NAMESPACE_END
#endif

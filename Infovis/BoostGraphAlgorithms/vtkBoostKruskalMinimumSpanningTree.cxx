// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
#include "vtkBoostKruskalMinimumSpanningTree.h"

#include "vtkBoostGraphAdapter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkMutableDirectedGraph.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkSelection.h"
#include "vtkSelectionNode.h"
#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkTree.h"
#include "vtkUndirectedGraph.h"

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/pending/queue.hpp>

using namespace boost;

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkBoostKruskalMinimumSpanningTree);

// Constructor/Destructor
vtkBoostKruskalMinimumSpanningTree::vtkBoostKruskalMinimumSpanningTree()
{
  this->EdgeWeightArrayName = nullptr;
  this->OutputSelectionType = nullptr;
  this->SetOutputSelectionType("MINIMUM_SPANNING_TREE_EDGES");
  this->NegateEdgeWeights = false;
  this->EdgeWeightMultiplier = 1;
}

vtkBoostKruskalMinimumSpanningTree::~vtkBoostKruskalMinimumSpanningTree()
{
  this->SetEdgeWeightArrayName(nullptr);
}

void vtkBoostKruskalMinimumSpanningTree::SetNegateEdgeWeights(bool value)
{
  this->NegateEdgeWeights = value;
  if (this->NegateEdgeWeights)
    this->EdgeWeightMultiplier = -1.0;
  else
    this->EdgeWeightMultiplier = 1.0;

  this->Modified();
}

int vtkBoostKruskalMinimumSpanningTree::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkGraph* input = vtkGraph::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkSelection* output = vtkSelection::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Retrieve the edge-weight array.
  if (!this->EdgeWeightArrayName)
  {
    vtkErrorMacro("Edge-weight array name is required");
    return 0;
  }
  vtkDataArray* edgeWeightArray = input->GetEdgeData()->GetArray(this->EdgeWeightArrayName);

  // Does the edge-weight array exist at all?
  if (edgeWeightArray == nullptr)
  {
    vtkErrorMacro("Could not find edge-weight array named " << this->EdgeWeightArrayName);
    return 0;
  }

  // Send the property map through both the multiplier and the
  // helper (for edge_descriptor indexing)
  typedef vtkGraphPropertyMapMultiplier<vtkDataArray*> mapMulti;
  mapMulti multi(edgeWeightArray, this->EdgeWeightMultiplier);
  vtkGraphEdgePropertyMapHelper<mapMulti> weight_helper(multi);

  // Run the algorithm
  std::vector<vtkEdgeType> mstEdges;
  if (vtkDirectedGraph::SafeDownCast(input))
  {
    vtkDirectedGraph* g = vtkDirectedGraph::SafeDownCast(input);
    kruskal_minimum_spanning_tree(g, std::back_inserter(mstEdges), weight_map(weight_helper));
  }
  else
  {
    vtkUndirectedGraph* g = vtkUndirectedGraph::SafeDownCast(input);
    kruskal_minimum_spanning_tree(g, std::back_inserter(mstEdges), weight_map(weight_helper));
  }

  // Select the minimum spanning tree edges.
  if (!strcmp(OutputSelectionType, "MINIMUM_SPANNING_TREE_EDGES"))
  {
    vtkIdTypeArray* ids = vtkIdTypeArray::New();

    // Add the ids of each MST edge.
    for (std::vector<vtkEdgeType>::iterator i = mstEdges.begin(); i != mstEdges.end(); ++i)
    {
      ids->InsertNextValue(i->Id);
    }

    vtkSmartPointer<vtkSelectionNode> node = vtkSmartPointer<vtkSelectionNode>::New();
    output->AddNode(node);
    node->SetSelectionList(ids);
    node->SetContentType(vtkSelectionNode::INDICES);
    node->SetFieldType(vtkSelectionNode::EDGE);
    ids->Delete();
  }

  return 1;
}

//------------------------------------------------------------------------------
int vtkBoostKruskalMinimumSpanningTree::FillInputPortInformation(int port, vtkInformation* info)
{
  // now add our info
  if (port == 0)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkGraph");
  }
  return 1;
}

//------------------------------------------------------------------------------
int vtkBoostKruskalMinimumSpanningTree::FillOutputPortInformation(int port, vtkInformation* info)
{
  // now add our info
  if (port == 0)
  {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkSelection");
  }
  return 1;
}

//------------------------------------------------------------------------------
void vtkBoostKruskalMinimumSpanningTree::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "EdgeWeightArrayName: "
     << (this->EdgeWeightArrayName ? this->EdgeWeightArrayName : "(none)") << endl;

  os << indent << "OutputSelectionType: "
     << (this->OutputSelectionType ? this->OutputSelectionType : "(none)") << endl;

  os << indent << "NegateEdgeWeights: " << (this->NegateEdgeWeights ? "true" : "false") << endl;

  os << indent << "EdgeWeightMultiplier: " << this->EdgeWeightMultiplier << endl;
}
VTK_ABI_NAMESPACE_END

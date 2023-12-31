// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
#include "vtkBoostBrandesCentrality.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkSmartPointer.h"
#include "vtkStringArray.h"

#include "vtkBoostGraphAdapter.h"
#include "vtkDirectedGraph.h"
#include "vtkUndirectedGraph.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/properties.hpp>

using namespace boost;

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkBoostBrandesCentrality);

//------------------------------------------------------------------------------
vtkBoostBrandesCentrality::vtkBoostBrandesCentrality()
  : UseEdgeWeightArray(false)
  , InvertEdgeWeightArray(false)
  , EdgeWeightArrayName(nullptr)
{
}

//------------------------------------------------------------------------------
vtkBoostBrandesCentrality::~vtkBoostBrandesCentrality()
{
  this->SetEdgeWeightArrayName(nullptr);
}

//------------------------------------------------------------------------------
void vtkBoostBrandesCentrality::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "UseEdgeWeightArray: " << this->UseEdgeWeightArray << endl;

  os << indent << "InvertEdgeWeightArray: " << this->InvertEdgeWeightArray << endl;

  os << indent << "this->EdgeWeightArrayName: "
     << (this->EdgeWeightArrayName ? this->EdgeWeightArrayName : "nullptr") << endl;
}

//------------------------------------------------------------------------------
int vtkBoostBrandesCentrality::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkGraph* input = vtkGraph::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkGraph* output = vtkGraph::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Send the data to output.
  output->ShallowCopy(input);

  // Compute betweenness centrality

  // Property map for vertices
  vtkFloatArray* vertexCMap = vtkFloatArray::New();
  vertexCMap->SetName("centrality");
  identity_property_map imap;

  // Property map for edges
  vtkFloatArray* edgeCMap = vtkFloatArray::New();
  edgeCMap->SetName("centrality");
  vtkGraphEdgePropertyMapHelper<vtkFloatArray*> helper(edgeCMap);

  vtkSmartPointer<vtkDataArray> edgeWeight(nullptr);
  if (this->UseEdgeWeightArray && this->EdgeWeightArrayName)
  {
    if (!this->InvertEdgeWeightArray)
    {
      edgeWeight = input->GetEdgeData()->GetArray(this->EdgeWeightArrayName);
    }
    else
    {
      vtkDataArray* weights = input->GetEdgeData()->GetArray(this->EdgeWeightArrayName);

      if (!weights)
      {
        vtkErrorMacro(<< "Error: Edge weight array " << this->EdgeWeightArrayName
                      << " is set but not found or not a data array.\n");
        return 0;
      }

      edgeWeight.TakeReference(vtkDataArray::CreateDataArray(weights->GetDataType()));

      double range[2];
      weights->GetRange(range);

      if (weights->GetNumberOfComponents() > 1)
      {
        return 0;
      }

      for (int i = 0; i < weights->GetDataSize(); ++i)
      {
        edgeWeight->InsertNextTuple1(range[1] - weights->GetTuple1(i));
      }
    }

    if (!edgeWeight)
    {
      vtkErrorMacro(<< "Error: Edge weight array " << this->EdgeWeightArrayName
                    << " is set but not found or not a data array.\n");
      return 0;
    }
  }

  // Is the graph directed or undirected
  if (vtkDirectedGraph::SafeDownCast(output))
  {
    vtkDirectedGraph* g = vtkDirectedGraph::SafeDownCast(output);
    if (edgeWeight)
    {
      vtkGraphEdgePropertyMapHelper<vtkDataArray*> helper2(edgeWeight);
      brandes_betweenness_centrality(g,
        centrality_map(vertexCMap)
          .edge_centrality_map(helper)
          .vertex_index_map(imap)
          .weight_map(helper2));
    }
    else
    {
      brandes_betweenness_centrality(
        g, centrality_map(vertexCMap).edge_centrality_map(helper).vertex_index_map(imap));
    }
  }
  else
  {
    vtkUndirectedGraph* g = vtkUndirectedGraph::SafeDownCast(output);
    if (edgeWeight)
    {
      vtkGraphEdgePropertyMapHelper<vtkDataArray*> helper2(edgeWeight);
      brandes_betweenness_centrality(g,
        centrality_map(vertexCMap)
          .edge_centrality_map(helper)
          .vertex_index_map(imap)
          .weight_map(helper2));
    }
    else
    {
      brandes_betweenness_centrality(
        g, centrality_map(vertexCMap).edge_centrality_map(helper).vertex_index_map(imap));
    }
  }

  // Add the arrays to the output and dereference
  output->GetVertexData()->AddArray(vertexCMap);
  vertexCMap->Delete();
  output->GetEdgeData()->AddArray(edgeCMap);
  edgeCMap->Delete();

  return 1;
}
VTK_ABI_NAMESPACE_END

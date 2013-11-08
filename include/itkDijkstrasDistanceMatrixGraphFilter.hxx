/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstrasPathGraphFilter.hxx,v $
  Language:  C++
  Date:
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDijkstrasDistanceMatrixGraphFilter_hxx
#define __itkDijkstrasDistanceMatrixGraphFilter_hxx

#include "itkDijkstrasDistanceMatrixGraphFilter.h"
#include "itkGraphToGraphFilter.h"
#include "vnl/vnl_numeric_traits.h"
#include <algorithm>
#include <utility>

namespace itk {

template <class TGraph, typename TValueType>
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::DijkstrasDistanceMatrixGraphFilter()
{
}

/** Generate the data */
template <class TGraph, typename TValueType>
void
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::GenerateData()
{

  this->m_Tree.clear();
 
  unsigned long nNodes = this->GetInput()->GetTotalNumberOfNodes();
  this->m_DistanceMatrix.SetSize( nNodes, nNodes );
  this->m_DistanceMatrix.Fill( 0.0 );
  this->m_NStepMatrix.SetSize( nNodes, nNodes );
  this->m_NStepMatrix.Fill( 0.0 );
    
  for (unsigned int i = 0; i < (nNodes-1); i++)
    {
    this->Initialize();
    this->GenerateDistancesFromSource( i );
    this->GrowTree();
    for ( unsigned int j=(i+1); j < nNodes; j++ )
      {
      this->m_DistanceMatrix(i,j) = this->GetOutput()->GetNodePointer(j)->Weight;
      this->m_DistanceMatrix(j,i) = this->m_DistanceMatrix(i,j);

      NodeListType path = this->BackTrace( j );      
      this->m_NStepMatrix(i,j) = path.size();
      this->m_NStepMatrix(j,i) = path.size();
      }
    }

  this->m_CharacteristicPathLength = 0.0;
  this->m_GlobalEfficiency = 0.0;
  this->m_NodeEccentricity.SetSize(nNodes);
  this->m_NodeEccentricity.Fill(0.0);
  this->m_Radius = nNodes;
  this->m_Diameter = 0.0;
  //unsigned long actualValues = 0;

  for (unsigned int i = 0; i < nNodes; i++)
    {
    for (unsigned int j = 0; j< nNodes; j++)
      {
      if ( i != j ) 
        {
        // throw exception if (NStep == 0)?

        // Exclude unreachable nodes
        if ( this->m_NStepMatrix(i,j) > 0 ) 
          {
          //++actualValues;
          this->m_CharacteristicPathLength += this->m_NStepMatrix(i,j);
          this->m_GlobalEfficiency += (1.0 / this->m_NStepMatrix(i,j));
          if ( this->m_NStepMatrix(i,j) > this->m_NodeEccentricity[i] ) 
            {
            this->m_NodeEccentricity[i] = this->m_NStepMatrix(i,j);
            }
          }
        }
      }
    if ( this->m_NodeEccentricity[i] < this->m_Radius )
      {
      this->m_Radius = this->m_NodeEccentricity[i];
      }
    if ( this->m_NodeEccentricity[i] > this->m_Diameter )
      {
      this->m_Diameter = this->m_NodeEccentricity[i];
      }
    }

  ValueType nValues = ((nNodes * nNodes) - nNodes );

  this->m_CharacteristicPathLength /= nValues;
  this->m_GlobalEfficiency /= nValues;
}


template <class TGraph, typename TValueType>
void
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::GenerateDistancesFromSource(unsigned int sourceNodeId)
{
  // Reset node values
  this->Reset();
  this->m_Output->GetNodePointer(sourceNodeId)->IsSource = true;
  this->m_Output->GetNodePointer(sourceNodeId)->IsActive = true;

  BoostGraphType bGraph( m_Output->GetTotalNumberOfNodes() );
  std::vector<BoostEdgeType> edgeVec( m_Output->GetTotalNumberOfEdges() );

  property_map<BoostGraphType, edge_weight_t>::type weightmap = get(edge_weight, bGraph);

  for (unsigned long i=0; i<m_Output->GetTotalNumberOfEdges(); i++)
    {

    // Check
    EdgePointerType graphEdge = m_Output->GetEdgePointer( i );
    // FIXME - throw an exception here
    if (graphEdge->Weight < 0)
      {
      itkExceptionMacro( "itk::DijkstrasDistanceMatrixGraphFilter::GenerateDistancesFromSource() - negative edge weights not allowed" );
      }

    BoostEdgeDescriptor e;
    bool inserted;
    int curNodeNum = graphEdge->SourceIdentifier;
    int toNodeNum = graphEdge->TargetIdentifier;
    tie(e, inserted) = add_edge(curNodeNum, toNodeNum, bGraph);
    weightmap[e] = graphEdge->Weight;
    }

  //int startNodeNum = this->m_SourceNodes[sourceNodeId];
  this->m_PreviousVertexList.clear();
  this->m_PreviousVertexList.reserve( num_vertices(bGraph) );

  std::vector<int> distance(num_vertices(bGraph));
  BoostVertexDescriptor s = vertex(sourceNodeId, bGraph);
  typename property_map<BoostGraphType, vertex_index_t>::type indexmap = get(vertex_index, bGraph);

  dijkstra_shortest_paths_no_color_map(bGraph, s, &(this->m_PreviousVertexList[0]), &distance[0], weightmap, indexmap,
                                       std::less<float>(), closed_plus<float>(),
                                       (std::numeric_limits<int>::max)(), 0,
                                       default_dijkstra_visitor());

  for (unsigned long i=0; i<m_Output->GetTotalNumberOfNodes(); i++)
    {
    NodePointerType node = m_Output->GetNodePointer( i );
    if (distance[i] < (std::numeric_limits<int>::max)() )
      {
      node->Weight = distance[i];
      node->Visited = true;
      node->PreviousNodes.clear();
      node->PreviousNodes.push_back( indexmap[this->m_PreviousVertexList[vertex(i,bGraph)]] );
      }
    else
      {
      node->Weight = 0;
      node->Visited = false;
      }
    }
  
}

template <class TGraph, typename TValueType>
typename DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>::NodeListType
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::BackTrace(NodeIdentifierType target)
{
  NodeListType nodeList;
  NodePointerType current = m_Output->GetNodePointer( target );
  nodeList.push_front(current);
  while (!current->IsSource && (current->PreviousNodes.size() > 0) )
    {
    current = this->m_Output->GetNodePointer( current->PreviousNodes[0] );
    nodeList.push_front(current);
    }

  // No path found
  if (!current->IsSource)
    {
    nodeList.clear();
    }
  return nodeList;
}

template <class TGraph, typename TValueType>
void
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::GrowTree( )
{
  for (unsigned int i=0; i<this->GetInput()->GetTotalNumberOfNodes(); i++)
    {
    NodeListType path = this->BackTrace( i );
    if (path.size() > 0)
      {
      this->m_Tree.push_back(path);
      }
    }
}

template <class TGraph, typename TValueType>
void
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::Initialize( )
{
  this->AllocateOutputs();
  this->m_Output = this->GetOutput();

  NodeIteratorType It( this->m_Output );
  for( It.GoToBegin(); !It.IsAtEnd(); ++It )
    {
    NodePointerType node = It.GetPointer();
    node->TimeStamp = 0;
    node->IsSource = false;
    node->IsTarget = false;
    node->Visited = false;
    node->Weight = -1.0;
    node->IsActive = true;
    node->PreviousNodes.clear();
    }
}

template <class TGraph, typename TValueType>
void
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::Reset( )
{
  this->AllocateOutputs();
  this->m_Output = this->GetOutput();

  NodeIteratorType It( this->m_Output );
  for( It.GoToBegin(); !It.IsAtEnd(); ++It )
    {
    NodePointerType node = It.GetPointer();
    node->TimeStamp = 0;
    node->IsSource = false;
    node->IsTarget = false;
    node->Visited = false;
    node->Weight = -1.0;
    node->PreviousNodes.clear();
    }
}

template <class TGraph, typename TValueType>
void
DijkstrasDistanceMatrixGraphFilter<TGraph,TValueType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif

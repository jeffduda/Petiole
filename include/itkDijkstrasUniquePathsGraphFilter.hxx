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
#ifndef __itkDijkstrasUniquePathsGraphFilter_hxx
#define __itkDijkstrasUniquePathsGraphFilter_hxx

#include "itkDijkstrasUniquePathsGraphFilter.h"
#include "itkGraphToGraphFilter.h"
#include "vnl/vnl_numeric_traits.h"
#include <algorithm>
#include <utility>

namespace itk {

template <class TGraph, typename TValueType>
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
::DijkstrasUniquePathsGraphFilter()
{
}

/** Generate the data */
template <class TGraph, typename TValueType>
void
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
::GenerateData()
{

  this->m_Tree.clear();
 
  unsigned long nNodes = this->GetInput()->GetTotalNumberOfNodes();
  this->m_UniquePathsMatrix.SetSize( nNodes, nNodes );
   
  for (unsigned int i = 0; i < (nNodes-1); i++)
    {
    for ( unsigned int j=(i+1); j<nNodes; j++)
      {
      this->Initialize();

      // First find direct connections
      unsigned long nActiveEdges = 0;
      for (unsigned int e = 0; e < this->GetOutput()->GetTotalNumberOfEdges(); e++)
        {
        EdgePointerType edge = this->GetOutput()->GetEdgePointer( e );
        if ( ( ( edge->SourceIdentifier == i) || ( edge->SourceIdentifier == j ) ) &&
             ( ( edge->TargetIdentifier == i) || ( edge->TargetIdentifier == j) ) )
          {
          this->m_UniquePathsMatrix(i,j) = 1;
          edge->IsActive = false;
          }
        if ( edge->IsActive ) 
          {
          ++nActiveEdges;
          }
        }

      bool scanning = (nActiveEdges > 0);
      unsigned int iterations = 0;
      while ( scanning ) 
        {      
        ++iterations;
        ValueType distance = this->GenerateDistanceFromSourceToTarget( i, j );
        if ( (distance < 0) || (iterations > 5) )
          {
          scanning = false;
          }
        else
          {
          this->m_UniquePathsMatrix(i,j) = this->m_UniquePathsMatrix(i,j)+1;
          }      
        }
      }
    }

  for ( unsigned int i=1; i<nNodes; i++)
    {
    for (unsigned int j=0; j<i; j++)
      {
      this->m_UniquePathsMatrix(i,j) = this->m_UniquePathsMatrix(j,i);
      }
    }

}


template <class TGraph, typename TValueType>
double
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
::GenerateDistanceFromSourceToTarget(unsigned int sourceNodeId, unsigned int targetNodeId)
{
  double returnDistance = -1;

  // Reset node values
  this->Reset();
  this->m_Output->GetNodePointer(sourceNodeId)->IsSource = true;
  this->m_Output->GetNodePointer(sourceNodeId)->IsActive = true;
  this->m_Output->GetNodePointer(targetNodeId)->IsTarget = true;
  this->m_Output->GetNodePointer(targetNodeId)->IsActive = true;

  BoostGraphType bGraph( m_Output->GetTotalNumberOfNodes() );
  std::vector<BoostEdgeType> edgeVec( m_Output->GetTotalNumberOfEdges() );

  property_map<BoostGraphType, edge_weight_t>::type weightmap = get(edge_weight, bGraph);

  unsigned long nActiveEdges = 0;
  unsigned long nSourceEdges = 0;
  unsigned long nTargetEdges = 0;
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
    if ( graphEdge->IsActive &&
         ( this->GetOutput()->GetNodePointer(curNodeNum)->IsActive ) &&
         ( this->GetOutput()->GetNodePointer(toNodeNum)->IsActive ) )
      {
      tie(e, inserted) = add_edge(curNodeNum, toNodeNum, bGraph);
      weightmap[e] = graphEdge->Weight;
      ++nActiveEdges;

      if ( (graphEdge->SourceIdentifier == sourceNodeId) || (graphEdge->TargetIdentifier == sourceNodeId ) ) 
        {
        ++nSourceEdges;
        }
      if ( (graphEdge->SourceIdentifier == targetNodeId ) || (graphEdge->TargetIdentifier == targetNodeId) )
        {
        ++nTargetEdges;
        }

      }
    }

  this->m_PreviousVertexList.clear();


  if ( (nActiveEdges < 2) || (nTargetEdges < 1) || (nSourceEdges < 1) )
    {
    return -1;
    }

  this->m_PreviousVertexList.reserve( num_vertices(bGraph) );

  std::vector<float> distance(num_vertices(bGraph));
  BoostVertexDescriptor s = vertex(sourceNodeId, bGraph);
  typename property_map<BoostGraphType, vertex_index_t>::type indexmap = get(vertex_index, bGraph);

  dijkstra_shortest_paths_no_color_map(bGraph, s, &(this->m_PreviousVertexList[0]), &distance[0], weightmap, indexmap,
                                       std::less<float>(), closed_plus<float>(),
                                       (std::numeric_limits<float>::max)(), 0,
                                       default_dijkstra_visitor());

  for (unsigned long i=0; i<m_Output->GetTotalNumberOfNodes(); i++)
    {
    NodePointerType node = m_Output->GetNodePointer( i );
    if (distance[i] < (std::numeric_limits<float>::max)() )
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

  returnDistance =  distance[ targetNodeId ];
  if ( returnDistance <  (std::numeric_limits<float>::max)() )
    {
    NodeListType path = this->BackTrace( targetNodeId );
    for (unsigned int i=1; i<(path.size()-1); i++)
      {
      this->GetOutput()->GetNodePointer( path[i]->Identifier )->IsActive = false;
      }
    }
  else 
    {
    returnDistance = -1.0;
    }
    
  return returnDistance;
}

template <class TGraph, typename TValueType>
typename DijkstrasUniquePathsGraphFilter<TGraph,TValueType>::NodeListType
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
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
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
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
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
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

  EdgeIteratorType eIt( this->m_Output );
  for( eIt.GoToBegin(); !eIt.IsAtEnd(); ++eIt )
    {
    EdgePointerType edge = eIt.GetPointer();
    edge->IsActive = true;
    }

}

template <class TGraph, typename TValueType>
void
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
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
DijkstrasUniquePathsGraphFilter<TGraph,TValueType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif

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
#ifndef __itkDijkstrasPathGraphFilter_hxx
#define __itkDijkstrasPathGraphFilter_hxx

#include "itkDijkstrasPathGraphFilter.h"
#include "itkGraphToGraphFilter.h"
#include "vnl/vnl_numeric_traits.h"
#include <algorithm>
#include <utility>

//#include <boost/config.hpp>
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//using namespace boost;

namespace itk {

template <class TGraph>
DijkstrasPathGraphFilter<TGraph>
::DijkstrasPathGraphFilter()
{
  this->m_SourceNodes = RegionType::New();
  this->m_TargetNodes = RegionType::New();
}

/** Generate the data */
template <class TGraph>
void
DijkstrasPathGraphFilter<TGraph>
::GenerateData()
{

  if ( this->m_SourceNodes->Size() == 0 )
    {
    itkExceptionMacro( "itk::DijkstrasPathGraphFilter::GenerateData() - Source nodes container is empty" );
    }
  else if ( this->m_TargetNodes->Size() == 0 )
    {
    itkExceptionMacro( "itk::DijkstrasPathGraphFilter::GenerateData() - Target nodes container is empty" );
    }

  this->m_Tree.clear();
 
  
  //for (unsigned int srcN=0; srcN<this->m_SourceNodes->Size(); srcN++)
  //  {
  for (unsigned int srcN=0; srcN<this->GetInput()->GetTotalNumberOfNodes(); srcN++)
    {
    //std::cout << "source node " << srcN << std::endl;
    this->Initialize();
    //this->GenerateDistancesFromSource(
    //this->m_SourceNodes->GetElement( srcN ) );
    this->GenerateDistancesFromSource( srcN );
    this->GrowTree();
    for ( unsigned int i=0; i<this->GetOutput()->GetTotalNumberOfNodes(); i++)
      {
      std::cout << this->GetOutput()->GetNodePointer(i)->Weight << " ";
      }
    std::cout << std::endl;
    }
  
}


template <class TGraph>
void
DijkstrasPathGraphFilter<TGraph>
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
      itkExceptionMacro( "itk::DijkstrasPathGraphFilter::GenerateDistancesFromSource() - negative edge weights not allowed" );
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


template <class TGraph>
typename DijkstrasPathGraphFilter<TGraph>::NodeListType
DijkstrasPathGraphFilter<TGraph>
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

template <class TGraph>
void
DijkstrasPathGraphFilter<TGraph>
::GrowTree( )
{
  for (unsigned int i=0; i<this->m_TargetNodes->Size(); i++)
    {
    NodeListType path = this->BackTrace( this->m_TargetNodes->GetElement(i) );
    if (path.size() > 0)
      {
      this->m_Tree.push_back(path);
      }
    }
}

template <class TGraph>
void
DijkstrasPathGraphFilter<TGraph>
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

template <class TGraph>
void
DijkstrasPathGraphFilter<TGraph>
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

template <class TGraph>
void
DijkstrasPathGraphFilter<TGraph>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif

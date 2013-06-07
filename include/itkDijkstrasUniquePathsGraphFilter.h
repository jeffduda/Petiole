/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstrasUniquePathsGraphFilter.h,v $
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
#ifndef __itkDijkstrasUniquePathsGraphFilter_h
#define __itkDijkstrasUniquePathsGraphFilter_h

#include "itkInPlaceGraphFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>

#include <deque>

using namespace boost;

namespace itk {

/** \class itkDijkstrasUniquePathsGraphFilter
 * \brief Partitions a graph into two subsets based on the min-cut criterion.
 *
 * \par
 * This class implements the min-cut/max-flow algorithm of Boykov and
 * Kolmogorov given in the reference below.  Given a weighted, directed
 * graph with a source node and a sink node, finding the max-flow
 * through the specified network is equivalent to finding the cut which
 * minimizes the sum of the weights between the two subsets associated
 * with the sink and the source.
 *
 * \par
 * Other max-flow/min-cut algorithms exist such as Goldberg-Tarjan style
 * "push-relabel" methods and Ford-Fulkerson "augmenting paths" methods.
 * Even though these other algorithms have lower theoretical complexity
 * (e.g. O(|E||V|^2) for the Dinic algorithm versus O(|E||V|^2|C|) for
 * the Boykov-Kolmogorov method where |C| is the cost of the minimum cut),
 * the Boykov-Kolmogorov method has been tailored to typical problems in
 * vision making its experimental performance better than other
 * algorithms.
 *
 * \par
 * This class is derived from the InPlaceGraphFilter since the output
 * is simply a labeling of the input (each node is designated to be
 * associated with either the source or the sink terminal.  Also,
 * instead of actually creating the terminal nodes and associated
 * links, memory usage is minimized by maintaing the difference between
 * the terminal link weights as the node weight.
 *
 * \par REFERENCE
 * Y. Boykov and V. Kolmogorov, "An Experimental Comparison of Min-
 * Cut/Max-Flow Algorithms for Energy Minimization in Vision,"
 * IEEE-PAMI, 26(9):1124-1137, 2004.
 *
 **/

template<class TGraph, typename TValueType=float>
class ITK_EXPORT DijkstrasUniquePathsGraphFilter
: public InPlaceGraphFilter<TGraph>
{
public:
  /** Standard "Self" typedef. */
  typedef DijkstrasUniquePathsGraphFilter   Self;
  typedef InPlaceGraphFilter<TGraph> Superclass;
  typedef SmartPointer<Self>         Pointer;
  typedef SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( DijkstrasUniquePathsGraphFilter, InPlaceGraphFilter );

  /** Hold on to the type information specified by the template parameters. */
  typedef TGraph                                        GraphType;
  typedef typename GraphType::GraphTraitsType           GraphTraitsType;
  typedef typename GraphType::NodeIterator              NodeIteratorType;
  typedef typename GraphType::ConstNodeIterator         ConstNodeIteratorType;
  typedef typename GraphType::EdgeIterator              EdgeIteratorType;
  typedef typename GraphTraitsType::NodeType            NodeType;
  typedef typename GraphTraitsType::EdgeType            EdgeType;
  typedef typename GraphTraitsType::NodePointerType     NodePointerType;
  typedef typename GraphTraitsType::EdgePointerType     EdgePointerType;
  typedef typename GraphTraitsType::NodeWeightType      WeightType;
  typedef typename GraphTraitsType::EdgeWeightType      EdgeWeightType;
  typedef typename GraphTraitsType
     ::EdgeIdentifierContainerType                      EdgeIdentifierContainerType;
  typedef typename GraphTraitsType::NodeIdentifierType  NodeIdentifierType;
  typedef typename GraphTraitsType::EdgeIdentifierType  EdgeIdentifierType;
  typedef std::vector<NodeIdentifierType>               NodeIdentifierContainerType;

  typedef TValueType                           ValueType;

  typedef adjacency_list < vecS, vecS, undirectedS, no_property, property < edge_weight_t, double > >
                                                               BoostGraphType;
  typedef graph_traits< BoostGraphType >::vertex_descriptor    BoostVertexDescriptor;
  typedef graph_traits< BoostGraphType >::edge_descriptor      BoostEdgeDescriptor;
  typedef std::pair<int, int>                                  BoostEdgeType;
  typedef std::vector<BoostVertexDescriptor>                   BoostVertexDescriptorList;

  /** Define other types */
  typedef double                                             RealType;
  typedef typename std::deque<NodePointerType>               NodeListType;
  typedef typename std::deque<EdgePointerType>               EdgeListType;
  typedef typename std::deque<NodeListType>                  NodeListContainerType;
  
  typedef VariableSizeMatrix<ValueType>                      MatrixType;
  typedef VariableLengthVector<ValueType>                    VectorType;

  itkGetMacro( UniquePathsMatrix, MatrixType );

  // Find shortest path to a node
  NodeListType BackTrace( NodeIdentifierType target );

  // Return paths to all target nodes, from all source nodes
  NodeListContainerType GetTree( void )
    {
    return this->m_Tree;
    }

protected:
  DijkstrasUniquePathsGraphFilter();
  ~DijkstrasUniquePathsGraphFilter() {}
  void PrintSelf( std::ostream& os, Indent indent ) const;

  void GenerateData();
  virtual void GenerateOutputInformation(){}; // do nothing

private:
  DijkstrasUniquePathsGraphFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  /** Private functions for processing the graph */
  double GenerateDistanceFromSourceToTarget( unsigned int sourceNode, unsigned int targetNode );

  // Find shortest path for each target node
  void GrowTree( void );

  // Initialize node properties
  void Initialize();

  // Reset node properties
  void Reset();

  inline bool IsEqual( WeightType m, WeightType n )
    { return ( vnl_math_abs( static_cast<RealType>( m-n ) ) < 1e-10 ); }

  BoostVertexDescriptorList m_PreviousVertexList;

  MatrixType m_UniquePathsMatrix;

  NodeListContainerType m_Tree;

  typename GraphType::Pointer m_Output;
};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDijkstrasUniquePathsGraphFilter.hxx"
#endif

#endif

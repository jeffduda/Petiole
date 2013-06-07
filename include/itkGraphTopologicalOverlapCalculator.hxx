/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkGraphTopologicalOverlapCalculator_hxx
#define __itkGraphTopologicalOverlapCalculator_hxx

#include "itkGraphTopologicalOverlapCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputGraph, typename TValueType >
GraphTopologicalOverlapCalculator< TInputGraph, TValueType >
::GraphTopologicalOverlapCalculator()
{
  this->m_Degree = 0; 
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputGraph, typename TValueType >
void
GraphTopologicalOverlapCalculator< TInputGraph, TValueType >
::Compute(void)
{
  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();
  unsigned long nEdges = this->m_Graph->GetTotalNumberOfEdges();

  this->m_Overlap.SetSize( nNodes, nNodes );
  this->m_Overlap.Fill( 0.0 );

  for ( unsigned int i=0; i<nEdges; i++)
    {
    unsigned long source = this->m_Graph->GetEdge( i ).SourceIdentifier;
    unsigned long target = this->m_Graph->GetEdge( i ).TargetIdentifier;
    this->m_Overlap(source,target) = 1.0;
    this->m_Overlap(target,source) = 1.0;
    }
  for (unsigned int i=0; i<nNodes; i++)
    {
    if ( this->m_Degree > 0 )
      {
      this->m_Overlap(i,i) = 1.0;
      }
    else
      {
      this->m_Overlap(i,i) = 0.0;
      }
    }
  
  if ( this->m_Degree > 0 )
    {
    for (unsigned int i=0; i<nNodes; i++)
      {
      for (unsigned int j=0; j<i; j++)
        {

        RegionPointerType iNeighbors = this->GetNeighbors( i, this->m_Degree );
        RegionPointerType jNeighbors = this->GetNeighbors( j, this->m_Degree );
        unsigned long count = 0;
        
        for ( unsigned long iN=0; iN<iNeighbors->Size(); iN++)
          {
          for (unsigned long jN=0; jN<jNeighbors->Size(); jN++)
            {
            if ( jNeighbors->GetElement(jN) == iNeighbors->GetElement(iN) )
              {
              ++count;
              }
              }
          }
        
        ValueType aij = this->m_Overlap(i,j);
        ValueType min = iNeighbors->Size();
        if ( jNeighbors->Size() < min ) 
          {
          min = jNeighbors->Size();
          }
        ValueType overlap = (count + aij ) / ( min + 1.0 - aij );
        this->m_Overlap(i,j) = overlap;
        this->m_Overlap(j,i) = overlap;
        
        }
      }
    }
  
}


/**
 * Compute Min and Max of m_Image
 */
template< class TInputGraph, typename TValueType >
typename TInputGraph::RegionType::Pointer
GraphTopologicalOverlapCalculator< TInputGraph, TValueType >
::GetNeighbors(unsigned long node, unsigned long m)
{
  RegionPointerType neighbors = RegionType::New();
  neighbors->Initialize();

  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();
  bool neigh[nNodes];
  unsigned long nEdges = this->m_Graph->GetTotalNumberOfEdges();

  for (unsigned int i=0; i<nNodes; i++)
    {
    neigh[i] = false;
    }
  neigh[node] = true;

  for (unsigned int i=0; i<m; i++)
    {
    for (unsigned int j=0; j<nNodes; j++)
      {
      if ( neigh[j] )
        {
        for (unsigned int e=0; e<nEdges; e++)
          {
          neigh[ this->m_Graph->GetEdge( e ).SourceIdentifier ] = true;
          neigh[ this->m_Graph->GetEdge( e ).TargetIdentifier ] = true;
          }
        }
      }
    }

  unsigned long idx = 0;
  for (unsigned int i=0; i<nNodes; i++)
    {
    if ( neigh[i] && ( i != node ) ) 
      {
      neighbors->InsertElement(idx, i);
      ++idx;
      }
    }

  return neighbors;

}

/*
template< class TInputImage >
void
GraphTopologicalOverlapCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphTopologicalOverlapCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "TopologicalOverlap: "
     << this->m_Overlap
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif

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
#ifndef __itkGraphNodeNeighborhoodOverlapCalculator_hxx
#define __itkGraphNodeNeighborhoodOverlapCalculator_hxx

#include "itkGraphNodeNeighborhoodOverlapCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphNodeNeighborhoodOverlapCalculator< TInputImage, TValueType >
::GraphNodeNeighborhoodOverlapCalculator()
{
  this->m_Overlap = OVERLAP_SYMMETRIC;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeNeighborhoodOverlapCalculator< TInputImage, TValueType >
::Compute(void)
{
  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();

  this->m_NeighborhoodOverlap.SetSize( nNodes, nNodes );
  this->m_NeighborhoodOverlap.Fill( -1.0 );

  for ( unsigned int i=0; i<nNodes; i++ )
    {
    unsigned long startId = 0;
    if ( this->m_Overlap == OVERLAP_SYMMETRIC ) 
      {
      startId = i+1;
      }
    for ( unsigned int j=startId; j<nNodes; j++)
      {
      
      if ( i != j )
        {

        std::set<NodeIdentifierType> commonN;
        unsigned long totalN = 0;

        if ( ( this->m_Overlap == OVERLAP_SYMMETRIC ) ||
             ( this->m_Overlap == OVERLAP_OUT ) )
          {
          EdgeIdentifierContainerType iN = this->m_Graph->GetNode( i ).OutgoingEdges;
          EdgeIdentifierContainerType jN = this->m_Graph->GetNode( j ).OutgoingEdges;
          
          for ( unsigned long n=0; n<iN.size(); n++ )
            {
            if ( this->m_Graph->GetEdge( iN[n] ).TargetIdentifier != j ) 
              {
              ++totalN;
              commonN.insert( this->m_Graph->GetEdge( iN[n] ).TargetIdentifier );
              }
            }

          for ( unsigned long n=0; n<jN.size(); n++ )
            {
            if ( this->m_Graph->GetEdge( jN[n] ).TargetIdentifier != i ) 
              {
              ++totalN;
              commonN.insert( this->m_Graph->GetEdge( jN[n] ).TargetIdentifier );
              }
            }
          }

        if ( ( this->m_Overlap == OVERLAP_SYMMETRIC ) ||
             ( this->m_Overlap == OVERLAP_IN ) )
          {
          EdgeIdentifierContainerType iN2 = this->m_Graph->GetNode( i ).IncomingEdges;
          EdgeIdentifierContainerType jN2 = this->m_Graph->GetNode( j ).IncomingEdges;

          for ( unsigned long n=0; n<iN2.size(); n++ )
            {
            if ( this->m_Graph->GetEdge( iN2[n] ).SourceIdentifier != j ) 
              {
              ++totalN;
              commonN.insert( this->m_Graph->GetEdge( iN2[n] ).SourceIdentifier );
              }
            }
          
          for ( unsigned long n=0; n<jN2.size(); n++ )
            {
            if ( this->m_Graph->GetEdge( jN2[n] ).SourceIdentifier != i ) 
              {
              ++totalN;
              commonN.insert( this->m_Graph->GetEdge( jN2[n] ).SourceIdentifier );
              }
            }
          
          }

        if ( totalN > 0 )
          {
          this->m_NeighborhoodOverlap(i,j) =  (ValueType)(commonN.size()) / totalN ;
          if ( this->m_Overlap == OVERLAP_SYMMETRIC ) 
            {
            this->m_NeighborhoodOverlap(j,i) = this->m_NeighborhoodOverlap(i,j);
            }
          }
        }

      }

    }
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeNeighborhoodOverlapCalculator< TInputImage, TValueType >
::ComputeOutgoingNeighborhoodOverlap(void)
{
  this->m_Overlap = OVERLAP_OUT;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeNeighborhoodOverlapCalculator< TInputImage, TValueType >
::ComputeIncomingNeighborhoodOverlap(void)
{
  this->m_Overlap = OVERLAP_IN;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeNeighborhoodOverlapCalculator< TInputImage, TValueType >
::ComputeUndirectedNeighborhoodOverlap(void)
{
  this->m_Overlap = OVERLAP_SYMMETRIC;
  this->Compute();
}

/*
template< class TInputImage >
void
GraphNodeNeighborhoodOverlapCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphNodeNeighborhoodOverlapCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "NeighborhoodOverlap: "
     << this->m_NeighborhoodOverlap
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif

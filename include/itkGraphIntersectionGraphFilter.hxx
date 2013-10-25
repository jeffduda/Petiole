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
#ifndef __itkGraphIntersectionGraphFilter_hxx
#define __itkGraphIntersectionGraphFilter_hxx

#include "itkGraphIntersectionGraphFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputGraph, typename TOutputGraph >
GraphIntersectionGraphFilter< TInputGraph, TOutputGraph >
::GraphIntersectionGraphFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 1 );

  OutputGraphPointer output
    = dynamic_cast<OutputGraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 2 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

}


template <class TInputGraph, class TOutputGraph>
void
GraphIntersectionGraphFilter<TInputGraph,TOutputGraph>
::GenerateData( void )
{
  OutputGraphPointer output = this->GetOutput();  
  unsigned long nNodes1 = this->GetInput(0)->GetTotalNumberOfNodes();
  unsigned long nNodes2 = this->GetInput(1)->GetTotalNumberOfNodes();

  if ( nNodes1 != nNodes2 ) 
    {
    itkExceptionMacro( "GraphIntersectionGraphFilter only works for inputs with the same # of nodes" );
    }

  for ( unsigned long i=0; i<nNodes1; i++)
    {
    output->CreateNewNode();
    }

  for (unsigned long i=0; i<this->GetInput(0)->GetTotalNumberOfEdges(); i++)
    {
    bool matched = false;
    unsigned long j=0;
    typename OutputGraphType::EdgeType edge1 = this->GetInput(0)->GetEdge(i);
    while ( (!matched) && (j < this->GetInput(1)->GetTotalNumberOfEdges()) ) 
      {
      if ( this->GetInput(1)->HasEdge(edge1.SourceIdentifier, edge1.TargetIdentifier) )
        {
        matched = true;
        }
      else if ( this->GetInput(0)->IsDirected() &&
                this->GetInput(1)->HasEdge( edge1.TargetIdentifier, edge1.SourceIdentifier ) )
        {
        matched = true;
        }

      if ( matched ) 
        {
        typename OutputGraphType::EdgePointerType newedge = output->CreateNewEdge();
        newedge->SourceIdentifier = edge1.SourceIdentifier;
        newedge->TargetIdentifier = edge1.TargetIdentifier;
        newedge->Weight = 1;
        }
      
      ++j;
      }
    }

  float n = 0.5 * nNodes1 * (nNodes1 -1);
  if ( this->GetInput(0)->IsDirected() )
    {
    n *= 2.0;
    }

  float numer = n * output->GetTotalNumberOfEdges() - 
    ( this->GetInput(0)->GetTotalNumberOfEdges() * this->GetInput(1)->GetTotalNumberOfEdges() );

  float xdenom = sqrt( this->GetInput(0)->GetTotalNumberOfEdges() * ( n-2.0 ) );
  float ydenom = sqrt( this->GetInput(1)->GetTotalNumberOfEdges() * ( n-2.0 ) );

  this->m_BinaryPearsonCorrelation = numer / (xdenom * ydenom);

}

template <class TInputGraph, class TOutputGraph>
void
GraphIntersectionGraphFilter<TInputGraph,TOutputGraph>
::GenerateOutputInformation( void )
{
}

template< class TInputImage, typename TValueType >
void
GraphIntersectionGraphFilter< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "GraphIntersectionGraphFilter: "
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif

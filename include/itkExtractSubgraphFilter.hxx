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
#ifndef __itkExtractSubgraphFilter_hxx
#define __itkExtractSubgraphFilter_hxx

#include "itkExtractSubgraphFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
ExtractSubgraphFilter< TInputImage, TValueType >
::ExtractSubgraphFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 1 );

  OutputGraphPointer output
    = dynamic_cast<OutputGraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

  this->m_RegionSetByUser = false;
}


template <class TInputGraph, class TOutputGraph>
void
ExtractSubgraphFilter<TInputGraph,TOutputGraph>
::GenerateData( void )
{
  OutputGraphPointer output = this->GetOutput();
  typename InputGraphType::GraphTraitsType traits;  

  if ( this->m_RegionSetByUser ) 
    {
    itk::Array<unsigned long> newNodeId;
    newNodeId.SetSize( this->GetInput()->GetTotalNumberOfNodes() );

    for ( unsigned int i=0; i<this->m_ExtractionRegion->Size(); i++ )
      {
      NodePointerType node = output->CreateNewNode();
      newNodeId[ this->m_ExtractionRegion->GetElement(i) ] = node->Identifier;
      traits.CopyNodeStaticData( this->GetInput()->GetNode( this->m_ExtractionRegion->GetElement( i ) ), node );
      }
    
    for ( unsigned int i=0; i<this->GetInput()->GetTotalNumberOfEdges(); i++)
      {
      unsigned long source = this->GetInput()->GetEdge( i ).SourceIdentifier;
      unsigned long target = this->GetInput()->GetEdge( i ).TargetIdentifier;
      bool hasSource = false;
      bool hasTarget = false;
      for (unsigned int j=0; j<this->m_ExtractionRegion->Size(); j++)
        {
        if ( source == this->m_ExtractionRegion->GetElement( j ) )
          {
          hasSource = true;
          }
        if ( target == this->m_ExtractionRegion->GetElement( j ) )
          {
          hasTarget = true;
          }
        }
      if ( hasSource && hasTarget )
        {
        EdgePointerType edge = output->CreateNewEdge( newNodeId[source], newNodeId[target] );
        traits.CopyEdgeStaticData( this->GetInput()->GetEdge( i ), edge );
        }
      }
    }
  else
    {
    itkExceptionMacro( "itk::ExtractSubgraphFilter::GenerateData() needs an Region to extract " );
    }
}

template< class TInputGraph, class TOutputGraph >
void
ExtractSubgraphFilter<TInputGraph,TOutputGraph>
::SetExtractionRegion(RegionPointer extractRegion)
{
  m_ExtractionRegion = extractRegion;
  m_RegionSetByUser = true;
}

template <class TInputGraph, class TOutputGraph>
void
ExtractSubgraphFilter<TInputGraph,TOutputGraph>
::GenerateOutputInformation( void )
{
}

template< class TInputImage, typename TValueType >
void
ExtractSubgraphFilter< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ExtractSubgraphFilter: "
     << std::endl;
  os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif

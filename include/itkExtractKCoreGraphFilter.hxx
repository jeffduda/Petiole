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
#ifndef __itkExtractKCoreGraphFilter_hxx
#define __itkExtractKCoreGraphFilter_hxx

#include "itkExtractKCoreGraphFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
ExtractKCoreGraphFilter< TInputImage, TValueType >
::ExtractKCoreGraphFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 1 );

  OutputGraphPointer output
    = dynamic_cast<OutputGraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

  this->m_Level = 0;
}


template <class TInputGraph, class TOutputGraph>
void
ExtractKCoreGraphFilter<TInputGraph,TOutputGraph>
::GenerateData( void )
{
  OutputGraphPointer output = this->GetOutput();
  typename InputGraphType::GraphTraitsType traits;  

  DegreeCalculatorPointer degrees = DegreeCalculatorType::New();
  degrees->SetGraph( this->GetInput() );
  degrees->ComputeUndirectedDegree();
  
  unsigned long nNodes = this->GetInput()->GetTotalNumberOfNodes();

  typename OutputGraphType::RegionType::Pointer subgraph = OutputGraphType::RegionType::New();
  unsigned long idx = 0;
  for ( unsigned int i=0; i<nNodes; i++)
    {
    if ( degrees->GetDegreeCentrality()->GetElement(i) > this->m_Level )
      {
      subgraph->InsertElement( idx, i );
      ++idx;
      } 
    }

  if ( subgraph->Size() > 0 ) 
    {
    
    /** FIXME - Grafting Output does not work
    ExtractFilterPointer extract = ExtractFilterType::New();
    extract->SetInput( this->GetInput() );
    extract->SetExtractionRegion( subgraph );
    extract->GraftOutput( this->GetOutput() );
    extract->Update();
    this->GraftOutput( extract->GetOutput() );
    */

    itk::Array<unsigned long> newNodeId;
    newNodeId.SetSize( this->GetInput()->GetTotalNumberOfNodes() );

    for ( unsigned int i=0; i<subgraph->Size(); i++ )
      {
      NodePointerType node = output->CreateNewNode();
      newNodeId[ subgraph->GetElement(i) ] = node->Identifier;
      traits.CopyNodeStaticData( this->GetInput()->GetNode( subgraph->GetElement( i ) ), node );
      }
    
    for ( unsigned int i=0; i<this->GetInput()->GetTotalNumberOfEdges(); i++)
      {
      unsigned long source = this->GetInput()->GetEdge( i ).SourceIdentifier;
      unsigned long target = this->GetInput()->GetEdge( i ).TargetIdentifier;
      bool hasSource = false;
      bool hasTarget = false;
      for (unsigned int j=0; j<subgraph->Size(); j++)
        {
        if ( source == subgraph->GetElement( j ) )
          {
          hasSource = true;
          }
        if ( target == subgraph->GetElement( j ) )
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
    itkExceptionMacro( "itk::ExtractKCoreGraphFilter::GenerateData() needs an Region to extract " );
    }
}

template <class TInputGraph, class TOutputGraph>
void
ExtractKCoreGraphFilter<TInputGraph,TOutputGraph>
::GenerateOutputInformation( void )
{
}

template< class TInputImage, typename TValueType >
void
ExtractKCoreGraphFilter< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ExtractKCoreGraphFilter: "
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif

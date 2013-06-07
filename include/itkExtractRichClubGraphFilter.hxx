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
#ifndef __itkExtractRichClubGraphFilter_hxx
#define __itkExtractRichClubGraphFilter_hxx

#include "itkExtractRichClubGraphFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
ExtractRichClubGraphFilter< TInputImage, TValueType >
::ExtractRichClubGraphFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 1 );

  OutputGraphPointer output
    = dynamic_cast<OutputGraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

  this->m_IsWeighted = false;
  this->m_Level = 0;
  this->m_RichClubCoefficient = 0;
}


template <class TInputGraph, class TOutputGraph>
void
ExtractRichClubGraphFilter<TInputGraph,TOutputGraph>
::GenerateData( void )
{
  OutputGraphPointer output = this->GetOutput();
  typename InputGraphType::GraphTraitsType traits;  

  DegreeCalculatorPointer degrees = DegreeCalculatorType::New();
  degrees->SetGraph( this->GetInput() );
  degrees->ComputeUndirectedDegree();
  
  unsigned long nNodes = this->GetInput()->GetTotalNumberOfNodes();
  unsigned long nEdges = this->GetInput()->GetTotalNumberOfEdges();
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

  if ( subgraph->Size() < 2 )
    {
    this->m_RichClubCoefficient = 0;
    }
  else
    {
    std::vector<float> weights;
    typename OutputGraphType::RegionType::Pointer subedges = OutputGraphType::RegionType::New();
    idx = 0;
    ValueType weightsum = 0.0;
    for ( unsigned int i=0; i<nEdges; i++)
      {
      weights.push_back( this->GetInput()->GetEdge(i).Weight );
      if ( ( degrees->GetDegreeCentrality()->GetElement( this->GetInput()->GetEdge( i ).SourceIdentifier ) > this->m_Level ) &&
           ( degrees->GetDegreeCentrality()->GetElement( this->GetInput()->GetEdge( i ).TargetIdentifier ) > this->m_Level ) )
        {
        subedges->InsertElement(idx,i);
        weightsum += this->GetInput()->GetEdge( i ).Weight;
        ++idx;
        }
      }
    
    ValueType rcNodes = subgraph->Size();
    ValueType rcEdges = subedges->Size();

    if ( this->m_IsWeighted )
      {
      std::sort( weights.begin(), weights.end() );
      ValueType normweighted = 0.0;
      for (unsigned int i=(nEdges-subedges->Size()); i<nEdges; i++)
        {
        normweighted += weights[i];
        }
      this->m_RichClubCoefficient = weightsum / normweighted;
      }
    else
      {
      this->m_RichClubCoefficient = rcEdges / ( rcNodes * (rcNodes-1) ); 
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
    itkExceptionMacro( "itk::ExtractRichClubGraphFilter::GenerateData() needs an Region to extract " );
    }
}

template <class TInputGraph, class TOutputGraph>
void
ExtractRichClubGraphFilter<TInputGraph,TOutputGraph>
::GenerateOutputInformation( void )
{
}

template< class TInputImage, typename TValueType >
void
ExtractRichClubGraphFilter< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ExtractRichClubGraphFilter: "
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif

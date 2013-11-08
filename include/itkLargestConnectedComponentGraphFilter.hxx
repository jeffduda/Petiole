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
#ifndef __itkLargestConnectedComponentGraphFilter_hxx
#define __itkLargestConnectedComponentGraphFilter_hxx

#include "itkLargestConnectedComponentGraphFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
LargestConnectedComponentGraphFilter< TInputImage, TValueType >
::LargestConnectedComponentGraphFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 1 );

  OutputGraphPointer output
    = dynamic_cast<OutputGraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

}


template <class TInputGraph, class TOutputGraph>
void
LargestConnectedComponentGraphFilter<TInputGraph,TOutputGraph>
::GenerateData( void )
{
  OutputGraphPointer output = this->GetOutput();

  // Convert input to search friendly graph
  typename SearchGraphType::Pointer dgraph = SearchGraphType::New();
  for (unsigned long i=0; i<this->GetInput()->GetTotalNumberOfNodes(); i++)
    {
    dgraph->CreateNewNode();
    }
  for (unsigned long i=0; i<this->GetInput()->GetTotalNumberOfEdges(); i++)
    {
    typename SearchGraphType::EdgePointerType edge = dgraph->CreateNewEdge();
    edge->SourceIdentifier = this->GetInput()->GetEdge(i).SourceIdentifier;
    edge->TargetIdentifier = this->GetInput()->GetEdge(i).TargetIdentifier;
    edge->Weight = 1.0;
    }
  
  typename DistanceMatrixFilter::Pointer distanceFilter = DistanceMatrixFilter::New();
  distanceFilter->SetInput( dgraph );
  distanceFilter->Update();

  typename DistanceMatrixFilter::MatrixType mat = distanceFilter->GetDistanceMatrix();
 
  typename itk::Array<unsigned long> labels;
  labels.SetSize(this->GetInput()->GetTotalNumberOfNodes());
  labels.Fill(0);
  
  unsigned long nLabels = 0;
  unsigned long nLabeled = 0;
  unsigned long idx = 0;
  while ( nLabeled < this->GetInput()->GetTotalNumberOfNodes() )
    {
    if ( labels[idx] == 0 ) 
      {
      ++nLabels;
      labels[idx] = nLabels;
      ++nLabeled;
      for ( unsigned long i=0; i<this->GetInput()->GetTotalNumberOfNodes(); i++ )
        {
        if ( ( i != idx ) && ( mat(i,idx) > 0 ) )
          {
          labels[i] = nLabels;
          ++nLabeled;
          }
        }
      }
    else
      {
      ++idx;
      }
    }

  typename itk::Array<unsigned long> labelSizes;
  labelSizes.SetSize( nLabels );
  labelSizes.Fill(0);
  for ( unsigned long i=0; i<labels.Size(); i++ )
    {
    labelSizes[ labels[i]-1 ] += 1;
    }

  unsigned long maxLabel = 1;
  unsigned long maxSize = labelSizes[0];

  for (unsigned long i=0; i<labelSizes.Size(); i++)
    {
    if ( labelSizes[i] > maxSize ) 
      {
      maxSize = labelSizes[i];
      maxLabel = i+1;
      }
    }

  m_Map.SetSize( this->GetInput()->GetTotalNumberOfNodes() );
  m_Map.Fill(0);
  unsigned long newLabel = 1;
  for (unsigned int i=0; i<this->GetInput()->GetTotalNumberOfNodes(); i++)
    {
    if ( labels[i] == maxLabel ) 
      {
      m_Map[i] = newLabel;
      ++newLabel;
      }
    }

  // Create output graph
  for (unsigned long i=0; i<maxSize; i++)
    {
    output->CreateNewNode();
    }
  for ( unsigned long i=0; i<this->GetInput()->GetTotalNumberOfEdges(); i++)
    {
    unsigned long sourceid = this->GetInput()->GetEdge(i).SourceIdentifier;
    unsigned long targetid = this->GetInput()->GetEdge(i).TargetIdentifier;

    if ( (labels[sourceid] == maxLabel) && (labels[targetid] == maxLabel) )
      {
      //std::cout << "insert edge " << m_Map[sourceid]-1 << "-" << m_Map[targetid]-1 << std::endl;
      typename OutputGraphType::EdgePointerType edge = output->CreateNewEdge( m_Map[sourceid]-1, m_Map[targetid]-1 );
      edge->Weight =  this->GetInput()->GetEdge(i).Weight;
      }
    }
}

template <class TInputGraph, class TOutputGraph>
void
LargestConnectedComponentGraphFilter<TInputGraph,TOutputGraph>
::GenerateOutputInformation( void )
{
}

template< class TInputImage, typename TValueType >
void
LargestConnectedComponentGraphFilter< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "LargestConnectedComponentGraphFilter: "
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif

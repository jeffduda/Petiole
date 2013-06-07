/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNWBDataFileReader.h,v $
  Language:  C++
  Date:      $Date: 2009/03/04 23:10:58 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNWBDataFileReader_h
#define __itkNWBDataFileReader_h

#include "itkMesh.h"
#include "itkMeshSource.h"

#include "itkArray.h"
#include "itkImage.h"
#include "itkVectorContainer.h"

#include <vector>
#include <fstream>

namespace itk {

/** \class NWBDataFileReader
 * \brief
 * Reads a file and creates an itkMesh.
 *
 */
template <class TOutputMesh>
class NWBDataFileReader
: public MeshSource<TOutputMesh>
{
public:
  /** Standard "Self" typedef. */
  typedef NWBDataFileReader               Self;
  typedef MeshSource<TOutputMesh>         Superclass;
  typedef SmartPointer<Self>              Pointer;
  typedef SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Extract dimension from the output mesh. */
  itkStaticConstMacro( Dimension, unsigned int,
                       TOutputMesh::PointType::Dimension );


  /** Run-time type information (and related methods). */
  itkTypeMacro( NWBDataFileReader, MeshSource );

  /** Hold on to the type information specified by the template parameters. */
  typedef TOutputMesh                             OutputMeshType;
  typedef typename OutputMeshType::MeshTraits     MeshTraits;
  typedef typename OutputMeshType::Superclass     PointSetType;
  typedef typename OutputMeshType::PointType      PointType;
  typedef typename MeshTraits::PixelType          PixelType;
  typedef Array<unsigned long>                    LineType;

  typedef VectorContainer<long, LineType>         LineSetType;

  typedef Image<PixelType,
    itkGetStaticConstMacro( Dimension )>          NWBDataImageType;

  typedef std::vector<PixelType>                  LabelSetType;
  

  /** Set/Get the name of the file to be read. */
  itkSetStringMacro( FileName );
  itkGetStringMacro( FileName );

  LineSetType* GetUndirectedEdges()
    { return this->m_UndirectedEdges.GetPointer(); }

  LineSetType* GetDirectedEdges()
  { return this->m_DirectedEdges.GetPointer(); }

protected:
  NWBDataFileReader();
  ~NWBDataFileReader() {}
  void PrintSelf( std::ostream& os, Indent indent ) const;

  /** Reads the file */
  void GenerateData();

  bool                                            m_ExtractBoundaryPoints;

  std::string                                     m_FileName;
  double                                          m_RandomPercentage;
  LabelSetType                                    m_LabelSet;

  typename LineSetType::Pointer                   m_UndirectedEdges;
  typename LineSetType::Pointer                   m_DirectedEdges;

private:
  NWBDataFileReader( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented

  void ReadNWBFile();
  void ReadNodes(std::string headerline);
  void ReadEdges(std::string headerline, bool isDirected);

  std::ifstream  m_File;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNWBDataFileReader.hxx"
#endif

#endif //_itkNWBDataFileReader_h

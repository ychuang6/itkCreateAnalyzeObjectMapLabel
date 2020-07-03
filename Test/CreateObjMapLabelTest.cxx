/*=================================================================================
 *  Program:   Insight Segmentation & Registration Toolkit
 *  Modules:   itkAnalyzeObjectMap
 *
 *  Copyright NumFOCUS
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
 *  Note: 
 *  This script is modified based on AnalyzeObjectMapTest.cxx and CreatingObjects.cxx 
 *    in the ITK4 Remote Module itkAnalyzeObjectMap
 *
 *  Function: Read in a nifti file and create a new object map containing 
 *    entries (with labels/analyzeobject) specified by an input text file.
 *    An AnalyzeObjectMap file [.obj] will be created as output
 *
 *  Modified by: Y. J. Chuang ychuang6@waisman.wisc.edu
 *
==================================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkAnalyzeObjectMap.h"
#include "itkAnalyzeObjectLabelMapImageIOFactory.h"
#include "itkAnalyzeObjectLabelMapImageIO.h"
#include "itksys/SystemTools.hxx"
#include <fstream>
#include <sstream>

int main( int argc, char ** argv )
{
  int error_count = 0;
  if ( argc != 4 )
    {
    std::cerr << " Missing parameters. " << std::endl;
    std::cerr << " USAGE: " << argv[0] << "<inputFileName> <outputFileName> <labelsNameTextFile>" << std::endl;
    return EXIT_FAILURE;
    }

  const char *NiftiFile = argv[1];
  const char *ObjMapWithLabels = argv[2];
  const std::string labelsNameTextFile = argv[3];

  if ( !itksys::SystemTools::FileExists(labelsNameTextFile.c_str()))
  {
      itk::ExceptionObject eo(__FILE__, __LINE__, "Text file does not exist");
      throw eo;
  }
  typedef unsigned char       InputPixelType;
  typedef unsigned char       OutputPixelType;
  const   unsigned int        Dimension = 3;

  typedef itk::Image< InputPixelType,  Dimension >    InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >    OutputImageType;

  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  //This is very important to use if you are not going to install the Analyze Object map code directly into
  //itk.  This means that you can build the Analyze Object map outside of ITK and still use it and treat
  //the code as if it is in ITK.
  itk::ObjectFactoryBase::RegisterFactory( itk::AnalyzeObjectLabelMapImageIOFactory::New() );

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //Here the nifti file is read in - original example used a nifti file created by Hans and Jeffrey, containing two squares and a circle with different intensity values
  reader->SetFileName(NiftiFile);
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  itk::AnalyzeObjectMap<InputImageType>::Pointer CreateObjectMap = itk::AnalyzeObjectMap<InputImageType>::New();

  // Original examples specified intensity searched for, the name of the entry and RGB values. 
  // But here I altered the setup to read (optional: intensity value) and name/label of the entry 
  // from a comma-delimited files to parse a line like [ 250,"Circle" ] as input to <intensity> and <name of the entry>
  // if comma-delimited lines are not provided, intensity value will be specified in order of label names provided in labelsNameTextFile (.txt) 

  std::fstream file;
  std::string word;
  file.open(labelsNameTextFile.c_str());

  int ind = 1;
  while(file >> word)
  {
    // determine if input line is comma-delimited
    std::stringstream ss(word);
    std::vector<std::string> subwords;
    while (ss.good())
    {
      std::string substr;
      getline(ss, substr, ',');
      subwords.push_back(substr);
    }

    // if input line is indeed comma-delimited, take first value as intensity value, second value as entry name
    if (subwords.size() > 1)
    {
      int indexvalue;
      std::istringstream(subwords[0]) >> indexvalue;
      CreateObjectMap->AddObjectEntryBasedOnImagePixel(reader->GetOutput(), indexvalue, subwords[1], 250, 0, 0);
      ind++;
    }
    else{
      CreateObjectMap->AddObjectEntryBasedOnImagePixel(reader->GetOutput(), ind, word, 250, 0, 0);
      ind++;
    }
  }
  file.close();

  //Now write an object file
  writer->SetInput(CreateObjectMap);
  writer->SetFileName(ObjMapWithLabels);

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;

}

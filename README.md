itkCreateAnalyzeObjectMapLabel
==============================

This repository is a modification of examples demonstrated in [richardbeare](http://github.com/richardbeare)'s [repository](https://github.com/richardbeare/itkAnalyzeObjectMap) using the itkAnalyzeObjectMap reading/writing class for ITK by [Hawley & Johnson](https://github.com/InsightSoftwareConsortium/itkAnalyzeObjectMap/blob/master/AnalyzeObjectLabelMap.pdf).

CreateAnalyzeObjectMapLabel takes in a nifti file containing objects of different intensities and converts to Mayo Clinic Analyze Object Map format [.obj]. Modification made allows labelling of objects according to name specified in the required input text file. 

## Requirement 

ITK += 4.0
CMAKE += 2.6

## Install

```
mkdir itkCreateAnalyzeObjectMapLabel/build
cd itkCreateAnalyzeObjectMapLabel/build
ccmake ../ -DCMAKE_CXX_FLAGS=-std=c++11 -DCMAKE_BUILD_TYPE=Release
make
```

Test should pass without failure
 

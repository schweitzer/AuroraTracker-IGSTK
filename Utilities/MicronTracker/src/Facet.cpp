/**************************************************************
*
*     Micron Tracker: Example C++ wrapper and Multi-platform demo
*   
*     Written by: 
*      Shahram Izadyar, Robarts Research Institute - London- Ontario , www.robarts.ca
*      Claudio Gatti, Ahmad Kolahi, Claron Technology - Toronto -Ontario, www.clarontech.com
*
*     Copyright Claron Technology 2000-2003
*
***************************************************************/
#include "Facet.h"

Facet::Facet(int h)
{
  if (h != 0)
    this->m_handle = h;
  else
    this->m_handle = Facet_New();

  this->ownedByMe = TRUE;
}

/****************************/
/** */
Facet::~Facet()
{
  if (this->m_handle != 0 && this->ownedByMe)
    Facet_Free(this->m_handle);
}

/****************************/
/** */

int Facet::getXpoints( MCamera *cam, double *result2x3x2x2)
 //[LV/SV][L/R/M][base/head][X/Y]
{
  int camHandle;
  if (cam == NULL) {
    camHandle = NULL;
  } else {
    camHandle = cam->Handle();
  }
  int result = 0;
  result = Facet_IdentifiedXPointsGet(this->m_handle, camHandle, result2x3x2x2 );
  return result;
}

vector<Vector *> Facet::TemplateVectors()
{
  vector<Vector *> iv;
  int r;

  iv.push_back(new Vector);
  iv.push_back(new Vector);

  r = Facet_TemplateVectorsGet(this->m_handle, iv[0]->Handle(), iv[1]->Handle());
  return iv;
}

vector<Vector *> Facet::IdentifiedVectors()
{
  vector<Vector *> iv;
  int r;

  iv.push_back(new Vector);
  iv.push_back(new Vector);

  r = Facet_IdentifiedVectorsGet(this->m_handle, iv[0]->Handle(), iv[1]->Handle());
  return iv;
}

/****************************/
/** */
bool Facet::setVectorsFromSample(vector<Collection*> &sampledVectorSets, string &outCompletionExplanation, double maxSampleErrorAllowedMM)
{
  long mtCode;
  Collection* handlesCollection = new Collection();
  for (unsigned int i=0; i<sampledVectorSets.size(); i++)
  {
    if(sampledVectorSets[i]->count() == 2)
    {
      handlesCollection->add(sampledVectorSets[i]->itemI(1));
      handlesCollection->add(sampledVectorSets[i]->itemI(2));
    }
  }
  
  if (handlesCollection->count() == 0 ) return false;
  mtCode = Facet_SetTemplateVectorsFromSamples(this->m_handle, handlesCollection->getHandle(), maxSampleErrorAllowedMM);
  
  if (mtCode == mtOK)
    return true;
  else
  {
//    outCompletionExplanation = MTErrorString(mtCode);
    (void) outCompletionExplanation; // Get rid of unused variable warning
    return false;
  }

  return 0;
}

/****************************/
/** */
bool Facet::identify(MCamera* cam, vector<Vector*> vectorSet, double positionToleranceMM)
{
  bool result;
  int* vectorHandles;

  vectorHandles = (int *)malloc(vectorSet.size()*sizeof(int));

  for(unsigned int i=0; i<vectorSet.size(); i++)
    vectorHandles[i] = vectorSet[i]->Handle();


  int camHandle;
  if(cam == NULL)
    camHandle = 0;
  else
    camHandle = cam->Handle();

  Facet_Identify(this->m_handle, cam->Handle(), vectorHandles, vectorSet.size(),  &result);
  free(vectorHandles);
  
  (void) positionToleranceMM; //Get rid of unused variable warning
  return result;
}

/****************************/
/** */
bool Facet::validateTemplate(double positionToleranceMM, string )
{
  int mtCode;
  mtCode = Facet_ValidateTemplateVectors(this->m_handle);
  (void) positionToleranceMM; //Get rid of unused variable warning
  
  if (mtCode == mtOK)
    return true;
  else
    return false;
}

/****************************/
/** */
Xform3D* Facet::getFacet2CameraXf(MCamera* cam)
{
  Xform3D* facet2CameraXf = new Xform3D();
  Facet_Facet2CameraXfGet(this->m_handle, cam->Handle(), facet2CameraXf->getHandle());
  return facet2CameraXf;
}

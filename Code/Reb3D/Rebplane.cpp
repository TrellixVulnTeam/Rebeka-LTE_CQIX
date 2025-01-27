/*******************************************************************
 * RebEngine!                                                      *
 * (c)2003 by Stefan Zerbst | www.Reb.info                         *
 *-----------------------------------------------------------------*
 * File: Rebplane.cpp                                              *
 * part of static math lib implementing basic 3D math objects      *
 *******************************************************************/

// I N C L U D E S /////////////////////////////////////////////////

#include "Reb3d.h"      // class definitions and stuff


// G L O B A L S ///////////////////////////////////////////////////

extern bool g_bSSE;



// F U N C T I O N S ///////////////////////////////////////////////

 void RebPlane::Set(const RebVector &vcN, const RebVector &vcP) {
   m_fD      = - ( vcN * vcP);
   m_vcN     = vcN;
   m_vcPoint = vcP;
   }
/*----------------------------------------------------------------*/

 void RebPlane::Set(const RebVector &vcN, const RebVector &vcP,
                          float fD) {
   m_vcN     = vcN;
   m_fD      = fD;
   m_vcPoint = vcP;
   }
/*----------------------------------------------------------------*/

 void RebPlane::Set(const RebVector &v0, const RebVector &v1, 
                          const RebVector &v2) {
   RebVector vcEdge1 = v1 - v0;
   RebVector vcEdge2 = v2 - v0;

   m_vcN.Cross(vcEdge1, vcEdge2);
   m_vcN.Normalize();
   m_fD = - (m_vcN * v0);
   m_vcPoint = v0;
   }
/*----------------------------------------------------------------*/

// Calculate distance to point. Plane normal must be normalized.
 float RebPlane::Distance(const RebVector &vcPoint) {
   return ( _fabs((m_vcN*vcPoint) - m_fD) );
   }
/*----------------------------------------------------------------*/


// Classify point to plane.
 int RebPlane::Classify(const RebVector &vcPoint) {
   float f = (vcPoint * m_vcN) + m_fD;
   
   if (f >  0.00001) return RebFRONT;
   if (f < -0.00001) return RebBACK;
   return RebPLANAR;
   }
/*----------------------------------------------------------------*/


// Classify polygon with respect to this plane
int RebPlane::Classify(const RebPolygon &Poly) {
   int NumFront=0, NumBack=0, NumPlanar=0;
   int nClass;

   // cast away const
   RebPolygon *pPoly = ((RebPolygon*)&Poly);

   int NumPoints = pPoly->GetNumPoints();

   // loop through all points
   for (int i=0; i < NumPoints; i++) {
      nClass = Classify( pPoly->m_pPoints[i] );
      
      if (nClass == RebFRONT)     NumFront++;
      else if (nClass == RebBACK) NumBack++;
      else {
         NumFront++;
         NumBack++;
         NumPlanar++;
         }
      } // for
   
   // all points are planar
   if (NumPlanar == NumPoints)
      return RebPLANAR;
   // all points are in front of plane
   else if (NumFront == NumPoints)
      return RebFRONT;
   // all points are on backside of plane
   else if (NumBack == NumPoints)
      return RebBACK;
   // poly is intersecting the plane
   else
      return RebCLIPPED;
   } // Classify
/*----------------------------------------------------------------*/



// clips a ray into two segments if it intersects the plane
bool RebPlane::Clip(const RebRay *_pRay, float fL, RebRay *pF, RebRay *pB) {
   RebVector vcHit(0.0f,0.0f,0.0f);
   
   RebRay *pRay = (RebRay*)_pRay;

   // ray intersects plane at all?
   if ( !pRay->Intersects( *this, false, fL, NULL, &vcHit) ) 
      return false;

   int n = Classify( _pRay->m_vcOrig );

   // ray comes fron planes backside
   if ( n == RebBACK ) {
      if (pB) pB->Set(pRay->m_vcOrig, pRay->m_vcDir);
      if (pF) pF->Set(vcHit, pRay->m_vcDir);
      }
   // ray comes from planes front side
   else if ( n == RebFRONT ) {
      if (pF) pF->Set(pRay->m_vcOrig, pRay->m_vcDir);
      if (pB) pB->Set(vcHit, pRay->m_vcDir);
      }

   return true;
   } // Clip [ray]
/*----------------------------------------------------------------*/


// Intersection of two planes. If third parameter is given the line
// of intersection will be calculated. (www.magic-software.com)
bool RebPlane::Intersects(const RebPlane &plane, RebRay *pIntersection) {
   RebVector vcCross;
   float     fSqrLength;
   
   // if crossproduct of normals 0 than planes parallel
   vcCross.Cross(this->m_vcN, plane.m_vcN);
   fSqrLength = vcCross.GetSqrLength();

   if (fSqrLength < 1e-08f) 
      return false;

   // find line of intersection
   if (pIntersection) {
      float fN00 = this->m_vcN.GetSqrLength();
      float fN01 = this->m_vcN * plane.m_vcN;
      float fN11 = plane.m_vcN.GetSqrLength();
      float fDet = fN00*fN11 - fN01*fN01;

      if (_fabs(fDet) < 1e-08f) 
         return false;

      float fInvDet = 1.0f/fDet;
      float fC0 = (fN11*this->m_fD - fN01*plane.m_fD) * fInvDet;
      float fC1 = (fN00*plane.m_fD - fN01*this->m_fD) * fInvDet;

      (*pIntersection).m_vcDir  = vcCross;
      (*pIntersection).m_vcOrig = this->m_vcN*fC0 + plane.m_vcN*fC1;
      }

   return true;
   } // Intersects(Plane)
/*----------------------------------------------------------------*/


// Intersection of a plane with a triangle. If all vertices of the
// triangle are on the same side of the plane, no intersection occured. 
bool RebPlane::Intersects(const RebVector &vc0, const RebVector &vc1, 
                          const RebVector &vc2) {
   int n = this->Classify(vc0);

   if ( (n == this->Classify(vc1)) && 
        (n == this->Classify(vc2)) )
      return false;
   return true;
   } // Intersects(Tri)
/*----------------------------------------------------------------*/


// Intersection with AABB. Search for AABB diagonal that is most
// aligned to plane normal. Test its two vertices against plane.
// (M�ller/Haines, "Real-Time Rendering")
bool RebPlane::Intersects(const RebAabb &aabb) {
   RebVector Vmin, Vmax;

   // x component
   if (m_vcN.x >= 0.0f) {
      Vmin.x = aabb.vcMin.x;
      Vmax.x = aabb.vcMax.x;
      }
   else {
      Vmin.x = aabb.vcMax.x;
      Vmax.x = aabb.vcMin.x;
      }

   // y component
   if (m_vcN.y >= 0.0f) {
      Vmin.y = aabb.vcMin.y;
      Vmax.y = aabb.vcMax.y;
      }
   else {
      Vmin.y = aabb.vcMax.y;
      Vmax.y = aabb.vcMin.y;
      }
   
   // z component
   if (m_vcN.z >= 0.0f) {
      Vmin.z = aabb.vcMin.z;
      Vmax.z = aabb.vcMax.z;
      }
   else {
      Vmin.z = aabb.vcMax.z;
      Vmax.z = aabb.vcMin.z;
      }

   if ( ((m_vcN * Vmin) + m_fD) > 0.0f)
      return false;
   
   if ( ((m_vcN * Vmax) + m_fD) >= 0.0f)
      return true;
  
   return false;
   } // Intersects(AABB)
/*----------------------------------------------------------------*/


// Intersection with OBB. Same as obb culling to frustrum planes.
bool RebPlane::Intersects(const RebObb &obb) {
    float fRadius = _fabs( obb.fA0 * (m_vcN * obb.vcA0) ) 
                  + _fabs( obb.fA1 * (m_vcN * obb.vcA1) ) 
                  + _fabs( obb.fA2 * (m_vcN * obb.vcA2) );

    float fDistance = this->Distance(obb.vcCenter);
    return (fDistance <= fRadius);
   } // Intersects(OBB)
/*----------------------------------------------------------------*/

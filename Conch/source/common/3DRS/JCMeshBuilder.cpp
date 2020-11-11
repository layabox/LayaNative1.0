/**
@file			JCMeshBuilder.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_31
*/

#include "JCMeshBuilder.h"

namespace laya
{
       void JCMeshBuilder::BuildePlaneGeo(int nWSeg, int nHSeg, float fWidth, float fHeight, PlaneType plane, JCMeshBuilder::Geom& outGeo)
       {
       }
       void JCMeshBuilder::BuildBoxGeo(bool bSepratePlane, float fWidth, float fLength, float fHeight, JCMeshBuilder::Geom& outGeo){
           outGeo.nIdxNum = 36;
           float xmin = -fWidth/2.0f, xmax = -xmin;
           float ymin = -fLength/2.0f, ymax = -ymin;
           float zmin = -fHeight/2.0f, zmax = -zmin;
           /*
              0   1
              3   2
           */
           if(bSepratePlane){
               float vertex[]={
                   xmin,ymin,zmax,  xmax,ymin,zmax,  xmax,ymin,zmin, xmin,ymin,zmin,//前面 0
                   xmax,ymin,zmax,  xmax,ymax,zmax,  xmax,ymax,zmin,  xmax,ymin,zmin,//右面 4
                   xmax,ymax,zmax,  xmin,ymax,zmax,  xmin,ymax,zmin,  xmax,ymax,zmin,//后面 8
                   xmin,ymax,zmax,  xmin,ymin,zmax,  xmin,ymin,zmin,  xmin,ymax,zmin,//左面 12
                   xmin,ymin,zmin,  xmax,ymin,zmin, xmax,ymax,zmin,  xmin,ymax,zmin,//下面 16
                   xmin,ymin,zmax,  xmax,ymin,zmax, xmax,ymax,zmax,  xmin,ymax,zmax//上面 20
               };
               unsigned short index[6]={0,1,2, 2,3,0};
               float uv[]={0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f};
               float norm[]={0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 
                   1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 
                   0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 
                   -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, 
                   0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 
                   0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f };
               outGeo.nVertNum = 24;
               outGeo.pVertex = new float[outGeo.nVertNum*3];
               outGeo.pIdx = new unsigned short [outGeo.nIdxNum];
               outGeo.setHasElement((int)Geom::GEO_UV,true);
               outGeo.pUV = new float [outGeo.nVertNum*2];
               outGeo.setHasElement((int)Geom::GEO_NORMAL,true);
               outGeo.pNorm = new float[outGeo.nVertNum*3];
               //pos
               memcpy(outGeo.pVertex, vertex,outGeo.nVertNum*3*sizeof(float) );
               //index
               unsigned short off = 0;
               unsigned int idxpos = 0;
               for(int i=0; i<6; i++){
                   outGeo.pIdx[idxpos+0] = index[0]+off;
                   outGeo.pIdx[idxpos+1] = index[1]+off;
                   outGeo.pIdx[idxpos+2] = index[2]+off;
                   outGeo.pIdx[idxpos+3] = index[3]+off;
                   outGeo.pIdx[idxpos+4] = index[4]+off;
                   outGeo.pIdx[idxpos+5] = index[5]+off;
                   idxpos+=6;
                   off+=4;
               }
               //uv
               off=0;
               for(int i=0; i<6; i++){
                   memcpy(&outGeo.pUV[off],uv,sizeof(uv));
                   off+=8;
               }
               //normal
               memcpy(outGeo.pNorm,norm,outGeo.nVertNum*sizeof(float)*3);
           }else{
               float vertex[]={
                   xmin,ymin,zmax,  xmax,ymin,zmax,  xmax,ymin,zmin, xmin,ymin,zmin,//前面 0
                   xmax,ymax,zmax,  xmin,ymax,zmax,  xmin,ymax,zmin,  xmax,ymax,zmin//后面 8
               };
               unsigned short index[36]={0,1,2, 2,3,0,//前面
                   1,4,7, 7,2,1,//右面
                   4,5,6, 6,7,4,//后面
                   5,0,3, 3,6,5,//左面
                   3,2,7, 7,6,3,//下面
                   0,1,4, 4,5,0 //上面
               };
               float uv[]={0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f};
               float norm[]={1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f ,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f };//TODO 这个还没做

               outGeo.nVertNum = 8;
               outGeo.pVertex = new float[outGeo.nVertNum*3];
               memcpy(outGeo.pVertex, vertex,outGeo.nVertNum*3*sizeof(float) );
               outGeo.pIdx = new unsigned short [outGeo.nIdxNum];
               memcpy(outGeo.pIdx, index, 36*sizeof(unsigned short));
               outGeo.setHasElement((int)Geom::GEO_UV,true);
               outGeo.pUV = new float [outGeo.nVertNum*2];
               outGeo.setHasElement((int)Geom::GEO_NORMAL,true);
               outGeo.pNorm = new float[outGeo.nVertNum*3];
               memcpy(outGeo.pUV, uv,sizeof(float)*outGeo.nVertNum*2);
               memcpy(outGeo.pNorm, norm,sizeof(float)*outGeo.nVertNum*3);
           }
       }

       void JCMeshBuilder::BuildBoxGeo(float fWidth, float fLength, float fHeight, int nWSeg, int nLSeg, int nHSeg, JCMeshBuilder::Geom& outGeo){
       }

       void JCMeshBuilder::BuildSphereGeo(int nSeg, float fR, JCMeshBuilder::Geom& outGeo){
       }

       void JCMeshBuilder::CalcNormal(JCMeshBuilder::Geom& geo,   CalcNormalType type, float*& pOutNorm ){
       }

       void JCMeshBuilder::MapUV_Plane(float fOx, float fOy, float fU1, float fV1, JCMeshBuilder::Geom& geo, float*& outUV){
       }

       void JCMeshBuilder::MapUV_Box( float* vec3Ori, 
           float fUXY, float fVXY, 
           float fUXZ, float fVXZ, 
           float fUYZ, float fVYZ,
           JCMeshBuilder::Geom& geo, float*& out){
       }

      void JCMeshBuilder::MapUV_Sphere(float* vec3Ori, JCMeshBuilder::Geom& geo, float*& outUV ){
      }
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

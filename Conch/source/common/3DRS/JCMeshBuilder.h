/**
@file           JCMeshBuilder.h
@brief
@author         guo
@version        1.0
@date           2016_5_27
*/

#ifndef __JCMeshBuilder_H__
#define __JCMeshBuilder_H__

#include "JCMesh.h"

namespace laya{
    /*
        这里采用的全是右手坐标系
        z    
        |   y
        |  /
        | /
        |/________x
    */

    class JCMeshBuilder
    {
    public:
        struct Geom
        {
            enum geoElement
            {
                GEO_UV=0,
                GEO_NORMAL=1,
            };
            Geom()
            {
                pVertex=0;
                nVertNum=0;
                pIdx=0;
                nIdxNum=0;
                nFlag=0;
                pUV=0;
                pNorm=0;
            }
            bool getHasElement(int e){
                return (nFlag&(1<<e))!=0;
            }
            void setHasElement(int e, bool b){
                if(b)nFlag|=(1<<e);
                else nFlag&=~(1<<e);
            }
            float*  pVertex;      
            int     nVertNum;
            unsigned short* pIdx;
            int             nIdxNum;
            int     nFlag;  //其他成分
            float* pUV;
            float* pNorm;
        };
        enum CalcNormalType{
            NORM_FLAT=0,
            NORM_SMOOTH=1
        };
        enum UVMapType{
            UVMAP_PLANE=0,
            UVMAP_BOX,
            UVMAP_SPHERE,
        };
        enum PlaneType{
            PLANE_XY=0,
            PLANE_XZ,
            PLANE_YZ,
        };
    public:
        /** @brief
            创建一个平面
         *  @param[in] 
         *  @param[in]
         *  @param[in]
         *  @param[out] 
         *  @return 
        */
        static void BuildePlaneGeo(int nWSeg, int nHSeg, float fWidth, float fHeight, PlaneType plane, Geom& outGeo);

        /*
        创建一个盒子的数据。
                   height
                    |
                    |  /
                    | /length
            ________|/
               width
            bSepratePlane: true表示24个点。false表示只有8个点
            pOutPos  输出的位置，每3个表示一个位置，对应x,y,z。一共3*nVertNum个
        */
        static void BuildBoxGeo(bool bSepratePlane, float fWidth, float fLength, float fHeight, Geom& outGeo);

        static void BuildBoxGeo(float fWidth, float fLength, float fHeight, int nWSeg, int nLSeg, int nHSeg, Geom& outGeo);
        
        /*
        */
        static void BuildSphereGeo(int nSeg, float fR, Geom& outGeo);

        static void CalcNormal(Geom& geo,   CalcNormalType type, float*& pOutNorm );

        /*
            fUX,fUY是uv为1的值
        */
        static void MapUV_Plane(float fOx, float fOy, float fU1, float fV1, Geom& geo, float*& outUV);

        static void MapUV_Box( float* vec3Ori, float fUXY, float fVXY, float fUXZ, float fVXZ, float fUYZ, float fVYZ,Geom& geo, float*& out);

        static void MapUV_Sphere(float* vec3Ori, Geom& geo, float*& outUV );

        static void TransMesh(Geom& geo, Geom& out, float* mat44);
    };
};

#endif //__JCMeshBuilder_H__


/**
@file			JCParticleSetting.h
@brief			
@author			hugao
@version		1.0
@date			2016_10_31
*/

#ifndef __JCParticleSetting2D_H__
#define __JCParticleSetting2D_H__


//包含头文件
//------------------------------------------------------------------------------
namespace laya
{
    struct JCParticleSetting2D
    {
    public:
        /** @brief构造函数
        */
        JCParticleSetting2D();

        /** @brief析构函数
        */
        ~JCParticleSetting2D();
    public:
        /**贴图个数,默认为1可不设置*/
        int m_nTextureCount;
        /**由于循环队列判断算法，最大饱和粒子数为maxPartices-1*/
        int m_nMaxPartices;
        int  m_nBlendState;// 0;

        /**是否使用结束弧度,false为结束时与起始弧度保持一致,true为根据minHorizontalEndRadian、maxHorizontalEndRadian、minVerticalEndRadian、maxVerticalEndRadian计算结束弧度。*/
        int m_bUseEndRadian;
        /**false代表RGBA整体插值，true代表RGBA逐分量插值*/
        int m_bColorComponentInter;// false;
		int m_bDisableColor;//false;

        /**粒子持续时间(单位:秒）*/
        float m_nDuration;
        /**如果大于0，某些粒子的持续时间会小于其他粒子,并具有随机性(单位:无）*/
        float m_nAgeAddScale;
        /**粒子受发射器速度的敏感度（需在自定义发射器中编码设置）*/
        float m_nEmitterVelocitySensitivity;
        /**最小开始尺寸（单位：2D像素、3D坐标）*/
        float m_nMinStartSize;
        /**最大开始尺寸（单位：2D像素、3D坐标）*/
        float m_nMaxStartSize;
        /**最小结束尺寸（单位：2D像素、3D坐标）*/
        float m_nMinEndSize;
        /**最大结束尺寸（单位：2D像素、3D坐标）*/
        float m_nMaxEndSize;
        /**最小水平速度（单位：2D像素、3D坐标）*/
        float m_nMinHorizontalVelocity;
        /**最大水平速度（单位：2D像素、3D坐标）*/
        float m_nMaxHorizontalVelocity;
        /**最小垂直速度（单位：2D像素、3D坐标）*/
        float m_nMinVerticalVelocity;
        /**最大垂直速度（单位：2D像素、3D坐标）*/
        float m_nMaxVerticalVelocity;
        /**等于1时粒子从出生到消亡保持一致的速度，等于0时粒子消亡时速度为0，大于1时粒子会保持加速（单位：无）*/
        float m_nEndVelocity;


        /**最小旋转速度（单位：2D弧度/秒、3D弧度/秒）*/
        float m_nMinRotateSpeed;
        /**最大旋转速度（单位：2D弧度/秒、3D弧度/秒）*/
        float m_nMaxRotateSpeed;

        /**最小开始半径（单位：2D像素、3D坐标）*/
        float m_nMinStartRadius;
        /**最大开始半径（单位：2D像素、3D坐标）*/
        float m_nMaxStartRadius;
        /**最小结束半径（单位：2D像素、3D坐标）*/
        float m_nMinEndRadius;
        /**最大结束半径（单位：2D像素、3D坐标）*/
        float m_nMaxEndRadius;

        /**最小水平开始弧度（单位：2D弧度、3D弧度）*/
        float m_nMinHorizontalStartRadian;
        /**最大水平开始弧度（单位：2D弧度、3D弧度）*/
        float m_nMaxHorizontalStartRadian;
        /**最小垂直开始弧度（单位：2D弧度、3D弧度）*/
        float m_nMinVerticalStartRadian;
        /**最大垂直开始弧度（单位：2D弧度、3D弧度）*/
        float m_nMaxVerticalStartRadian;


        /**最小水平结束弧度（单位：2D弧度、3D弧度）*/
        float m_nMinHorizontalEndRadian;
        /**最大水平结束弧度（单位：2D弧度、3D弧度）*/
        float m_nMaxHorizontalEndRadian;
        /**最小垂直结束弧度（单位：2D弧度、3D弧度）*/
        float m_nMinVerticalEndRadian;
        /**最大垂直结束弧度（单位：2D弧度、3D弧度）*/
        float m_nMaxVerticalEndRadian;

        /**（单位：2D像素、3D坐标）*/
        float m_vGravity[3];//float 3

        /**最小开始颜色*/
        float m_vMinStartColor[4];//	/**最小开始颜色 new Float32Array([1, 1, 1, 1]);*/
		/**最大开始颜色*/
        float m_vMaxStartColor[4];// new Float32Array([1, 1, 1, 1]);
		/**最小结束颜色*/
        float m_vMinEndColor[4];//  new Float32Array([1, 1, 1, 1]);
		/**最大结束颜色*/
        float m_vMaxEndColor[4];// new Float32Array([1, 1, 1, 1]);
	
		/**混合模式，待调整，引擎中暂无BlendState抽象*/


       	/**发射器位置随机值,2D使用*/
        float m_vPositionVariance[3];
        //public var positionVariance : Float32Array = new Float32Array([0, 0, 0]);
    };
}
//------------------------------------------------------------------------------


#endif //__JCParticleSetting2D_H__

//-----------------------------END FILE--------------------------------
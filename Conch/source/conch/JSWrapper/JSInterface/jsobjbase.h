
#ifndef __JSOBJECT_BASE_NODE_H___
#define __JSOBJECT_BASE_NODE_H___

#include <util/ListNode.h>

namespace laya {
    struct JsObjClassInfo {
        const char*				ClassName;
        const JsObjClassInfo*	ParentClass;
        const unsigned int		ClsID;
        void*					objTemplate;
    };
    /*
        js对象对应的c端对象。
        所有导出给js的对象都要从这个继承
        并且在创建和删除的时候进行管理
    */
    class JSObjNode :public ListNode {
    public:
        static JsObjClassInfo JSCLSINFO;
        static simpList*     s_pListJSObj;			//属于本线程的所有的js对象

        JSObjNode();
        virtual ~JSObjNode();				//这个会引入虚函数表，不知道有没有问题
        const JsObjClassInfo*		m_pClsInfo;// const char* m_pClassName;
        //cls是不是从自己继承的
        bool IsSubClass(JsObjClassInfo* cls) {
            const JsObjClassInfo* cur = (const JsObjClassInfo*)cls;
            while (cur) {
                if (cur->ClsID == m_pClsInfo->ClsID)
                    return true;
                cur = cur->ParentClass;
            }
            return false;
        }
    };
    extern unsigned int gnJSClsID;
#define ADDJSCLSINFO(cls,par)	JsObjClassInfo cls::JSCLSINFO = { #cls,&par::JSCLSINFO,gnJSClsID++,0 };

    template<class T>
    bool IsSubClass(JsObjClassInfo* cls) {
        const JsObjClassInfo* cur = (const JsObjClassInfo*)cls;
        while (cur) {
            if (cur->ClsID == T::JSCLSINFO.ClsID)
                return true;
            cur = cur->ParentClass;
        }
        return false;
    }
}
#endif

//-----------------------------END FILE--------------------------------
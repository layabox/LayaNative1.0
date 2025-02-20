#ifndef __PLATFORMDEFINE_H__
#define __PLATFORMDEFINE_H__

#include <assert.h>
#include "util/Log.h"

#define CC_DLL

#define CC_NO_MESSAGE_PSEUDOASSERT(cond)                                                \
    if (!(cond))                                                                        \
    {                                                                                   \
        LOGI("[laya assert] %s function:%s line:%d", __FILE__, __FUNCTION__, __LINE__); \
    }

#define CC_MESSAGE_PSEUDOASSERT(cond, msg)                                                            \
    if (!(cond))                                                                                      \
    {                                                                                                 \
        LOGI("[laya assert] file:%s function:%s line:%d, %s", __FILE__, __FUNCTION__, __LINE__, msg); \
    }

#define CC_ASSERT(cond) CC_NO_MESSAGE_PSEUDOASSERT(cond)

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#endif

#endif

namespace laya
{
    struct CMDToBase64
    {
        unsigned int fid;
        float encoderOptions;
        unsigned int length;
    };

    struct CMDDrawPath
    {
        unsigned int fid;
        float x;
        float y;
        int brushLen;
        int penLen;
        int pointsLen;
    };

    struct CMDPen
    {
        int strokeColor;
        int lineJoin;
        int lineCap;
        float lineWidth;
        float miterLimit;
    };
    struct CMDIBVB
    {
        unsigned int fid;
        int ibId;
        int vbId;
        int vertType;
        int shaderId;
        int imgId;
        int startIndex;
        int offset;
        int numElement;
        float x;
        float y;
        float mat[6];
    };

    struct CMDSkinMesh
    {
        unsigned int fid;
        int ibId;
        int vbId;
        int vertType;
        int numElement;
        int startIndex;
        int imgId;
        int shaderId;
        float mat[6];
        float x;
        float y;
    };
    struct CMDFloat3
    {
        unsigned int fid;
        float x;
        float y;
        float z;
    };
    struct CMDFloat4
    {
        unsigned int fid;
        float x;
        float y;
        float z;
        float w;
    };

    struct CMDFloat5
    {
        unsigned int fid;
        float x;
        float y;
        float z;
        float w;
        float h;
    };
    struct CMDDrawLines:public CMDFloat3
    {
        int lc;
        int pl;
    };

    struct CMDDrawImage3
    {
        unsigned int fid;
        unsigned int imgID;
        float x;
        float y;
    };
    struct CMDDrawImage5
    {
        unsigned int fid;
        unsigned int imgID;
        float x;
        float y;
        float w;
        float h;
    };
    struct CMDDrawImage9
    {
        unsigned int fid;
        unsigned int imgID;
        float sx;
        float sy;
        float sw;
        float sh;
        float dx;
        float dy;
        float dw;
        float dh;
    };
    struct CMDDrawImageCanvas9 : CMDDrawImage9
    {
        int nType;
    };
    struct CMDDrawImageCanvas9Alpha : CMDDrawImageCanvas9
    {
        float alpha;
    };
    struct CMDFloatRect
    {
        unsigned int fid;
        float x;
        float y;
        float w;
        float h;
    };

    struct CMDDrawLine:public CMDFloatRect
    {
        float lineWidth;
        int lc;
    };
    struct CMDPattern
    {
        unsigned int fid;
        int imgid;
        int repetition;
        float x;
        float y;
        float w;
        float h;
    };
    struct CMDFloatPoint
    {
        unsigned int fid;
        float x;
        float y;
    };

    struct CMDOFloatPoint
    {
        float x;
        float y;
    };

    struct CMDOFloatRect
    {
        float x;
        float y;
        float w;
        float h;
    };

    struct CMDFII
    {
        float f;
        int i1;
        int i2;
        int flag;
    };

    struct CMDOIntPoint
    {
        int x;
        int y;
    };
    struct CMDMatrix
    {
        unsigned int fid;
        float a;
        float b;
        float c;
        float d;
        float tx;
        float ty;
    };

    struct CMDDrawImageCanvasM:public CMDDrawImageCanvas9Alpha
    {
        float a;
        float b;
        float c;
        float d;
        float tx;
        float ty;
    };
    struct CMDFillText
    {
        unsigned int fid;
        float x;
        float y;
        unsigned int len;
    };
    struct CMDFillText1
    {
        unsigned int fid;
        float x;
        float y;
        int maxWidth;
        unsigned int len;
    };
    struct  CMDFillBorderText
    {
        unsigned int fid;
        float x;
        float y;
        float nLineWidth;
        int   nFillColor;
        int   nBorderColor;
        int   nTextAlign;
        int   nFontLen;
    };
    struct CMDArc
    {
        unsigned int fid;
        float x;
        float y;
        float r;
        float sAngle;
        float eAngle;
        unsigned int bCounterclockwise;
    };
    struct CMDArcTo
    {
        unsigned int fid;
        float x1;
        float y1;
        float x2;
        float y2;
        float radium;
    };
    struct CMDFloat
    {
        unsigned int fid;
        float f;
    };
    struct CMDInt
    {
        unsigned int fid;
        int i;
    };
    struct CMDInt2
    {
        unsigned int fid;
        int x;
        int y;
    };
    struct CMDInt3
    {
        unsigned int fid;
        int i;
        int j;
        int k;
    };
    struct CMDInt4
    {
        unsigned int fid;
        int i;
        int j;
        int k;
        int l;
    };
    struct CMDInt5
    {
        unsigned int fid;
        int i;
        int j;
        int k;
        int l;
        int m;
    };
    struct CMDNoParam
    {
        unsigned int fid;
    };
    struct CMDFilterMatrix
    {
        unsigned int fid;
        float matrix[16];
        float alpha[4];
    };
    struct CMDGlowFilterParam
    {
        unsigned int fid;
        int color;
        float blur;
        float offX;
        float offY;
    };
    struct CMDFillImage
    {
        unsigned int fid;
        int imgID;
        int repetition;
        float args[10];

    };
    struct CMDDrawToCanvas
    {
        unsigned int fid;
        int canvasID;
        float offsetX;
        float offsetY;
    };
}
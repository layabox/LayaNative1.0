'use strict';
class ConchNode {
    constructor() {
        this._nativeObj = new conchNode2D();
        this.id = this._nativeObj.conchID;
    }
    setRootNode() {
        this._nativeObj.setRootNode();
    }
    setFPSNode() {
        this._nativeObj.setFPSNode();
    }
    addChildAt(c, i) {
        if (!c)
            return;
        var _ = conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(0);
        _.wu32(c.id);
        _.wu32(i);
    }
    removeChild(c) {
        if (!c)
            return;
        _Buffer.oneInt(this, 1, c.id);
    }
    size(w, h) {
        _Buffer.point(this, 2, w, h);
    }
    pos(x, y) {
        _Buffer.point(this, 3, x, y);
    }
    pivot(x, y) {
        _Buffer.point(this, 4, x, y);
    }
    scale(x, y) {
        _Buffer.point(this, 5, x, y);
    }
    skew(x, y) {
        _Buffer.point(this, 6, x, y);
    }
    rotate(r) {
        _Buffer.oneNum(this, 7, r);
    }
    bgColor(bg) {
        if (!bg) {
            this.removeType(0x81);
            return;
        }
        var n = ColorUtil.getColorNum(bg);
        if (isNaN(n)) {
            console.error("bgColor:" + bg + "not  support");
            n = 0xFFFFFFFF;
        }
        _Buffer.oneInt(this, 8, n);
    }
    font(str) {
        var _ = conch.bf;
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(9);
        _.wstring(str, true);
    }
    text(d) {
    }
    transform(a, b, c, d, tx, ty) {
        _Buffer.setMatrix(this, 11, a, b, c, d, tx, ty);
    }
    setTransform(a, b, c, d, tx, ty) {
        _Buffer.setMatrix(this, 12, a, b, c, d, tx, ty);
    }
    alpha(a) {
        _Buffer.oneNum(this, 13, a);
    }
    filter(r, g, b, gray) {
        _Buffer.rect(this, 14, r, g, b, gray);
    }
    visible(b) {
        _Buffer.oneInt(this, 15, b ? 1 : 0);
    }
    blendMode(v) {
        if (!v) {
            this.removeType(0x08);
            return;
        }
        var d = CanvasRenderingContext2D.BLEND[v] || 0;
        _Buffer.oneInt(this, 16, d);
    }
    scrollRect(x, y, w, h) {
        _Buffer.rect(this, 17, x, y, w, h);
    }
    mask(node) {
        var i = node ? node.id : -1;
        _Buffer.oneInt(this, 18, i);
    }
    graphics(g) {
        var i = g ? g.id : -1;
        _Buffer.oneInt(this, 19, i);
    }
    custom(t) {
        if (t == null) {
            this.removeType(0x400);
        }
        else
            _Buffer.oneInt(this, 20, t.id);
    }
    removeType(t) {
        switch (t) {
            case 0x10:
                t = 0x20;
                break;
            case 0x40:
                t = 0x80;
                break;
            case 0x100:
                t = 0x200;
                break;
            default:
                break;
        }
        _Buffer.oneInt(this, 21, t);
    }
    cacheAs(t) {
        _Buffer.oneInt(this, 22, t);
    }
    border(b) {
        if (!b) {
            this.removeType(0x82);
            return;
        }
        var bc = ColorUtil.getColorNum(b);
        if (isNaN(bc)) {
            console.error("border：" + b + "not support");
            bc = 0xFFFFFFFF;
        }
        _Buffer.oneInt(this, 23, bc);
    }
    optimizeScrollRect(b) {
        _Buffer.oneInt(this, 24, b ? 1 : 0);
    }
    blurFilter(strength) {
        _Buffer.oneNum(this, 25, strength);
    }
    glowFilter(color, blur, offX, offY) {
        var c = ColorUtil.getColorNum(color);
        if (isNaN(c)) {
            console.error("growFilter" + color + "not support");
            c = 0xFFFFFFFF;
        }
        var _ = conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(26);
        _.wu32(c);
        _.wf32(blur);
        _.wf32(offX);
        _.wf32(offY);
    }
    setFilterMatrix(mat, alpha) {
        if (!mat || !alpha) {
            this.removeType(0x20);
            return;
        }
        var _ = conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(27);
        _.wab(mat, 64);
        _.wab(alpha, 16);
    }
    repaint() {
        var _ = conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(28);
    }
    setZOrder(z) {
        _Buffer.oneNum(this, 29, z);
    }
    updateZOrder() {
        _Buffer.noParm(this, 30);
    }
    drawToCanvas(canvas, offsetX, offsetY) {
        var _ = conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(31);
        _.wu32(canvas._ctx2d.id);
        _.wf32(offsetX);
        _.wf32(offsetY);
    }
}
class ConchGraphics {
    constructor() {
        this._cmds = null;
        this._nativeObj = new _conchGraphics();
        this.id = this._nativeObj.conchID;
    }
    _addCmd(a) {
        if (!this._cmds)
            this._one = a;
        this._cmds = this._cmds || [];
        this._cmds.push(a);
    }
    _repaint() {
    }
    clear() {
        this._repaint();
        this._cmds && (this._cmds.length = 0);
        _Buffer.noParm(this, 0);
    }
    destroy() {
        this._sp = null;
        this._cmds = null;
        this._temp = null;
        this._bounds = null;
        this._rstBoundPoints = null;
        _Buffer.noParm(this, 1);
    }
    alpha(a) {
        _Buffer.oneNum(this, 2, a);
    }
    rotate(angle, pivotX, pivotY) {
        var _ = conch.bf;
        _.needsz(20);
        _.wu32(this.id);
        _.wu32(3);
        _.wf32(angle);
        _.wf32(pivotX || 0);
        _.wf32(pivotY || 0);
        this._addCmd([3, angle, pivotX || 0, pivotY || 0]);
    }
    transform(mat, pivotX, pivotY) {
        _Buffer.setMatrix(this, 4, mat.a, mat.b, mat.c, mat.d, mat.tx - (pivotX || 0), mat.ty - pivotY || 0);
        this._addCmd([4, mat, pivotX || 0, pivotY || 0]);
    }
    scale(x, y, pivotX, pivotY) {
        _Buffer.rect(this, 5, x, y, pivotX || 0, pivotY || 0);
        this._addCmd([5, x, y, pivotX || 0, pivotY || 0]);
    }
    translate(x, y) {
        _Buffer.point(this, 6, x, y);
        this._addCmd([6, x, y]);
    }
    save() {
        _Buffer.noParm(this, 7);
        this._addCmd([7]);
    }
    restore() {
        _Buffer.noParm(this, 8);
        this._addCmd([8]);
    }
    clipRect(x, y, w, h) {
        _Buffer.rect(this, 9, x, y, w, h);
    }
    blendMode(s) {
        _Buffer.oneInt(this, 10, CanvasRenderingContext2D.BLEND[s] || 0);
    }
    fillText(text, x, y, font, color, textAlign, decorationLine) {
        var d = ColorUtil.getColorNum(color);
        if (isNaN(d)) {
            console.error("graphic fillText color :" + color + " not support");
            d = 0xFFFFFFFF;
        }
        if (decorationLine > 0) {
            font += " " + 0 + " " + "#000000" + " " + decorationLine + " " + color;
        }
        var _ = conch.bf;
        _.needsz(28);
        _.wu32(this.id);
        _.wu32(11);
        _.wf32(x);
        _.wf32(y);
        _.wu32(d);
        _.wu32(CanvasRenderingContext2D.TEXTALIGN[textAlign] || 0);
        _.wstring(font, true);
        _.needsz(4);
        _.wstring(text, true);
    }
    fillBorderText(text, x, y, font, fillColor, borderColor, lineWidth, textAlign, decorationLine) {
        if (borderColor) {
            font += " " + 1 + " " + borderColor;
            if (decorationLine > 0) {
                font += " " + decorationLine + " " + fillColor;
            }
        }
        else {
            if (decorationLine > 0) {
                font += " " + 0 + " " + "#000000" + " " + decorationLine + " " + fillColor;
            }
        }
        this.fillText(text, x, y, font, fillColor, textAlign, 0);
    }
    _fands(fillColor, lineColor, lineWidth) {
        var _ = conch.bf;
        lineWidth = lineWidth || 1;
        var fd = -1, ld = -1, flag = 1;
        if (fillColor) {
            fd = ColorUtil.getColorNum(fillColor);
            if (isNaN(fd)) {
                console.error("graphics drawRect fillcolor:" + fillColor + " not support");
                fd = 0xFFFFFFFF;
            }
        }
        else {
            flag = -1;
        }
        if (lineColor) {
            ld = ColorUtil.getColorNum(lineColor);
            if (isNaN(ld)) {
                console.error("graphics drawRect lineColor" + lineColor + " not support");
                ld = 0xFFFFFFFF;
            }
        }
        else {
            if (flag > 0)
                flag = -2;
            else
                flag = -3;
        }
        _.needsz(16);
        _.wf32(lineWidth);
        _.wu32(fd);
        _.wu32(ld);
        _.wu32(flag);
    }
    drawRect(x, y, width, height, fillColor, lineColor, lineWidth) {
        var offset = lineColor ? 0.5 : 0;
        _Buffer.rect(this, 13, x + offset, y + offset, width, height);
        this._fands(fillColor, lineColor, lineWidth);
        this._addCmd([13, x + offset, y + offset, width, height]);
    }
    drawCircle(x, y, radius, fillColor, lineColor, lineWidth) {
        var offset = lineColor ? 0.5 : 0;
        var _ = conch.bf;
        _.needsz(20);
        _.wu32(this.id);
        _.wu32(14);
        _.wf32(x + offset);
        _.wf32(y + offset);
        _.wf32(radius);
        this._fands(fillColor, lineColor, lineWidth);
        this._addCmd([14, x + offset, y + offset, radius]);
    }
    drawPie(x, y, radius, startAngle, endAngle, fillColor, lineColor, lineWidth) {
        var offset = lineColor ? 0.5 : 0;
        var _ = conch.bf;
        _.needsz(28);
        _.wu32(this.id);
        _.wu32(15);
        _.wf32(x + offset);
        _.wf32(y + offset);
        _.wf32(radius);
        _.wf32(startAngle);
        _.wf32(endAngle);
        this._fands(fillColor, lineColor, lineWidth);
        this._addCmd([15, x + offset, y + offset, radius, startAngle, endAngle]);
    }
    drawImageM(img, sx, sy, sw, sh, dx, dy, dw, dh, mat, alpha) {
        alpha = alpha ? alpha : 1.0;
        var sz = mat ? 76 : 52;
        var fid = mat ? 17 : 16;
        var _ = conch.bf;
        _.needsz(sz);
        var bImage = img instanceof HTMLImageElement;
        _.wu32(this.id);
        _.wu32(fid);
        _.wu32(bImage ? img.imgId : img._ctx2d.id);
        _.wf32Rect(sx, sy, sw, sh);
        _.wf32Rect(dx, dy, dw, dh);
        _.wu32(bImage ? 0 : 1);
        _.wf32(alpha);
        if (mat) {
            _.wf32Rect(mat.a, mat.b, mat.c, mat.d);
            _.wf32(mat.tx);
            _.wf32(mat.ty);
            this._addCmd([17, dx, dy, dw, dh, mat, img]);
        }
        else {
            this._addCmd([16, dx, dy, dw, dh, img]);
        }
    }
    drawPoly(x, y, points, fillColor, lineColor, lineWidth) {
        var _ = conch.bf, pl = points.length, i;
        _.needsz(pl * 4 + 36);
        _.wu32(this.id);
        _.wu32(18);
        _.wu32(pl);
        _.wf32(x);
        _.wf32(y);
        this._fands(fillColor, lineColor, lineWidth);
        for (i = 0; i < pl; i++) {
            _.wf32(points[i]);
        }
        this._addCmd([18, x, y, points]);
    }
    drawPath(x, y, paths, brush, pen) {
        var _ = conch.bf, pl = paths.length, i, bl = 0, plen = 0, fc, sc, ab, bfl = 0;
        if (brush) {
            bl = 1;
            fc = ColorUtil.getColorNum(brush.fillStyle);
            if (isNaN(fc)) {
                console.error("brush fillStyle is not support" + brush.fillStyle);
                fc = 0xFFFFFFFF;
            }
        }
        if (pen) {
            plen = 5;
            sc = ColorUtil.getColorNum(pen.strokeStyle);
            if (isNaN(sc)) {
                console.error("pen strokeStyle is not support" + pen.strokeStyle);
                sc = 0xFFFFFFFF;
            }
        }
        if (pl > 0) {
            ab = new ArrayBuffer(pl * 24);
            var dv = new DataView(ab);
            for (var i = 0; i < pl; i++) {
                var path = paths[i];
                switch (path[0]) {
                    case "moveTo":
                        dv.setUint32(bfl, 0, true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[1], true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[2], true);
                        bfl += 4;
                        break;
                    case "lineTo":
                        dv.setUint32(bfl, 1, true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[1], true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[2], true);
                        bfl += 4;
                        break;
                    case "arcTo":
                        dv.setUint32(bfl, 2, true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[1], true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[2], true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[3], true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[4], true);
                        bfl += 4;
                        dv.setFloat32(bfl, path[5], true);
                        bfl += 4;
                        break;
                    case "closePath":
                        dv.setUint32(bfl, 3, true);
                        bfl += 4;
                        break;
                }
            }
            ;
        }
        else
            return;
        _.needsz((bl + plen + 7) * 4 + bfl);
        _.wu32(this.id);
        _.wu32(19);
        _.wf32(x + 0.5);
        _.wf32(y + 0.5);
        _.wu32(bl);
        _.wu32(plen);
        _.wu32(bfl);
        _.wab(ab, bfl);
        if (bl > 0) {
            _.wu32(fc);
        }
        if (plen > 0) {
            _.wu32(sc);
            _.wu32(CanvasRenderingContext2D.LINEJOIN[pen.lineJoin] || 0);
            _.wu32(CanvasRenderingContext2D.LINECAP[pen.lineCap] || 0);
            _.wf32(pen.lineWidth || 1);
            _.wf32(pen.miterLimit);
        }
        this._addCmd([19, x + 0.5, y + 0.5, paths]);
    }
    drawLine(x1, y1, x2, y2, lineColor, lineWidth) {
        var lc = ColorUtil.getColorNum(lineColor);
        if (isNaN(lc)) {
            console.error("drawLine lineColor:" + lineColor + "not support");
            lc = 0xFFFFFFFF;
        }
        var _ = conch.bf;
        x1 += 0.5;
        y1 += 0.5;
        x2 += 0.5;
        y2 += 0.5;
        _.needsz(32);
        _.wu32(this.id);
        _.wu32(20);
        _.wf32Rect(x1, y1, x2, y2);
        _.wf32(lineWidth || 1);
        _.wu32(lc);
        this._addCmd([20, x1, y1, x2, y2]);
    }
    _drawPs(fid, x, y, points, lineColor, lineWidth) {
        var lc = ColorUtil.getColorNum(lineColor), pl = points.length;
        if (isNaN(lc)) {
            console.error("_drawPs lineColor:" + lineColor + "not support");
            lc = 0xFFFFFFFF;
        }
        var _ = conch.bf;
        _.needsz(28 + pl * 4);
        _.wu32(this.id);
        _.wu32(fid);
        _.wf32(x + 0.5);
        _.wf32(y + 0.5);
        _.wf32(lineWidth || 1);
        _.wu32(lc);
        _.wu32(pl);
        for (var i = 0; i < pl; i++) {
            _.wf32(points[i]);
        }
    }
    drawLines(x, y, points, lineColor, lineWidth) {
        this._drawPs(21, x, y, points, lineColor, lineWidth);
    }
    drawCurves(x, y, points, lineColor, lineWidth) {
        this._drawPs(22, x, y, points, lineColor, lineWidth);
        this._addCmd([22, x + 0.5, y + 0.5, points]);
    }
    replaceText(text) {
        var _ = conch.bf;
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(28);
        _.wstring(text, true);
        return true;
    }
    replaceTextColor(c) {
        var fc = ColorUtil.getColorNum(c);
        if (isNaN(fc)) {
            console.error("replaceTextColor:" + c + "not support");
            fc = 0xFFFFFFFF;
        }
        _Buffer.oneInt(this, 23, fc);
    }
    _fillImage(pat, x, y, fx, fy, w, h) {
        var _ = conch.bf;
        _.needsz(56);
        _.wu32(this.id);
        _.wu32(24);
        _.wu32(pat.image);
        _.wu32(pat.repetition);
        _.wf32Rect(pat.x, pat.y, pat.w, pat.h);
        switch (pat.repetition) {
            case 0:
                break;
            case 1:
                if (fy > 0) {
                    if (pat.h - fy < h) {
                        h = pat.h - fy;
                    }
                }
                else {
                    y -= fy;
                    if (pat.h - fy > h) {
                        h = h + fy;
                    }
                    else {
                        h = pat.h;
                    }
                    fy = 0;
                }
                break;
            case 2:
                if (fx > 0) {
                    if (pat.w - fx < w)
                        w = pat.w - fx;
                }
                else {
                    x -= fx;
                    if (pat.w - fx > w) {
                        w = w + fx;
                    }
                    else {
                        w = pat.w;
                    }
                    fx = 0;
                }
                break;
            default:
                break;
        }
        _.wf32Rect(x, y, fx, fy);
        _.wf32(w);
        _.wf32(h);
        this._addCmd([24, x + fx, y + fy, w, h]);
    }
    setSkinMesh(ib, vb, eleNum, iStart, texture, mat) {
        var img = texture.source;
        var _ = conch.bf;
        if (ib.conch_id === undefined || !ib._upload) {
            ib.conch_id = CanvasRenderingContext2D.ID++;
            ib.conchRef = new ArrayBufferRef();
            ib.conchRef.id = ib.conch_id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(11);
            _.wu32(ib.conch_id);
            _.wu32(ib.length * 2);
            ib.buffer = new Uint16Array(ib);
            _.wab(ib.buffer, ib.length * 2);
            ib._upload = true;
        }
        if (vb.conch_id === undefined || !vb._upload) {
            vb.conch_id = CanvasRenderingContext2D.ID++;
            vb.conchRef = new ArrayBufferRef();
            vb.conchRef.id = vb.conch_id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(11);
            _.wu32(vb.conch_id);
            _.wu32(vb.byteLength);
            _.wab(vb, vb.byteLength);
            vb._upload = true;
        }
        var shader = CanvasRenderingContext2D.skinMeshShader;
        if (shader.conch_id === undefined) {
            _.needsz(16);
            _.wu32(0);
            _.wu32(12);
            _.wu32(shader.id);
            _.wstring(shader.vs, true);
            _.needsz(4);
            _.wstring(shader.ps, true);
            shader.conch_id = shader.id;
        }
        _.needsz(68);
        _.wu32(this.id);
        _.wu32(25);
        _.wu32(ib.conch_id);
        _.wu32(vb.conch_id);
        _.wu32(1);
        _.wu32(eleNum);
        _.wu32(iStart);
        _.wu32(img.imgId);
        _.wu32(shader.id);
        var uv = texture.uv;
        _.wf32(mat.a);
        _.wf32(mat.b);
        _.wf32(mat.c);
        _.wf32(mat.d);
        _.wf32(mat.tx);
        _.wf32(mat.ty);
        _.wf32(0);
        _.wf32(0);
    }
    drawParticle(par) {
        var _ = conch.bf;
        _.needsz(20);
        _.wu32(this.id);
        _.wu32(26);
        _.wu32(par ? par.id : -1);
    }
    drawImageS(img, sx, sy, sw, sh, dx, dy, dw, dh, pos) {
        var _ = conch.bf;
        var len;
        if (!(pos instanceof Float32Array)) {
            if (CanvasRenderingContext2D.DRAWIMAGES.length < pos.length) {
                CanvasRenderingContext2D.DRAWIMAGES = new Float32Array(pos);
            }
            else {
                CanvasRenderingContext2D.DRAWIMAGES.set(pos);
            }
            len = pos.length * 4;
            pos = CanvasRenderingContext2D.DRAWIMAGES;
        }
        len = len || pos.byteLength;
        _.needsz(52);
        _.wu32(this.id);
        _.wu32(27);
        var bImage = img instanceof HTMLImageElement;
        _.wu32(bImage ? img.imgId : img._ctx2d.id);
        _.wf32(sx);
        _.wf32(sy);
        _.wf32(sw);
        _.wf32(sh);
        _.wf32(dx);
        _.wf32(dy);
        _.wf32(dw);
        _.wf32(dh);
        _.wu32(bImage ? 0 : 1);
        _.wu32(len);
        _.wab(pos, len);
    }
}
window["ConchNode"] = ConchNode;
window["ConchGraphics"] = ConchGraphics;
conch["platCallBack"] = function (ret) {
    var objid, m, rs, c, rJSON;
    if (ret == null)
        return;
    objid = ret.objId;
    m = ret.mName;
    c = ret.cName;
    rs = ret.v;
    var platC;
    if (objid != -1) {
        platC = PlatformObj.objMap[objid];
    }
    else {
        platC = PlatformClass.clsMap[c];
    }
    if (platC) {
        var callback = platC.callBackList[m];
        callback && callback(rs);
    }
};
class PlatformBase {
    call(methodName, p1, p2, p3, p4, p5, p6, p7, p8, p9) {
        var a = Array.prototype.slice.call(arguments);
        a.shift();
        try {
            var rs = JSON.parse(conch.callMethod(this.objid, true, this.className, methodName, JSON.stringify(a)) || "{}");
            return rs.v;
        }
        catch (e) {
            return null;
        }
    }
    callWithBack(callback, methodName, p1, p2, p3, p4, p5, p6, p7, p8, p9) {
        this.callBackList[methodName] = callback;
        var a = Array.prototype.slice.call(arguments);
        a.splice(0, 2);
        try {
            var rs = JSON.parse(conch.callMethod(this.objid, false, this.className, methodName, JSON.stringify(a)) || "{}");
            return rs.v;
        }
        catch (e) {
            return null;
        }
    }
}
class PlatformClass extends PlatformBase {
    constructor(clsName) {
        super();
        this.objid = -1;
        this.className = clsName;
        this.callBackList = {};
        PlatformClass.clsMap[clsName] = this;
    }
    static createClass(clsName) {
        return PlatformClass.clsMap[clsName] || new PlatformClass(clsName);
    }
    newObject(p1, p2, p3, p4, p5, p6, p7, p8, p9) {
        var a = Array.prototype.slice.call(arguments);
        a.splice(0, 0, "<init>");
        var rs = new PlatformObj(this);
        rs.init.apply(rs, a);
        return rs;
    }
}
PlatformClass.clsMap = {};
class PlatformObj extends PlatformBase {
    constructor(cls) {
        super();
        this.cls = cls;
        this.className = cls.className;
        this.callBackList = {};
    }
    init() {
        this.objid = PlatformObj.objNum++;
        this.call.apply(this, arguments) || -1;
        PlatformObj.objMap[this.objid] = this;
    }
}
PlatformObj.objMap = {};
PlatformObj.objNum = 0;
class ConchFps extends ConchNode {
    constructor(color, fontSize, x, y) {
        super();
        this.pos(x, y);
        this.color = color;
        this.fontSize = fontSize;
        this.fontstr = "normal 100 " + fontSize + "px Arial";
        this._repaint = false;
        this.fps = this.vertex = this.drawCall = this.countNode = 0;
        this.textAligh = "left";
        this.left = new ConchNode();
        this.addChildAt(this.left, 0);
        var g = new ConchGraphics();
        this._graphics = g;
        g.fillText("FPS", 2, 2, this.fontstr, color, this.textAligh, 0);
        g.fillText("Vertex", 2, fontSize + 10, this.fontstr, color, this.textAligh, 0);
        g.fillText("DrawCall", 2, fontSize * 2 + 20, this.fontstr, color, this.textAligh, 0);
        g.fillText("Sprite", 2, fontSize * 3 + 30, this.fontstr, color, this.textAligh, 0);
        this.graphics(g);
        this.left.pos(fontSize * 5, 0);
        this.left._graphics = new ConchGraphics();
        this.left.graphics(this.left._graphics);
        var g = this.left._graphics;
        g.fillText("0", 0, 2, this.fontstr, this.color, this.textAligh, 0);
        g.fillText("0", 0, this.fontSize + 10, this.fontstr, this.color, this.textAligh, 0);
        g.fillText("0", 0, this.fontSize * 2 + 20, this.fontstr, this.color, this.textAligh, 0);
        g.fillText("0", 0, this.fontSize * 3 + 30, this.fontstr, this.color, this.textAligh, 0);
        this.setFPSNode();
        this.size(this.fontSize * 7 + 10, this.fontSize * 4 + 40);
        this.bgColor("rgba(150,150,150,0.8)");
    }
    render(f) {
        var t = conch.getCountVertext();
        var d = conch.getCountGroup();
        var countNode = conch.getCountNode();
        if (this.preVetex != undefined) {
            d = d - 2;
            var last = ('' + this.preVetex + this.preFps + this.preDrawCall).length;
            t -= (last * 4 + 72);
            if (d < 0)
                d = 0;
            if (t < 0)
                t = 0;
        }
        this.preVetex = t;
        this.preDrawCall = d;
        this.preFps = f;
        if (this.countNode != countNode) {
            this.countNode = countNode;
            this._repaint = true;
        }
        if (this.vertex != t) {
            this.vertex = t;
            this._repaint = true;
        }
        if (this.fps != f) {
            this.fps = f;
            this._repaint = true;
        }
        if (this.drawCall != d) {
            this.drawCall = d;
            this._repaint = true;
        }
        if (this._repaint) {
            var g = this.left._graphics;
            g.clear();
            g.fillText(this.fps + "", 0, 2, this.fontstr, this.color, this.textAligh, 0);
            g.fillText(this.vertex + "", 0, this.fontSize + 10, this.fontstr, this.color, this.textAligh, 0);
            g.fillText(this.drawCall + "", 0, this.fontSize * 2 + 20, this.fontstr, this.color, this.textAligh, 0);
            g.fillText(this.countNode + "", 0, this.fontSize * 3 + 30, this.fontstr, this.color, this.textAligh, 0);
        }
    }
}
window["PlatformClass"] = PlatformClass;
window["PlatformObj"] = PlatformObj;
function loadRawCache(cache, relUrl, encode) {
    var cpath = cache.getCachePath();
    var relFile = relUrl;
    var id = new Uint32Array([cache.hashstr(relFile)])[0].toString(16);
    var fn = cpath + '/files/' + id.substr(0, 2) + '/' + id.substr(2);
    var ab = fs_readFileSync(fn);
    if (ab) {
        var content = new Uint8Array(ab, 48);
        if (encode === 'utf8') {
            var strCont = String.fromCharCode.apply(null, content);
            return strCont;
        }
        else
            return content.buffer;
    }
    return null;
}
class ConchParticleTemplate2D {
    constructor() {
        this._nativeObj = new _conchParticleTemplate2D();
        this.id = this._nativeObj.conchID;
        this.setShader();
    }
    set settings(st) {
        var _ = conch.bf;
        _.needsz(32);
        _.wu32(this.id);
        _.wu32(0);
        _.wu32(216);
        _.wu32(st.textureCount);
        _.wu32(st.maxPartices);
        _.wu32(st.blendState === 7 ? 7 : st.blendState + 11);
        _.wu32(st.useEndRadian ? 1 : 0);
        _.wu32(st.colorComponentInter ? 1 : 0);
        _.wu32(st.disableColor ? 1 : 0);
        var temp = new Float32Array([
            st.duration,
            st.ageAddScale,
            st.emitterVelocitySensitivity,
            st.minStartSize,
            st.maxStartSize,
            st.minEndSize,
            st.maxEndSize,
            st.minHorizontalVelocity,
            st.maxHorizontalVelocity,
            st.minVerticalVelocity,
            st.maxVerticalVelocity,
            st.endVelocity,
            st.minRotateSpeed,
            st.maxRotateSpeed,
            st.minStartRadius,
            st.maxStartRadius,
            st.minEndRadius,
            st.maxEndRadius,
            st.minHorizontalStartRadian,
            st.maxHorizontalStartRadian,
            st.minVerticalStartRadian,
            st.maxVerticalStartRadian,
            st.minHorizontalEndRadian,
            st.maxHorizontalEndRadian,
            st.minVerticalEndRadian,
            st.maxVerticalEndRadian,
            st.gravity[0],
            st.gravity[1],
            st.gravity[2],
            st.minStartColor[0],
            st.minStartColor[1],
            st.minStartColor[2],
            st.minStartColor[3],
            st.maxStartColor[0],
            st.maxStartColor[1],
            st.maxStartColor[2],
            st.maxStartColor[3],
            st.minEndColor[0],
            st.minEndColor[1],
            st.minEndColor[2],
            st.minEndColor[3],
            st.maxEndColor[0],
            st.maxEndColor[1],
            st.maxEndColor[2],
            st.maxEndColor[3],
            st.positionVariance[0],
            st.positionVariance[1],
            st.positionVariance[2],
        ]);
        _.wab(temp, 192);
    }
    setShader() {
        var _ = conch.bf;
        var shader = CanvasRenderingContext2D.particleShader;
        if (shader.conch_id == undefined) {
            shader.conch_id = shader.id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(12);
            _.wu32(shader.conch_id);
            _.wstring(shader.vs, true);
            _.needsz(4);
            _.wstring(shader.ps, true);
        }
    }
    get texture() {
        return this._tex;
    }
    set texture(tex) {
        this._tex = tex;
        _Buffer.oneInt(this, 1, tex.source.imgId);
    }
    dispose() {
    }
    play() {
        _Buffer.noParm(this, 2);
    }
    stop() {
        _Buffer.noParm(this, 3);
    }
}
window["ConchParticleTemplate2D"] = ConchParticleTemplate2D;
class WindowBase64 {
    constructor() {
        this.atob = function () { return null; };
        this.btoa = function () { return null; };
    }
}
window["WindowBase64"] = WindowBase64;
class _Buffer {
    constructor() {
        this.ABSIZE = 20 * 1024;
        this._cmdBuf = new DataView(new ArrayBuffer(this.ABSIZE));
        this._cmdPos = new Uint32Array(this._cmdBuf.buffer, 0, 1);
        this._byteArray = new Uint8Array(this._cmdBuf.buffer);
        this._shortArray = new Uint16Array(this._cmdBuf.buffer);
        this._intArray = new Uint32Array(this._cmdBuf.buffer);
        this._floatArray = new Float32Array(this._cmdBuf.buffer);
        this._nativeobj = conch;
    }
    needsz(d) {
        if (this._cmdPos[0] + d > this.ABSIZE) {
            this._nativeobj.flushRenderCommands();
        }
    }
    initPos(d) {
        this._cmdPos[0] = d;
    }
    wu8(v) {
        this._byteArray[this._cmdPos[0]++] = v;
    }
    wu32(v) {
        this._intArray[(this._cmdPos[0] += 4) / 4 - 1] = v;
    }
    wu16(v) {
        this._shortArray[(this._cmdPos[0] += 2) / 2 - 1] = v;
    }
    wf32(v) {
        this._floatArray[(this._cmdPos[0] += 4) / 4 - 1] = v;
    }
    wf32Rect(x, y, w, h) {
        this.wf32(x);
        this.wf32(y);
        this.wf32(w);
        this.wf32(h);
    }
    wu64(value) {
        var temp = value.toString(16);
        var len = temp.length;
        var pre, last;
        if (len > 8 && value > 0) {
            pre = parseInt(temp.substr(len - 8), 16);
            last = parseInt(temp.substr(0, len - 8), 16);
        }
        else {
            pre = value;
            last = 0;
        }
        this.wu32(pre);
        this.wu32(last);
    }
    wab(arraybuffer, length, offset) {
        offset = offset ? offset : 0;
        if (length > this.ABSIZE / 2) {
            this.flush();
            this._nativeobj.appendBuffer(arraybuffer, length, offset);
            return;
        }
        if (length < 0)
            throw "wab error - Out of bounds";
        var i = length % 4;
        this.needsz(length + (4 - i));
        var uint8array;
        if (arraybuffer instanceof ArrayBuffer) {
            uint8array = new Uint8Array(arraybuffer, offset, length);
        }
        else if (arraybuffer.buffer) {
            uint8array = new Uint8Array(arraybuffer.buffer, offset + arraybuffer.byteOffset, length);
        }
        else
            throw "not arraybuffer/dataview ";
        this._byteArray.set(uint8array, this._cmdPos[0]);
        this._cmdPos[0] += length;
        if (i != 0) {
            for (var j = 0; j < 4 - i; j++) {
                this.wu8(0);
            }
        }
    }
    wstring(str, is32) {
        var ab = conch.strTobufer(str);
        var len = ab.byteLength;
        if (is32)
            this.wu32(len);
        else
            this.wu16(len);
        if (len == 0) {
            return;
        }
        this.wab(ab, len);
    }
    flush() {
        if (this._cmdPos[0] <= 4)
            return;
        this._nativeobj.flushRenderCommands();
    }
    static readString(bf, pos, len) {
        len = bf.getUint16(pos, true);
        pos += 2;
        var value = "";
        var max = pos + len;
        var c, c2, c3;
        while (pos < max) {
            c = bf.getUint8(pos++);
            if (c < 0x80) {
                if (c != 0) {
                    value += String.fromCharCode(c);
                }
            }
            else if (c < 0xE0) {
                value += String.fromCharCode(((c & 0x3F) << 6) | (bf.getUint8(pos++) & 0x7F));
            }
            else if (c < 0xF0) {
                c2 = bf.getUint8(pos++);
                value += String.fromCharCode(((c & 0x1F) << 12) | ((c2 & 0x7F) << 6) | (bf.getUint8(pos++) & 0x7F));
            }
            else {
                c2 = bf.getUint8(pos++);
                c3 = bf.getUint8(pos++);
                value += String.fromCharCode(((c & 0x0F) << 18) | ((c2 & 0x7F) << 12) | ((c3 << 6) & 0x7F) | (bf.getUint8(pos++) & 0x7F));
            }
        }
        return value;
    }
    static setMatrix(that, fid, m11, m12, m21, m22, dx, dy) {
        var _ = conch.bf;
        _.needsz(32);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32(m11);
        _.wf32(m12);
        _.wf32(m21);
        _.wf32(m22);
        _.wf32(dx);
        _.wf32(dy);
    }
    static rect(that, fid, x, y, w, h) {
        var _ = conch.bf;
        _.needsz(24);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32Rect(x, y, w, h);
    }
    static noParm(that, fid) {
        var _ = conch.bf;
        _.needsz(8);
        _.wu32(that.id);
        _.wu32(fid);
    }
    static oneNum(that, fid, n) {
        var _ = conch.bf;
        _.needsz(12);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32(n);
    }
    static oneInt(that, fid, n) {
        var _ = conch.bf;
        _.needsz(12);
        _.wu32(that.id);
        _.wu32(fid);
        _.wu32(n);
    }
    static point(that, fid, x, y) {
        var _ = conch.bf;
        _.needsz(16);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32(x);
        _.wf32(y);
    }
    static wany(_, ab, t) {
        switch (t) {
            case 0:
                _.needsz(4);
                var i = ab.byteLength;
                _.wu32(i);
                _.wab(ab, i);
                break;
            case 1:
                _.needsz(8);
                _.wu32(4);
                _.wu32(ab);
                break;
            case 2:
                _.needsz(8);
                _.wu32(4);
                _.wf32(ab);
                break;
            default:
                break;
        }
    }
}
conch.bf = new _Buffer();
conch.setCmdBuffer(conch.bf._byteArray.buffer);
conch.bf.initPos(4);
function _process() {
    this.pid = 0;
    this.cwd = function () {
        return 'd:/temp';
    };
    this.mainModule = 'index.js';
    this.argv = ['conch.exe', 'index.js'];
    this.version = '1.3.1';
    this._require = function (f) { console.log('process require(' + f + ')'); return function nop() { }; };
    this._debugObject = {};
}
window.process = new _process();
class SubtleCrypto {
    decrypt(algorithm, key, data) {
    }
    deriveBits(algorithm, baseKey, length) {
    }
    deriveKey(algorithm, baseKey, derivedKeyType, extractable, keyUsages) {
    }
    digest(algorithm, data) {
    }
    encrypt(algorithm, key, data) {
    }
    exportKey(format, key) {
    }
    generateKey(algorithm, extractable, keyUsages) {
    }
    importKey(format, keyData, algorithm, extractable, keyUsages) {
    }
    sign(algorithm, key, data) {
    }
    unwrapKey(format, wrappedKey, unwrappingKey, unwrapAlgorithm, unwrappedKeyAlgorithm, extractable, keyUsages) {
    }
    verify(algorithm, key, signature, data) {
    }
    wrapKey(format, key, wrappingKey, wrapAlgorithm) {
    }
}
class Crypto {
    constructor() {
        this.subtle = new SubtleCrypto();
    }
    getRandomValues(array) {
        return null;
    }
}
var LogLevel;
(function (LogLevel) {
    LogLevel[LogLevel["Warn"] = 0] = "Warn";
    LogLevel[LogLevel["Error"] = 1] = "Error";
    LogLevel[LogLevel["Debug"] = 2] = "Debug";
    LogLevel[LogLevel["Info"] = 3] = "Info";
    LogLevel[LogLevel["Runtime"] = 4] = "Runtime";
})(LogLevel || (LogLevel = {}));
class Console {
    constructor() {
    }
    assert(test, message) {
        var c = _console;
        if (test) {
            c.log(3, message);
        }
        ;
    }
    clear() { }
    count(countTitle) {
    }
    debug(message) {
        var c = _console;
        c.log(LogLevel.Debug, message);
    }
    dir(value) {
    }
    dirxml(value) {
    }
    error(message) {
        var c = _console;
        c.log(LogLevel.Error, message);
    }
    group(groupTitle) { }
    groupCollapsed(groupTitle) { }
    groupEnd() { }
    info(message) {
        var c = _console;
        c.log(LogLevel.Info, message);
    }
    log(message) {
        var c = _console;
        c.log(LogLevel.Info, message);
    }
    profile(reportName) {
    }
    profileEnd() { }
    select(element) { }
    time(timerName) { }
    timeEnd(timerName) { }
    trace(message) {
    }
    warn(message) {
        var c = _console;
        c.log(LogLevel.Warn, message);
    }
}
class GlobalEventHandlers {
    constructor() {
    }
    onpointercancel(ev) {
        return null;
    }
    onpointerdown(ev) {
        return null;
    }
    onpointerenter(ev) {
        return null;
    }
    onpointerleave(ev) {
        return null;
    }
    onpointermove(ev) {
        return null;
    }
    onpointerout(ev) {
        return null;
    }
    onpointerover(ev) {
        return null;
    }
    onpointerup(ev) {
        return null;
    }
}
class ErrorEvent {
    constructor() {
    }
    initErrorEvent(typeArg, canBubbleArg, cancelableArg, messageArg, filenameArg, linenoArg) {
    }
}
class Event {
    constructor(type, eventInitDict) {
        this._propagationStopped = false;
        this.type = type;
        this.timeStamp = Date.now();
        this.bubbles = false;
        this.cancelable = false;
        this.eventPhase = Event.AT_TARGET;
        if (eventInitDict) {
            this.bubbles = eventInitDict.bubbles;
            this.cancelable = eventInitDict.cancelable;
        }
    }
    initEvent(eventTypeArg, canBubbleArg, cancelableArg) {
        this.type = eventTypeArg;
        this.bubbles = canBubbleArg;
        this.cancelable = cancelableArg;
    }
    preventDefault() {
        if (!this.cancelable)
            return;
        this.defaultPrevented = true;
    }
    stopImmediatePropagation() {
    }
    stopPropagation() {
        this._propagationStopped = true;
    }
}
Event.AT_TARGET = 2;
Event.BUBBLING_PHASE = 3;
Event.CAPTURING_PHASE = 1;
var _lbEvent = window['Event'] = Event;
var _lbMap = Map;
class EventTarget {
    constructor() {
        this._evtMaps = new _lbMap();
        this.removeEventListener = (type, listener, useCapture) => {
            var listeners = this._evtMaps.get(type);
            if (!listeners)
                return;
            var newlisteners = [];
            for (var i = 0, sz = listeners.length; i < sz; i++) {
                if (listeners[i].listener != listener || listeners[i].useCapture != useCapture) {
                    newlisteners.push(listeners[i]);
                }
            }
            this._evtMaps.set(type, newlisteners);
        };
        this.dispatchEvent = this._dispatchEvent.bind(this);
        this.addEventListener = this._addEventListener.bind(this);
        this.fireEventListeners = this._fireEventListeners.bind(this);
    }
    addEventListener(type, listener, useCapture) {
    }
    _addEventListener(type, listener, useCapture) {
        var listeners = this._evtMaps.get(type) || [];
        listeners.push({ listener: listener, useCapture: useCapture });
        this._evtMaps.set(type, listeners);
    }
    _dispatchEvent(evt) {
        this.fireEventListeners(evt);
        return !evt.defaultPrevented;
    }
    _fireEventListeners(evt) {
        var listeners = this._evtMaps.get(evt.type);
        if (listeners) {
            listeners.forEach(function (listener) {
                switch (evt.eventPhase) {
                    case Event.CAPTURING_PHASE:
                        if (listener.useCapture && listener.listener)
                            listener.listener(evt);
                        break;
                    case Event.AT_TARGET:
                        if (listener.listener)
                            listener.listener(evt);
                        break;
                    case Event.BUBBLING_PHASE:
                        if (!listener.useCapture && listener.listener)
                            listener.listener(evt);
                        break;
                }
            });
        }
    }
}
class UIEvent extends Event {
    constructor(type, eventInitDict) {
        super(type);
        this.bubbles = true;
        if (eventInitDict) {
            this.initUIEvent(type, eventInitDict.bubbles, eventInitDict.cancelable, eventInitDict.view, eventInitDict.detail);
        }
    }
    initUIEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg) {
        this.type = typeArg;
        this.cancelable = canBubbleArg;
        this.cancelable = cancelableArg;
        this.view = viewArg;
        this.detail = detailArg;
    }
}
class PointerEvent {
    constructor(typeArg, eventInitDict) {
    }
}
class MouseEvent extends UIEvent {
    constructor(typeArg, eventInitDict) {
        super(typeArg, eventInitDict);
        if (eventInitDict) {
            for (var v in eventInitDict) {
                this[v] = eventInitDict[v];
            }
        }
    }
    getModifierState(keyArg) {
        return false;
    }
    initMouseEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg, screenXArg, screenYArg, clientXArg, clientYArg, ctrlKeyArg, altKeyArg, shiftKeyArg, metaKeyArg, buttonArg, relatedTargetArg) {
        var args = arguments;
        ['type', 'bubbles', 'cancelable', 'view', 'detail', 'screenX', 'screenY', 'clientX', 'clientY', 'ctrlKey', 'altKey', 'shiftKey', 'metaKey', 'button', 'relatedTarget'].forEach((v, i, a) => {
            this[v] = args[i];
        });
    }
}
var _lbMouseEvent = window['MouseEvent'] = MouseEvent;
class MouseWheelEvent extends MouseEvent {
    constructor() {
        super("mousewheel");
    }
    initMouseWheelEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg, screenXArg, screenYArg, clientXArg, clientYArg, buttonArg, relatedTargetArg, modifiersListArg, wheelDeltaArg) {
    }
}
class WheelEvent extends MouseEvent {
    constructor(typeArg, eventInitDict) {
        super(typeArg, eventInitDict);
        if (eventInitDict) {
            this.deltaMode = eventInitDict.deltaMode;
            this.deltaX = eventInitDict.deltaX;
            this.deltaY = eventInitDict.deltaY;
            this.deltaZ = eventInitDict.deltaZ;
        }
    }
    getCurrentPoint(element) {
    }
    initWheelEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg, screenXArg, screenYArg, clientXArg, clientYArg, buttonArg, relatedTargetArg, modifiersListArg, deltaXArg, deltaYArg, deltaZArg, deltaMode) {
    }
}
WheelEvent.DOM_DELTA_LINE = 1;
WheelEvent.DOM_DELTA_PAGE = 2;
WheelEvent.DOM_DELTA_PIXEL = 0;
class Touch {
    constructor() {
    }
}
class TouchList extends Array {
    item(index) {
        return this[index];
    }
}
class DeviceMotionEvent extends Event {
    constructor() {
        super('devicemotion');
    }
    initDeviceMotionEvent(type, bubbles, cancelable, acceleration, accelerationIncludingGravity, rotationRate, interval) {
    }
}
window["DeviceMotionEvent"] = DeviceMotionEvent;
class DeviceOrientationEvent extends Event {
    constructor() {
        super('deviceorientation');
    }
    initDeviceOrientationEvent(type, bubbles, cancelable, alpha, beta, gamma, absolute) {
    }
}
window["DeviceOrientationEvent"] = DeviceOrientationEvent;
class DocumentEvent {
    constructor() {
    }
    createEvent(eventInterface) {
        return null;
    }
}
class ProgressEvent extends Event {
    constructor(type) {
        super(type);
    }
    initProgressEvent(typeArg, canBubbleArg, cancelableArg, lengthComputableArg, loadedArg, totalArg) {
        this.type = typeArg;
        this.cancelBubble = canBubbleArg;
        this.cancelable = cancelableArg;
        this.lengthComputable = lengthComputableArg;
        this.loaded = loadedArg;
        this.total = totalArg;
    }
}
var _lbProgressEvent = window["ProgressEvent"] = ProgressEvent;
var VendorIDSource;
(function (VendorIDSource) {
    VendorIDSource[VendorIDSource["bluetooth"] = 0] = "bluetooth";
    VendorIDSource[VendorIDSource["usb"] = 1] = "usb";
})(VendorIDSource || (VendorIDSource = {}));
;
class BluetoothDevice {
    connectGATT() {
        return null;
    }
}
class Bluetooth extends EventTarget {
    requestDevice(options) {
        return null;
    }
}
class Gamepad {
}
class GamepadEvent extends Event {
}
class Storage {
    constructor() {
        this.storagePath = conchConfig.getStoragePath();
        this.filename = '';
        this.db = {};
        this._len = 0;
    }
    get length() {
        return this._len;
    }
    getItem(key) {
        return this.db[key] || null;
    }
    key(index) {
        var keys = Object.keys(this.db);
        keys.sort();
        return keys[index] || null;
    }
    removeItem(key) {
        if (this.db[key])
            this._len--;
        delete this.db[key];
        this.savedb();
    }
    _setItem(key, data) {
        if (this.db[key] == null)
            this._len++;
        this.db[key] = data;
        Object.defineProperty(this, key, {
            get: function () {
                return this.db[key];
            },
            enumerable: true,
            configurable: true
        });
    }
    setItem(key, data) {
        this._setItem(key, data);
        this.savedb();
    }
    create(url) {
        url = location.fullpath;
        if (location.protocol == "file:") {
            this.filename = url.substring(8).replace(/:/g, '_').replace(/[\\\/]/g, '__');
            this.fileNamePre = this.storagePath + '/' + this.filename;
        }
        else {
            this.fileNamePre = this.storagePath + '/' + url.split('/')[2].replace(':', '_');
        }
        this.filename = this.fileNamePre + '.txt';
        var strdb = readFileSync(this.filename, 'utf8') || '{}';
        var db = JSON.parse(strdb);
        for (var v in db) {
            this._setItem(v, db[v]);
        }
        return this;
    }
    onChange(changes) {
        if (changes && changes.length) {
        }
    }
    clear() {
        this.db = {};
        this.savedb();
    }
    savedb() {
        writeStrFileSync(this.filename, JSON.stringify(this.db));
    }
}
window["Storage"] = Storage;
class WindowLocalStorage {
    constructor() {
        this.localStorage = new Storage();
    }
}
class WindowSessionStorage {
    getItem(i) {
        return this[i] || null;
    }
    setItem(i, b) {
        this[i] = b;
    }
}
window.loadLocalStorage = function (url) {
    return new Storage().create(url);
};
class _Cookie {
    constructor() {
        this.domain = "";
    }
    static addCookie(s) {
        var result = _Cookie.sreg.exec(s);
        if (result == null) {
            console.warn("设置cookie无效");
            return null;
        }
        else {
            var temp = new _Cookie();
            temp.key = result[1];
            temp.value = result[2];
            if (result[4]) {
                temp.expires = new Date(result[4]);
            }
            return temp;
        }
    }
    static pushCookie(c) {
        if (!c.isValid())
            return false;
        for (var i = 0, len = _Cookie.cookies.length; i < len; i++) {
            var temp = _Cookie.cookies[i];
            if (temp.key == c.key) {
                _Cookie.cookies[i] = c;
                return true;
            }
        }
        _Cookie.cookies.push(c);
        return true;
    }
    static flush() {
        writeStrFileSync(document._cookiePath, _Cookie.getString("in", "\""));
    }
    static getString(type, joinstr) {
        var a = [];
        for (var i = 0, len = _Cookie.cookies.length; i < len; i++) {
            var temp = _Cookie.cookies[i];
            if (temp.isValid()) {
                a.push(temp.toLocalString(type));
            }
            else {
                _Cookie.cookies.slice(i, 1);
                i--;
                len--;
            }
        }
        return a.join(joinstr);
    }
    static toLocalString() {
        return _Cookie.getString("out", "; ");
    }
    isValid() {
        if (this.expires && (this.expires.getTime() < Date.now())) {
            return false;
        }
        return true;
    }
    static init(s) {
        if (s) {
            var temp = s.split("\"");
            for (var i = 0, len = temp.length; i < len; i++) {
                var t = _Cookie.addCookie(temp[i]);
                console.warn(">>>>>>>>>>addCookie" + temp[i]);
                if (t) {
                    t.value = decodeURIComponent(t.value);
                    t.domain = decodeURIComponent(t.domain);
                }
                _Cookie.pushCookie(t);
            }
        }
    }
    toLocalString(type) {
        switch (type) {
            case "in":
                return this.key + "=" + encodeURIComponent(this.value) + "; expires=" + this.expires.toGMTString() + "; domain=" + encodeURIComponent(this.domain);
            case "out":
                return this.key + "=" + this.value;
            default:
                return null;
        }
    }
}
_Cookie.cookies = [];
_Cookie.sreg = new RegExp("([^=]*)\\s*=\\s*([^;]*)\\s*;\\s*(expires\\s*=\\s*(.{23,26}GMT)|)");
class KeyboardEvent extends UIEvent {
    constructor(typeArg, eventInitDict) {
        super(typeArg);
        if (eventInitDict) {
            var ini = eventInitDict;
            this.altKey = eventInitDict.altKey;
            this.initKeyboardEvent(typeArg, ini.bubbles, ini.cancelable, ini.view, ini.key, ini.location, null, ini.repeat, null);
        }
    }
    initKeyboardEvent(typeArg, canBubbleArg, cancelableArg, viewArg, keyArg, locationArg, modifiersListArg, repeat, locale) {
        this.type = typeArg;
        this.cancelable = canBubbleArg;
        this.cancelable = cancelableArg;
        this.key = keyArg;
        this.location = locationArg;
        this.locale = locale;
    }
}
KeyboardEvent.DOM_KEY_LOCATION_LEFT = 1;
KeyboardEvent.DOM_KEY_LOCATION_NUMPAD = 3;
KeyboardEvent.DOM_KEY_LOCATION_RIGHT = 2;
KeyboardEvent.DOM_KEY_LOCATION_STANDARD = 0;
var _lbKeyboardEvent = window["KeyboardEvent"] = KeyboardEvent;
(function (layaDoc) {
    'use strict';
    var APP_TOUCH_DOWN = 0;
    var APP_TOUCH_UP = 1;
    var APP_TOUCH_MOV = 2;
    var APP_TOUCH_PT_DOWN = 5;
    var APP_TOUCH_PT_UP = 6;
    var m_vTouchs = [];
    class touchEvt extends UIEvent {
        constructor(type, id, name, x, y) {
            super('');
            this.changedTouches = [];
            this.touches = [];
            this.targetTouches = this.changedTouches;
            var touch = new Touch();
            touch.identifier = id;
            touch.pageX = touch.screenX = touch.clientX = x;
            touch.pageY = touch.screenY = touch.clientY = y;
            this.changedTouches.push(touch);
            switch (type) {
                case APP_TOUCH_DOWN:
                case APP_TOUCH_PT_DOWN:
                    m_vTouchs.push(touch);
                    break;
                case APP_TOUCH_PT_UP:
                    {
                        var epos = 0;
                        var tnum = m_vTouchs.length;
                        while (epos < tnum) {
                            var ce = m_vTouchs[epos];
                            if (ce.identifier == id) {
                                break;
                            }
                            epos++;
                        }
                        if (epos >= tnum) {
                        }
                        else {
                            m_vTouchs.splice(epos, 1);
                        }
                    }
                    break;
                case APP_TOUCH_MOV:
                    {
                        var tnum = m_vTouchs.length;
                        var ti = 0;
                        while (ti < tnum) {
                            var ce = m_vTouchs[ti];
                            if (ce.identifier == id) {
                                m_vTouchs[ti] = touch;
                                break;
                            }
                            ti++;
                        }
                    }
                    break;
                case APP_TOUCH_UP:
                    m_vTouchs = [];
                    break;
            }
            this.touches = m_vTouchs;
            switch (type) {
                case 0:
                case 5:
                    this.type = "touchstart";
                    break;
                case 1:
                case 6:
                    this.type = "touchend";
                    break;
                case 2:
                    this.type = "touchmove";
                    break;
            }
        }
    }
    var joystickEvt = function (TL_xOffset, TL_yOffset, TR_xOffset, TR_yOffset, LT_Offset, RT_Offset) {
        this.THUMBL_xOffset = TL_xOffset;
        this.THUMBL_yOffset = TL_yOffset;
        this.THUMBR_xOffset = TR_xOffset;
        this.THUMBR_yOffset = TR_yOffset;
        this.LT_Offset = LT_Offset;
        this.RT_Offset = RT_Offset;
    };
    var keyEvt = function (name, code, keychar, flag) { this.type = name; this.keyCode = code; this.keyChar = keychar; this.altKey = flag & 0x1; this.shiftKey = (flag & 0x2) != 0; this.ctrlKey = (flag & 0x4) != 0; this.preventDefault = function () { }; };
    conch.setTouchEvtFunction((touchtype, id, etype, x, y) => {
        if (conch.disableMultiTouch && id != 0)
            return;
        var doc = window.document;
        if (!doc) {
            console.log('touch event cant dispatch!');
            return;
        }
        var evt = new touchEvt(touchtype, id, etype, x, y);
        evt.target = doc.pickElement(x, y);
        doc.dispatchEvent(evt);
    });
    function keyEventHandle() {
        var ke = new _lbKeyboardEvent('');
        return function (type, keycode, keychar, AltShiftCtrl) {
            var doc = window.document;
            if (!doc)
                return;
            var keyinit = {};
            var kc = String.fromCharCode(keycode);
            ke.defaultPrevented = false;
            ke._propagationStopped = false;
            ke.type = type;
            ke.key = kc;
            ke.keyCode = keycode;
            ke.altKey = (AltShiftCtrl & 0x4) != 0;
            ke.shiftKey = (AltShiftCtrl & 0x2) != 0;
            ke.ctrlKey = (AltShiftCtrl & 0x1) != 0;
            ke.target = doc._topElement;
            doc.dispatchEvent(ke);
            if (!ke.defaultPrevented) {
                var f = doc['on' + type];
                if (f) {
                    f.call(doc, ke);
                }
            }
        };
    }
    conch.setKeyEvtFunction(keyEventHandle());
    conch.setMouseEvtFunction(function (touchtype, type, x, y, wheel) {
        var doc = window.document;
        if (!doc) {
            console.log('mouse event cant dispatch!');
            return;
        }
        var target = doc.pickElement(x, y);
        if (wheel != 0) {
            var evt1 = new MouseWheelEvent();
            evt1.clientX = evt1.pageX = evt1.screenX = x;
            evt1.clientY = evt1.pageY = evt1.screenY = y;
            evt1.target = target;
            evt1.wheelDelta = wheel;
            doc.dispatchEvent(evt1);
        }
        else {
            var evt = new _lbMouseEvent(type);
            evt.button = 0;
            evt.clientX = evt.pageX = evt.screenX = x;
            evt.clientY = evt.pageY = evt.screenY = y;
            evt.target = target;
            doc.dispatchEvent(evt);
        }
    });
    conch.otherBuffer = new ArrayBuffer(40);
    conch.otherDataView = new DataView(conch.otherBuffer);
    conch.setBuffer(conch.otherBuffer);
    conch.setDeviceMotionEvtFunction(function (type, ra, rb, rg) {
        if ((typeof ra) != "undefined") {
            var e = new DeviceOrientationEvent();
            e.alpha = ra;
            e.beta = rb;
            e.gamma = rg;
            window.dispatchEvent(e);
        }
        else {
            var d = conch.otherDataView;
            var evt = new DeviceMotionEvent();
            evt.acceleration = { x: d.getFloat32(0, true), y: d.getFloat32(4, true), z: d.getFloat32(8, true) };
            evt.accelerationIncludingGravity = { x: d.getFloat32(12, true), y: d.getFloat32(16, true), z: d.getFloat32(20, true) };
            evt.rotationRate = { alpha: d.getFloat32(24, true), beta: d.getFloat32(28, true), gamma: d.getFloat32(32, true) };
            evt.interval = d.getFloat32(36, true);
            window.dispatchEvent(evt);
        }
    });
    setJoystickEvtFunction(function (type, thumbL_xoff, thumbL_yoff, thumbR_xoff, thumbR_yoff, LT_offset, RT_offset) {
    });
    conch.setNetworkEvtFunction(function (type) {
        var event = new Event("network");
        event["code"] = type;
        document.dispatchEvent(event);
    });
})(window.document);
class Navigator {
    constructor() {
    }
    get appName() { return 'Netscape'; }
    get appVersion() { return this.userAgent; }
    ;
    get platform() { return window.layabox.devinfo.os; }
    ;
    get userAgent() {
        var os = window.layabox.devinfo.os;
        if (os == "ios")
            return "LayaBox(iPhone; CPU iPhone OS Mac OS X)";
        else if (os == "android")
            return "LayaBox Android";
        else
            return 'LayaBox/2.1';
    }
    ;
    get sv() {
        var v = window.layabox.devinfo.osversion;
        var t = parseFloat(v);
        if (isNaN(t)) {
            t = v.charCodeAt(0) - 71;
        }
        return t;
    }
    ;
    get appCodeName() { return 'Mozilla'; }
    get language() { return 'zh-CN'; }
    ;
    get userLanguage() { return 'zh-CN'; }
    getGamepads() {
        return null;
    }
    javaEnabled() { return false; }
    vibrate(pattern) {
        return false;
    }
    addEventListener(type, listener, useCapture) {
    }
}
class Node extends EventTarget {
    constructor() {
        super();
        this._childs = [];
        this._zip = 0;
        this._z = 0;
        this.nodeType = 1;
        this._zip = Node._nodeid++;
    }
    get firstChild() {
        return this._childs ? this._childs[0] : null;
    }
    set firstChild(node) {
    }
    get childNodes() {
        return this._childs;
    }
    set childNodes(nodes) {
    }
    insertBefore(newChild, oldChild) {
        this.appendChild(newChild);
    }
    appendChild(newChild) {
        if (newChild == null)
            return null;
        if (newChild == this)
            return;
        if (this._childs.indexOf(newChild) < 0) {
            this._childs.push(newChild);
        }
        newChild.parentNode = this;
        newChild._z = this._z + 1;
        var doc = newChild.ownerDocument || this.ownerDocument;
        if (doc && newChild._z >= doc._topElement._z && newChild instanceof HTMLCanvasElement && !(window.document._topElement instanceof HTMLCanvasElement)) {
            var ele = newChild;
            if (ele.__visible) {
                window.document._topElement = newChild;
            }
        }
        return newChild;
    }
    cloneNode(deep) {
        if (deep) {
            deep = false;
        }
        function clone(obj) {
            if (typeof (obj) != 'object' || obj == null)
                return obj;
            var newObj = Object.create(obj.__proto__);
            for (var i in obj) {
                if (!deep)
                    newObj[i] = obj[i];
                else
                    newObj[i] = clone(obj[i]);
            }
            return newObj;
        }
        return clone(this);
    }
    removeChild(oldChild) {
        var p = this._childs.indexOf(oldChild);
        if (p >= 0) {
            this._childs.splice(p, 1);
            if (window.document._topElement == oldChild)
                window.document._topElement = oldChild.parentElement;
            oldChild.parentNode = null;
            oldChild.parentElement = null;
            return oldChild;
        }
        else
            return null;
    }
    smpJson() {
        var ret = '{';
        var me = this;
        for (var v in me) {
            var val = me[v];
            if (typeof (val) == 'function')
                val = 'Function';
            ret += '   ' + v + '=' + val + '\n';
        }
        ret += '}';
        return ret;
    }
    getAncestorsNode() {
        var ret = new Array();
        var obj = this;
        while (obj.parentNode) {
            ret.push(obj.parentNode);
            obj = obj.parentNode;
        }
        return ret;
    }
}
Node._nodeid = 0;
class NodeSelector {
    querySelector(selectors) {
        return null;
    }
    querySelectorAll(selectors) {
        return [];
    }
}
class ChildNode {
    constructor() { }
    remove() {
    }
}
class ElementTraversal {
    constructor() {
    }
}
class Element extends Node {
    constructor() {
        super();
        this.__visible = true;
    }
    set id(s) {
        this._id = s;
        document.all.push(this);
    }
    get id() {
        return this._id;
    }
    setAttribute(name, value) {
        if (!this._attribs)
            this._attribs = [];
        this._attribs[name] = value;
    }
    getAttribute(name) {
        return this._attribs ? this._attribs[name] : null;
    }
}
applyMixins(Element, [Node, GlobalEventHandlers, ElementTraversal, NodeSelector, ChildNode]);
var ns_Timer;
(function (ns_Timer) {
    class timerobj {
        constructor(curtm, interval, obj, func, args, num) {
            this.del = false;
            this.interval = (interval < 18) ? -1 : interval;
            if (Math.abs(conch.maxInterval - interval) < 1)
                this.interval = -1;
            this.tm = curtm + interval;
            this.obj = obj;
            this.func = func;
            this.args = args;
            this.num = num;
        }
    }
    var timerqueue = function () {
        this.AniFrame = [];
        this.tmq = new Array();
        this.addTimer = function (tm, obj, func, args, num) {
            var tmobj = new timerobj(Date.now(), tm, obj, func, args, num);
            this.tmq.push(tmobj);
            return tmobj;
        };
        this.delTimer = function (obj) {
            for (var i = 0, sz = this.tmq.length; i < sz; i++) {
                if (this.tmq[i] === obj) {
                    this.tmq[i].del = true;
                    break;
                }
            }
        };
        this.update = function () {
            this.curtm = Date.now();
            var i = 0;
            var btmq = [];
            for (var sz = this.tmq.length; i < sz; i++) {
                var ctm = this.tmq[i];
                if (ctm.del)
                    continue;
                var dt = ctm.tm - this.curtm;
                if (dt < 0 || ctm.interval < 0 || Math.abs(conch.maxInterval - ctm.interval) < 1) {
                    if (typeof (ctm.func) === 'function') {
                        ctm.func.apply(null, ctm.obj);
                    }
                    else {
                        eval(ctm.func);
                    }
                    if (ctm.num < 0 || --ctm.num > 0) {
                        if (dt < 0)
                            dt = 0;
                        ctm.tm = dt + this.curtm + ctm.interval;
                        btmq.push(ctm);
                    }
                }
                else {
                    btmq.push(ctm);
                }
            }
            this.tmq.splice(0, sz);
            this.tmq = this.tmq.concat(btmq);
        };
    };
    var gTimerQueue = new timerqueue();
    class _WindowTimersExtension {
        clearImmediate(handle) {
        }
        setImmediate(expression) {
            return 0;
        }
    }
    ns_Timer._WindowTimersExtension = _WindowTimersExtension;
    class _WindowTimers extends _WindowTimersExtension {
        constructor() {
            super();
        }
        clearInterval(handle) {
            gTimerQueue.delTimer(handle);
        }
        clearTimeout(handle) {
            gTimerQueue.delTimer(handle);
        }
        setInterval(handler, timeout, parm) {
            var b = _WindowTimers.ET;
            if (arguments.length > 2) {
                b = Array.prototype.slice.call(arguments);
                b.splice(0, 2);
                handler.arg = b;
            }
            return gTimerQueue.addTimer(timeout, b, handler, null, -1);
        }
        setTimeout(handler, timeout, parm) {
            if (!timeout)
                timeout = 0;
            var b = _WindowTimers.ET;
            if (arguments.length > 2) {
                b = Array.prototype.slice.call(arguments);
                b.splice(0, 2);
                handler.arg = b;
            }
            return gTimerQueue.addTimer(timeout, b, handler, null, 1);
        }
    }
    _WindowTimers.ET = [];
    ns_Timer._WindowTimers = _WindowTimers;
    var animfrm = 0;
    function requestAnimationFrame(callback) {
        var id = animfrm++;
        gTimerQueue.AniFrame.push({ id: id, func: callback });
        if (!gTimerQueue.AniFrameStTm)
            gTimerQueue.AniFrameStTm = Date.now();
        return id;
    }
    ns_Timer.requestAnimationFrame = requestAnimationFrame;
    function cancelAnimationFrame(handle) {
    }
    ns_Timer.cancelAnimationFrame = cancelAnimationFrame;
    var framCount = 0;
    var fpsTimeout = 0;
    conch.setOnFrame(function () {
        if (framCount % 30 == 0) {
            var cur = Date.now();
            var space = (cur - fpsTimeout) / 30;
            fpsTimeout = cur;
            window["conchFps"] && window["conchFps"].render(Math.round(1000 / space));
        }
        framCount++;
        gTimerQueue.update();
        if (document)
            document.onframeend();
    });
    conch.setOnDraw((vsync) => {
        var curAnimFrms = gTimerQueue.AniFrame;
        gTimerQueue.AniFrame = [];
        curAnimFrms.forEach(function (af) {
            if (af.del)
                return;
            af.func(vsync);
        });
    });
})(ns_Timer || (ns_Timer = {}));
var WindowTimers = ns_Timer._WindowTimers;
var requestAnimationFrame = ns_Timer.requestAnimationFrame;
var cancelAnimationFrame = ns_Timer.cancelAnimationFrame;
class Location {
    constructor() {
        this._nativeObj = conch;
        this.setHref = (url) => {
            if (!url || url.length < 8) {
                alert("您的地址不符合要求");
                return;
            }
            var s = new RegExp("(http|file|https)://([^/:]*)(:(\\d+)|)([^?]+)(.*|)", "g");
            var result;
            if ((result = s.exec(url)) != null) {
                this._href = result[0];
                this._hostname = result[2];
                this._host = result[2] + result[3];
                this._pathname = result[5];
                this._port = result[4];
                this._search = result[6];
                this._protocol = result[1] + ":";
                this._origin = this._protocol + "//" + this._host;
                var i = this._pathname ? this._pathname.lastIndexOf("/") : -1;
                if (i != -1) {
                    var temp = this._pathname.substring(0, i);
                    this._fullpath = this._origin + temp;
                }
                else {
                    this._fullpath = this._origin + this._pathname;
                }
            }
            else {
                alert("您的地址不符合要求");
            }
            this.bk_setHref(url);
            window.localStorage.create(this._fullpath + '/');
        };
        this.bk_setHref = this._nativeObj.setHref.bind(this._nativeObj);
    }
    get hostname() {
        return this._hostname;
    }
    get host() {
        return this._host;
    }
    get fullpath() {
        return this._fullpath;
    }
    get pathname() {
        return this._pathname;
    }
    get protocol() {
        return this._protocol;
    }
    get search() {
        return this._search;
    }
    get port() {
        return this._port;
    }
    get origin() {
        return this._origin;
    }
    get href() {
        return this._href;
    }
    set href(url) {
        url = this.normalizeUrl(url);
        var oldhref = this._href;
        if (url != oldhref) {
            this.setHref(url);
            if (this._href != oldhref)
                reloadJS(true);
        }
    }
    setBaseHref(basehref) {
    }
    getBaseHref() {
        return "";
    }
    assign(url) {
    }
    reload(forcedReload) {
        reloadJS(forcedReload);
    }
    replace(url) {
    }
    toString() {
        return this._href;
    }
    normalizeUrl(url) {
        url = url.replace(/\\/g, '/');
        if (url[0] === '/')
            url = 'file://' + url;
        else if (url[1] === ':')
            url = 'file:///' + url;
        return url.trim();
    }
    parseQuery(url) {
        var ret = {};
        var p1 = url.indexOf('?');
        if (p1 < 0)
            return null;
        var q = url.substr(p1 + 1);
        q && q.split('&').forEach((v, i, a) => {
            var kv = v.split('=');
            if (kv.length === 2) {
                ret[kv[0].trim()] = kv[1].trim();
            }
        });
        return ret;
    }
    resolve(fileName) {
        var urlcache = Location.__urlCache__[fileName];
        if (urlcache != null)
            return urlcache;
        if (fileName == null) {
            return "";
        }
        if (fileName.indexOf("//") == 0) {
            return this.protocol + fileName;
        }
        if (fileName.indexOf("file:///") == 0 || fileName.indexOf("http://") == 0 || fileName.indexOf("https://") == 0)
            return fileName;
        if ((fileName.charAt(1) == ':' && fileName.charAt(2) == '/'))
            fileName = "file://" + fileName;
        if (fileName.charAt(0) == "/") {
            return this._origin + fileName;
        }
        var basePath = this._fullpath;
        var urlfull = basePath + "/" + fileName;
        urlcache = Location.__urlCache__[urlfull];
        if (urlcache != null)
            return urlcache;
        if (fileName.indexOf("://") < 0)
            fileName = basePath + "/" + fileName;
        var urls = fileName.split("/");
        urls[1] = "";
        var str, i = 2, size = urls.length;
        while (i < size) {
            str = urls[i];
            if (str == null)
                break;
            if (str == '' || str == '.') {
                urls.splice(i, 1);
                continue;
            }
            if (str == "..") {
                if (i <= 3 && this._protocol != "file:") {
                    urls.splice(i, 1);
                }
                else {
                    urls.splice(i - 1, 2);
                    i -= 1;
                }
                continue;
            }
            i += 1;
        }
        fileName = urls.join("/");
        Location.__urlCache__[fileName] = fileName;
        Location.__urlCache__[urlfull] = fileName;
        return fileName;
    }
}
Location.__urlCache__ = {};
class ConchVirtualBitmap {
}
function applyMixins(derivedCtor, baseCtors) {
    baseCtors.forEach(baseCtor => {
        Object.getOwnPropertyNames(baseCtor.prototype).forEach(name => {
            if (name !== 'constructor') {
                derivedCtor.prototype[name] = baseCtor.prototype[name];
            }
        });
    });
}
class IDBEnvironment {
}
class _Window extends EventTarget {
    constructor() {
        super();
        this.timer = new WindowTimers();
        this.clearInterval = this.timer.clearInterval;
        this.clearTimeout = this.timer.clearTimeout;
        this.setInterval = this.timer.setInterval;
        this.setTimeout = this.timer.setTimeout;
        this._removeEventListener = this.removeEventListener;
        this.removeEventListener = this.removeEventListenernew;
    }
    _addEventListener(type, listener, useCapture) {
        if (type == "devicemotion" || type == "deviceorientation") {
            conch.setSensorAble(true);
        }
        super._addEventListener(type, listener, useCapture);
    }
    removeEventListenernew(type, listener, useCapture) {
        this._removeEventListener(type, listener, useCapture);
        if (type == "devicemotion" || type == "deviceorientation") {
            var de = this._evtMaps.get("devicemotion");
            var deo = this._evtMaps.get("deviceorientation");
            if ((!de || de.length == 0) && (!deo || deo.length == 0)) {
                conch.setSensorAble(false);
            }
        }
    }
}
class XMLHttpRequest extends EventTarget {
    constructor() {
        super();
        this._hasReqHeader = false;
        this.withCredentials = false;
        this.setResponseHeader = function (name, value) {
            this._head = value;
        };
        this.xhr = new _XMLHttpRequest();
        this._readyState = 0;
        this._responseText = this._response = this._responseType = this._url = "";
        this._responseType = "text";
        this._method = "GET";
        this.xhr._t = this;
        this.xhr.set_onreadystatechange(function (r) {
            var _t = this._t;
            if (r == 1) {
                _t._readyState = 1;
            }
            if (_t._onrchgcb) {
                var e = new _lbEvent("readystatechange");
                e.target = _t;
                _t._onrchgcb(e);
            }
            var ev;
            if (_t._status == 200) {
                ev = new _lbEvent("load");
                ev.target = _t;
                _t.dispatchEvent(ev);
            }
            else if (_t._status == 404) {
                ev = new _lbEvent("error");
                ev.target = _t;
                _t.dispatchEvent(ev);
            }
        });
    }
    setRequestHeader(name, value) {
        this.xhr.setRequestHeader(name, value);
        this._hasReqHeader = true;
    }
    getAllResponseHeaders() {
        return this._head;
    }
    get responseText() {
        return this._responseText;
    }
    get response() {
        return this._response;
    }
    get responseType() {
        return this._responseType;
    }
    set responseType(type) {
        this._responseType = type;
        if (type == 'blob') {
            this.xhr.responseTypeCode = 4;
        }
        else if (type == 'arraybuffer') {
            this.xhr.responseTypeCode = 5;
        }
        else {
            this.xhr.responseTypeCode = 1;
        }
    }
    get url() {
        return this._url;
    }
    get async() {
        return this._async;
    }
    get readyState() {
        return this._readyState;
    }
    get status() {
        return this._status;
    }
    _loadsus() {
        var e = new _lbEvent("load");
        e.target = this;
        this._onloadcb(e);
    }
    set onreadystatechange(listen) {
        this._onrchgcb = listen;
        if (listen == null)
            return;
        if (this._readyState != 0) {
            var e = new _lbEvent("readystatechange");
            e.target = this;
            this._onrchgcb(e);
        }
    }
    get onreadystatechange() {
        return this._onrchgcb;
    }
    set onload(listen) {
        this._onloadcb = listen;
        if (listen == null)
            return;
        if (this._readyState == 4 && this._status == 200) {
            this._loadsus();
        }
    }
    get onload() {
        return this._onloadcb;
    }
    getResponseHeader() {
        return this._head;
    }
    open(type, url, async) {
        console.log('xhr.' + type + ' url=' + url);
        if (!url)
            return;
        type = type.toUpperCase();
        async = true;
        url = location.resolve(url);
        this._method = (type === 'POST' ? 'POST' : 'GET');
        this._url = url;
        this._async = (async == null || async == undefined || async == true) ? true : false;
        this.xhr._open(this._method, this._url, this._async);
    }
    overrideMimeType(mime) {
        if (this._responseType == "text" || this._responseText == "")
            this._responseType = "arraybuffer";
        this.xhr.mimeType = "1";
    }
    send(body) {
        if (body) {
            if (body instanceof ArrayBuffer || ArrayBuffer.isView(body) || body instanceof DataView)
                this._responseType = 'arraybuffer';
            else if (body instanceof Object) {
                body = JSON.stringify(body);
            }
        }
        this.xhr._t = this;
        var onPostLoad = function (buf, strbuf) {
            var _t = this._t;
            if (_t.responseType == 'arraybuffer') {
                _t._response = buf;
                _t._responseText = strbuf;
            }
            else {
                _t._response = _t._responseText = buf;
            }
            _t._readyState = 4;
            _t._status = 200;
            _t.xhr._changeState(4);
            if (_t._onloadcb) {
                _t._loadsus();
            }
            onPostLoad.ref = onPostError.ref = null;
        };
        var onPostError = function (e1, e2) {
            var _t = this._t;
            _t._readyState = 4;
            _t._status = 404;
            _t.xhr._changeState(4);
            if (_t.onerror) {
                var ev = new _lbEvent("error");
                ev.target = _t;
                ev['ecode1'] = e1;
                ev['ecode2'] = e2;
                _t.onerror(ev);
            }
            onPostLoad.ref = onPostError.ref = null;
        };
        if (this._method == 'POST' && body) {
            onPostLoad.ref = onPostError.ref = this.xhr;
            this.xhr.setPostCB(onPostLoad, onPostError);
            this.xhr.postData(this.url, body);
        }
        else if (this._hasReqHeader) {
            onPostLoad.ref = onPostError.ref = this.xhr;
            this.xhr.setPostCB(onPostLoad, onPostError);
            this.xhr.getData(this.url);
        }
        else {
            var file = new conch_File(this.url);
            var fileRead = new FileReader();
            fileRead.sync = !this.async;
            if (this._responseType == "text" || this._responseType == "TEXT") {
                fileRead.responseType = 0;
            }
            else if (this._responseType == "arraybuffer") {
                fileRead.responseType = 1;
            }
            else {
                console.log("XMLhttpRequest 暂不支持的类型 responseType=" + this.responseType);
            }
            fileRead._t = this;
            fileRead.onload = function () {
                var _t = this._t;
                if (_t._responseType == "arraybuffer") {
                    _t._response = this.result;
                }
                else {
                    _t._response = _t._responseText = this.result;
                    if (_t._responseType == "json") {
                        _t._response = JSON.parse(this.result);
                    }
                }
                if (_t.xhr.mimeType) {
                    var u8arr = new Uint8Array(_t._response);
                    var strret = "";
                    u8arr.forEach((v, i, arr) => {
                        if (v >= 0x80) {
                            strret += String.fromCharCode(0xf700 | v);
                        }
                        else if (v == 0) {
                            strret += '\0';
                        }
                        else {
                            strret += String.fromCharCode(v);
                        }
                    });
                    _t._responseText = strret;
                }
                _t._readyState = 4;
                _t._status = 200;
                _t.xhr._changeState(4);
                if (_t._onloadcb) {
                    _t._loadsus();
                }
                fileRead.onload = null;
                fileRead.onerror = null;
            };
            fileRead.onerror = function () {
                var _t = this._t;
                _t._readyState = 4;
                _t._status = 404;
                _t.xhr._changeState(4);
                if (_t.onerror) {
                    var ev = new _lbEvent("error");
                    ev.target = _t;
                    _t.onerror(ev);
                }
                fileRead.onload = null;
                fileRead.onerror = null;
            };
            if (this.onerror) {
                fileRead.setIgnoreError(true);
            }
            if (this.responseType == "arraybuffer")
                fileRead.readAsArrayBuffer(file);
            else
                fileRead.readAsText(file);
        }
    }
}
window.XMLHttpRequest = XMLHttpRequest;
class ColorUtil {
    static getColorNum(str) {
        if (!str)
            return 0;
        if (typeof (str) == "number") {
            if (str >= 0xFFFFFF)
                return str;
            return str | 0xFF000000;
        }
        var len;
        if (str[0] == "#") {
            var r = ColorUtil.getRGBByRGBStr(str);
            if (r) {
                return parseInt(r, 16);
            }
            else
                return NaN;
        }
        else if (str.indexOf("rgb") != -1) {
            return ColorUtil.getColNum(str, true);
        }
        else if (str.indexOf("hsl") != -1) {
            return ColorUtil.getColNum(str, false);
        }
        else {
            var c = ColorUtil._COLOR_MAP[str];
            return c ? ColorUtil.getColorNum(c) : NaN;
        }
    }
    static getColNum(c, isRgb) {
        var temp, i, s, result = [], st;
        var t = isRgb ? ColorUtil.RGBReg.exec(c) : ColorUtil.HSLReg.exec(c);
        if (t) {
            var r = t[2].split(",");
            i = 0;
            s = r.length;
            while (i < s) {
                result[i] = parseFloat(r[i]);
                i++;
            }
            var ta = result;
            if (!isRgb) {
                ta = ColorUtil.hsl2rgb(result[0] / 360, result[1] / 100, result[2] / 100);
            }
            if (i == 3 && !t[1]) {
                return 0xff000000 + (ta[0] << 16) + (ta[1] << 8) + ta[2];
            }
            else if (i == 4 && t[1]) {
                return parseInt(Math.floor(0xff * result[3]).toString(16) + "000000", 16) + (ta[0] << 16) + (ta[1] << 8) + ta[2];
            }
            return NaN;
        }
        else
            return NaN;
    }
    static getRGBByRGBStr(str) {
        str = str.substr(1);
        var i, len = str.length, result;
        switch (len) {
            case 3:
                result = "ff";
                for (i = 0; i < len; i++) {
                    result += str[i] + str[i];
                }
                break;
            case 6:
                result = "ff" + str;
            default:
                break;
        }
        return result;
    }
    static toHexColor(color) {
        if (color < 0 || isNaN(color))
            return null;
        if (color > 0xff000000) {
            color -= 0xff000000;
        }
        else {
            return "rgba(" + (color >> 16 & 0xff) + "," + (color >> 8 & 0xff) + "," + (color & 0xff) + "," + ((color >> 24 & 0xff) / 0xff) + ")";
        }
        var str = color.toString(16);
        while (str.length < 6)
            str = "0" + str;
        return "#" + str;
    }
    static hue2rgb(p, q, t) {
        if (t < 0)
            t += 1;
        if (t > 1)
            t -= 1;
        if (t < 1 / 6)
            return p + (q - p) * 6 * t;
        if (t < 1 / 2)
            return q;
        if (t < 2 / 3)
            return p + (q - p) * (2 / 3 - t) * 6;
        return p;
    }
    static hsl2rgb(h, s, l) {
        var r, g, b;
        if (s == 0) {
            r = g = b = l;
        }
        else {
            var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            var p = 2 * l - q;
            r = ColorUtil.hue2rgb(p, q, h + 1 / 3);
            g = ColorUtil.hue2rgb(p, q, h);
            b = ColorUtil.hue2rgb(p, q, h - 1 / 3);
        }
        return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
    }
}
ColorUtil._COLOR_MAP = { "white": '#FFFFFF', "red": '#FF0000', "green": '#00FF00', "blue": '#0000FF', "black": '#000000', "yellow": '#FFFF00', 'gray': '#AAAAAA' };
ColorUtil.RGBReg = new RegExp("rgb([a]{0,1})\\s*[(]([\\d,.\\s-]*)?[)]");
ColorUtil.HSLReg = new RegExp("hsl([a]{0,1})\\s*[(]([\\d,.\\s-%]*)?[)]");
class ContextDataInfo {
    constructor() {
    }
    ru16(i) {
        return this.buffer.getUint16(this.curpos + i, true);
    }
    wu16(i, v) {
        this.buffer.setUint16(this.curpos + i, v, true);
    }
    ru32(i) {
        return this.buffer.getUint32(this.curpos + i, true);
    }
    wu32(i, v) {
        this.buffer.setUint32(this.curpos + i, v, true);
    }
    rf32(i) {
        return this.buffer.getFloat32(this.curpos + i, true);
    }
    wf32(i, v) {
        this.buffer.setFloat32(this.curpos + i, v, true);
    }
    get fillStyle() {
        return this.ru32(0);
    }
    set fillStyle(t) {
        this.wu32(0, t);
    }
    get storkeStyle() {
        return this.ru32(4);
    }
    set storkeStyle(t) {
        this.wu32(4, t);
    }
    get lineWidth() {
        return this.rf32(8);
    }
    set lineWidth(t) {
        this.wf32(8, t);
    }
    get textBaseLineType() {
        return this.ru16(12);
    }
    set textBaseLineType(t) {
        this.wu16(12, t);
    }
    get textAlignType() {
        return this.ru16(14);
    }
    set textAlignType(t) {
        this.wu16(14, t);
    }
    get blendType() {
        return this.ru16(16);
    }
    set blendType(t) {
        this.wu16(16, t);
    }
    get repetition() {
        return this.ru16(18);
    }
    set repetition(v) {
        this.wu16(18, v);
    }
    get shadowColor() {
        return this.ru32(20);
    }
    set shadowColor(t) {
        this.wu32(20, t);
    }
    get shadowBlur() {
        return this.rf32(24);
    }
    set shadowBlur(t) {
        this.wf32(24, t);
    }
    get shadowOffsetX() {
        return this.rf32(28);
    }
    set shadowOffsetX(t) {
        this.wf32(28, t);
    }
    get shadowOffsetY() {
        return this.rf32(32);
    }
    set shadowOffsetY(t) {
        this.wf32(32, t);
    }
    get lineCap() {
        return this.ru16(36);
    }
    set lineCap(t) {
        this.wu16(36, t);
    }
    get lineJoin() {
        return this.ru16(38);
    }
    set lineJoin(t) {
        this.wu16(38, t);
    }
    get miterLimit() {
        return this.rf32(40);
    }
    set miterLimit(t) {
        this.wf32(40, t);
    }
    get alpha() {
        return this.rf32(44);
    }
    set alpha(t) {
        this.wf32(44, t);
    }
    get font() {
        return _Buffer.readString(this.buffer, this.curpos + 48, 64);
    }
    set font(f) {
        if (!f)
            f = "";
        var pos = 48;
        var ab = conch.strTobufer(f);
        var size = ab.byteLength;
        if (size + 2 > 64) {
            console.error("font不能超过64字符");
            return;
        }
        this.wu16(pos, size);
        pos += 2;
        var uint8array = new Uint8Array(ab, 0, size);
        this.arrayBuffer.set(uint8array, this.curpos + pos);
        pos += size;
        for (var i = size + 2; i < 64; ++i) {
            this.buffer.setUint8(pos, 0);
            pos++;
        }
    }
    get fillType() {
        return this.ru16(112);
    }
    set fillType(t) {
        this.wu16(112, t);
    }
    get storkeType() {
        return this.ru16(114);
    }
    set storkeType(t) {
        this.ru16(114);
    }
    get x() {
        return this.ru32(116);
    }
    set x(t) {
        this.wu32(116, t);
    }
    get y() {
        return this.ru32(120);
    }
    set y(t) {
        this.wu32(120, t);
    }
    get w() {
        return this.ru32(124);
    }
    set w(t) {
        this.wu32(124, t);
    }
    get h() {
        return this.ru32(128);
    }
    set h(t) {
        this.wu32(128, t);
    }
    init() {
        this.fillStyle = 0xffffffff;
        this.lineWidth = 1;
        this.alpha = 1;
    }
}
class ContextSaveData {
    constructor() {
        this.curData = new ContextDataInfo();
        this.sizeofData = 132;
        this.datasize = 20 * this.sizeofData;
        this.curData.buffer = new DataView(new ArrayBuffer(this.datasize));
        this.curData.arrayBuffer = new Uint8Array(this.curData.buffer.buffer);
        this.curData.curpos = 0;
        this.curData.init();
    }
    save() {
        this.curData.curpos += this.sizeofData;
        if (this.curData.curpos >= this.datasize) {
            var enlargeSize = 20 * this.sizeofData;
            var dv = new DataView(new ArrayBuffer(this.datasize + enlargeSize));
            var ab = new Uint8Array(dv.buffer);
            ab.set(this.curData.arrayBuffer, 0);
            this.datasize += enlargeSize;
            this.curData.buffer = dv;
            this.curData.arrayBuffer = ab;
        }
        var temp = new Uint8Array(this.curData.buffer.buffer, this.curData.curpos - this.sizeofData, this.sizeofData);
        this.curData.arrayBuffer.set(temp, this.curData.curpos);
        return true;
    }
    restore() {
        if (this.curData.curpos > 0) {
            this.curData.curpos -= this.sizeofData;
            return true;
        }
        else {
            throw "save and restore don't make a pair.";
        }
    }
}
class CanvasPattern {
    constructor(img, r, x, y, w, h) {
        if (img !== undefined) {
            this.image = img;
            this.x = x;
            this.y = y;
            this.w = w;
            this.h = h;
            this.repetition = r;
        }
    }
}
class CanvasGradient {
    addColorStop(fs, color) {
    }
}
class CanvasRenderingContext2D {
    constructor() {
        this._nativeObj = new context();
        this._saveData = new ContextSaveData();
        this.id = this._nativeObj.conchID;
    }
    setSize(w, h) {
        if (!isNaN(w) && !isNaN(h)) {
            this._nativeObj.setSize(w, h);
        }
    }
    setCanvasType(type) {
        var _ = conch.bf;
        _.needsz(16);
        _.wu32(0);
        _.wu32(9);
        _.wu32(this.id);
        _.wu32(type);
    }
    setCanvasClip(b) {
        var _ = conch.bf;
        _.needsz(16);
        _.wu32(0);
        _.wu32(17);
        _.wu32(this.id);
        _.wu32(b ? 1 : 0);
    }
    save() {
        _Buffer.noParm(this, 0);
        this._saveData.save();
    }
    restore() {
        _Buffer.noParm(this, 1);
        this._saveData.restore();
    }
    beginPath() {
        _Buffer.noParm(this, 2);
    }
    drawImage(image, offsetX, offsetY, width, height, canvasOffsetX, canvasOffsetY, canvasImageWidth, canvasImageHeight) {
        var n = arguments.length, _ = conch.bf;
        if (image instanceof HTMLImageElement) {
            switch (n) {
                case 3:
                    _.needsz(20);
                    _.wu32(this.id);
                    _.wu32(3);
                    break;
                case 5:
                    _.needsz(28);
                    _.wu32(this.id);
                    _.wu32(4);
                    break;
                case 9:
                    _.needsz(44);
                    _.wu32(this.id);
                    _.wu32(5);
                default:
                    break;
            }
            _.wu32(image._nativeObj.conchImgId);
        }
        else if (image instanceof HTMLCanvasElement) {
            switch (n) {
                case 3:
                    _.needsz(20);
                    _.wu32(this.id);
                    _.wu32(35);
                    break;
                case 5:
                    _.needsz(28);
                    _.wu32(this.id);
                    _.wu32(36);
                    break;
                case 9:
                    _.needsz(44);
                    _.wu32(this.id);
                    _.wu32(37);
                default:
                    break;
            }
            _.wu32(image._ctx2d._nativeObj.conchID);
        }
        else {
            throw "不支持改对象的drawImage";
        }
        _.wf32(offsetX);
        _.wf32(offsetY);
        if (n > 3) {
            _.wf32(width);
            _.wf32(height);
        }
        if (n > 5) {
            _.wf32(canvasOffsetX);
            _.wf32(canvasOffsetY);
            _.wf32(canvasImageWidth);
            _.wf32(canvasImageHeight);
        }
    }
    rect(x, y, w, h) {
        _Buffer.rect(this, 6, x, y, w, h);
    }
    clip(fillRule) {
        _Buffer.noParm(this, 7);
    }
    scale(x, y) {
        _Buffer.point(this, 8, x, y);
    }
    rotate(angle) {
        _Buffer.oneNum(this, 9, angle);
    }
    translate(x, y) {
        _Buffer.point(this, 10, x, y);
    }
    transform(m11, m12, m21, m22, dx, dy) {
        _Buffer.setMatrix(this, 11, m11, m12, m21, m22, dx, dy);
    }
    setTransform(m11, m12, m21, m22, dx, dy) {
        _Buffer.setMatrix(this, 12, m11, m12, m21, m22, dx, dy);
    }
    set globalAlpha(v) {
        _Buffer.oneNum(this, 13, v);
        this._saveData.curData.alpha = v;
    }
    get globalAlpha() { return this._saveData.curData.alpha; }
    clearRect(x, y, w, h) {
        _Buffer.rect(this, 14, x, y, w, h);
    }
    closePath() {
        _Buffer.noParm(this, 15);
    }
    set fillStyle(fs) {
        var _ = conch.bf, s = this._saveData.curData;
        ;
        if (typeof (fs) == "string") {
            var d = ColorUtil.getColorNum(fs);
            if (isNaN(d)) {
                console.error("fillStyle 不支持color:" + fs);
                d = 0xFFFFFFFF;
            }
            _Buffer.oneInt(this, 16, d);
            s.fillType = 0;
            s.fillStyle = d;
        }
        else if (fs instanceof CanvasPattern) {
            var imgid = fs.image, x = fs.x, y = fs.y, w = fs.w, h = fs.h, rt = fs.repetition;
            _.needsz(32);
            _.wu32(this.id);
            _.wu32(47);
            _.wu32(imgid);
            _.wu32(rt);
            _.wf32Rect(x, y, w, h);
            s.fillType = 1;
            s.fillStyle = imgid;
            s.repetition = rt;
            s.x = x;
            s.y = y;
            s.w = w;
            s.h = h;
        }
        else
            console.error("fillStyle 不支持此方式:" + fs);
    }
    get fillStyle() {
        var s = this._saveData.curData, t = s.fillType;
        if (t == 0)
            return ColorUtil.toHexColor(s.fillStyle);
        else {
            return new CanvasPattern(s.fillStyle, s.repetition, s.x, s.y, s.w, s.h);
        }
    }
    fillRect(x, y, w, h) {
        _Buffer.rect(this, 17, x, y, w, h);
    }
    set strokeStyle(v) {
        if (typeof (v) == "string") {
            var d = ColorUtil.getColorNum(v);
            if (isNaN(d)) {
                console.error("strokeStyle 不支持color:" + v);
                d = 0xFFFFFFFF;
            }
            _Buffer.oneInt(this, 18, d);
            this._saveData.curData.storkeStyle = d;
        }
        else {
            throw ("strokeStyle Not Support yet");
        }
    }
    get strokeStyle() {
        var d = this._saveData.curData.storkeStyle;
        return ColorUtil.toHexColor(d);
    }
    stroke() {
        _Buffer.noParm(this, 19);
    }
    strokeRect(x, y, w, h) {
        _Buffer.rect(this, 20, x, y, w, h);
    }
    _drawText(fid, text, x, y, maxWidth) {
        if (!text)
            return;
        var _ = conch.bf, j = (maxWidth === undefined) ? 0 : 1;
        _.needsz(20 + j * 4);
        _.wu32(this.id);
        _.wu32(fid + j);
        _.wf32(x);
        _.wf32(y);
        j && _.wu32(maxWidth);
        _.wstring(text, true);
    }
    strokeText(text, x, y, maxWidth) {
        this._drawText(21, text, x, y, maxWidth);
    }
    fillText(text, x, y, maxWidth) {
        this._drawText(23, text, x, y, maxWidth);
    }
    fill(fillRule) {
        _Buffer.noParm(this, 25);
    }
    set lineCap(v) {
        var d = CanvasRenderingContext2D.LINECAP[v] || 0;
        _Buffer.oneInt(this, 26, d);
        this._saveData.curData.lineCap = d;
    }
    get lineCap() {
        return CanvasRenderingContext2D.LINECAP[this._saveData.curData.lineCap] || "butt";
    }
    set lineWidth(v) {
        _Buffer.oneNum(this, 27, v);
        this._saveData.curData.lineWidth = v;
    }
    get lineWidth() {
        return this._saveData.curData.lineWidth;
    }
    set lineJoin(v) {
        var d = CanvasRenderingContext2D.LINEJOIN[v] || 0;
        _Buffer.oneInt(this, 28, d);
        this._saveData.curData.lineJoin = d;
    }
    get lineJoin() {
        return CanvasRenderingContext2D.LINEJOIN[this._saveData.curData.lineJoin] || "miter";
    }
    moveTo(x, y) {
        _Buffer.point(this, 29, x, y);
    }
    lineTo(x, y) {
        _Buffer.point(this, 30, x, y);
    }
    arc(x, y, radius, startAngle, endAngle, anticlockwise) {
        var _ = conch.bf;
        _.needsz(32);
        _.wu32(this.id);
        _.wu32(31);
        _.wf32(x);
        _.wf32(y);
        _.wf32(radius);
        _.wf32(startAngle);
        _.wf32(endAngle);
        _.wu32(anticlockwise ? 1 : 0);
    }
    arcTo(x1, y1, x2, y2, radius) {
        var _ = conch.bf;
        _.needsz(28);
        _.wu32(this.id);
        _.wu32(32);
        _.wf32(x1);
        _.wf32(y1);
        _.wf32(x2);
        _.wf32(y2);
        _.wf32(radius);
    }
    bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y) {
        _Buffer.setMatrix(this, 33, cp1x, cp1y, cp2x, cp2y, x, y);
    }
    quadraticCurveTo(cpx, cpy, x, y) {
        _Buffer.rect(this, 34, cpx, cpy, x, y);
    }
    set font(f) {
        var _ = conch.bf;
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(38);
        _.wstring(f, true);
        this._saveData.curData.font = f;
    }
    get font() {
        return this._saveData.curData.font;
    }
    set globalCompositeOperation(v) {
        var b = CanvasRenderingContext2D.BLEND[v] || 0;
        _Buffer.oneInt(this, 39, b);
        this._saveData.curData.blendType = b;
    }
    get globalCompositeOperation() {
        return CanvasRenderingContext2D.BLEND[this._saveData.curData.blendType] || "source-over";
    }
    set textBaseline(v) {
        var d = CanvasRenderingContext2D.TEXTBASELINE[v] || 0;
        _Buffer.oneInt(this, 40, d);
        this._saveData.curData.textBaseLineType = d;
    }
    get textBaseline() {
        return CanvasRenderingContext2D.TEXTBASELINE[this._saveData.curData.textBaseLineType] || "alphabetic";
    }
    set textAlign(v) {
        var d = CanvasRenderingContext2D.TEXTALIGN[v] || 0;
        _Buffer.oneInt(this, 41, d);
        this._saveData.curData.textAlignType = d;
    }
    get textAlign() {
        return CanvasRenderingContext2D.TEXTALIGN[this._saveData.curData.textAlignType] || "start";
    }
    set miterLimit(v) {
        _Buffer.oneNum(this, 42, v);
        this._saveData.curData.miterLimit = v;
    }
    get miterLimit() {
        return this._saveData.curData.miterLimit;
    }
    set shadowOffsetX(v) {
        _Buffer.oneNum(this, 43, v);
        this._saveData.curData.shadowOffsetX = v;
    }
    get shadowOffsetX() {
        return this._saveData.curData.shadowOffsetX;
    }
    set shadowOffsetY(v) {
        _Buffer.oneNum(this, 44, v);
        this._saveData.curData.shadowOffsetY = v;
    }
    get shadowOffsetY() {
        return this._saveData.curData.shadowOffsetY;
    }
    set shadowColor(v) {
        var n = ColorUtil.getColorNum(v);
        if (isNaN(n)) {
            console.error("shadowColor 不支持color:" + v);
            n = 0xFFFFFFFF;
        }
        _Buffer.oneInt(this, 45, n);
        this._saveData.curData.shadowColor = n;
    }
    get shadowColor() {
        return ColorUtil.toHexColor(this._saveData.curData.shadowColor);
    }
    set shadowBlur(v) {
        _Buffer.oneNum(this, 46, v);
        this._saveData.curData.shadowBlur = v;
    }
    get shadowBlur() {
        return this._saveData.curData.shadowBlur;
    }
    setFilter(r, g, b, gray) {
        _Buffer.rect(this, 48, r, g, b, gray);
    }
    fillBorderText(text, x, y, font, fillColor, borderColor, lineWidth, textAlign) {
        var _ = conch.bf;
        var fc = ColorUtil.getColorNum(fillColor);
        var bc = ColorUtil.getColorNum(borderColor);
        var ta = CanvasRenderingContext2D.TEXTALIGN[textAlign] || 0;
        if (isNaN(fc)) {
            console.error("fillBorderText fillcolor或者borderColor不符合规范" + fillColor + "," + borderColor);
            fc = 0xFFFFFFFF;
        }
        if (isNaN(bc)) {
            console.error("fillBorderText fillcolor或者borderColor不符合规范" + fillColor + "," + borderColor);
            bc = 0xFFFFFFFF;
        }
        var cur = this._saveData.curData;
        _.needsz(36);
        _.wu32(this.id);
        _.wu32(49);
        _.wf32(x);
        _.wf32(y);
        _.wf32(lineWidth);
        _.wu32(fc);
        _.wu32(bc);
        _.wu32(ta);
        _.wstring(font, true);
        _.needsz(4);
        _.wstring(text, true);
        cur.lineWidth = lineWidth;
        cur.font = font;
        cur.fillStyle = fc;
        cur.textAlignType = ta;
        cur.textBaseLineType = 2;
    }
    clipRect(x, y, w, h) {
        _Buffer.rect(this, 50, x, y, w, h);
    }
    createLinearGradient(x0, y0, x1, y1) {
        return null;
    }
    clear() {
    }
    setSkinMesh(x, y, ib, vb, eleNum, iStart, texture, mat) {
        var img = texture.source;
        var _ = conch.bf;
        if (ib.conch_id === undefined || !ib._upload) {
            ib.conch_id = CanvasRenderingContext2D.ID++;
            ib.conchRef = new ArrayBufferRef();
            ib.conchRef.id = ib.conch_id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(11);
            _.wu32(ib.conch_id);
            _.wu32(ib.length * 2);
            ib.buffer = new Uint16Array(ib);
            _.wab(ib, ib.length * 2);
            ib._upload = true;
        }
        if (vb.conch_id === undefined || !vb._upload) {
            vb.conch_id = CanvasRenderingContext2D.ID++;
            vb.conchRef = new ArrayBufferRef();
            vb.conchRef.id = vb.conch_id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(11);
            _.wu32(vb.conch_id);
            _.wu32(vb.byteLength);
            _.wab(vb, vb.byteLength);
            vb._upload = true;
        }
        var shader = CanvasRenderingContext2D.skinMeshShader;
        if (shader.conch_id === undefined) {
            _.needsz(16);
            _.wu32(0);
            _.wu32(12);
            _.wu32(shader.id);
            _.wstring(shader.vs, true);
            _.needsz(4);
            _.wstring(shader.ps, true);
            shader.conch_id = shader.id;
        }
        _.needsz(68);
        _.wu32(this.id);
        _.wu32(53);
        _.wu32(ib.conch_id);
        _.wu32(vb.conch_id);
        _.wu32(1);
        _.wu32(eleNum);
        _.wu32(iStart);
        _.wu32(img.imgId);
        _.wu32(shader.id);
        var uv = texture.uv;
        _.wf32(mat.a);
        _.wf32(mat.b);
        _.wf32(mat.c);
        _.wf32(mat.d);
        _.wf32(mat.tx);
        _.wf32(mat.ty);
        _.wf32(x);
        _.wf32(y);
    }
    drawParticle(x, y, par) {
        if (par.texture == null || par.texture.source == null)
            return;
        var _ = conch.bf;
        _.needsz(20);
        _.wu32(this.id);
        _.wu32(54);
        _.wu32(par.id);
        _.wf32(x);
        _.wf32(y);
    }
    _setIBVB(x, y, ib, vb, numElement, mat, shader, shaderValues, startIndex, offset) {
        var vertType = vb._vertType ? vb._vertType : 1;
        var _ = conch.bf;
        var imgid = -1;
        for (var i = 0; i < 2; i++) {
            var bf = (i == 0 ? ib : vb);
            if (bf.conch_id === undefined || bf._upload) {
                bf.conch_id = bf.id;
                bf.conchRef = new ArrayBufferRef();
                bf.conchRef.id = bf.conch_id;
                _.needsz(16);
                _.wu32(0);
                _.wu32(11);
                _.wu32(bf.conch_id);
                _.wu32(bf.bufferLength);
                _.wab(bf._buffer, bf.bufferLength);
                bf._upload = false;
            }
        }
        if (shaderValues.textureHost) {
            var img = shaderValues.textureHost.source;
            if (img) {
                imgid = img.imgId;
            }
        }
        if (imgid == -1)
            return;
        if (shader.conch_id === undefined) {
            _.needsz(16);
            _.wu32(0);
            _.wu32(12);
            _.wu32(shader.id);
            _.wstring(shader._vs, true);
            _.needsz(4);
            _.wstring(shader._ps, true);
            shader.conch_id = shader.id;
        }
        _.needsz(72);
        _.wu32(this.id);
        _.wu32(51);
        _.wu32(ib.id);
        _.wu32(vb.id);
        _.wu32(vertType);
        _.wu32(shader.id);
        _.wu32(imgid);
        _.wu32(startIndex);
        _.wu32(offset);
        _.wu32(numElement);
        _.wf32(x);
        _.wf32(y);
        if (mat) {
            _.wf32(mat.a);
            _.wf32(mat.b);
            _.wf32(mat.c);
            _.wf32(mat.d);
            _.wf32(mat.tx);
            _.wf32(mat.ty);
        }
        else {
            _.wf32(1.0);
            _.wf32(0.0);
            _.wf32(0.0);
            _.wf32(1.0);
            _.wf32(0.0);
            _.wf32(0.0);
        }
    }
    setFilterMatrix(mat, alpha) {
        if (!mat || !alpha) {
            return;
        }
        var _ = conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(52);
        _.wab(mat, 64);
        _.wab(alpha, 16);
    }
    toBase64(type, encoderOptions, callback) {
        this._nativeObj.setToBase64Callback(callback);
        var _ = conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(56);
        _.wf32(encoderOptions);
        _.wstring(type, true);
    }
    getPixels(x, y, w, h, callback) {
        this._nativeObj.setGetPixelsCallback(callback);
        var _ = conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(55);
        _.wf32(x);
        _.wf32(y);
        _.wf32(w);
        _.wf32(h);
    }
    isPointInPath(x, y, fillRule) {
        return true;
    }
    measureText(text) {
        return this._nativeObj.measureText(this._saveData.curData.font, text);
    }
    disableClip(b) {
    }
    createPattern(image, repetition, x, y, w, h) {
        var r = new CanvasPattern();
        r.image = image._nativeObj.conchImgId;
        r.repetition = CanvasRenderingContext2D.PATTERN[repetition] || 0;
        if (x !== undefined) {
            r.x = x;
            r.y = y;
            r.w = w;
            r.h = h;
        }
        else {
            r.x = r.y = 0;
            r.w = image.width;
            r.h = image.height;
        }
        return r;
    }
    set lineDashOffset(v) {
    }
    get lineDashOffset() {
        return 0;
    }
    createImageData(imageDataOrSw, sh) {
        return null;
    }
    createRadialGradient(x0, y0, r0, x1, y1, r1) {
        return null;
    }
    getImageData(sx, sy, sw, sh) {
        return null;
    }
    getLineDash() {
        return null;
    }
    putImageData(imagedata, dx, dy, dirtyX, dirtyY, dirtyWidth, dirtyHeight) {
    }
    setLineDash(segments) {
    }
}
CanvasRenderingContext2D.LINECAP = ["butt", "round", "square"];
CanvasRenderingContext2D.TEXTALIGN = ["start", "left", "center", "end", "right"];
CanvasRenderingContext2D.TEXTBASELINE = ["alphabetic", "top", "hanging", "middle", "ideographic", "bottom"];
CanvasRenderingContext2D.LINEJOIN = ["miter", "round", "bevel"];
CanvasRenderingContext2D.BLEND = ["source-over", "source-atop", "source-in", "source-out", "destination-over", "destination-atop", "destination-in", "destination-out", "lighter", "copy", 'xor',
    'normal', 'add', 'multiply', 'screen', 'overlay', 'light', 'mask'];
CanvasRenderingContext2D.DRAWIMAGES = new Float32Array(0);
CanvasRenderingContext2D.skinMeshShader = {
    conch_id: undefined,
    id: 1025,
    vs: `attribute vec2 position;
                        attribute vec2 texcoord;
                        attribute vec4 color;
                        uniform vec4 g_DevInfo;
                        uniform mat4 mmat;
                        varying vec2 v_texcoord;
                        varying vec4 v_color;
                        void main() {
                        vec4 pos=mmat*vec4(position.x,position.y,0,1 );
                        gl_Position = vec4((pos.x/g_DevInfo.x-0.5)*2.0,(0.5-pos.y/g_DevInfo.y)*2.0,pos.z,1.0);
                        v_color = color;
                        v_color.rgb *= v_color.a;
                        v_texcoord = texcoord;  
                        }`,
    ps: `precision mediump float;
            varying vec2 v_texcoord;
            varying vec4 v_color;
            uniform sampler2D texture;
            uniform float g_Alpha;
            void main() {
                vec4 t_color = texture2D(texture, v_texcoord);
                gl_FragColor = t_color * v_color;
				gl_FragColor *= g_Alpha;
            }`
};
CanvasRenderingContext2D.particleShader = {
    conch_id: undefined,
    id: 1024,
    ps: `precision mediump float;
        varying vec4 v_Color;
        varying vec2 v_TextureCoordinate;
        uniform sampler2D texture;

        void main()
        {	
            gl_FragColor=texture2D(texture,v_TextureCoordinate)*v_Color;
            gl_FragColor.rgb*=v_Color.a;
        }
        `,
    vs: `
        attribute vec4 a_CornerTextureCoordinate;
attribute vec3 a_Position;
attribute vec3 a_Velocity;
attribute vec4 a_StartColor;
attribute vec4 a_EndColor;
attribute vec3 a_SizeRotation;
attribute vec2 a_Radius;
attribute vec4 a_Radian;
attribute float a_AgeAddScale;
attribute float a_Time;

varying vec4 v_Color;
varying vec2 v_TextureCoordinate;

uniform  float u_CurrentTime;
uniform float u_Duration;
uniform float u_EndVelocity;
uniform vec3 u_Gravity;

 uniform vec2 size;
 uniform mat4 mmat;

vec4 ComputeParticlePosition(in vec3 position, in vec3 velocity,in float age,in float normalizedAge)
{

   float startVelocity = length(velocity);
   float endVelocity = startVelocity * u_EndVelocity;

   float velocityIntegral = startVelocity * normalizedAge +(endVelocity - startVelocity) * normalizedAge *normalizedAge/2.0;
   
   vec3 addPosition = normalize(velocity) * velocityIntegral * u_Duration; 
   addPosition += u_Gravity * age * normalizedAge;
   
   float radius=mix(a_Radius.x, a_Radius.y, normalizedAge);
   float radianHorizontal =mix(a_Radian.x,a_Radian.z,normalizedAge);
   float radianVertical =mix(a_Radian.y,a_Radian.w,normalizedAge);
   
   float r =cos(radianVertical)* radius;
   addPosition.y += sin(radianVertical) * radius;
	
   addPosition.x += cos(radianHorizontal) *r;
   addPosition.z += sin(radianHorizontal) *r;
  
   addPosition.y=-addPosition.y;
   position+=addPosition;
    return  vec4(position,1.0);
}

float ComputeParticleSize(in float startSize,in float endSize, in float normalizedAge)
{    
    float size = mix(startSize, endSize, normalizedAge);
	 return size;
}

mat2 ComputeParticleRotation(in float rot,in float age)
{    
    float rotation =rot * age;
    float c = cos(rotation);
    float s = sin(rotation);
    return mat2(c, -s, s, c);
}

vec4 ComputeParticleColor(in vec4 startColor,in vec4 endColor,in float normalizedAge)
{
	vec4 color=mix(startColor,endColor,normalizedAge);
    color.a *= normalizedAge * (1.0-normalizedAge) * (1.0-normalizedAge) * 6.7;
   
    return color;
}

void main()
{
   float age = u_CurrentTime - a_Time;
   age *= 1.0 + a_AgeAddScale;
   float normalizedAge = clamp(age / u_Duration,0.0,1.0);
   gl_Position = ComputeParticlePosition(a_Position, a_Velocity, age, normalizedAge);
   float pSize = ComputeParticleSize(a_SizeRotation.x,a_SizeRotation.y, normalizedAge);
   mat2 rotation = ComputeParticleRotation(a_SizeRotation.z, age);
   mat4 mat=mmat;
   gl_Position=vec4((mat*gl_Position).xy,0.0,1.0);
   gl_Position.xy += (rotation*a_CornerTextureCoordinate.xy) * pSize*vec2(mat[0][0],mat[1][1]);
   gl_Position=vec4((gl_Position.x/size.x-0.5)*2.0,(0.5-gl_Position.y/size.y)*2.0,0.0,1.0);
   v_Color = ComputeParticleColor(a_StartColor,a_EndColor, normalizedAge);
   v_TextureCoordinate =a_CornerTextureCoordinate.zw;
}


        `
};
CanvasRenderingContext2D.PATTERN = ["repeat", "repeat-x", "repeat-y", "no-repeat"];
CanvasRenderingContext2D.ID = 0;
function _init(v) {
    for (var i = 0, sz = v.length; i < sz; i++) {
        v[v[i]] = i;
    }
}
_init(CanvasRenderingContext2D.LINECAP);
_init(CanvasRenderingContext2D.TEXTALIGN);
_init(CanvasRenderingContext2D.TEXTBASELINE);
_init(CanvasRenderingContext2D.LINEJOIN);
_init(CanvasRenderingContext2D.BLEND);
_init(CanvasRenderingContext2D.PATTERN);
window["CanvasRenderingContext2D"] = CanvasRenderingContext2D;
class ProgramLocationTable {
    constructor() {
        this.FADELOC_BASE = 50000;
        this._obj = {};
    }
    getFadeLocation(fadeProgramID, name) {
        var key = fadeProgramID + "-" + name;
        var fadeID = this._obj[key];
        if (fadeID == undefined) {
            fadeID = this.FADELOC_BASE;
            this.FADELOC_BASE++;
            this._obj[key] = fadeID;
        }
        return fadeID;
    }
}
class WebGLRenderingContext {
    constructor() {
        this.drawingBufferWidth = getInnerWidth();
        this.drawingBufferHeight = getInnerHeight();
        this.currentTextureID = 0;
        this.viewportx = 0;
        this.viewporty = 0;
        this.viewportw = 0;
        this.viewporth = 0;
        this._i = 0;
        this._fadeArray = [];
        this.curFBO = 0;
        this.width = 0;
        this.height = 0;
        this._locTable = new ProgramLocationTable();
        this.DEPTH_BUFFER_BIT = 0x00000100;
        this.STENCIL_BUFFER_BIT = 0x00000400;
        this.COLOR_BUFFER_BIT = 0x00004000;
        this.POINTS = 0x0000;
        this.LINES = 0x0001;
        this.LINE_LOOP = 0x0002;
        this.LINE_STRIP = 0x0003;
        this.TRIANGLES = 0x0004;
        this.TRIANGLE_STRIP = 0x0005;
        this.TRIANGLE_FAN = 0x0006;
        this.ZERO = 0;
        this.ONE = 1;
        this.SRC_COLOR = 0x0300;
        this.ONE_MINUS_SRC_COLOR = 0x0301;
        this.SRC_ALPHA = 0x0302;
        this.ONE_MINUS_SRC_ALPHA = 0x0303;
        this.DST_ALPHA = 0x0304;
        this.ONE_MINUS_DST_ALPHA = 0x0305;
        this.DST_COLOR = 0x0306;
        this.ONE_MINUS_DST_COLOR = 0x0307;
        this.SRC_ALPHA_SATURATE = 0x0308;
        this.FUNC_ADD = 0x8006;
        this.BLEND_EQUATION = 0x8009;
        this.BLEND_EQUATION_RGB = 0x8009;
        this.BLEND_EQUATION_ALPHA = 0x883D;
        this.FUNC_SUBTRACT = 0x800A;
        this.FUNC_REVERSE_SUBTRACT = 0x800B;
        this.BLEND_DST_RGB = 0x80C8;
        this.BLEND_SRC_RGB = 0x80C9;
        this.BLEND_DST_ALPHA = 0x80CA;
        this.BLEND_SRC_ALPHA = 0x80CB;
        this.CONSTANT_COLOR = 0x8001;
        this.ONE_MINUS_CONSTANT_COLOR = 0x8002;
        this.CONSTANT_ALPHA = 0x8003;
        this.ONE_MINUS_CONSTANT_ALPHA = 0x8004;
        this.BLEND_COLOR = 0x8005;
        this.ARRAY_BUFFER = 0x8892;
        this.ELEMENT_ARRAY_BUFFER = 0x8893;
        this.ARRAY_BUFFER_BINDING = 0x8894;
        this.ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
        this.STREAM_DRAW = 0x88E0;
        this.STATIC_DRAW = 0x88E4;
        this.DYNAMIC_DRAW = 0x88E8;
        this.BUFFER_SIZE = 0x8764;
        this.BUFFER_USAGE = 0x8765;
        this.CURRENT_VERTEX_ATTRIB = 0x8626;
        this.FRONT = 0x0404;
        this.BACK = 0x0405;
        this.FRONT_AND_BACK = 0x0408;
        this.CULL_FACE = 0x0B44;
        this.BLEND = 0x0BE2;
        this.DITHER = 0x0BD0;
        this.STENCIL_TEST = 0x0B90;
        this.DEPTH_TEST = 0x0B71;
        this.SCISSOR_TEST = 0x0C11;
        this.POLYGON_OFFSET_FILL = 0x8037;
        this.SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
        this.SAMPLE_COVERAGE = 0x80A0;
        this.INVALID_ENUM = 0x0500;
        this.INVALID_VALUE = 0x0501;
        this.INVALID_OPERATION = 0x0502;
        this.OUT_OF_MEMORY = 0x0505;
        this.CW = 0x0900;
        this.CCW = 0x0901;
        this.LINE_WIDTH = 0x0B21;
        this.ALIASED_POINT_SIZE_RANGE = 0x846D;
        this.ALIASED_LINE_WIDTH_RANGE = 0x846E;
        this.CULL_FACE_MODE = 0x0B45;
        this.FRONT_FACE = 0x0B46;
        this.DEPTH_RANGE = 0x0B70;
        this.DEPTH_WRITEMASK = 0x0B72;
        this.DEPTH_CLEAR_VALUE = 0x0B73;
        this.DEPTH_FUNC = 0x0B74;
        this.STENCIL_CLEAR_VALUE = 0x0B91;
        this.STENCIL_FUNC = 0x0B92;
        this.STENCIL_FAIL = 0x0B94;
        this.STENCIL_PASS_DEPTH_FAIL = 0x0B95;
        this.STENCIL_PASS_DEPTH_PASS = 0x0B96;
        this.STENCIL_REF = 0x0B97;
        this.STENCIL_VALUE_MASK = 0x0B93;
        this.STENCIL_WRITEMASK = 0x0B98;
        this.STENCIL_BACK_FUNC = 0x8800;
        this.STENCIL_BACK_FAIL = 0x8801;
        this.STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
        this.STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
        this.STENCIL_BACK_REF = 0x8CA3;
        this.STENCIL_BACK_VALUE_MASK = 0x8CA4;
        this.STENCIL_BACK_WRITEMASK = 0x8CA5;
        this.VIEWPORT = 0x0BA2;
        this.SCISSOR_BOX = 0x0C10;
        this.COLOR_CLEAR_VALUE = 0x0C22;
        this.COLOR_WRITEMASK = 0x0C23;
        this.UNPACK_ALIGNMENT = 0x0CF5;
        this.PACK_ALIGNMENT = 0x0D05;
        this.MAX_TEXTURE_SIZE = 0x0D33;
        this.MAX_VIEWPORT_DIMS = 0x0D3A;
        this.SUBPIXEL_BITS = 0x0D50;
        this.RED_BITS = 0x0D52;
        this.GREEN_BITS = 0x0D53;
        this.BLUE_BITS = 0x0D54;
        this.ALPHA_BITS = 0x0D55;
        this.DEPTH_BITS = 0x0D56;
        this.STENCIL_BITS = 0x0D57;
        this.POLYGON_OFFSET_UNITS = 0x2A00;
        this.POLYGON_OFFSET_FACTOR = 0x8038;
        this.TEXTURE_BINDING_2D = 0x8069;
        this.SAMPLE_BUFFERS = 0x80A8;
        this.SAMPLES = 0x80A9;
        this.SAMPLE_COVERAGE_VALUE = 0x80AA;
        this.SAMPLE_COVERAGE_INVERT = 0x80AB;
        this.COMPRESSED_TEXTURE_FORMATS = 0x86A3;
        this.DONT_CARE = 0x1100;
        this.FASTEST = 0x1101;
        this.NICEST = 0x1102;
        this.GENERATE_MIPMAP_HINT = 0x8192;
        this.BYTE = 0x1400;
        this.UNSIGNED_BYTE = 0x1401;
        this.SHORT = 0x1402;
        this.UNSIGNED_SHORT = 0x1403;
        this.INT = 0x1404;
        this.UNSIGNED_INT = 0x1405;
        this.FLOAT = 0x1406;
        this.DEPTH_COMPONENT = 0x1902;
        this.ALPHA = 0x1906;
        this.RGB = 0x1907;
        this.RGBA = 0x1908;
        this.LUMINANCE = 0x1909;
        this.LUMINANCE_ALPHA = 0x190A;
        this.UNSIGNED_SHORT_4_4_4_4 = 0x8033;
        this.UNSIGNED_SHORT_5_5_5_1 = 0x8034;
        this.UNSIGNED_SHORT_5_6_5 = 0x8363;
        this.FRAGMENT_SHADER = 0x8B30;
        this.VERTEX_SHADER = 0x8B31;
        this.MAX_VERTEX_ATTRIBS = 0x8869;
        this.MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
        this.MAX_VARYING_VECTORS = 0x8DFC;
        this.MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
        this.MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
        this.MAX_TEXTURE_IMAGE_UNITS = 0x8872;
        this.MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
        this.SHADER_TYPE = 0x8B4F;
        this.DELETE_STATUS = 0x8B80;
        this.LINK_STATUS = 0x8B82;
        this.VALIDATE_STATUS = 0x8B83;
        this.ATTACHED_SHADERS = 0x8B85;
        this.ACTIVE_UNIFORMS = 0x8B86;
        this.ACTIVE_ATTRIBUTES = 0x8B89;
        this.SHADING_LANGUAGE_VERSION = 0x8B8C;
        this.CURRENT_PROGRAM = 0x8B8D;
        this.NEVER = 0x0200;
        this.LESS = 0x0201;
        this.EQUAL = 0x0202;
        this.LEQUAL = 0x0203;
        this.GREATER = 0x0204;
        this.NOTEQUAL = 0x0205;
        this.GEQUAL = 0x0206;
        this.ALWAYS = 0x0207;
        this.KEEP = 0x1E00;
        this.REPLACE = 0x1E01;
        this.INCR = 0x1E02;
        this.DECR = 0x1E03;
        this.INVERT = 0x150A;
        this.INCR_WRAP = 0x8507;
        this.DECR_WRAP = 0x8508;
        this.VENDOR = 0x1F00;
        this.RENDERER = 0x1F01;
        this.VERSION = 0x1F02;
        this.NEAREST = 0x2600;
        this.LINEAR = 0x2601;
        this.NEAREST_MIPMAP_NEAREST = 0x2700;
        this.LINEAR_MIPMAP_NEAREST = 0x2701;
        this.NEAREST_MIPMAP_LINEAR = 0x2702;
        this.LINEAR_MIPMAP_LINEAR = 0x2703;
        this.TEXTURE_MAG_FILTER = 0x2800;
        this.TEXTURE_MIN_FILTER = 0x2801;
        this.TEXTURE_WRAP_S = 0x2802;
        this.TEXTURE_WRAP_T = 0x2803;
        this.TEXTURE_2D = 0x0DE1;
        this.TEXTURE = 0x1702;
        this.TEXTURE_CUBE_MAP = 0x8513;
        this.TEXTURE_BINDING_CUBE_MAP = 0x8514;
        this.TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
        this.TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
        this.TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
        this.TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
        this.TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
        this.TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
        this.MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
        this.TEXTURE0 = 0x84C0;
        this.TEXTURE1 = 0x84C1;
        this.TEXTURE2 = 0x84C2;
        this.TEXTURE3 = 0x84C3;
        this.TEXTURE4 = 0x84C4;
        this.TEXTURE5 = 0x84C5;
        this.TEXTURE6 = 0x84C6;
        this.TEXTURE7 = 0x84C7;
        this.TEXTURE8 = 0x84C8;
        this.TEXTURE9 = 0x84C9;
        this.TEXTURE10 = 0x84CA;
        this.TEXTURE11 = 0x84CB;
        this.TEXTURE12 = 0x84CC;
        this.TEXTURE13 = 0x84CD;
        this.TEXTURE14 = 0x84CE;
        this.TEXTURE15 = 0x84CF;
        this.TEXTURE16 = 0x84D0;
        this.TEXTURE17 = 0x84D1;
        this.TEXTURE18 = 0x84D2;
        this.TEXTURE19 = 0x84D3;
        this.TEXTURE20 = 0x84D4;
        this.TEXTURE21 = 0x84D5;
        this.TEXTURE22 = 0x84D6;
        this.TEXTURE23 = 0x84D7;
        this.TEXTURE24 = 0x84D8;
        this.TEXTURE25 = 0x84D9;
        this.TEXTURE26 = 0x84DA;
        this.TEXTURE27 = 0x84DB;
        this.TEXTURE28 = 0x84DC;
        this.TEXTURE29 = 0x84DD;
        this.TEXTURE30 = 0x84DE;
        this.TEXTURE31 = 0x84DF;
        this.ACTIVE_TEXTURE = 0x84E0;
        this.REPEAT = 0x2901;
        this.CLAMP_TO_EDGE = 0x812F;
        this.MIRRORED_REPEAT = 0x8370;
        this.FLOAT_VEC2 = 0x8B50;
        this.FLOAT_VEC3 = 0x8B51;
        this.FLOAT_VEC4 = 0x8B52;
        this.INT_VEC2 = 0x8B53;
        this.INT_VEC3 = 0x8B54;
        this.INT_VEC4 = 0x8B55;
        this.BOOL = 0x8B56;
        this.BOOL_VEC2 = 0x8B57;
        this.BOOL_VEC3 = 0x8B58;
        this.BOOL_VEC4 = 0x8B59;
        this.FLOAT_MAT2 = 0x8B5A;
        this.FLOAT_MAT3 = 0x8B5B;
        this.FLOAT_MAT4 = 0x8B5C;
        this.SAMPLER_2D = 0x8B5E;
        this.SAMPLER_CUBE = 0x8B60;
        this.VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
        this.VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
        this.VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
        this.VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
        this.VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
        this.VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
        this.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;
        this.IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
        this.IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;
        this.COMPILE_STATUS = 0x8B81;
        this.LOW_FLOAT = 0x8DF0;
        this.MEDIUM_FLOAT = 0x8DF1;
        this.HIGH_FLOAT = 0x8DF2;
        this.LOW_INT = 0x8DF3;
        this.MEDIUM_INT = 0x8DF4;
        this.HIGH_INT = 0x8DF5;
        this.FRAMEBUFFER = 0x8D40;
        this.RENDERBUFFER = 0x8D41;
        this.RGBA4 = 0x8056;
        this.RGB5_A1 = 0x8057;
        this.RGB565 = 0x8D62;
        this.DEPTH_COMPONENT16 = 0x81A5;
        this.STENCIL_INDEX = 0x1901;
        this.STENCIL_INDEX8 = 0x8D48;
        this.DEPTH_STENCIL = 0x84F9;
        this.RENDERBUFFER_WIDTH = 0x8D42;
        this.RENDERBUFFER_HEIGHT = 0x8D43;
        this.RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
        this.RENDERBUFFER_RED_SIZE = 0x8D50;
        this.RENDERBUFFER_GREEN_SIZE = 0x8D51;
        this.RENDERBUFFER_BLUE_SIZE = 0x8D52;
        this.RENDERBUFFER_ALPHA_SIZE = 0x8D53;
        this.RENDERBUFFER_DEPTH_SIZE = 0x8D54;
        this.RENDERBUFFER_STENCIL_SIZE = 0x8D55;
        this.FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
        this.FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
        this.FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
        this.FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;
        this.COLOR_ATTACHMENT0 = 0x8CE0;
        this.DEPTH_ATTACHMENT = 0x8D00;
        this.STENCIL_ATTACHMENT = 0x8D20;
        this.DEPTH_STENCIL_ATTACHMENT = 0x821A;
        this.NONE = 0;
        this.FRAMEBUFFER_COMPLETE = 0x8CD5;
        this.FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
        this.FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
        this.FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
        this.FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
        this.FRAMEBUFFER_BINDING = 0x8CA6;
        this.RENDERBUFFER_BINDING = 0x8CA7;
        this.MAX_RENDERBUFFER_SIZE = 0x84E8;
        this.INVALID_FRAMEBUFFER_OPERATION = 0x0506;
        this.UNPACK_FLIP_Y_WEBGL = 0x9240;
        this.UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241;
        this.CONTEXT_LOST_WEBGL = 0x9242;
        this.UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243;
        this.BROWSER_DEFAULT_WEBGL = 0x9244;
        this._nativeObj = new WebGLContext();
        this.id = this._nativeObj.conchID;
        this._fadeArray.push(0);
    }
    createFadeID() {
        for (var i = 0, n = this._fadeArray.length; i < n; i++) {
            if (this._fadeArray[i] == -1) {
                this._fadeArray[i] = i;
                return i;
            }
        }
        var nID = ++this._i;
        this._fadeArray.push(nID);
        return nID;
    }
    deleteFadeID(nID) {
        this._fadeArray[nID] = -1;
    }
    getContextAttributes() {
        return {};
    }
    onCssTransform() {
        if (this.__priv_canvas) {
            this.viewport(this.viewportx, this.viewporty, this.viewportw, this.viewporth);
        }
    }
    isContextLost() {
        return false;
    }
    getShaderPrecisionFormat(a, b) {
        return {
            precision: 23,
            rangeMax: 127,
            rangeMin: 127
        };
    }
    getExtension(name) {
        conch.bf.needsz(8);
        conch.bf.wu32(this.id);
        conch.bf.wu32(0);
    }
    createBuffer() {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(1);
        conch.bf.wu32(fadeID);
        return fadeID;
    }
    bindBuffer(target, buffer) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(2);
        conch.bf.wu32(target);
        conch.bf.wu32(buffer);
    }
    flush() {
        conch.bf.flush();
    }
    bufferData(target, sizeOrArray, usage) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        if (typeof (arguments[1]) === 'number') {
            conch.bf.wu32(4);
            conch.bf.wu32(arguments[0]);
            conch.bf.wu32(arguments[2]);
            conch.bf.wu32(arguments[1]);
        }
        else {
            conch.bf.wu32(3);
            conch.bf.wu32(arguments[0]);
            conch.bf.wu32(arguments[2]);
            var length = arguments[1].byteLength;
            if ((length % 4) != 0) {
                alert("bufferData,len is not 4 align,length=" + length);
            }
            conch.bf.wu32(length);
            conch.bf.wab(arguments[1], length);
        }
    }
    createShader(type) {
        var fadeID = this.createFadeID();
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(5);
        conch.bf.wu32(type);
        conch.bf.wu32(fadeID);
        return fadeID;
    }
    shaderSource(shader, source) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(6);
        conch.bf.wu32(shader);
        conch.bf.wstring(source, true);
    }
    compileShader(shader) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(7);
        conch.bf.wu32(shader);
    }
    getShaderParameter(shader, pname) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(8);
        conch.bf.wu32(shader);
        conch.bf.wu32(pname);
        return true;
    }
    createProgram() {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(9);
        conch.bf.wu32(fadeID);
        return fadeID;
    }
    attachShader(program, shader) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(10);
        conch.bf.wu32(program);
        conch.bf.wu32(shader);
    }
    linkProgram(program) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(11);
        conch.bf.wu32(program);
    }
    getProgramParameter(program, pname) {
        return 0;
    }
    useProgram(program) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(13);
        conch.bf.wu32(program);
    }
    getUniformLocation(program, name) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(14);
        conch.bf.wu32(program);
        var fadeLoc = this._locTable.getFadeLocation(program, name);
        conch.bf.wu32(fadeLoc);
        conch.bf.wstring(name, true);
        return fadeLoc;
    }
    getAttribLocation(program, name) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(15);
        conch.bf.wu32(program);
        var fadeLoc = this._locTable.getFadeLocation(program, name);
        conch.bf.wu32(fadeLoc);
        conch.bf.wstring(name, true);
        return fadeLoc;
    }
    disable(cap) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(16);
        conch.bf.wu32(cap);
    }
    enable(cap) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(17);
        conch.bf.wu32(cap);
    }
    colorMask(red, green, blue, alpha) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(18);
        conch.bf.wu8(red ? 1 : 0);
        conch.bf.wu8(green ? 1 : 0);
        conch.bf.wu8(blue ? 1 : 0);
        conch.bf.wu8(alpha ? 1 : 0);
    }
    viewport(x, y, width, height) {
        this.viewportx = x;
        this.viewporty = y;
        this.viewportw = width;
        this.viewporth = height;
        if (!this.curFBO && this.__priv_canvas._hasTransform) {
            var t = this.__priv_canvas._tranform;
            x += t[4];
            y += t[5];
            width *= t[0];
            height *= t[3];
        }
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(19);
        conch.bf.wu16(x);
        conch.bf.wu16(y);
        conch.bf.wu16(width);
        conch.bf.wu16(height);
    }
    clear(mask) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(20);
        conch.bf.wu32(mask);
    }
    clearColor(red, green, blue, alpha) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(21);
        conch.bf.wf32(red);
        conch.bf.wf32(green);
        conch.bf.wf32(blue);
        conch.bf.wf32(alpha);
    }
    createTexture(invalidCallback, restoreCallback) {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(22);
        conch.bf.wu32(fadeID);
        return fadeID;
    }
    bindTexture(target, texture) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(23);
        conch.bf.wu32(target);
        conch.bf.wu32(texture);
        this.currentTextureID = texture;
    }
    pixelStorei(pname, param) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(24);
        conch.bf.wu32(pname);
        conch.bf.wu32(param);
    }
    texImage2D(target, level, internalformat, width, height, border, format, type, pixels) {
        if (arguments.length == 9) {
            this.texImage2DEx(false, target, level, internalformat, width, height, border, format, type, pixels);
        }
        else if (arguments.length == 6) {
            this.texImage2DEx(false, target, level, internalformat, width, height, border);
        }
    }
    texImage2DEx(premultiplyAlpha, target, level, internalformat, width, height, border, format, type, pixels) {
        if (arguments.length == 10) {
            conch.bf.needsz(42);
            conch.bf.wu32(this.id);
            conch.bf.wu32(26);
            conch.bf.wu32(arguments[0 + 1]);
            conch.bf.wu32(arguments[1 + 1]);
            conch.bf.wu32(arguments[2 + 1]);
            conch.bf.wu32(arguments[3 + 1]);
            conch.bf.wu32(arguments[4 + 1]);
            conch.bf.wu32(arguments[5 + 1]);
            conch.bf.wu32(arguments[6 + 1]);
            conch.bf.wu32(arguments[7 + 1]);
            conch.bf.wu32(arguments[0] ? 1 : 0);
            var argobj = arguments[5 + 1] && arguments[8 + 1]._nativeObj;
            if (argobj) {
                conch.bf.needsz(8);
                if (argobj && argobj.isCanvas && argobj.isCanvas()) {
                    alert("texSubImage2D:禁止2D和3D混合了 " + (new Error())['stack']);
                    conch.bf.wu32(1);
                }
                else if (argobj.isMemoryCanvas && argobj.isMemoryCanvas()) {
                    conch.bf.wu32(2);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else if (argobj.isTextCanvas && argobj.isTextCanvas()) {
                    conch.bf.wu32(3);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else {
                    conch.bf.wu32(0);
                }
                conch.bf.wu32(argobj.conchImgId);
            }
            else {
                conch.bf.needsz(12);
                conch.bf.wu32(0);
                conch.bf.wu32(-1);
            }
        }
        else if (arguments.length == 7) {
            conch.bf.needsz(38);
            conch.bf.wu32(this.id);
            conch.bf.wu32(25);
            conch.bf.wu32(arguments[0 + 1]);
            conch.bf.wu32(arguments[1 + 1]);
            conch.bf.wu32(arguments[2 + 1]);
            conch.bf.wu32(arguments[3 + 1]);
            conch.bf.wu32(arguments[4 + 1]);
            conch.bf.wu32(arguments[0] ? 1 : 0);
            var argobj = arguments[5 + 1] && arguments[5 + 1]._nativeObj;
            if (argobj) {
                if (argobj && argobj.isCanvas && argobj.isCanvas()) {
                    alert("texSubImage2D:禁止2D和3D混合了 " + (new Error())['stack']);
                    conch.bf.wu32(1);
                }
                else if (argobj.isMemoryCanvas && argobj.isMemoryCanvas()) {
                    conch.bf.wu32(2);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else if (argobj.isTextCanvas && argobj.isTextCanvas()) {
                    conch.bf.wu32(3);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else {
                    conch.bf.wu32(0);
                }
                conch.bf.wu32(argobj.conchImgId);
            }
            else {
                conch.bf.wu32(0);
                conch.bf.wu32(-1);
            }
        }
    }
    texSubImage2D(target, level, xoffset, yoffset, format, type, source) {
        if (arguments.length == 6) {
            this.texSubImage2DEx(false, target, level, xoffset, yoffset, format, type);
        }
        else if (arguments.length == 7) {
            this.texSubImage2DEx(false, target, level, xoffset, yoffset, format, type, source);
        }
    }
    texSubImage2DEx(premultiplyAlpha, target, level, xoffset, yoffset, format, type, source) {
        conch.bf.needsz(42);
        conch.bf.wu32(this.id);
        conch.bf.wu32(27);
        conch.bf.wu32(arguments[0 + 1]);
        conch.bf.wu32(arguments[1 + 1]);
        conch.bf.wu32(arguments[2 + 1]);
        conch.bf.wu32(arguments[3 + 1]);
        conch.bf.wu32(arguments[4 + 1]);
        conch.bf.wu32(arguments[5 + 1]);
        conch.bf.wu32(arguments[0] ? 1 : 0);
        var argobj = arguments[6 + 1] && (arguments[6 + 1]._nativeObj || arguments[6 + 1]);
        if (argobj) {
            if (argobj && argobj.isCanvas && argobj.isCanvas()) {
                var e = new Error('dd');
                alert("texSubImage2D:禁止2D和3D混合了 " + e.stack);
                conch.bf.wu32(1);
            }
            else if (argobj.isMemoryCanvas && argobj.isMemoryCanvas()) {
                conch.bf.wu32(2);
                argobj.setTargetTextureID(this.currentTextureID);
            }
            else if (argobj.isTextCanvas && argobj.isTextCanvas()) {
                conch.bf.wu32(3);
                argobj.setTargetTextureID(this.currentTextureID);
            }
            else {
                conch.bf.wu32(0);
            }
            conch.bf.wu32(argobj.conchImgId);
        }
        else {
            conch.bf.wu32(0);
            conch.bf.wu32(-1);
        }
    }
    texParameterf(target, pname, param) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(28);
        conch.bf.wu32(target);
        conch.bf.wu32(pname);
        conch.bf.wf32(param);
    }
    texParameteri(target, pname, param) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(29);
        conch.bf.wu32(target);
        conch.bf.wu32(pname);
        conch.bf.wu32(param);
    }
    activeTexture(texture) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(30);
        conch.bf.wu32(texture);
    }
    getShaderInfoLog(shader) {
        return "unknown log";
    }
    uniform1f(location, x) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(32);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
    }
    uniform1i(location, x) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(33);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
    }
    uniform2f(location, x, y) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(34);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
        conch.bf.wf32(y);
    }
    uniform2i(location, x, y) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(35);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
    }
    uniform3f(location, x, y, z) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(36);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
        conch.bf.wf32(y);
        conch.bf.wf32(z);
    }
    uniform3i(location, x, y, z) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(37);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
        conch.bf.wu32(z);
    }
    uniform4f(location, x, y, z, w) {
        conch.bf.needsz(28);
        conch.bf.wu32(this.id);
        conch.bf.wu32(38);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
        conch.bf.wf32(y);
        conch.bf.wf32(z);
        conch.bf.wf32(w);
    }
    uniform4i(location, x, y, z, w) {
        conch.bf.needsz(28);
        conch.bf.wu32(this.id);
        conch.bf.wu32(39);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
        conch.bf.wu32(z);
        conch.bf.wu32(w);
    }
    uniform1fv(location, v) {
        this.uniformxv(40, location, v);
    }
    uniform1iv(location, v) {
        this.uniformxv(41, location, v);
    }
    uniform2fv(location, v) {
        this.uniformxv(42, location, v);
    }
    uniform2iv(location, v) {
        this.uniformxv(43, location, v);
    }
    uniform3fv(location, v) {
        this.uniformxv(44, location, v);
    }
    uniform3iv(location, v) {
        this.uniformxv(45, location, v);
    }
    uniform4fv(location, v) {
        this.uniformxv(46, location, v);
    }
    uniform4iv(location, v) {
        this.uniformxv(47, location, v);
    }
    uniformxv(cmd, location, v) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(cmd);
        conch.bf.wu32(location);
        var length = v.byteLength;
        conch.bf.wu32(length);
        conch.bf.wab(v, length);
    }
    _uniformMatrixfv_x(cmd, location, transpose, values, number) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(cmd);
        conch.bf.wu32(location);
        conch.bf.wu32(transpose);
        if (values.byteLength == null) {
            var len = values.length;
            conch.bf.wu32(len * 4);
            conch.bf.needsz(len * 4);
            for (var i = 0; i < len; i++) {
                conch.bf.wf32(values[i]);
            }
            return;
        }
        var length = values.byteLength;
        if ((length % 4) != 0) {
            alert("_uniformMatrixfv_x- len is not 4 align!");
        }
        conch.bf.wu32(length);
        conch.bf.wab(values, length);
    }
    uniformMatrix2fv(location, transpose, values, number) {
        this._uniformMatrixfv_x(48, location, transpose, values, number);
    }
    uniformMatrix3fv(location, transpose, values, number) {
        this._uniformMatrixfv_x(49, location, transpose, values, number);
    }
    uniformMatrix4fv(location, transpose, values, number) {
        this._uniformMatrixfv_x(50, location, transpose, values, number);
    }
    vertexAttribPointer(index, size, type, normalized, stride, offset) {
        conch.bf.needsz(32);
        conch.bf.wu32(this.id);
        conch.bf.wu32(51);
        conch.bf.wu32(index);
        conch.bf.wu32(size);
        conch.bf.wu32(type);
        conch.bf.wu32(normalized);
        conch.bf.wu32(stride);
        conch.bf.wu32(offset);
    }
    enableVertexAttribArray(index) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(52);
        conch.bf.wu32(index);
    }
    blendFunc(sfactor, dfactor) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(53);
        conch.bf.wu16(sfactor);
        conch.bf.wu16(dfactor);
    }
    bufferSubData(target, offset, data) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(54);
        conch.bf.wu32(target);
        conch.bf.wu32(offset);
        var length = data.byteLength;
        conch.bf.wu32(length);
        conch.bf.wab(data, length, 0);
    }
    drawElements(mode, count, type, offset) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(55);
        conch.bf.wu32(mode);
        conch.bf.wu32(count);
        conch.bf.wu32(type);
        conch.bf.wu32(offset);
    }
    deleteTexture(texture) {
        this.deleteFadeID(texture);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(56);
        conch.bf.wu32(texture);
    }
    drawArrays(mode, first, count) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(57);
        conch.bf.wu32(mode);
        conch.bf.wu32(first);
        conch.bf.wu32(count);
    }
    bindAttribLocation(program, index, name) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(58);
        conch.bf.wu32(program);
        conch.bf.wu32(index);
        conch.bf.wstring(name, true);
    }
    cullFace(mode) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(59);
        conch.bf.wu32(mode);
    }
    createFramebuffer() {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(60);
        conch.bf.wu32(fadeID);
        return fadeID;
    }
    getParameter(pname) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(61);
        conch.bf.wu32(pname);
        var fadeValue = 0;
        if (pname == this.FRAMEBUFFER_BINDING) {
            fadeValue = this.curFBO;
        }
        conch.bf.wu32(fadeValue);
        return fadeValue;
    }
    bindFramebuffer(target, framebuffer) {
        this.curFBO = framebuffer;
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(62);
        conch.bf.wu32(target);
        conch.bf.wu32(framebuffer);
    }
    depthFunc(func) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(63);
        conch.bf.wu32(func);
    }
    depthMask(flag) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(64);
        conch.bf.wu32(flag);
    }
    depthRange(zNear, zFar) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(65);
        conch.bf.wf32(zNear);
        conch.bf.wf32(zFar);
    }
    stencilFunc(func, ref, mask) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(66);
        conch.bf.wu32(func);
        conch.bf.wu32(ref);
        conch.bf.wu32(mask);
    }
    stencilMask(mask) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(67);
        conch.bf.wu32(mask);
    }
    stencilOp(fail, zfail, zpass) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(68);
        conch.bf.wu32(fail);
        conch.bf.wu32(zfail);
        conch.bf.wu32(zpass);
    }
    deleteBuffer(buffer) {
        this.deleteFadeID(buffer);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(69);
        conch.bf.wu32(buffer);
    }
    deleteFramebuffer(framebuffer) {
        this.deleteFadeID(framebuffer);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(70);
        conch.bf.wu32(framebuffer);
    }
    deleteProgram(program) {
        this.deleteFadeID(program);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(71);
        conch.bf.wu32(program);
    }
    deleteRenderbuffer(renderbuffer) {
        this.deleteFadeID(renderbuffer);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(72);
        conch.bf.wu32(renderbuffer);
    }
    deleteShader(shader) {
        this.deleteFadeID(shader);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(73);
        conch.bf.wu32(shader);
    }
    isBuffer(buffer) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(74);
        conch.bf.wu32(buffer);
        return true;
    }
    disableVertexAttribArray(index) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(75);
        conch.bf.wu32(index);
    }
    scissor(x, y, width, height) {
        if (!this.curFBO && this.__priv_canvas._hasTransform) {
            var t = this.__priv_canvas._tranform;
            width *= t[0];
            height *= t[3];
            x = x * t[0] + t[4];
            y = y * t[3] + t[5];
        }
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(76);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
        conch.bf.wu32(width);
        conch.bf.wu32(height);
    }
    generateMipmap(target) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(77);
        conch.bf.wu32(target);
    }
    framebufferTexture2D(target, attachment, textarget, texture, level) {
        conch.bf.needsz(28);
        conch.bf.wu32(this.id);
        conch.bf.wu32(78);
        conch.bf.wu32(target);
        conch.bf.wu32(attachment);
        conch.bf.wu32(textarget);
        conch.bf.wu32(texture);
        conch.bf.wu32(level);
    }
    checkFramebufferStatus(target) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(79);
        conch.bf.wu32(target);
        if (target == this.FRAMEBUFFER) {
            return this.FRAMEBUFFER_COMPLETE;
        }
        return 0;
    }
    createRenderbuffer() {
        var _ = conch.bf;
        var fadeID = this.createFadeID();
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(80);
        _.wu32(fadeID);
        return fadeID;
    }
    bindRenderbuffer(target, buffer) {
        var _ = conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(81);
        _.wu32(target);
        _.wu32(buffer);
    }
    renderbufferStorage(target, internalformat, width, height) {
        var _ = conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(82);
        _.wu32(target);
        _.wu32(internalformat);
        _.wu32(width);
        _.wu32(height);
    }
    framebufferRenderbuffer(target, attachment, renderbuffertarget, buffer) {
        var _ = conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(83);
        _.wu32(target);
        _.wu32(attachment);
        _.wu32(renderbuffertarget);
        _.wu32(buffer);
    }
    blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha) {
        var _ = conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(84);
        _.wu16(srcRGB);
        _.wu16(dstRGB);
        _.wu16(srcAlpha);
        _.wu16(dstAlpha);
    }
    frontFace(mode) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(85);
        conch.bf.wu32(mode);
    }
    getProgramParameterEx(vs, ps, define, pname) {
        return this._nativeObj.getProgramParameterEx(vs, ps, define, pname);
    }
    getActiveAttribEx(vs, ps, define, index) {
        return this._nativeObj.getActiveAttribEx(vs, ps, define, index);
    }
    getActiveUniformEx(vs, ps, define, index) {
        return this._nativeObj.getActiveUniformEx(vs, ps, define, index);
    }
}
class CSSStyleDeclaration {
    constructor() {
        this._transform = new Float32Array([1, 0, 0, 1, 0, 0]);
        this._styleNumber = 0;
    }
    get length() {
        return this._styleNumber;
    }
    getPropertyPriority(propertyName) {
        return '';
    }
    getPropertyValue(propertyName) {
        return '';
    }
    item(index) {
        return '';
    }
    removeProperty(propertyName) {
        return '';
    }
    setProperty(propertyName, value, priority) {
    }
    set left(l) {
        var nl = parseInt(l);
        this._htmlEle.clientLeft = nl;
    }
    set top(t) {
        var nt = parseInt(t);
        this._htmlEle.clientTop = nt;
    }
    set height(h) {
    }
    get height() {
        return this._htmlEle.clientHeight;
    }
    set width(w) {
    }
    get width() {
        return this._htmlEle.clientWidth;
    }
    set transform(trans) {
        var s1 = trans.split(/[\(,\)]/);
        if (s1 && s1.length > 2) {
            if (s1[0] === 'matrix') {
                if (s1.length >= 7) {
                    this._transform[0] = 1 * s1[1];
                    this._transform[1] = 1 * s1[2];
                    this._transform[2] = 1 * s1[3];
                    this._transform[3] = 1 * s1[4];
                    this._transform[4] = 1 * s1[5];
                    this._transform[5] = 1 * s1[6];
                    if (this._htmlEle && this._htmlEle.onCSS_Transform) {
                        this._htmlEle.onCSS_Transform(this._transform);
                    }
                }
                else if (s1[0] === 'scale') {
                    alert('css scale not implemented');
                }
            }
        }
    }
}
class HTMLElement extends Element {
    constructor() {
        super();
        this.style = new CSSStyleDeclaration();
        this.style._htmlEle = this;
    }
    onCSS_Transform(mat) { }
    ;
    onerror(ev) {
    }
    onload(ev) {
    }
}
window["HTMLElement"] = HTMLElement;
class HTMLDivElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "DIV";
    }
}
class HTMLScriptElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "SCRIPT";
    }
    set src(url) {
        var _t = this;
        this._src = location.resolve(url);
        console.log("HTMLScriptElement set src" + this._src);
        document.uploadScript({ "src": this._src, "obj": this });
    }
    get src() {
        return this._src;
    }
    set text(t) {
        this._text = t;
        document.uploadScript({ "text": this._text, "obj": this });
    }
    get text() {
        return this._text;
    }
}
class HTMLCanvasElement extends HTMLElement {
    constructor() {
        super();
        this._isFirst = false;
        this._tranform = null;
        this._hasTransform = false;
        this._clientRect = { left: 0, top: 0, width: 0, height: 0, right: 0, bottom: 0 };
        this.clientLeft;
        this.tagName = "CANVAS";
        if (HTMLCanvasElement._isFirstCanv) {
            this._isFirst = true;
            HTMLCanvasElement._isFirstCanv = false;
            if (HTMLCanvasElement.rootContext)
                return HTMLCanvasElement.rootContext["canvas"];
        }
    }
    getBoundingClientRect() {
        var cr = this._clientRect;
        if (this._hasTransform) {
            cr.width = this._width * this._tranform[0];
            cr.height = this._height * this._tranform[3];
            cr.left = this._tranform[4];
            cr.top = this._tranform[5];
            cr.right = cr.left + cr.width;
            cr.bottom = cr.left + cr.height;
        }
        else {
            cr.right = cr.width = this._width;
            cr.bottom = cr.height = this.height;
        }
        return cr;
    }
    getContext(contextId) {
        if (contextId === '2d') {
            if (this._ctx2d)
                return this._ctx2d;
            var ctx2d;
            if (this._isFirst && HTMLCanvasElement.rootContext)
                ctx2d = HTMLCanvasElement.rootContext;
            else {
                ctx2d = new CanvasRenderingContext2D();
                if (this._width)
                    ctx2d.setSize(this._width, this._height);
            }
            ctx2d.canvas = this;
            if (this._isFirst) {
                if (!HTMLCanvasElement.rootContext) {
                    ctx2d.setCanvasType(0);
                }
            }
            this._ctx2d = ctx2d;
            return ctx2d;
        }
        else if (contextId === 'webgl' || contextId === 'experimental-webgl') {
            var ctx3d = new WebGLRenderingContext();
            ctx3d.__priv_canvas = this;
            this.addEventListener('csstransform', ctx3d.onCssTransform.bind(ctx3d));
            return ctx3d;
        }
        else if (contextId == "conch") {
            HTMLCanvasElement._isFirstCanv = true;
            ctx2d = new CanvasRenderingContext2D();
            ctx2d.canvas = this;
            HTMLCanvasElement.rootContext = ctx2d;
            this._ctx2d = ctx2d;
            ctx2d.setCanvasType(0);
        }
        return null;
    }
    onCSS_Transform(mat) {
        this._tranform = mat;
        this._hasTransform = true;
        var e = new Event('csstransform');
        this.dispatchEvent(e);
        var sw = mat[0];
        var sh = mat[3];
        conchConfig.setScreenScale(sw, sh, mat[4], mat[5]);
        if (sw == 0 || sh == 0) {
            if (mat[4] > mat[5]) {
                console.warn("设置横竖屏错误! 请在初始html页面meta中设置screenorientation为横屏landscape");
            }
            else {
                console.warn("设置横竖屏错误! 请在初始html页面meta中设置screenorientation为竖屏portrait");
            }
        }
    }
    toDataURL(type) {
        return null;
    }
    setSize(w, h) {
        this._width = w;
        this._height = h;
        this._ctx2d && this._ctx2d.setSize(w, h);
    }
    set width(w) {
        if (this._isFirst && HTMLCanvasElement.RS) {
            this._w = w;
            return;
        }
        if (this._width != w) {
            this._width = w;
            this._ctx2d && this._ctx2d.setSize(this._width, this._height);
        }
    }
    get width() {
        return this._width;
    }
    set height(h) {
        if (this._isFirst && HTMLCanvasElement.RS) {
            this._h = h;
            return;
        }
        if (this._height != h) {
            this._height = h;
            this._ctx2d && this._ctx2d.setSize(this._width, this._height);
        }
    }
    get height() {
        return this._height;
    }
    get clientWidth() {
        return this._width;
    }
    get clientHeight() {
        return this._height;
    }
    toBase64(type, encoderOptions, callback) {
        if (this._ctx2d) {
            this._ctx2d.toBase64(type, encoderOptions, callback);
        }
    }
    getPixels(x, y, w, h, callBack) {
        if (this._ctx2d) {
            this._ctx2d.getPixels(x, y, w, h, callBack);
        }
    }
}
HTMLCanvasElement._isFirstCanv = true;
window.HTMLCanvasElement = HTMLCanvasElement;
class HTMLImageElement extends HTMLElement {
    constructor() {
        super();
        this._nativeObj = null;
        this.complete = false;
        this._enableMerageInAtlas = true;
        this.tagName = 'IMG';
        this._nativeObj = new image();
        this._nativeObj.srcs = 0;
        this._nativeObj.obj = this;
    }
    set src(url) {
        this._nativeObj.onerror = this._nativeOnError;
        this._nativeObj.onload = this._nativeOnload;
        this._nativeObj.srcs = this._nativeObj.srcs || 0;
        if (this._nativeObj.srcs == 0) {
            this._nativeObj.obj = this;
        }
        this._nativeObj.srcs++;
        url = location.resolve(url);
        this._nativeObj.setSrc(url);
    }
    get src() {
        return this._nativeObj.src;
    }
    _nativeOnload() {
        this.srcs = this.srcs || 1;
        this.srcs--;
        var temp = this.obj;
        var e = new Event('load');
        temp.complete = true;
        e.target = this;
        arguments.length === 1 ? temp.onload(e, arguments[0]) : temp.onload(e);
        temp.dispatchEvent(e);
        if (this.srcs == 0) {
            this.obj = null;
        }
    }
    _nativeOnError(e) {
        this.srcs = this.srcs || 1;
        this.srcs--;
        var temp = this.obj;
        var evt = new Event('error');
        evt.target = this;
        if (typeof (e) === 'object') {
            evt['extCode'] = e.extCode;
        }
        else
            evt['extCode'] = e;
        temp.onerror(evt);
        temp.dispatchEvent(evt);
        if (this.srcs == 0) {
            this.obj = null;
        }
    }
    get width() {
        if (this._nativeObj)
            return this._nativeObj.width;
        return 0;
    }
    get height() {
        if (this._nativeObj)
            return this._nativeObj.height;
        return 0;
    }
    get imgId() {
        return this._nativeObj.conchImgId;
    }
    putImageData(data, w, h) {
        this._nativeObj.putBitmapData(data, w, h);
    }
    getImageData(sx, sy, sw, sh) {
        return this._nativeObj.getImageData(sx, sy, sw, sh);
    }
    conchDestroy() {
        this._nativeObj.destroy();
    }
    set enableMerageInAtlas(value) {
        this._enableMerageInAtlas = value;
        this._nativeObj.enableMerageInAtlas(value);
    }
    get enableMerageInAtlas() {
        return this._enableMerageInAtlas;
    }
    releaseTexture() {
        this._nativeObj.releaseTexture();
    }
}
class HTMLInputElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "Input";
        this._nativeObj = new ConchInput();
        this.setLeft = this._nativeObj.setLeft.bind(this._nativeObj);
        this.setTop = this._nativeObj.setTop.bind(this._nativeObj);
        this.setWidth = this._nativeObj.setWidth.bind(this._nativeObj);
        this.setHeight = this._nativeObj.setHeight.bind(this._nativeObj);
        this.setOpacity = this._nativeObj.setOpacity.bind(this._nativeObj);
        this.setValue = this._nativeObj.setValue.bind(this._nativeObj);
        this.getValue = this._nativeObj.getValue.bind(this._nativeObj);
        this.setStyle = this._nativeObj.setStyle.bind(this._nativeObj);
        this.setVisible = this._nativeObj.setVisible.bind(this._nativeObj);
        this.focus = this._nativeObj.focus.bind(this._nativeObj);
        this.blur = this._nativeObj.blur.bind(this._nativeObj);
        this.setColor = this._nativeObj.setColor.bind(this._nativeObj);
        this.setFontSize = this._nativeObj.setFontSize.bind(this._nativeObj);
        this.setPos = this._nativeObj.setPos.bind(this._nativeObj);
        this.setSize = this._nativeObj.setSize.bind(this._nativeObj);
        this.setCursorPosition = this._nativeObj.setCursorPosition.bind(this._nativeObj);
        this.setScale = this._nativeObj.setScale.bind(this._nativeObj);
        this.setMaxLength = this._nativeObj.setMaxLength.bind(this._nativeObj);
        this.setType = this._nativeObj.setType.bind(this._nativeObj);
        this.setNumberOnly = this._nativeObj.setNumberOnly.bind(this._nativeObj);
        this.setRegular = this._nativeObj.setRegular.bind(this._nativeObj);
        this.setFont = this._nativeObj.setFont.bind(this._nativeObj);
        this.setMultiAble = this._nativeObj.setMultiAble.bind(this._nativeObj);
        this.setForbidEdit = this._nativeObj.setForbidEdit.bind(this._nativeObj);
        if (this._nativeObj.setBgColor) {
            this.setBgColor = this._nativeObj.setBgColor.bind(this._nativeObj);
        }
        else {
            this.setBgColor = function (c) { };
        }
    }
    set maxLength(v) {
        this.setMaxLength(v);
    }
    set left(v) {
        this._nativeObj.left = v;
    }
    get left() {
        return this._nativeObj.left;
    }
    set top(v) {
        this._nativeObj.top = v;
    }
    get top() {
        return this._nativeObj.top;
    }
    set width(v) {
        this._nativeObj.width = v;
    }
    get width() {
        return this._nativeObj.width;
    }
    set height(v) {
        this._nativeObj.height = v;
    }
    get height() {
        return this._nativeObj.height;
    }
    set opacity(v) {
        this._nativeObj.opacity = v;
    }
    get opacity() {
        return this._nativeObj.opacity;
    }
    get clientLeft() {
        return this._nativeObj.left;
    }
    set clientLeft(v) {
        this._nativeObj.left = v;
    }
    get clientTop() {
        return this._nativeObj.top;
    }
    set clientTop(v) {
        this._nativeObj.top = v;
    }
    set clientWidth(v) {
        this._nativeObj.width = v;
    }
    get clientWidth() {
        return this._nativeObj.width;
    }
    set clientHeight(v) {
        this._nativeObj.height = v;
    }
    get clientHeight() {
        return this._nativeObj.height;
    }
    set value(v) {
        this._nativeObj.value = v;
    }
    get value() {
        return this._nativeObj.value;
    }
    set visible(v) {
        this._nativeObj.visible = v;
    }
    get visible() {
        return this._nativeObj.visible;
    }
    _addEventListener(type, listener, useCapture) {
        super._addEventListener(type, listener, useCapture);
        var _t = this;
        this._nativeObj.addEventListener(type, function (e) {
            var ev = new _lbEvent(type);
            ev.target = this;
            _t.dispatchEvent(ev);
        });
    }
}
class HTMLMediaElement extends HTMLElement {
}
class HTMLMetaElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "META";
    }
    get httpEquiv() {
        return this["http-equiv"];
    }
    set name(n) {
        this._name = n;
        document._elements[n] = document._elements[n] || [];
        document._elements[n].push(this);
    }
    get name() {
        return this._name;
    }
}
class HTMLAudioElement extends HTMLMediaElement {
    constructor() {
        super();
        this.readyState = 0;
        this.tagName = "AUDIO";
        this._nativeObj = new ConchAudio();
    }
    load() {
    }
    setLoop(loop) {
        this._nativeObj.setLoop(loop);
    }
    play() {
        this._nativeObj.play();
    }
    pause() {
        this._nativeObj.pause();
    }
    stop() {
        this._nativeObj.stop();
    }
    set autoplay(v) {
        this._nativeObj.autoplay = v;
    }
    get autoplay() {
        return this._nativeObj.autoplay;
    }
    set loop(v) {
        this._nativeObj.loop = v;
    }
    get loop() {
        return this._nativeObj.loop;
    }
    set muted(v) {
        this._nativeObj.muted = v;
    }
    get muted() {
        return this._nativeObj.muted;
    }
    set src(v) {
        v = location.resolve(v);
        this._nativeObj.src = v;
    }
    get src() {
        return this._nativeObj.src;
    }
    set volume(v) {
        this._nativeObj.volume = v;
    }
    get volume() {
        return this._nativeObj.volume;
    }
    set currentTime(v) {
        this._nativeObj.currentTime = v;
    }
    get currentTime() {
        return this._nativeObj.currentTime;
    }
    _addEventListener(type, listener, useCapture) {
        super._addEventListener(type, listener, useCapture);
        var _t = this;
        this._nativeObj.addEventListener(type, function (e) {
            var ev = new _lbEvent(type);
            ev.target = this;
            if (type == "canplaythrough")
                _t.readyState = 4;
            _t.dispatchEvent(ev);
        });
    }
}
class HTMLBodyElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "BODY";
    }
    get clientHeight() {
        return window.innerHeight;
    }
    set clientHeight(h) {
    }
    get clientWidth() {
        return window.innerWidth;
    }
    set clientWidth(w) {
    }
}
class NodeList extends Array {
    constructor() {
        super();
    }
    item(index) {
        return this[index];
    }
}
class HTMLHeadElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = 'head';
        this.__visible = false;
    }
}
class HTMLCollection extends Array {
    item(nameOrIndex, optionalIndex) {
        return this[nameOrIndex];
    }
    namedItem(name) {
        return null;
    }
}
class HTMLVideoElement extends HTMLDivElement {
    constructor() {
        super();
    }
    canPlayType() {
        return false;
    }
}
class Document extends Node {
    constructor() {
        super();
        this.createMap = new Map();
        this._frameEndEvt = new Event('frameend');
        this.scriptTextList = [];
        this._loading = 0;
        this._evalNum = 0;
        this._eventPathCache = null;
        this.all = new HTMLCollection();
        this._elements = [];
        window.document = this;
        this.defaultView = window;
        var cm = this.createMap;
        cm.set('div', this.create_div);
        cm.set('img', this.create_img);
        cm.set('image', this.create_img);
        cm.set('canvas', this.create_canvas);
        cm.set('virtualBitmap', this.create_vbitmap);
        cm.set('audio', this.create_audio);
        cm.set('input', this.create_input);
        cm.set('textarea', this.create_input);
        cm.set('video', this.create_video);
        cm.set('script', this.create_script);
        cm.set('meta', this.create_meta);
        var html = new HTMLElement();
        var ww = getInnerWidth();
        var wh = getInnerHeight();
        html.clientWidth = ww;
        html.clientHeight = wh;
        html.tagName = "HTML";
        html.ownerDocument = this;
        this.documentElement = html;
        this._topElement = html;
        this.appendChild(this.documentElement);
        var body = new HTMLBodyElement();
        body.ownerDocument = this;
        this.body = body;
        this.documentElement.appendChild(this.body);
        this.head = new HTMLHeadElement();
        this.documentElement.appendChild(this.head);
        this.dispatchEvent = this._dispatchEvent.bind(this);
        this._frameEndEvt.bubbles = false;
        this.nodeType = 9;
        this.location = window.location;
        var _t = this;
        var temp;
    }
    setReferrer(s) {
        this.referrer = s;
    }
    uploadScript(d) {
        var _t = this;
        d.i = this._loading;
        this._loading++;
        if (d.src) {
            console.log("_downloadAysn:temp.src" + d.src);
            window.downloadfile(d.src, false, function (data) {
                d._stext = data + "\n//@ sourceURL=" + d.src;
                _t._downloadOk(d);
            }, function () {
                var e = new Event("error");
                e.target = e.currentTarget = d.obj;
                d.obj.onerror && d.obj.onerror(e);
            });
        }
        else {
            d._stext = d.text;
            _t._downloadOk(d);
        }
    }
    _downloadOk(d) {
        this.scriptTextList[d.i] = d;
        for (var i = this._evalNum, len = this.scriptTextList.length; i < len; i++) {
            var t = this.scriptTextList[i];
            if (!t)
                return;
            console.log(">>>>>>>>>>>>>>>eval" + t.src);
            var t1 = Date.now();
            window.eval(t._stext);
            console.log(">>>>>>>>>>>>>>>>>eval take time:" + (Date.now() - t1));
            var e = new Event("load");
            e.target = e.currentTarget = t.obj;
            t.obj.onload && t.obj.onload(e);
            this._evalNum++;
        }
        if (this._loading == this._evalNum) {
            this._loading = this._evalNum = 0;
            this.scriptTextList.length = 0;
        }
    }
    pickElement(screenx, screeny) {
        return this._topElement;
    }
    createElement(tagName) {
        tagName = tagName.toLowerCase();
        var f = this.createMap.get(tagName);
        var ret = null;
        if (f) {
            ret = f.call(this);
        }
        else {
            return new div();
        }
        return ret;
    }
    createElementNS(tagName) {
        return this.createElement(tagName);
    }
    create_div() {
        var ret = new HTMLDivElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_img() {
        var ret = new HTMLImageElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_canvas() {
        var ret = new HTMLCanvasElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_vbitmap() {
        return new ConchVirtualBitmap();
    }
    create_audio() {
        var ret = new HTMLAudioElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_input() {
        var rs = new HTMLInputElement();
        rs.ownerDocument = this;
        return rs;
    }
    create_video() {
        var ret = new HTMLVideoElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_script() {
        var ret = new HTMLScriptElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_meta() {
        var ret = new HTMLMetaElement();
        ret.ownerDocument = this;
        return ret;
    }
    createDocumentFragment() {
        return new HTMLDivElement();
    }
    onframeend() {
        this._frameEndEvt.eventPhase = Event.AT_TARGET;
        super._fireEventListeners(this._frameEndEvt);
    }
    _dispatchEvent(evt) {
        var ancestores = null;
        if (evt.target)
            ancestores = evt.target.getAncestorsNode();
        if (ancestores == null || ancestores.length == 0) {
            return super._dispatchEvent(evt);
        }
        var ancLen = ancestores.length;
        evt.eventPhase = Event.CAPTURING_PHASE;
        var stop = (function () {
            window.dispatchEvent(evt);
            if (evt._propagationStopped)
                return true;
            for (var i = ancLen - 1; i >= 0; i--) {
                var cnode = ancestores[i];
                evt.currentTarget = cnode;
                cnode.fireEventListeners(evt);
                if (evt._propagationStopped) {
                    return true;
                }
            }
            return false;
        })();
        if (!stop) {
            evt.eventPhase = Event.AT_TARGET;
            evt.currentTarget = evt.target;
            stop = (function () {
                evt.target.fireEventListeners(evt);
                if (evt._propagationStopped)
                    return true;
                return false;
            })();
        }
        if (!stop && !evt.cancelable) {
            evt.eventPhase = Event.BUBBLING_PHASE;
            for (var i = 0; i < ancLen; i++) {
                var cnode = ancestores[i];
                evt.currentTarget = cnode;
                cnode.fireEventListeners(evt);
                if (evt._propagationStopped) {
                    stop = true;
                    break;
                }
            }
            if (!stop) {
                evt.currentTarget = window;
                window.dispatchEvent(evt);
            }
        }
        if (evt.preventDefault) {
        }
        return true;
    }
    set cookie(v) {
        var t = _Cookie.addCookie(v);
        if (t && _Cookie.pushCookie(t)) {
            _Cookie.flush();
        }
    }
    get cookie() {
        return _Cookie.toLocalString();
    }
    loadCookie() {
        this._cookiePath = window.localStorage.fileNamePre + "_cookie.txt";
        var temp = readFileSync(this._cookiePath, "utf8");
        _Cookie.init(temp);
        return true;
    }
    open(url, name, features, replace) {
        throw 'not implements';
    }
    getElementsByTagName(tagname) {
        var d = new NodeList();
        if ("body" == tagname)
            d.push(this.body);
        else if ("head" == tagname)
            d.push(this.head);
        return d;
    }
    onkeydown(ev) {
    }
    onkeypress(ev) {
    }
    onkeyup(ev) {
    }
    onmousedown(ev) {
    }
    onmousemove(ev) { }
    onmouseout(ev) { }
    onmouseover(ev) { }
    onmouseup(ev) { }
    onmousewheel(ev) { }
    ontouchcancel(ev) { }
    ontouchend(ev) { }
    ontouchmove(ev) { }
    ontouchstart(ev) { }
    getElementById(elementId) {
        for (var i = 0, sz = this.all.length; i < sz; i++) {
            var c = this.all[i];
            if (c.id === elementId)
                return c;
        }
        return null;
    }
    getElementsByClassName(classNames) {
        throw 'not implemented';
    }
    getElementsByName(name) {
        return document._elements[name] || [];
    }
}
applyMixins(Document, [Node, GlobalEventHandlers, NodeSelector, DocumentEvent]);
class CloseEvent extends Event {
    constructor() {
        super('close');
    }
    initCloseEvent(typeArg, canBubbleArg, cancelableArg, wasCleanArg, codeArg, reasonArg) {
    }
}
class MessageEvent extends Event {
    constructor(type, eventInitDict) {
        super(type);
    }
    initMessageEvent(typeArg, canBubbleArg, cancelableArg, dataArg, originArg, lastEventIdArg, sourceArg) {
    }
}
class WebSocket extends EventTarget {
    constructor(url) {
        super();
        this._nativeObj = null;
        this._nativeObj = new ConchWebSocket(url);
        this._nativeObj._onmessage = (data) => {
            var evt = new MessageEvent('message');
            evt.data = data;
            evt.target = evt.currentTarget = this;
            this.onmessage && this.onmessage(evt);
        };
    }
    get binaryType() {
        return this._nativeObj.binaryType;
    }
    set binaryType(b) {
        this._nativeObj.binaryType = b;
    }
    get timegap() {
        return this._nativeObj.timegap;
    }
    set onopen(f) {
        this._nativeObj.onopen = function () {
            var e = new Event("open");
            e.target = e.currentTarget = this;
            f(e);
        };
    }
    set onclose(f) {
        this._nativeObj.onclose = f;
    }
    set onerror(f) {
        this._nativeObj.onerror = f;
    }
    close() {
        this._nativeObj.close();
    }
    send(msg) {
        this._nativeObj.send(msg);
    }
    get readyState() {
        return this._nativeObj.readyState;
    }
}
WebSocket.CLOSED = 3;
WebSocket.CLOSING = 2;
WebSocket.CONNECTING = 0;
WebSocket.OPEN = 1;
window.WebSocket = WebSocket;
class Socket extends EventTarget {
    constructor(url) {
        super();
        this._nativeObj = null;
        this._nativeObj = new ConchSocket(url);
        this._nativeObj._onmessage = (data) => {
            var evt = new MessageEvent('message');
            evt.data = data;
            evt.target = evt.currentTarget = this;
            this.onmessage && this.onmessage(evt);
        };
    }
    get binaryType() {
        return this._nativeObj.binaryType;
    }
    set binaryType(b) {
        this._nativeObj.binaryType = b;
    }
    get timegap() {
        return this._nativeObj.timegap;
    }
    set onopen(f) {
        this._nativeObj.onopen = function () {
            var e = new Event("open");
            e.target = e.currentTarget = this;
            f(e);
        };
    }
    set onclose(f) {
        this._nativeObj.onclose = f;
    }
    set onerror(f) {
        this._nativeObj.onerror = f;
    }
    close() {
        this._nativeObj.close();
    }
    send(msg) {
        this._nativeObj.send(msg);
    }
    get readyState() {
        return this._nativeObj.readyState;
    }
}
Socket.CLOSED = 3;
Socket.CLOSING = 2;
Socket.CONNECTING = 0;
Socket.OPEN = 1;
window.Socket = Socket;
class DOMParser {
    constructor() {
        this._parser = new _DOMParser();
    }
    set src(s) {
        this._src = location.resolve(s);
        this._parser.src = this._src;
    }
    get src() {
        return this._src;
    }
    set onload(callback) {
        this._parser._onload = callback;
        this._parser.onload = this.nativeObjOnload;
    }
    nativeObjOnload() {
        this._onload();
    }
    get onload() {
        return this._parser._onload;
    }
    set onerror(callback) {
        this._parser._onerror = callback;
        this._parser.onerror = this.nativeObjOnerror;
    }
    nativeObjOnerror() {
        this._onerror();
    }
    get onerror() {
        return this._parser._onerror;
    }
    static initXMl(xml) {
        var result;
        if (!xml) {
            var temp = new _jsXmlNode();
            temp.nodeName = "parsererror";
            temp.textContent = "parsererror error";
            result = new _jsXmlNode();
            result.childNodes[0] = temp;
            return result;
        }
        result = new _jsXmlNode();
        result.nodeName = xml.nodeName;
        result.nodeValue = xml.nodeValue;
        result.nodeType = 1;
        if (result.nodeName == "#cdata-section") {
            result.nodeType = 3;
            result.nodeName = "#text";
        }
        result.textContent = xml.textContent;
        var attrs = xml.attributes;
        for (var i = 0, sz1 = attrs.length; i < sz1; i++) {
            var attr = attrs[i];
            var key = attr.nodeName;
            var tempAttr = new _jsXmlAttr(key, attr.nodeValue);
            result.attributes[i] = tempAttr;
            result.attributes[key] = tempAttr;
        }
        var childs = xml.childNodes;
        for (var i = 0, sz1 = childs.length; i < sz1; i++) {
            var chd = childs[i];
            var nodeName = chd.nodeName;
            result.childNodes[i] = DOMParser.initXMl(chd);
        }
        return result;
    }
    parseFromString(s, t) {
        var xml = this._parser.parseFromString(s, t);
        var root = new _jsXmlDocument();
        root.childNodes[0] = DOMParser.initXMl(xml.childNodes[0]);
        return root;
    }
    getResult() {
        if (!this._result) {
            this._result = new _jsXmlDocument();
            this._result.childNodes[0] = DOMParser.initXMl(this._parser.getResult().childNodes[0]);
        }
        return this._result;
    }
}
window["DOMParser"] = DOMParser;
class _jsXmlAttr {
    constructor(key, value) {
        this.nodeName = key;
        this.nodeValue = this.textContent = value;
    }
    get value() {
        return this.nodeValue;
    }
}
class _jsXmlNode extends _jsXmlAttr {
    constructor() {
        super("", "");
        this.childNodes = [];
        this.childNodes["item"] = function (i) {
            return this[i];
        };
        this.attributes = [];
    }
    get firstChild() {
        return this.childNodes ? this.childNodes[0] : null;
    }
    getElementsByTagName(name) {
        var result = [];
        if (this.nodeName == name)
            result.push(this);
        else {
            for (var i = 0, n = this.childNodes.length; i < n; i++) {
                var son = this.childNodes[i];
                result = result.concat(son.getElementsByTagName(name));
            }
        }
        return result;
    }
    getAttribute(name) {
        var attr = this.attributes[name];
        return attr ? attr["nodeValue"] : "";
    }
}
class _jsXmlDocument extends _jsXmlNode {
}
var GL_CAPS;
(function (GL_CAPS) {
    GL_CAPS[GL_CAPS["NONE"] = 0] = "NONE";
    GL_CAPS[GL_CAPS["TEXTURE_COMPRESSION_PVR"] = 2] = "TEXTURE_COMPRESSION_PVR";
    GL_CAPS[GL_CAPS["TEXTURE_COMPRESSION_ETC1"] = 4] = "TEXTURE_COMPRESSION_ETC1";
    GL_CAPS[GL_CAPS["TEXTURE_COMPRESSION_ETC2"] = 8] = "TEXTURE_COMPRESSION_ETC2";
    GL_CAPS[GL_CAPS["TEXTURE_TPG"] = 32] = "TEXTURE_TPG";
})(GL_CAPS || (GL_CAPS = {}));
var _window = new _Window();
window.getComputedStyle = function (ele, parm) {
    return null;
};
window.pageXOffset = window.pageYOffset = 0;
window.localStorage = new Storage();
window.sessionStorage = new WindowSessionStorage();
var location = window.location = new Location;
var console = window.console = new Console;
window.addEventListener = _window.addEventListener.bind(_window);
window.removeEventListener = _window.removeEventListener.bind(_window);
window.dispatchEvent = _window.dispatchEvent.bind(_window);
window.document = new Document();
window.layaDoc = window.document;
window.crypto = new Crypto();
window.devicePixelRatio = 1.0;
var Image = window.Image = HTMLImageElement;
var Audio = window.Audio = HTMLAudioElement;
window.requestAnimationFrame = requestAnimationFrame;
window.cancelAnimationFrame = cancelAnimationFrame;
var parent = window.parent = window;
var frames = window.frames = null;
var navigator = window.navigator = new Navigator();
window.open = (url, target, features, replace) => {
    createProcess('scripts/index.js', url);
    return this;
};
var onload = window.onload = null;
window.ConchVirtualBitmap = ConchVirtualBitmap;
function printstack() {
    var e = new Error();
    alert(e.stack);
}
var div = HTMLDivElement;
window.layabox = {
    devinfo: (function getDevInfo() {
        var devi = conchConfig.getDeviceInfo();
        window.console.log(devi);
        return JSON.parse(devi);
    })(),
    getDeviceInfo: function () { return this.devinfo; }
};
window.layaHtml5 = { File: File };
var _$innerWidth = getInnerWidth();
var _$innerHeight = getInnerHeight();
Object.defineProperty(window, 'innerWidth', { get: function () { return _$innerWidth; } });
Object.defineProperty(window, 'innerHeight', { get: function () { return _$innerHeight; } });
conch.setOnResize(function (w, h) {
    _$innerWidth = w;
    _$innerHeight = h;
    window.console.log(">>>>>>>>>>>>>>>>innerWidth:" + _$innerWidth + "innerHeight:" + _$innerHeight);
    var evt = new UIEvent('resize');
    evt.view = window;
    document._dispatchEvent(evt);
    window.dispatchEvent(evt);
});
conch.config = conchConfig;
class Screen {
    get width() {
        return _$innerWidth;
    }
    get height() {
        return _$innerHeight;
    }
}
window.screen = new Screen();
window.onresize = function (e) {
};
conch.onerror = function (message, filename, lineno, colno, error) {
    if (window.onerror) {
        var ln = decodeTemp(lineno);
        var cn = decodeTemp(colno);
        var er = decodeTemp(error);
        var mg = decodeTemp(message);
        var fn = decodeTemp(filename);
        var e = {
            message: decodeTemp(message),
            stack: er,
            name: ""
        };
        window.onerror(mg == "undefined" ? undefined : mg, fn == "undefined" ? undefined : fn, ln != "undefined" ? parseInt(ln) : undefined, cn != "undefined" ? parseInt(cn) : undefined, e);
    }
};
Object.defineProperty(window, 'onerror', { set: function (fun) {
        conch.__onerror = fun;
        showAlertOnJsException(false);
    }, get: function () {
        return conch.__onerror;
    } });
var document = window.document;
var addEventListener = window.addEventListener.bind(this);
var dispatchEvent = window.dispatchEvent.bind(this);
var removeEventListener = window.removeEventListener.bind(this);
var clearInterval = window.clearInterval = _window.clearInterval;
var clearTimeout = window.clearTimeout = _window.clearTimeout;
var setInterval = window.setInterval = _window.setInterval;
var setTimeout = window.setTimeout = _window.setTimeout;
Object.defineProperty(window, 'runtime', { get: function () { return true; } });
window.postRuntimeMessage = function (d) {
    if (typeof (d) == "object")
        d = JSON.stringify(d);
    d = decodeURIComponent(d);
    var e = new MessageEvent('message');
    e.data = JSON.parse(d);
    e.target = window;
    var s = new RegExp("(http|file|https)://([^/:]*)(:(\\d+)|)([^?]+)(.*|)");
    var rs = s.exec(document.referrer);
    if (rs) {
        e.origin = rs[1] + "://" + rs[2] + rs[3];
    }
    window.dispatchEvent(e);
};
window.SetupWebglContext = function () { };
window.downloadfile = function (url, force, onok, onerr) {
    if (force) {
        url = (function (url) {
            var ret = url;
            if (url.indexOf('?') < 0) {
                ret = url + '?rnd=' + Math.random();
            }
            else {
                ret = url + '&downloadrnd' + Math.random().toString().substr(2) + '=1';
            }
            return ret;
        })(url);
    }
    var file = new window.layaHtml5.File(url);
    var filereader = new FileReader();
    filereader.onload = function () { onok && onok(filereader.result); };
    filereader.onerror = function () { onerr && onerr(); };
    filereader.readAsText(file);
};
if (window.navigator.platform != "windows")
    window["ontouchstart"] = null;
window['GL_CAPS'] = GL_CAPS;
window.focus = function () { };
var nMem = conchConfig.getTotalMem();
if (nMem <= 524288) {
    conchConfig.atlasNum = 10;
    conchConfig.maxTextureMemSize = 64 * 1024 * 1024;
}
else if (nMem > 524288 && nMem <= 1048576) {
    conchConfig.atlasNum = 16;
    conchConfig.maxTextureMemSize = 84 * 1024 * 1024;
}
else if (nMem > 1048576) {
    conchConfig.atlasNum = 20;
    conchConfig.maxTextureMemSize = 128 * 1024 * 1024;
}
conch.showFPS = function (x, y) {
    window["conchFps"] = new ConchFps("#ffffff", 32, x, y);
};
(function () {
    'use strict';
    class AppInfo {
    }
    ;
    var appobj = null;
    try {
        appobj = JSON.parse(conch.readFileFromAsset('app.json', 'utf8'));
        if (appobj) {
            require(appobj.mainjs);
        }
    }
    catch (e) {
        require('index');
    }
})();

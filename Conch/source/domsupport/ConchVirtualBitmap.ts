/*
class ConchVirtualBitmap_es6 extends _ConchVirtualBitmap {
    private _img: image;
    constructor() {
        super();
    }
	setImage(img: image) {
        if (img instanceof HTMLImageElement)
            this._setImage((<HTMLImageElement><any>img)._nativeObj)
        else
			this._setImage(img);
		this._img = img;
	}
	getImage() {
		return this._img;
	}
	clone() {
        //先用原来的js的，再想怎么整理
		var result = <ConchVirtualBitmap><any>(this._clone());
		result._img = this._img;
		result.setImage = function (img) {
			this._setImage(img);
			this._img = img;
		}
		result.getImage = function () {
			return this._img;
		}
		return result;
	}
}
*/
class ConchVirtualBitmap {

}


package laya.game.wrapper;

import laya.game.browser.LayaWrapper;

public class LayaWrapperFactroy {
	public static ILayaLibWrapper createLayaWrapper(){
		return new LayaWrapper();
	}
}

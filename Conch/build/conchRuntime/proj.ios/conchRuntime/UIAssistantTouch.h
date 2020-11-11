
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@class UIAssistantTouch;

@interface UIAssistantTouchMainButton : UIButton
{
    UIAssistantTouch* m_pTouchView;
    UIPanGestureRecognizer* m_pPan;
}
-(id)initWithFrame:(CGRect)frame touchView:(UIAssistantTouch*)view;
@end

@interface UIAssistantTouch : UIView
{
    GLKView* m_pGLView;
    @public UIImageView* m_pPanelView;
    @public UIAssistantTouchMainButton* m_pMainButton;
    
    UIImage* m_pImageMainButtonNormal;
    UIImage* m_pImageMainButtonLeft;
    UIImage* m_pImageMainButtonRight;
}
-(void)showButtonPanel:(bool)bShow;
-(id)initWithParentView:(GLKView*)pGLView;
-(void)buttonClick:(id)sender;
-(void)resetPosition;
-(void)buttonClickBack:(id)sender;
-(void)buttonClickRefresh:(id)sender;
-(void)buttonClickDesktop:(id)sender;
-(void)onSetScreenOrientation;
@end





#import "UIAssistantTouch.h"
#import <objc/runtime.h>
#import "conchRuntime.h"
#import "../../../../source/conch/JCScrpitRuntime.h"
#import "conchConfig.h"
#import <UIKit/UIAlertController.h>

@implementation UIAssistantTouch

-(void)onSetScreenOrientation
{
    CGPoint* pResolution = [conchRuntime GetIOSConchRuntime]->m_pResolution;
    self.frame = CGRectMake(0,0,pResolution->x,pResolution->y);
}
-(void)buttonClick:(id)sender
{
    CGPoint center = m_pMainButton.center;

    CGFloat distance = 0.5f*(m_pMainButton.bounds.size.width + m_pPanelView.bounds.size.width) + m_pMainButton.bounds.size.width * 0.02f;

    if (m_pMainButton.center.x < self.bounds.size.width * 0.5f)
    {
        m_pPanelView.center = CGPointMake(center.x + distance,center.y);
    }
    else
    {
        m_pPanelView.center = CGPointMake(center.x - distance,center.y);
    }
    [self updateMainButtonImage];
    m_pMainButton.selected = !m_pMainButton.selected;
    m_pPanelView.hidden = !m_pMainButton.selected;
}

-(void)buttonClickBack:(id)sender
{
    laya::JCScriptRuntime::s_JSRT->jsUrlback();
}
-(void)buttonClickRefresh:(id)sender
{
    laya::JCScriptRuntime::s_JSRT->jsReloadUrl();
}
-(void)buttonClickDesktop:(id)sender
{
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"版本号" message:[conchConfig GetInstance]->m_sAppVersion delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
}
- (UIImage *)imagesNamedFromCustomBundle:(NSString *)imgName
{
    NSString *bundlePath = [[NSBundle mainBundle].resourcePath stringByAppendingPathComponent:@"libconchRuntime.bundle"];
    NSString *img_path = [bundlePath stringByAppendingPathComponent:imgName];
    return [UIImage imageWithContentsOfFile:img_path];
}
-(void)updateMainButtonImage
{
    [m_pMainButton setBackgroundImage:m_pImageMainButtonNormal forState:UIControlStateNormal];
    if (m_pMainButton.center.x < self.bounds.size.width * 0.5f)
    {
        [m_pMainButton setBackgroundImage:m_pImageMainButtonLeft forState:UIControlStateSelected];
    }
    else
    {
        [m_pMainButton setBackgroundImage:m_pImageMainButtonRight forState:UIControlStateSelected];
    }
}

-(id)initWithParentView:(GLKView*)pGLView
{
    m_pGLView = pGLView;
    CGPoint* pResolution = [conchRuntime GetIOSConchRuntime]->m_pResolution;
    CGRect kRect = CGRectMake( 0,0,pResolution->x,pResolution->y );
    //self = [super initWithFrame:CGRectMake(0, 0,m_pGLView.frame.size.width, m_pGLView.frame.size.height)];
    self = [super initWithFrame:kRect];
    if( self != nil )
    {
        self.layer.cornerRadius = 4.0f;
        self.clipsToBounds = YES;
        [m_pGLView addSubview:self];
        
        m_pImageMainButtonNormal = [self imagesNamedFromCustomBundle:@"small_view.png"];
        m_pImageMainButtonLeft = [self imagesNamedFromCustomBundle:@"left.png"];
        m_pImageMainButtonRight = [self imagesNamedFromCustomBundle:@"right.png"];
        
        //m_pMainButton = [[UIAssistantTouchMainButton alloc] initWithFrame:frame];
        m_pMainButton = [[UIAssistantTouchMainButton alloc] initWithFrame:CGRectMake(0,0,m_pImageMainButtonNormal.size.width,m_pImageMainButtonNormal.size.height) touchView:self];
     
        m_pMainButton.center = CGPointMake(0.5f*m_pMainButton.bounds.size.width, 100);
        [self updateMainButtonImage];
        
        [m_pMainButton addTarget:self action:@selector(buttonClick:) forControlEvents:UIControlEventTouchUpInside];

        [self addSubview:m_pMainButton];
        
        UIImage* pImagePanelbkg = [self imagesNamedFromCustomBundle:@"background.png"];

        //m_pPanelView = [[UIView alloc] initWithFrame:frame];
        m_pPanelView = [[UIImageView alloc] initWithFrame:CGRectMake(0,0,pImagePanelbkg.size.width,pImagePanelbkg.size.height)];
        m_pPanelView.hidden = TRUE;
        m_pPanelView.image = pImagePanelbkg;
        m_pPanelView.userInteractionEnabled = TRUE;
        [self addSubview:m_pPanelView];
        
        UIImage* pImageButtonBack = [self imagesNamedFromCustomBundle:@"back.png"];
        UIImage* pImageButtonRefresh = [self imagesNamedFromCustomBundle:@"plush.png"];
        UIImage* pImageButtonDesktop = [self imagesNamedFromCustomBundle:@"cancel.png"];
        
        float offset = pImagePanelbkg.size.width * 0.1f;
        float width = pImageButtonBack.size.width;
        float height = pImageButtonBack.size.height;
        float distance = (pImagePanelbkg.size.width - 2 * offset) / 3.0f;
        
        UIButton* pButtonBack = [[UIButton alloc] initWithFrame:CGRectMake(0,0,width,height)];
        pButtonBack.titleEdgeInsets = UIEdgeInsetsMake(44.0f, 0.0f, 0.0f, 0.0f);
        //[pButtonBack setTitle:@"后退" forState:UIControlStateNormal];
        [pButtonBack setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        pButtonBack.titleLabel.font = [UIFont systemFontOfSize:13.0];
        [pButtonBack setBackgroundImage:pImageButtonBack forState:UIControlStateNormal];
        pButtonBack.center = CGPointMake(offset + distance*0.5f,m_pPanelView.center.y);
        [pButtonBack addTarget:self action:@selector(buttonClickBack:) forControlEvents:UIControlEventTouchUpInside];
        [m_pPanelView addSubview:pButtonBack];
        
        UIButton* pButtonRefresh = [[UIButton alloc] initWithFrame:CGRectMake(0,0,width,height)];
        pButtonRefresh.titleEdgeInsets = UIEdgeInsetsMake(44.0f, 0.0f, 0.0f, 0.0f);
        //[pButtonRefresh setTitle:@"刷新" forState:UIControlStateNormal];
        [pButtonRefresh setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        pButtonRefresh.titleLabel.font = [UIFont systemFontOfSize:13.0];
        [pButtonRefresh setBackgroundImage:pImageButtonRefresh forState:UIControlStateNormal];
        pButtonRefresh.center = CGPointMake(offset + distance*1.5f,m_pPanelView.center.y);
        [pButtonRefresh addTarget:self action:@selector(buttonClickRefresh:) forControlEvents:UIControlEventTouchUpInside];
        [m_pPanelView addSubview:pButtonRefresh];
        
        UIButton* pButtonDesktop = [[UIButton alloc] initWithFrame:CGRectMake(0,0,width,height)];
        pButtonDesktop.titleEdgeInsets = UIEdgeInsetsMake(44.0f, 0.0f, 0.0f, 0.0f);
        //[pButtonDesktop setTitle:@"版本" forState:UIControlStateNormal];
        [pButtonDesktop setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        pButtonDesktop.titleLabel.font = [UIFont systemFontOfSize:13.0];
        [pButtonDesktop setBackgroundImage:pImageButtonDesktop forState:UIControlStateNormal];
        pButtonDesktop.center = CGPointMake(offset + distance*2.5f,m_pPanelView.center.y);
        [pButtonDesktop addTarget:self action:@selector(buttonClickDesktop:) forControlEvents:UIControlEventTouchUpInside];
        [m_pPanelView addSubview:pButtonDesktop];
    
    }
    return self;
    
}
-(void)resetPosition
{
    m_pMainButton.center = CGPointMake(m_pMainButton.bounds.size.width*0.5f, 100);
    [self updateMainButtonImage];
    m_pMainButton.selected = FALSE;
    m_pPanelView.hidden = TRUE;
}
-(void)showButtonPanel:(bool)bShow
{
    [self updateMainButtonImage];
    m_pPanelView.hidden = !bShow;
    m_pMainButton.selected = bShow;
}
-(id)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView* hitView = [super hitTest:point withEvent:event];
    if (hitView == self)
    {
        [self showButtonPanel:false];
        return nil;
    }
    return hitView;
}
@end

@implementation UIAssistantTouchMainButton
-(id)initWithFrame:(CGRect)frame touchView:(UIAssistantTouch*)view
{
    self = [super initWithFrame:frame];
    if( self != nil )
    {
        m_pTouchView = view;
        m_pPan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
        [self addGestureRecognizer:m_pPan];
    }
    return self;
    
}

-(void)pan:(UIPanGestureRecognizer*)gesture
{
    if (!self.superview )
        return;
    
    [m_pTouchView updateMainButtonImage];
    m_pTouchView->m_pMainButton.selected = FALSE;
    m_pTouchView->m_pPanelView.hidden = TRUE;
    
    CGPoint center = [gesture locationInView:self.superview];
        
    float halfx=CGRectGetMidX(self.bounds);
        
    center.x=MAX(halfx,center.x);
    center.x=MIN(self.superview.bounds.size.width-halfx,center.x);
        
    float halfy=CGRectGetMidY(self.bounds);
    center.y=MAX(halfy,center.y);
    center.y=MIN(self.superview.bounds.size.height-halfy,center.y);
        
    self.center=center;

    if (gesture.state == UIGestureRecognizerStateEnded){
    
        float x=self.center.x;
        float y=self.center.y;
        
        float w=self.superview.frame.size.width;
    
        if (x < w * 0.5f){
            x=self.frame.size.width * 0.5f;
        }
        else{
            x=w-self.frame.size.width * 0.5f;
        }
        [UIView animateWithDuration:0.25 animations:^(void){
            self.center=CGPointMake(x,y);
            } completion:^(BOOL finished){
            [m_pTouchView updateMainButtonImage];
        }];
    }
}

@end
 

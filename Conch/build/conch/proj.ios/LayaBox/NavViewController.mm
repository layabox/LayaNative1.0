#import "NavViewController.h"

@implementation NavViewController

- (BOOL)shouldAutorotate{
    //return [[self.viewControllers lastObject]shouldAutorotate];
    return YES;
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
    return [[self.viewControllers lastObject]supportedInterfaceOrientations];
    //return [conchConfig GetInstance]->m_nOrientationType;
}
- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation{
    return [[self.viewControllers lastObject] preferredInterfaceOrientationForPresentation];
}

@end

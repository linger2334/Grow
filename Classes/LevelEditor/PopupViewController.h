//
//  PopupViewController.h
//  Grow
//
//  Created by wlg on 14-9-14.
//
//

#import <Foundation/Foundation.h>

@interface PopupViewController : UIViewController

@property(nonatomic,strong)IBOutlet UIButton* backButton;
@property(nonatomic,assign)IBOutlet UIScrollView* scrollview;
-(IBAction)backEditor:(id)sender;

@end

//
//  PopupViewController.m
//  Grow
//
//  Created by wlg on 14-9-14.
//
//

#import "PopupViewController.h"

@implementation PopupViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
    
}


-(IBAction)backEditor:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}


@end

//
//  PopupViewController.m
//  Grow
//
//  Created by wlg on 14-9-14.
//
//

#import "PopupViewController.h"

@implementation PopupViewController

@synthesize scrollview;

-(void)viewDidLoad
{
    [super viewDidLoad];
    self.scrollview.contentSize = CGSizeMake(768, 1536);
    self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
    self.backButton.frame = CGRectMake(280, 900, 212, 110);
    [self.view addSubview:self.backButton];
}


-(IBAction)backEditor:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)dealloc
{
    self.backButton = nil;
    [super dealloc];
}

@end

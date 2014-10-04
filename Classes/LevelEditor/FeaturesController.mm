//
//  FeaturesController.m
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//
#import "FeaturesController.h"
#include "GameManager.h"
#include "LevelEditor.h"

@implementation FeaturesController

@synthesize _pickerView;

-(id)init:(int)localzorder
{
    self = [super init];
    if(self)
    {
        localZorder = localzorder;
        gameManager = GameManager::getInstance();
        editor_width = gameManager->editor_width;
        editor_height = gameManager->editor_height;
        editor_contentscale = gameManager->editor_contentscale;
    }
    
    return self;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    CGRect frame = CGRectMake(0, 0, editor_width*0.7,editor_height*0.7);
    [self.view setBackgroundColor:[UIColor whiteColor]];
    
    CGRect btnCancelFrame = CGRectMake(0.1*frame.size.width, 0.05*frame.size.height,0.1*frame.size.width, 0.05*frame.size.height);
    UIButton* cancelButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [cancelButton setFrame:btnCancelFrame];
    [cancelButton addTarget:self action:@selector(pickerHideOnCancel) forControlEvents:UIControlEventTouchUpInside];
    cancelButton.backgroundColor = [UIColor clearColor];
    [cancelButton setTitle:@"Cancel" forState:UIControlStateNormal];
    [self.view addSubview:cancelButton];
    
    CGRect btnOKFrame = CGRectMake(0.8*frame.size.width, 0.05*frame.size.height,0.1*frame.size.width, 0.05*frame.size.height);
    UIButton* okButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [okButton setFrame:btnOKFrame];
    [okButton addTarget:self action:@selector(pickerHideOnOK) forControlEvents:UIControlEventTouchUpInside];
    okButton.backgroundColor = [UIColor clearColor];
    [okButton setTitle:@"OK" forState:UIControlStateNormal];
    [self.view addSubview:okButton];
    
    CGRect pickerFrame = CGRectMake(0.2*frame.size.width, 0.1*frame.size.height, frame.size.width, frame.size.height*0.8);
    _pickerView = [[UIPickerView alloc] initWithFrame:pickerFrame];
    _pickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    _pickerView.showsSelectionIndicator = YES;
    _pickerView.delegate = self;
    _pickerView.dataSource = self;
    [_pickerView selectRow:localZorder/100 inComponent:0 animated:NO];
    [_pickerView reloadComponent:1];
    [_pickerView reloadComponent:2];
    [_pickerView selectRow:localZorder%100/10 inComponent:1 animated:NO];
    [_pickerView reloadComponent:2];
    [_pickerView selectRow:localZorder%10 inComponent:2 animated:NO];
    
    [self.view addSubview:_pickerView];
    [_pickerView release];
}

-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 3;
}

-(NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    int n = 0;
    int sum = gameManager->_levelEditor->_myViewController->_imageViews.size() - 1;
    switch (component) {
        case 0:
            n = sum/100 + 1;
            break;
        case 1:
            if([_pickerView selectedRowInComponent:0]<sum/100)
                n = 10;
            else
                n = sum%100/10 + 1;
            break;
        case 2:
            if ([_pickerView selectedRowInComponent:1]<sum%100/10)
                n = 10;
            else
                n = sum%10+1;
            break;
        default:
            break;
    }
    return n;
}

-(NSString*)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [NSString stringWithFormat:@"%d",row];
}

-(void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    switch (component) {
        case 0:
            [_pickerView reloadComponent:1];
            [_pickerView reloadComponent:2];
            break;
        case 1:
            [_pickerView reloadComponent:2];
            break;
        default:
            break;
    }
}

-(void)pickerHideOnCancel
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)pickerHideOnOK
{
    UIScrollView* scrollview = gameManager->_levelEditor->_myViewController->_scrollView;
    int preIndex = localZorder;
    int selectedIndex = 100*[_pickerView selectedRowInComponent:0] + 10*[_pickerView selectedRowInComponent:1] + 1*[_pickerView selectedRowInComponent:2];
    if (preIndex<selectedIndex) {
        [scrollview insertSubview:[scrollview.subviews objectAtIndex:preIndex] aboveSubview:[scrollview.subviews objectAtIndex:selectedIndex]];
    }else if(preIndex>selectedIndex){
        [scrollview insertSubview:[scrollview.subviews objectAtIndex:preIndex] belowSubview:[scrollview.subviews objectAtIndex:selectedIndex]];
    }
    [self dismissViewControllerAnimated:YES completion:nil];
    
}

@end

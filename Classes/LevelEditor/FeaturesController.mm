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

@synthesize _itemView;
@synthesize _hierarchyPickerView;

-(id)init:(ItemView*)itemview
{
    self = [super init];
    if(self)
    {
        _itemView = itemview;
        gameManager = GameManager::getInstance();
        width = 0.7*gameManager->editor_width;
        height = 0.7*gameManager->editor_height;
        contentscale = gameManager->editor_contentscale;
    }
    
    return self;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.view setBackgroundColor:[UIColor whiteColor]];
    //add cancel button
    CGRect btnCancelFrame = CGRectMake(0.1*width, 0.05*height,0.1*width, 0.05*height);
    UIButton* cancelButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [cancelButton setFrame:btnCancelFrame];
    [cancelButton addTarget:self action:@selector(pickerHideOnCancel) forControlEvents:UIControlEventTouchUpInside];
    cancelButton.backgroundColor = [UIColor clearColor];
    [cancelButton setTitle:@"Cancel" forState:UIControlStateNormal];
    [self.view addSubview:cancelButton];
    //add ok button
    CGRect btnOKFrame = CGRectMake(0.8*width, 0.05*height,0.1*width, 0.05*height);
    UIButton* okButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [okButton setFrame:btnOKFrame];
    [okButton addTarget:self action:@selector(pickerHideOnOK) forControlEvents:UIControlEventTouchUpInside];
    okButton.backgroundColor = [UIColor clearColor];
    [okButton setTitle:@"OK" forState:UIControlStateNormal];
    [self.view addSubview:okButton];
    //add hierarchypickerview
    int localZorder = [_itemView getSubviewIndex];
    CGRect hierarchyLabelFrame = CGRectMake(0.1*width, 0.1*height, 0.2*width, 0.1*height);
    UILabel* hierarchyLabel = [[UILabel alloc] initWithFrame:hierarchyLabelFrame];
    hierarchyLabel.text = @"Hierarchy:";
    hierarchyLabel.font = [UIFont systemFontOfSize:20];
    hierarchyLabel.textAlignment = NSTextAlignmentCenter;
    hierarchyLabel.textColor = [UIColor blackColor];
    [self.view addSubview:hierarchyLabel];
    [hierarchyLabel release];
    
    CGRect hierarchypickerFrame = CGRectMake(0.3*width, 0.04*height, width, 0.1*height);
    _hierarchyPickerView = [[UIPickerView alloc] initWithFrame:hierarchypickerFrame];
    _hierarchyPickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    _hierarchyPickerView.showsSelectionIndicator = YES;
    _hierarchyPickerView.delegate = self;
    _hierarchyPickerView.dataSource = self;
    [_hierarchyPickerView selectRow:localZorder/100 inComponent:0 animated:NO];
    [_hierarchyPickerView reloadComponent:1];
    [_hierarchyPickerView reloadComponent:2];
    [_hierarchyPickerView selectRow:localZorder%100/10 inComponent:1 animated:NO];
    [_hierarchyPickerView reloadComponent:2];
    [_hierarchyPickerView selectRow:localZorder%10 inComponent:2 animated:NO];
    
    [self.view addSubview:_hierarchyPickerView];
    [_hierarchyPickerView release];
    
    //add 
    
    
}

-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    if(pickerView == _hierarchyPickerView){
        return 3;
    }else{
        return 4;
    }
}

-(NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    int n = 0;
    int sum = gameManager->_levelEditor->_myViewController->itemViews.size() - 1;
    switch (component) {
        case 0:
            n = sum/100 + 1;
            break;
        case 1:
            if([_hierarchyPickerView selectedRowInComponent:0]<sum/100)
                n = 10;
            else
                n = sum%100/10 + 1;
            break;
        case 2:
            if ([_hierarchyPickerView selectedRowInComponent:1]<sum%100/10)
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
            [_hierarchyPickerView reloadComponent:1];
            [_hierarchyPickerView reloadComponent:2];
            break;
        case 1:
            [_hierarchyPickerView reloadComponent:2];
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
    int preIndex = [_itemView getSubviewIndex];
    int selectedIndex = 100*[_hierarchyPickerView selectedRowInComponent:0] + 10*[_hierarchyPickerView selectedRowInComponent:1] + 1*[_hierarchyPickerView selectedRowInComponent:2];
    if (preIndex<selectedIndex) {
        [scrollview insertSubview:[scrollview.subviews objectAtIndex:preIndex] aboveSubview:[scrollview.subviews objectAtIndex:selectedIndex]];
    }else if(preIndex>selectedIndex){
        [scrollview insertSubview:[scrollview.subviews objectAtIndex:preIndex] belowSubview:[scrollview.subviews objectAtIndex:selectedIndex]];
    }
    [self dismissViewControllerAnimated:YES completion:nil];
    
}

@end

//
//  FeaturesController.m
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//
#import "PropertyController.h"
#include "GameManager.h"
#include "LevelEditor.h"
#include "GameConstants.h"

@interface PropertyController(){
    UISwitch* animatedSwitch;
    UITextField* triggerTextField;
    UILabel* wDoubleDragonShowLabel;
    UILabel* absorptionShowLabel;
    UILabel* sinkSpeedShowLabel;
    UITextField* gapTextField;
    UILabel* startRateShowLabel;
}

@end



@implementation PropertyController

@synthesize _itemView;
@synthesize _hierarchyPickerView;
@synthesize _selectedIDs;

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
    [self addButtons];
    [self addHierarchyPickerView];
    [self addAnimatedSwitch];
    [self addTriggerTimeTextField];
    switch (_itemView->itemtype) {
        case DoubDragon_Anti:
            [self addWSlider];
            break;
        case DoubDragon_Clockwise:
            [self addWSlider];
            break;
        case Serpent_:
            [self addAbsorptionRateSlider];
            break;
        case Gear_Button:
            [self addSinkSpeedSlider];
            [self addCheckBoxes];
            break;
        case Gear_Gate:
            [self addGapTextField];
            [self addStartRateSlider];
            break;
        default:
            break;
    }
}

-(void)addButtons
{
    //add cancel button
    CGRect btnCancelFrame = CGRectMake(0.1*width, 0.05*height,0.1*width, 0.05*height);
    UIButton* cancelButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [cancelButton setFrame:btnCancelFrame];
    [cancelButton addTarget:self action:@selector(HideOnCancel) forControlEvents:UIControlEventTouchUpInside];
    cancelButton.backgroundColor = [UIColor clearColor];
    [cancelButton setTitle:@"Cancel" forState:UIControlStateNormal];
    [self.view addSubview:cancelButton];
    //add ok button
    CGRect btnOKFrame = CGRectMake(0.8*width, 0.05*height,0.1*width, 0.05*height);
    UIButton* okButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [okButton setFrame:btnOKFrame];
    [okButton addTarget:self action:@selector(HideOnOK) forControlEvents:UIControlEventTouchUpInside];
    okButton.backgroundColor = [UIColor clearColor];
    [okButton setTitle:@"OK" forState:UIControlStateNormal];
    [self.view addSubview:okButton];
}

-(void)addHierarchyPickerView
{
    int localZorder = [_itemView getSubviewIndex];
    CGRect hierarchyLabelFrame = CGRectMake(0.05*width, 0.1*height, 0.2*width, 0.1*height);
    UILabel* hierarchyLabel = [[UILabel alloc] initWithFrame:hierarchyLabelFrame];
    hierarchyLabel.text = @"Hierarchy:";
    hierarchyLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    hierarchyLabel.textAlignment = NSTextAlignmentCenter;
    hierarchyLabel.textColor = [UIColor blackColor];
    [self.view addSubview:hierarchyLabel];
    [hierarchyLabel release];
    
    CGRect hierarchypickerFrame = CGRectMake(0.25*width, 0.04*height, width, 0.1*height);
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
}

#pragma mark pickerview
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
        case 2:
            [_hierarchyPickerView reloadComponent:2];
            break;
        default:
            break;
    }
}

-(void)HideOnCancel
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)HideOnOK
{
    //hierarchy
    UIScrollView* scrollview = gameManager->_levelEditor->_myViewController->_scrollView;
    int preIndex = [_itemView getSubviewIndex];
    int selectedIndex = 100*[_hierarchyPickerView selectedRowInComponent:0] + 10*[_hierarchyPickerView selectedRowInComponent:1] + 1*[_hierarchyPickerView selectedRowInComponent:2];
    if (preIndex<selectedIndex) {
        [scrollview insertSubview:[scrollview.subviews objectAtIndex:preIndex] aboveSubview:[scrollview.subviews objectAtIndex:selectedIndex]];
    }else if(preIndex>selectedIndex){
        [scrollview insertSubview:[scrollview.subviews objectAtIndex:preIndex] belowSubview:[scrollview.subviews objectAtIndex:selectedIndex]];
    }
    //animate
    _itemView->isAnimated = animatedSwitch.on;
    float triggerTime = static_cast<float>(atof([triggerTextField.text UTF8String]));
    if (triggerTime<0) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"严重错误!" message:@"非法输入,请检查!" delegate:nil cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
        [alert show];
        [alert release];
        return ;
    }
    _itemView->triggerTime = triggerTime;
    //features
    switch (_itemView->itemtype) {
        case DoubDragon_Anti:
        {
            float w = static_cast<float>(atof([wDoubleDragonShowLabel.text UTF8String]));
            ((Features_DoubleDragon*)_itemView->features)->w = w;
        }
            break;
        case DoubDragon_Clockwise:
        {
            float w = static_cast<float>(atof([wDoubleDragonShowLabel.text UTF8String]));
            ((Features_DoubleDragon*)_itemView->features)->w = w;
        }
            break;
        case Serpent_:
        {            
            float absorptionRate = static_cast<float>(atof([absorptionShowLabel.text UTF8String]));
            ((Features_Serpent*)_itemView->features)->absorptionRate = absorptionRate;
        }
            break;
        case Gear_Button:
        {
            int sinkSpeed = atoi([sinkSpeedShowLabel.text UTF8String]);
            ((Features_GearButton*)_itemView->features)->sinkSpeed = sinkSpeed;
            if(_selectedIDs.size()>1){
                UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"非法选择！" message:@"不能绑定多个门ID!" delegate:nil cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
                [alert show];
                [alert release];
                return;
            }else if (_selectedIDs.size()<1){
                Features_GearButton* feat = (Features_GearButton*)_itemView->features;
                if (feat->bindID != kDefaultGearButtonBindID) {
                    //解除绑定
                    for(ItemView* gate : gameManager->_levelEditor->_myViewController->_gates){
                        if (gate.tag == feat->bindID) {
                            gate.bindButton = nil;
                            feat->bindID = kDefaultGearButtonBindID;
                            break;
                        }
                    }
                }
            }else{
                Features_GearButton* feat = (Features_GearButton*)_itemView->features;
                //是否需要重新绑定
                if (feat->bindID != *_selectedIDs.begin()) {
                    //按钮解绑
                    if (feat->bindID != kDefaultGearButtonBindID) {
                        for (ItemView* gate : gameManager->_levelEditor->_myViewController->_gates) {
                            if (gate.tag == feat->bindID) {
                                gate.bindButton = nil;
                                feat->bindID = kDefaultGearButtonBindID;
                                break;
                            }
                        }
                    }
                    
                    //重新绑定
                    for (ItemView* gate : gameManager->_levelEditor->_myViewController->_gates) {
                        if (gate.tag == *_selectedIDs.begin()) {
                            //门解绑
                            if (gate.bindButton) {
                                ((Features_GearButton*)gate.bindButton->features)->bindID = kDefaultGearButtonBindID;
                                gate.bindButton = nil;
                            }
                            //重新绑定
                            gate.bindButton = _itemView;
                            feat->bindID = gate.tag;
                            break;
                        }
                    }
                }
            }
        }
            break;
        case Gear_Gate:
        {
            int gap = atoi([gapTextField.text UTF8String]);
            if (gap < 0) {
                UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"非法输入！" message:@"间隙不能为负,请返回重新修改！" delegate:nil cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
                [alert show];
                [alert release];
                return;
            }
            ((Features_GearGate*)_itemView->features)->gap = gap;
            
            int startRate = atoi([startRateShowLabel.text UTF8String]);
            ((Features_GearGate*)_itemView->features)->startRate = startRate;
        }
            break;
        default:
            break;
    }
    
    //
    [self dismissViewControllerAnimated:YES completion:nil];
    
}

-(void)addAnimatedSwitch
{
    CGRect switchLabelFrame = CGRectMake(0.05*width, 0.2*height, 0.2*width, 0.1*height);
    UILabel* switchLabel = [[[UILabel alloc] initWithFrame:switchLabelFrame] autorelease];
    switchLabel.text = @"Animated?:";
    switchLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    switchLabel.textAlignment = NSTextAlignmentCenter;
    switchLabel.textColor = [UIColor blackColor];
    [self.view addSubview:switchLabel];
    
    CGRect switchFrame = CGRectMake(0.35*width, 0.23*height, 0.25*width, 0.1*height);
    animatedSwitch = [[[UISwitch alloc] initWithFrame:switchFrame] autorelease];
    animatedSwitch.on = _itemView->isAnimated;
    [animatedSwitch addTarget:self action:@selector(animatedSwitchChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:animatedSwitch];
}

-(void)animatedSwitchChanged:(id)sender
{
    UISwitch* animSwitch = (UISwitch*)sender;
    BOOL isAnimated = animSwitch.isOn;
    [animSwitch setOn:isAnimated animated:YES];
}
//
-(void)addTriggerTimeTextField
{
    CGRect triggerLableFrame = CGRectMake(0.05*width, 0.3*height, 0.2*width, 0.1*height);
    UILabel* triggerLable = [[[UILabel alloc] initWithFrame:triggerLableFrame] autorelease];
    triggerLable.text = @"触发时间(s):";
    triggerLable.textAlignment = NSTextAlignmentCenter;
    triggerLable.textColor = [UIColor blackColor];
    triggerLable.font = [UIFont systemFontOfSize:kDefaultFontSize];
    [self.view addSubview:triggerLable];
    
    CGRect triggerTextFieldFrame = CGRectMake(0.35*width, 0.33*height, 0.3*width, 0.05*height);
    triggerTextField = [[[UITextField alloc] initWithFrame:triggerTextFieldFrame] autorelease];
    triggerTextField.borderStyle = UITextBorderStyleRoundedRect;
    triggerTextField.backgroundColor = [UIColor whiteColor];
    triggerTextField.placeholder = @"触发时间";
    triggerTextField.text = [NSString stringWithFormat:@"%f",_itemView->triggerTime];
    triggerTextField.font = [UIFont systemFontOfSize:kDefaultFontSize];
    triggerTextField.textColor = [UIColor blackColor];
    triggerTextField.textAlignment = NSTextAlignmentLeft;
    triggerTextField.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    triggerTextField.clearButtonMode = UITextFieldViewModeAlways;
    triggerTextField.autocorrectionType = UITextAutocorrectionTypeNo;
    triggerTextField.clearsOnBeginEditing = NO;
    triggerTextField.keyboardType = UIKeyboardTypeDecimalPad;
    triggerTextField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    triggerTextField.returnKeyType = UIReturnKeyDefault;
    triggerTextField.keyboardAppearance = UIKeyboardAppearanceDefault;
    triggerTextField.delegate = self;
    
    [self.view addSubview:triggerTextField];
}

-(BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    return YES;
}

-(void)textFieldDidBeginEditing:(UITextField *)textField
{
    
}

-(BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    return YES;
}

-(BOOL)textFieldShouldClear:(UITextField *)textField
{
    return YES;
}

-(BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [triggerTextField resignFirstResponder];
    return YES;
}

#pragma DoubleDragon
-(void)addWSlider
{
    CGRect wSliderLabelFrame = CGRectMake(0.05*width, 0.4*height, 0.29*width, 0.1*height);
    UILabel* wSliderLabel = [[[UILabel alloc] initWithFrame:wSliderLabelFrame] autorelease];
    wSliderLabel.text = @"RotateSpeed(°/s):";
    wSliderLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    wSliderLabel.textAlignment = NSTextAlignmentLeft;
    wSliderLabel.textColor = [UIColor blackColor];
    [self.view addSubview:wSliderLabel];
    
    CGRect wSliderFrame = CGRectMake(0.5*width, 0.4*height, 0.4*width, 0.1*height);
    UISlider* wSlider = [[[UISlider alloc] initWithFrame:wSliderFrame] autorelease];
    wSlider.minimumValue = 0.0;
    wSlider.maximumValue = 10*kDefaultDragonW;
    wSlider.value = ((Features_DoubleDragon*)_itemView->features)->w;
    [wSlider addTarget:self action:@selector(wSliderChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:wSlider];
    
    CGRect wSliderShowFrame = CGRectMake(0.35*width, 0.4*height, 0.15*width, 0.1*height);
    wDoubleDragonShowLabel = [[[UILabel alloc] initWithFrame:wSliderShowFrame] autorelease];
    wDoubleDragonShowLabel.text = [NSString stringWithFormat:@"%f",wSlider.value];
    wDoubleDragonShowLabel.textAlignment = NSTextAlignmentLeft;
    wDoubleDragonShowLabel.textColor = [UIColor blackColor];
    wDoubleDragonShowLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    [self.view addSubview:wDoubleDragonShowLabel];
    
}

-(void)wSliderChanged:(id)sender
{
    UISlider* wSlider = (UISlider*)sender;
    wDoubleDragonShowLabel.text = [NSString stringWithFormat:@"%f",wSlider.value];
    
}

#pragma Serpent
-(void)addAbsorptionRateSlider
{
    CGRect absorptionLabelFrame = CGRectMake(0.05*width, 0.4*height, 0.18*width, 0.1*height);
    UILabel* absorptionLabel = [[[UILabel alloc] initWithFrame:absorptionLabelFrame] autorelease];
    absorptionLabel.text = @"吸收速率:";
    absorptionLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    absorptionLabel.textAlignment = NSTextAlignmentCenter;
    absorptionLabel.textColor = [UIColor blackColor];
    [self.view addSubview:absorptionLabel];
    
    CGRect absorptionSliderFrame = CGRectMake(0.5*width, 0.4*height, 0.4*width, 0.1*height);
    UISlider* absorptionSlider = [[[UISlider alloc] initWithFrame:absorptionSliderFrame] autorelease];
    absorptionSlider.minimumValue = 0.0;
    absorptionSlider.maximumValue = 10.0;
    absorptionSlider.value = ((Features_Serpent*)(_itemView->features))->absorptionRate;
    [absorptionSlider addTarget:self action:@selector(absorptionSliderChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:absorptionSlider];
    
    CGRect absorptionShowFrame = CGRectMake(0.35*width, 0.4*height, 0.15*width, 0.1*height);
    absorptionShowLabel = [[[UILabel alloc] initWithFrame:absorptionShowFrame] autorelease];
    absorptionShowLabel.text = [NSString stringWithFormat:@"%f",absorptionSlider.value];
    absorptionShowLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    absorptionShowLabel.textAlignment = NSTextAlignmentLeft;
    absorptionShowLabel.textColor = [UIColor blackColor];
    [self.view addSubview:absorptionShowLabel];
}

-(void)absorptionSliderChanged:(id)sender
{
    UISlider* slider = (UISlider*)sender;
    absorptionShowLabel.text = [NSString stringWithFormat:@"%f",slider.value];
}

#pragma gearbutton
-(void)addSinkSpeedSlider
{
    CGRect sinkSpeedLabelFrame = CGRectMake(0.05*width, 0.4*height, 0.25*width, 0.1*height);
    UILabel* sinkSpeedLabel = [[[UILabel alloc] initWithFrame:sinkSpeedLabelFrame] autorelease];
    sinkSpeedLabel.text = @"sinkSpeed(pix/s):";
    sinkSpeedLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    sinkSpeedLabel.textAlignment = NSTextAlignmentCenter;
    sinkSpeedLabel.textColor = [UIColor blackColor];
    [self.view addSubview:sinkSpeedLabel];
    
    CGRect sinkSpeedSliderFrame = CGRectMake(0.45*width, 0.4*height, 0.5*width, 0.1*height);
    UISlider* sinkSpeedSlider = [[[UISlider alloc] initWithFrame:sinkSpeedSliderFrame] autorelease];
    sinkSpeedSlider.minimumValue = 1;
    sinkSpeedSlider.maximumValue = 100;
    sinkSpeedSlider.value = ((Features_GearButton*)(_itemView->features))->sinkSpeed;
    [sinkSpeedSlider addTarget:self action:@selector(sinkSpeedSliderChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:sinkSpeedSlider];
    
    CGRect sinkSpeedShowFrame = CGRectMake(0.35*width, 0.4*height, 0.1*width, 0.1*height);
    sinkSpeedShowLabel = [[[UILabel alloc] initWithFrame:sinkSpeedShowFrame] autorelease];
    sinkSpeedShowLabel.text = [NSString stringWithFormat:@"%d",static_cast<int>(sinkSpeedSlider.value)];
    sinkSpeedShowLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    sinkSpeedShowLabel.textAlignment = NSTextAlignmentLeft;
    sinkSpeedShowLabel.textColor = [UIColor blackColor];
    [self.view addSubview:sinkSpeedShowLabel];
}

-(void)sinkSpeedSliderChanged:(id)sender
{
    UISlider* slider = (UISlider*)sender;
    sinkSpeedShowLabel.text = [NSString stringWithFormat:@"%d",static_cast<int>(slider.value)];
}

-(void)addCheckBoxes
{
    UILabel* prompt = [[[UILabel alloc] initWithFrame:CGRectMake(0.05*width, 0.5*height, 0.8*width, 0.05*height)] autorelease];
    prompt.text = @"请选择您想绑定的ID号(绿色为未绑定，红色为已绑定）:";
    prompt.textAlignment = NSTextAlignmentLeft;
    prompt.textColor = [UIColor blackColor];
    prompt.font = [UIFont systemFontOfSize:kDefaultFontSize];
    [self.view addSubview:prompt];
    
    std::vector<ItemView*>& Gates = GameManager::getInstance()->_levelEditor->_myViewController->_gates;
    CGRect checkBoxFrame;
    for(int i = 0;i<Gates.size();i++){
        checkBoxFrame = CGRectMake(0, 0.55*height, 0.15*width, 0.05*height);
        checkBoxFrame.origin.x += i%5*0.2*width;
        checkBoxFrame.origin.y += i/5*0.1*height;
        UIButton* checkBox = [[[UIButton alloc] initWithFrame:checkBoxFrame] autorelease];
        checkBox.contentMode = UIViewContentModeLeft;
        [checkBox setImage:[UIImage imageNamed:@"checkbox_off.png"] forState:UIControlStateNormal];
        [checkBox setImage:[UIImage imageNamed:@"checkbox_on.png"] forState:UIControlStateSelected];
        [checkBox setTag:Gates.at(i).tag];
        [checkBox addTarget:self action:@selector(checkBoxSelected:) forControlEvents:UIControlEventTouchUpInside];
        
        UILabel* gateID = [[[UILabel alloc] initWithFrame:CGRectMake(0.05*width, 0, 0.1*width, 0.05*height)] autorelease];
        gateID.text = [NSString stringWithFormat:@"%d",Gates.at(i).tag];
        gateID.textAlignment = NSTextAlignmentRight;
        if (Gates.at(i).bindButton) {
            gateID.textColor = [UIColor redColor];
            if (Gates.at(i).bindButton == _itemView) {
                [checkBox setSelected:YES];
                _selectedIDs.insert(Gates.at(i).tag);
            }
            
        }else{
            gateID.textColor = [UIColor greenColor];
        }
        gateID.font = [UIFont systemFontOfSize:kDefaultFontSize];
        [checkBox addSubview:gateID];
        
        [self.view addSubview:checkBox];
    }
}

-(void)checkBoxSelected:(UIButton*)sender
{
    sender.selected = !sender.isSelected;
    if (sender.selected) {
        _selectedIDs.insert(sender.tag);
    }else{
        _selectedIDs.erase(sender.tag);
    }
}

#pragma geargate
-(void)addGapTextField
{
    CGRect gapLableFrame = CGRectMake(0.05*width, 0.4*height, 0.2*width, 0.1*height);
    UILabel* gapLable = [[[UILabel alloc] initWithFrame:gapLableFrame] autorelease];
    gapLable.text = @"gap(pix):";
    gapLable.textAlignment = NSTextAlignmentCenter;
    gapLable.textColor = [UIColor blackColor];
    gapLable.font = [UIFont systemFontOfSize:kDefaultFontSize];
    [self.view addSubview:gapLable];
    
    CGRect gapTextFieldFrame = CGRectMake(0.35*width, 0.43*height, 0.3*width, 0.05*height);
    gapTextField = [[[UITextField alloc] initWithFrame:gapTextFieldFrame] autorelease];
    gapTextField.borderStyle = UITextBorderStyleRoundedRect;
    gapTextField.backgroundColor = [UIColor whiteColor];
    gapTextField.placeholder = @"gap";
    gapTextField.text = [NSString stringWithFormat:@"%d",static_cast<Features_GearGate*>(_itemView->features)->gap];
    gapTextField.font = [UIFont systemFontOfSize:kDefaultFontSize];
    gapTextField.textColor = [UIColor blackColor];
    gapTextField.textAlignment = NSTextAlignmentLeft;
    gapTextField.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    gapTextField.clearButtonMode = UITextFieldViewModeAlways;
    gapTextField.autocorrectionType = UITextAutocorrectionTypeNo;
    gapTextField.clearsOnBeginEditing = NO;
    gapTextField.keyboardType = UIKeyboardTypeNumberPad;
    gapTextField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    gapTextField.returnKeyType = UIReturnKeyDefault;
    gapTextField.keyboardAppearance = UIKeyboardAppearanceDefault;
    gapTextField.delegate = self;
    
    [self.view addSubview:gapTextField];
    
}

-(void)addStartRateSlider
{
    CGRect startRateLabelFrame = CGRectMake(0.05*width, 0.5*height, 0.25*width, 0.1*height);
    UILabel* startRateLabel = [[[UILabel alloc] initWithFrame:startRateLabelFrame] autorelease];
    startRateLabel.text = @"startRate(pix/s):";
    startRateLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    startRateLabel.textAlignment = NSTextAlignmentCenter;
    startRateLabel.textColor = [UIColor blackColor];
    [self.view addSubview:startRateLabel];
    
    CGRect startRateSliderFrame = CGRectMake(0.45*width, 0.5*height, 0.5*width, 0.1*height);
    UISlider* startRateSlider = [[[UISlider alloc] initWithFrame:startRateSliderFrame] autorelease];
    startRateSlider.minimumValue = 10;
    startRateSlider.maximumValue = 500;
    startRateSlider.value = (int)((Features_GearGate*)(_itemView->features))->startRate;
    [startRateSlider addTarget:self action:@selector(startRateSliderChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:startRateSlider];
    
    CGRect startRateShowFrame = CGRectMake(0.35*width, 0.5*height, 0.1*width, 0.1*height);
    startRateShowLabel = [[[UILabel alloc] initWithFrame:startRateShowFrame] autorelease];
    startRateShowLabel.text = [NSString stringWithFormat:@"%d",static_cast<int>(startRateSlider.value)];
    startRateShowLabel.font = [UIFont systemFontOfSize:kDefaultFontSize];
    startRateShowLabel.textAlignment = NSTextAlignmentLeft;
    startRateShowLabel.textColor = [UIColor blackColor];
    [self.view addSubview:startRateShowLabel];
}

-(void)startRateSliderChanged:(id)sender
{
    UISlider* slider = (UISlider*)sender;
    startRateShowLabel.text = [NSString stringWithFormat:@"%d",static_cast<int>(slider.value)];
}

@end

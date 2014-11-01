//
//  FeaturesController.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#import <Foundation/Foundation.h>
#import "UIViewExtension.h"

class GameManager;

@interface PropertyController : UIViewController<UIPickerViewDelegate,UIPickerViewDataSource,UITextFieldDelegate>
{
    float width;
    float height;
    float contentscale;
    GameManager* gameManager;
}

-(id)init:(ItemView*)itemview;

@property(nonatomic,readonly)ItemView* _itemView;
@property(nonatomic,readonly) UIPickerView* _hierarchyPickerView;

@end

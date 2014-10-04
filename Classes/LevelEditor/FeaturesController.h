//
//  FeaturesController.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#import <Foundation/Foundation.h>

class GameManager;

@interface FeaturesController : UIViewController<UIPickerViewDelegate,UIPickerViewDataSource>
{
    int localZorder;
    float editor_width;
    float editor_height;
    float editor_contentscale;
    GameManager* gameManager;
}

-(id)init:(int)localzorder;

@property(nonatomic,readonly) UIPickerView* _pickerView;

@end

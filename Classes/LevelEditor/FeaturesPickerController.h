//
//  FeaturesPickerController.h
//  Grow
//
//  Created by 林纲 王 on 14-10-19.
//
//

#import <UIKit/UIKit.h>

typedef enum : NSUInteger {
    ITEM,
    TRIGGER,
    PATHPOINT
} ViewType;

@protocol FeaturesPickerDelegate

-(void)optionSelected:(NSString*)option;

@end

@interface FeaturesPickerController : UITableViewController{
    NSMutableArray* bindIDs;
}
- (instancetype)initWithStyle:(UITableViewStyle)style andUIView:(UIView*)uiview;

@property(nonatomic,strong)UIView* _pressedView;
@property(nonatomic,assign)ViewType viewType;
@property(nonatomic,strong)NSMutableArray* _options;
@property(nonatomic,strong)NSMutableArray* _titleArray;
@property(nonatomic,strong)NSMutableArray* _dataArray;
@property(nonatomic,assign)id<FeaturesPickerDelegate> _delegate;

@end

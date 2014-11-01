//
//  FeaturesPickerController.h
//  Grow
//
//  Created by 林纲 王 on 14-10-19.
//
//

#import <UIKit/UIKit.h>

@protocol FeaturesPickerDelegate

-(void)optionSelected:(NSString*)option;

@end

@interface FeaturesPickerController : UITableViewController{
}

@property (nonatomic,strong)NSMutableArray* _options;
@property(nonatomic,assign)id<FeaturesPickerDelegate> _delegate;

@end

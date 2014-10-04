//
//  LevelEditor.h
//  GrowUp
//
//  Created by wlg on 9/10/14.
//
//

#ifndef __GrowUp__LevelEditor__
#define __GrowUp__LevelEditor__

#include <iostream>
#include "Macro.h"
#include "LevelFileHandler.h"
#include "OCScrollView.h"

@interface MyViewController:UIViewController<UIScrollViewDelegate,UIGestureRecognizerDelegate,UIActionSheetDelegate>
{
@public    LevelFileHandler* _fileHandler;
    float retinascale;
    float contentscale;
    OCScrollView* _scrollView;
    std::vector<UIImageView*> _imageViews;
    std::vector<UIButton*> needHidenButtons;
    BOOL isButtonHiden;
}

@property float width;
@property float height;

-(id)init;
-(IBAction)createItemAtCenter:(id)sender;

@end


class LevelEditor:public Layer
{
public:
    LevelEditor();
    ~LevelEditor();

    static Scene* createScene();
    CREATE_FUNC(LevelEditor);
    bool init();
    
    void drawLoadedLevel();
    void playLevel();

    Sprite* _background;
    
    MyViewController* _myViewController;

};

@interface UIView(Hierarchy)

-(int)getSubviewIndex;

-(void)bringToFront;

-(void)sendToBack;

-(void)bringOneLevelUp;

-(void)sendOneLevelDown;

-(BOOL)isInFront;

-(BOOL)isAtBack;

-(void)swapDepthsWithView:(UIView*)swapView;

@end
#endif /* defined(__GrowUp__LevelEditor__) */


//
//  LevelEditor.cpp
//  GrowUp
//
//  Created by wlg on 9/10/14.
//
//
#include <QuartzCore/QuartzCore.h>

#include "LevelEditor.h"
#include "SceneGame.h"
#include "StaticData.h"
#include "Item.h"
#import "CCEAGLView.h"
#import "AppController.h"
#import "PopupViewController.h"
#import "GHContextMenuView.h"
#import "FeaturesController.h"
#import "UIViewExtension.h"

LevelEditor::LevelEditor():_myViewController(nil)
{
    
}

LevelEditor::~LevelEditor()
{
    [_myViewController dismissViewControllerAnimated:NO completion:nil];
    [_myViewController release];
    GameManager::getInstance()->_levelEditor = nullptr;
}

Scene* LevelEditor::createScene()
{
    Scene* scene = Scene::create();
    LevelEditor* layer = LevelEditor::create();
    scene->addChild(layer);
    return scene;
}

bool LevelEditor::init()
{
    if(!Layer::init()) return false;
    GameManager::getInstance()->_levelEditor = this;
    //添加背景
    _background = Sprite::create("background2.jpg");
    _background->setPosition(Vec2(WinSize.width/2,WinSize.height/2));
    this->addChild(_background);
    
    this->drawLoadedLevel();
    
    return true;
}

void LevelEditor::playLevel()
{
    Director::getInstance()->replaceScene(SceneGame::createScene());
}

void LevelEditor::drawLoadedLevel()
{
    AppController* controller = (AppController*)[UIApplication sharedApplication].delegate;
    UIViewController* rootController = static_cast<UIViewController*>(controller.viewController);
    if (!_myViewController) {
        _myViewController = [[MyViewController alloc] init];
    }
    
    [rootController presentViewController:_myViewController animated:NO completion:nil];
    
}

@interface MyViewController()<GHContextOverlayViewDataSource,GHContextOverlayViewDelegate>{
    GameManager* _gameManager;
    std::set<ItemView*> _toDealWith;
    
}

@property(nonatomic,readonly)PopupViewController* popupViewController;
@property(nonatomic,readonly)GHContextMenuView* contextMenu;
@property(nonatomic,readonly)CAShapeLayer* solidLayer;
@property(nonatomic,readonly)CAShapeLayer* dashLayer;
@property(nonatomic,readonly)FeaturesController* featuresController;

@end

@implementation MyViewController

@synthesize width;
@synthesize height;
@synthesize popupViewController;
@synthesize contextMenu;
@synthesize solidLayer;
@synthesize dashLayer;
@synthesize featuresController;

-(id)init
{
    self = [super init];
    _gameManager = GameManager::getInstance();
    if(self){
        _fileHandler = _gameManager->_fileHandler;
    }
    
    CCEAGLView* glview = (CCEAGLView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    retinascale = 1.0/[glview contentScaleFactor];
    self.height = glview.getHeight*retinascale;
    self.width = 768.0/1024.0*self.height;
    contentscale = self.height/1024;

    _gameManager->editor_width = width;
    _gameManager->editor_height = height;
    _gameManager->editor_contentscale = contentscale;
    
    return self;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    [self addScrollView];
    
    [self addContextMenu];

    [self createItemFromFileHandler];

    [self createControlPanel];
    
    [self addLineLayer];
    
    [self addGestureRecognizerForSelf];
}

-(void)addLineLayer
{
    UIBezierPath* path_solid = [[UIBezierPath alloc] init];
    [path_solid moveToPoint:CGPointMake(64, 0)];
    [path_solid addLineToPoint:CGPointMake(64, height)];
    [path_solid moveToPoint:CGPointMake(704, 0)];
    [path_solid addLineToPoint:CGPointMake(704, height)];
    
    solidLayer = [CAShapeLayer layer];
    solidLayer.strokeColor = [UIColor grayColor].CGColor;
    solidLayer.fillColor = nil;
    solidLayer.lineWidth = 1;
    solidLayer.lineCap = kCALineCapSquare;
    solidLayer.path = path_solid.CGPath;
    
    [self.view.layer addSublayer:solidLayer];
    
    UIBezierPath* path_dash = [[UIBezierPath alloc] init];
    [path_dash moveToPoint:CGPointMake(94, 0)];
    [path_dash addLineToPoint:CGPointMake(94,height)];
    [path_dash moveToPoint:CGPointMake(674, 0)];
    [path_dash addLineToPoint:CGPointMake(674,height)];
    
    dashLayer = [CAShapeLayer layer];
    dashLayer.strokeColor = [UIColor grayColor].CGColor;
    dashLayer.fillColor = nil;
    dashLayer.lineDashPattern = @[@10,@10];
    dashLayer.lineDashPhase = 0;
    dashLayer.lineWidth = 1;
    dashLayer.path = path_dash.CGPath;
    
    [self.view.layer addSublayer:dashLayer];
    
    [path_solid release];
    [path_dash release];
}


-(void)addScrollView
{
    _scrollView = [[OCScrollView alloc] initWithFrame:CGRectMake(0, 0, width, height)];
    _scrollView.contentSize = CGSizeMake(width, PAGE_COUNTS*height);
    _scrollView.contentOffset = CGPointMake(0, (PAGE_COUNTS-1)*height - GameManager::getInstance()->scrollViewOffset);
    _scrollView.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@IMAGE_MATERIAL_DIRT]];
    _scrollView.bounces = NO;
    _scrollView.scrollEnabled = YES;
    _scrollView.directionalLockEnabled = YES;
    [_scrollView setPagingEnabled:NO];
    //    [_scrollView setDecelerationRate:0.0];
    _scrollView.delegate = self;
    _scrollView.levelName = _fileHandler->_filename.substr(_fileHandler->_filename.find("/")+1);
    [self.view addSubview:_scrollView];
    [_scrollView release];
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    [_scrollView setNeedsDisplay];
}

-(void)addContextMenu
{
    contextMenu = [[GHContextMenuView alloc] init];
    contextMenu.dataSource = self;
    contextMenu.delegate = self;
    [contextMenu release];
}

-(void)createItemFromFileHandler
{
    sort(_fileHandler->_items.begin(),_fileHandler->_items.end(),Itemlesser);
    
    for (Item& item : _fileHandler->_items) {
        ItemView* itemView = [[ItemView alloc] init:item];
        [itemView itemAddGestureRecognizerWithTarget:self];
        
        [_scrollView insertSubview:itemView atIndex:itemViews.size()];
        [itemView release];
        //加入编辑器数组
        itemViews.push_back(itemView);
        ids.push_back(itemView.tag);
    }
    
    _fileHandler->_items.clear();
}

-(void)handlePan:(UIPanGestureRecognizer*)recognizer
{
    if(recognizer.state == UIGestureRecognizerStateBegan){
        _scrollView.scrollEnabled = NO;
    }
    
    CGPoint translation = [recognizer translationInView:_scrollView];
    recognizer.view.center = CGPointMake(recognizer.view.center.x + translation.x,recognizer.view.center.y + translation.y);
    [recognizer setTranslation:CGPointZero inView:_scrollView];

    if(recognizer.state == UIGestureRecognizerStateEnded){
        _scrollView.scrollEnabled = YES;
    }
    
}

-(void) handlePinch:(UIPinchGestureRecognizer*) recognizer
{
    recognizer.view.transform = CGAffineTransformScale(recognizer.view.transform, recognizer.scale, recognizer.scale);
    recognizer.scale = 1;
}

-(void) handleRotate:(UIRotationGestureRecognizer*) recognizer
{
    recognizer.view.transform = CGAffineTransformRotate(recognizer.view.transform, recognizer.rotation);
    recognizer.rotation = 0;
}

-(void) handleTap:(UITapGestureRecognizer*) recognizer
{
    if(recognizer.state == UIGestureRecognizerStateEnded)
    {
        BOOL isBoderCreated = NO;
        for (CALayer* layer in recognizer.view.layer.sublayers) {
            if ([layer.name isEqualToString:@"boder"]) {
                isBoderCreated = YES;
                break;
            }
        }
        
        if (![recognizer.view getHeightLightState]&&!isBoderCreated) {
           
            CAShapeLayer* border1 = [CAShapeLayer layer];//便利构造器，autorelease
            [border1 setName:@"boder"];
            border1.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
            border1.fillColor = nil;
            border1.lineDashPattern = nil;
            [recognizer.view.layer addSublayer:border1];
            border1.path = [UIBezierPath bezierPathWithRect:recognizer.view.bounds].CGPath;
            border1.frame = recognizer.view.bounds;
            
            CAShapeLayer* border2 = [CAShapeLayer layer];
            [border2 setName:@"boder"];
            border2.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
            border2.fillColor = nil;
            border2.lineDashPattern = nil;
            [recognizer.view.layer addSublayer:border2];
            border2.path = [UIBezierPath bezierPathWithRect:recognizer.view.bounds].CGPath;
            border2.frame = recognizer.view.bounds;
            
            CAShapeLayer* border3 = [CAShapeLayer layer];
            [border3 setName:@"boder"];
            border3.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
            border3.fillColor = nil;
            border3.lineDashPattern = nil;
            [recognizer.view.layer addSublayer:border3];
            border3.path = [UIBezierPath bezierPathWithRect:recognizer.view.bounds].CGPath;
            border3.frame = recognizer.view.bounds;
            
            CAShapeLayer* border4 = [CAShapeLayer layer];
            [border4 setName:@"boder"];
            border4.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
            border4.fillColor = nil;
            border4.lineDashPattern = nil;
            [recognizer.view.layer addSublayer:border4];
            border4.path = [UIBezierPath bezierPathWithRect:recognizer.view.bounds].CGPath;
            border4.frame = recognizer.view.bounds;
            
            [recognizer.view setHeightLightState:YES];
            _toDealWith.insert(static_cast<ItemView*>(recognizer.view));
        }else if(![recognizer.view getHeightLightState]&&isBoderCreated){

            for(CAShapeLayer* shapeLayer in recognizer.view.layer.sublayers)
            {
                if ([shapeLayer.name isEqualToString:@"boder"]){
                    [shapeLayer setHidden:NO];
                }
            }
            [recognizer.view setHeightLightState:YES];
            _toDealWith.insert(static_cast<ItemView*>(recognizer.view));
        }else{
            
            for(CAShapeLayer* shapeLayer in recognizer.view.layer.sublayers)
            {
                if ([shapeLayer.name isEqualToString:@"boder"]){
                    [shapeLayer setHidden:YES];
                }
            }
            [recognizer.view setHeightLightState:NO];
            _toDealWith.erase(find(_toDealWith.begin(),_toDealWith.end(),recognizer.view));
        }
        
    }
    
}

-(void) handleLongPress:(UILongPressGestureRecognizer*) recognizer
{
    if(recognizer.state == UIGestureRecognizerStateBegan)
    {
        featuresController = [[FeaturesController alloc] init:static_cast<ItemView*>(recognizer.view)];
        featuresController.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
        featuresController.modalPresentationStyle = UIModalPresentationFormSheet;
        
        [self presentModalViewController:featuresController animated:YES];
        [featuresController release];
    }
}

-(BOOL) gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return NO;
}

-(void) createControlPanel
{
    UIButton* addButton = [[UIButton alloc] init];
    addButton.center = CGPointMake(1/16.0*width, 0.95*height);
    addButton.bounds = CGRectMake(0, 0,64*contentscale,64*contentscale);
    [addButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_ADDITEM_NORMAL] forState:UIControlStateNormal];
    [addButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_ADDITEM_SELECTED] forState:UIControlStateSelected];
    [addButton addTarget:self action:@selector(popup:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:addButton];
    needHidenButtons.push_back(addButton);
    [addButton release];
    
    UIButton* saveButton = [[UIButton alloc] init];
    saveButton.center = CGPointMake(3.0/16*width, 0.95*height);
    saveButton.bounds = CGRectMake(0, 0,64*contentscale,64*contentscale);
    [saveButton setBackgroundImage:[UIImage imageNamed:@IMAGE_EDITOR_SAVE_NORMAL] forState:UIControlStateNormal];
    [saveButton setBackgroundImage:[UIImage imageNamed:@IMAGE_EDITOR_SAVE_SELECTED] forState:UIControlStateSelected];
    [saveButton addTarget:self action:@selector(save:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:saveButton];
    needHidenButtons.push_back(saveButton);
    [saveButton release];
    
    UIButton* resetButton = [[UIButton alloc] init];
    resetButton.center = CGPointMake(5.0/16*width, 0.95*height);
    resetButton.bounds = CGRectMake(0, 0,64*contentscale,64*contentscale);
    [resetButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_RESET_NORMAL] forState:UIControlStateNormal];
    [resetButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_RESET_SELECTED] forState:UIControlStateSelected];
    [resetButton addTarget:self action:@selector(reset:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:resetButton];
    needHidenButtons.push_back(resetButton);
    [resetButton release];
    
    UIButton* copyButton = [[UIButton alloc] init];
    copyButton.center = CGPointMake(7.0/16*width,0.95*height);
    copyButton.bounds = CGRectMake(0,0,64*contentscale,64*contentscale);
    [copyButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_COPY_NORMAL] forState:UIControlStateNormal];
    [copyButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_COPY_SELECTED] forState:UIControlStateSelected];
    [copyButton addTarget:self action:@selector(copyItem:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:copyButton];
    needHidenButtons.push_back(copyButton);
    [copyButton release];
    
    UIButton* deleteButton = [[UIButton alloc] init];
    deleteButton.center = CGPointMake(9/16.0*width, 0.95*height);
    deleteButton.bounds = CGRectMake(0, 0, 64*contentscale, 64*contentscale);
    [deleteButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_DELETE_NORMAL] forState:UIControlStateNormal];
    [deleteButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_DELETE_SELECTED] forState:UIControlStateSelected];
    [deleteButton addTarget:self action:@selector(deleteItem:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:deleteButton];
    needHidenButtons.push_back(deleteButton);
    [deleteButton release];
    
    UIButton* playButton = [[UIButton alloc] init];
    playButton.center = CGPointMake(11/16.0*width, 0.95*height);
    playButton.bounds = CGRectMake(0, 0,64*contentscale,64*contentscale);
    [playButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_PLAY_NORMAL] forState:UIControlStateNormal];
    [playButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_PLAY_SELECTED] forState:UIControlStateSelected];
    [playButton addTarget:self action:@selector(play:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:playButton];
    needHidenButtons.push_back(playButton);
    [playButton release];
    
    UIButton* hideButton = [[UIButton alloc] init];
    hideButton.center = CGPointMake(15/16.0*width, 0.95*height);
    hideButton.bounds = CGRectMake(0, 0, 64*contentscale, 64*contentscale);
    [hideButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_HIDE_NORMAL] forState:UIControlStateNormal];
    [hideButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_HIDE_SELECTED] forState:UIControlStateSelected];
    [hideButton addTarget:self action:@selector(hide:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:hideButton];
    [hideButton release];

    isButtonHiden = NO;
}

-(void)popup:(id)sender
{
    NSString* nibFileName = @"PopupViewController_ipad";
    if ([[UIDevice currentDevice] userInterfaceIdiom]==UIUserInterfaceIdiomPhone) {
        nibFileName = @"PopupViewController_iphone";
    }
    
    popupViewController = [[PopupViewController alloc] initWithNibName:nibFileName bundle:nil];
    popupViewController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;

    [self presentModalViewController:popupViewController animated:YES];
    [popupViewController release];
}

-(void)save:(id)sender
{
    _fileHandler->_items.clear();
    
    for(ItemView* itemView: itemViews)
    {
        [self saveItemInformationInMemory:itemView];
    }
    int errNo = _fileHandler->saveFile();
    
    UILabel* saveSuccess = [[UILabel alloc] init];
    saveSuccess.center = CGPointMake(self.width/2, self.height/2);
    saveSuccess.bounds = CGRectMake(0, 0, 0.5*width, 0.3*height);
    
    if(errNo==0){
        saveSuccess.text = @"Save Success!";
    }else{
        saveSuccess.text = [NSString stringWithFormat:@"Save Failed!errno is %d",errNo];
    }
    [saveSuccess setTextAlignment:NSTextAlignmentCenter];
    [saveSuccess setFont:[UIFont systemFontOfSize:32*contentscale]];
    [saveSuccess setTextColor:[UIColor colorWithWhite:1 alpha:1]];
    
    [self.view addSubview:saveSuccess];
    [saveSuccess release];
    
    [UIView animateWithDuration:2 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^(){[saveSuccess setAlpha:0];} completion:^(BOOL finished){[saveSuccess removeFromSuperview];}];
}

-(void)reset:(id)sender
{
    UIActionSheet* reSetSheet = [[UIActionSheet alloc] initWithTitle:@"Are you sure you want to reset?" delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:@"Yes" otherButtonTitles:@"No,thanks",nil];
    reSetSheet.actionSheetStyle = UIActionSheetStyleBlackTranslucent;
    [reSetSheet showInView:self.view];
    [reSetSheet release];
}

-(void)copyItem:(id)sender
{
    for(std::set<ItemView*>::iterator it = _toDealWith.begin();it!=_toDealWith.end();it++)
    {
        ItemView* itemView = *it;
        
        Item_Type type = itemView->itemtype;
        int id = [self firstUnusedId];
        float x = (itemView.center.x + 0.4*itemView.bounds.size.width)/width;
        float y = PAGE_COUNTS - (itemView.center.y + 0.4*itemView.bounds.size.height)/height;
        float angle = DEFAULT_ANGLE;
        float scale = DEFAULT_SCALE;
        int localZorder = itemViews.size();
        bool iscreated = false;
        void* features = nullptr;
        
        switch(itemView->itemtype){
            case Cicada:
            {
                Features_Cicada* oldfeatures = (Features_Cicada*)itemView->features;
                if(oldfeatures){
                    Features_Cicada feat(*oldfeatures);
                    features = &feat;
                }else{
                    Features_Cicada feat;
                    features = &feat;
                }
            }
                break;
            case Dragon_Anti:
            {
                Features_Dragon* oldfeatures = (Features_Dragon*)itemView->features;
                if(oldfeatures){
                    Features_Dragon feat(*oldfeatures);
                    features = &feat;
                }else{
                    Features_Dragon feat;
                    features = &feat;
                }
            }
                break;
            case Dragon_Clockwise:
            {
                Features_Dragon* oldfeatures = (Features_Dragon*)itemView->features;
                if(oldfeatures){
                    Features_Dragon feat(*oldfeatures);
                    features = &feat;
                }else{
                    Features_Dragon feat;
                    features = &feat;
                }
            }
                break;
            default:
                break;
        }
        
        Item item(type,id,x,y,angle,scale,localZorder,iscreated,features);
        
        ItemView* newItem = [[ItemView alloc] init:item];
        //copy
        newItem.transform = itemView.transform;
        //
        [newItem itemAddGestureRecognizerWithTarget:self];
        
        [_scrollView insertSubview:newItem atIndex:itemViews.size()];
        [newItem release];
        //添加到数组
        itemViews.push_back(newItem);
        ids.push_back(newItem.tag);
    }
    
}

-(void)deleteItem:(id)sender
{
    for(std::set<ItemView*>::iterator it = _toDealWith.begin();it!=_toDealWith.end();it++)
    {
        ItemView* selectedItemview = *it;
        //从编辑器中删
        itemViews.erase(find(itemViews.begin(),itemViews.end(),selectedItemview));
        ids.erase(find(ids.begin(),ids.end(),selectedItemview.tag));
        
        //从视图中移除
        [selectedItemview removeFromSuperview];
    }
    
    _toDealWith.clear();
}

-(void)play:(id)sender
{
    _fileHandler->_items.clear();
    
    for(ItemView* itemView : itemViews)
    {
        [self saveItemInformationInMemory:itemView];
    }
    _gameManager->scrollViewOffset = (PAGE_COUNTS - 1)*height - _scrollView.contentOffset.y;
    _gameManager->_levelEditor->playLevel();
}

-(void)hide:(id)sender
{
    isButtonHiden = !isButtonHiden;
    for(UIButton* button : needHidenButtons)
    {
        [button setHidden:isButtonHiden];
    }
    
    [solidLayer setHidden:isButtonHiden];
    [dashLayer setHidden:isButtonHiden];
    
    _scrollView.isButtonHiden = isButtonHiden;
    [_scrollView setNeedsDisplay]; //刷新
    
    UIButton* hidebutton = static_cast<UIButton*>(sender);
    [hidebutton setAlpha:isButtonHiden? 0.3:1];
}


- (void)actionSheet:(UIActionSheet *)actionSheet willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if (buttonIndex==[actionSheet destructiveButtonIndex]) {
//        UIAlertView* alert =[[UIAlertView alloc] initWithTitle:@"Result" message:@"Complete!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
//        [alert show];
//        [alert release];
        //先把自己的清空
        for(ItemView* itemview : itemViews)
        {
            [itemview removeFromSuperview];
        }
        itemViews.clear();
        ids.clear();
        _toDealWith.clear();
        //在让文件管理器清空
        _fileHandler->reload();
        //最后重新展示出来
        [self createItemFromFileHandler];
    }
}

-(int)firstUnusedId
{
    int firstUnusedID = 0;
    int lastID = 0;
    ids.sort();
    for (int i : ids) {
        if(i - lastID >1){
            firstUnusedID = lastID + 1;
            break;
        }
        lastID++;
    }
    if (firstUnusedID==0) {
        firstUnusedID = lastID + 1;
    }
    return firstUnusedID;
}

-(IBAction)createItemAtCenter:(id)sender
{
    UIButton* btn = (UIButton*)sender;
    
    Item_Type type = static_cast<Item_Type>(btn.tag);
    int id = [self firstUnusedId];
    float x = 0.5 ;
    float y = PAGE_COUNTS - (_scrollView.contentOffset.y + 0.5*height)/height;
    float angle = DEFAULT_ANGLE;
    float scale = DEFAULT_SCALE;
    int localZorder = itemViews.size();
    bool iscreated = false;
    void* features = nullptr;
    
    switch(type){
        case Cicada:
        {
            Features_Cicada feat;
            features = &feat;
        }
            break;
        case Dragon_Anti:
        {
            Features_Dragon feat;
            features = &feat;
        }
            break;
        case Dragon_Clockwise:
        {
            Features_Dragon feat;
            features = &feat;
        }
            break;
        default:
            break;
    }
    
    Item item(type,id,x,y,angle,scale,localZorder,iscreated,features);
    
    ItemView* newItem = [[ItemView alloc] init:item];
    
    [newItem itemAddGestureRecognizerWithTarget:self];
    
    [_scrollView insertSubview:newItem atIndex:itemViews.size()];
    [newItem release];
    //添加到数组
    itemViews.push_back(newItem);
    ids.push_back(newItem.tag);
    
    //返回
    [popupViewController backEditor:newItem];
}

-(void)saveItemInformationInMemory:(ItemView*)itemView
{
    Item_Type type = itemView->itemtype;
    int id = static_cast<int>(itemView.tag);
    float x = itemView.center.x/width;
    float y = PAGE_COUNTS - itemView.center.y/height;
    float a = itemView.transform.a;
    float b = itemView.transform.b;
    float c = itemView.transform.c;
    float d = itemView.transform.d;
    
    float angle;
    float scale;
    int localZorder = [itemView getSubviewIndex];
    bool iscreated = false;
    void* features = nullptr;
    if(a ==0.0&&b>0.0&&c<0.0&&d==0.0)
    {
        angle = M_PI_2;
        scale = fabsf(b);
    }
    else if(a<0.0&&b==0.0&&c==0.0&&d<0.0)
    {
        angle = M_PI;
        scale = fabsf(a);
    }
    else if(a==0.0&&b<0.0&&c>0.0&&d==0.0)
    {
        angle = 3*M_PI_2;
        scale = fabsf(b);
    }
    else
    {
        angle = atanf(b/a);
        if (angle*b<0.0) {
            angle+=M_PI;
        }
        scale = fabsf(a/cosf(angle));
    }
    
    switch (type) {
        case Cicada:
        {
            Features_Cicada* oldfeatures = (Features_Cicada*)itemView->features;
            if(oldfeatures){
                Features_Cicada feat(*oldfeatures);
                features = &feat;
            }
            NSLog(@"i am Cicada");
        }
            break;
        case Dragon_Anti:
        {
            Features_Dragon* oldfeatures = (Features_Dragon*)itemView->features;
            if(oldfeatures){
                Features_Dragon feat(*oldfeatures);
                features = &feat;
            }
        }
            break;
        case Dragon_Clockwise:
        {
            Features_Dragon* oldfeatures = (Features_Dragon*)itemView->features;
            if(oldfeatures){
                Features_Dragon feat(*oldfeatures);
                features = &feat;
            }
        }
            break;
        default:
            break;
    }
    
    Item item(type,id,x,y,angle,scale,localZorder,iscreated,features);
    _fileHandler->_items.push_back(item);
}

#pragma mark GHContextMenu
-(NSInteger)numberOfMenuItems
{
    return 4;
}

-(UIImage*)imageForItemAtIndex:(NSInteger)index
{
    NSString* imageName = nil;
    switch (index) {
        case 0:
            imageName = @"OneLevelUp.png";
            break;
        case 1:
            imageName = @"ToTop.png";
            break;
        case 2:
            imageName = @"OneLevelDown.png";
            break;
        case 3:
            imageName = @"ToBottom.png";
            break;
        default:
            break;
    }
    return [UIImage imageNamed:imageName];
}

-(BOOL) shouldShowMenuAtPoint:(CGPoint) point
{
    return YES;
}

-(void)didSelectItemAtIndex:(NSInteger)selectedIndex forMenuAtPoint:(CGPoint)point
{
    UIImageView* selectedImageview = nil;
    for (UIImageView* imageview : itemViews) {
        if (CGRectContainsPoint(imageview.bounds,[imageview convertPoint:contextMenu.longPressLocation fromView:contextMenu])) {
            selectedImageview = imageview;
            break;
        }
    }
    
    if (selectedImageview) {
        switch (selectedIndex) {
            case 0:
                [selectedImageview bringOneLevelUp];
                break;
            case 1:
                [selectedImageview bringToFront];
                break;
            case 2:
                [selectedImageview sendOneLevelDown];
                break;
            case 3:
                [selectedImageview sendToBack];
                break;
            default:
                break;
        }
    }
}

-(void)addGestureRecognizerForSelf
{
    UITapGestureRecognizer* tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(cancelHeightLight)];
    tapRecognizer.numberOfTapsRequired = 1;
    [self.view addGestureRecognizer:tapRecognizer];
    [tapRecognizer release];
}

-(void)cancelHeightLight
{
    std::set<ItemView*>::iterator it;
    for(it = _toDealWith.begin();it!=_toDealWith.end();it++)
    {
        UIImageView* imageview = *it;
        if ([imageview getHeightLightState]) {
            for (CAShapeLayer* shapeLayer in imageview.layer.sublayers) {
                if([shapeLayer.name isEqualToString:@"boder"]){
                [shapeLayer setHidden:YES];
                }
            }
            [imageview setHeightLightState:NO];
        }
    }
    
    _toDealWith.clear();
}


-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
}

-(void)dealloc
{
    [super dealloc];
}

@end



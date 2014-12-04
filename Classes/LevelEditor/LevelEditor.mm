
//
//  LevelEditor.cpp
//  GrowUp
//
//  Created by wlg on 9/10/14.
//
//
#include <QuartzCore/QuartzCore.h>

#include "LevelEditor.h"
#include "Item.h"
#include "GameManager.h"
#include <Box2D/Common/b2Settings.h>
#include "LevelManager.h"
#import "CCEAGLView.h"
#import "AppController.h"
#import "PopupViewController.h"
#import "GHContextMenuView.h"
#import "UIViewExtension.h"


LevelEditor::LevelEditor():_myViewController(nil)
{
    
}

LevelEditor::~LevelEditor()
{
    [_myViewController dismissViewControllerAnimated:NO completion:nil];
    [_myViewController release];
    GameManager::getInstance()->_levelEditor = nullptr;
    CC_SAFE_RELEASE(GameManager::getInstance()->_fileHandler);
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
    CC_SAFE_RETAIN(GameManager::getInstance()->_fileHandler);

    this->drawLoadedLevel();
    
    return true;
}

void LevelEditor::playLevel()
{
    
    LevelManager::getInstance()->selectLevel(5);
    auto manager = GameManager::getInstance();
//    manager->releaseGameScene();
    manager->_fileHandler->_items.sort(OrderByHeight);
    manager->navigationToGameScene();
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
}

@property(nonatomic,readonly)PopupViewController* popupViewController;
@property(nonatomic,readonly)GHContextMenuView* contextMenu;
@property(nonatomic,readonly)CAShapeLayer* solidLayer;
@property(nonatomic,readonly)CAShapeLayer* dashLayer;

@end

@implementation MyViewController

@synthesize width;
@synthesize height;
@synthesize isItemViewClicked;
@synthesize _zoomInButton;
@synthesize _zoomOutButton;
@synthesize _hideButton;
@synthesize _selectedItemLabel;
@synthesize popupViewController;
@synthesize contextMenu;
@synthesize solidLayer;
@synthesize dashLayer;

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
    isItemViewClicked = NO;
    
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
    [self createPolygonsFromFileHandler];
    [self createPathPointsFromAllItems];
    
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
    _scrollView.currentZoomFactor = _fileHandler->scrollViewZoomFactor;
    _scrollView.bounds = CGRectMake(0, 0, _scrollView.currentZoomFactor*width, height);
    _scrollView.contentSize = CGSizeMake(_scrollView.currentZoomFactor*width, _scrollView.currentZoomFactor*PAGE_COUNTS*height);
    _scrollView.contentOffset = CGPointMake(0, _fileHandler->scrollViewContentOffSet);
    _scrollView.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"dirt_1.jpg"]];
    _scrollView.bounces = NO;
    _scrollView.scrollEnabled = YES;
    _scrollView.directionalLockEnabled = YES;
    [_scrollView setPagingEnabled:NO];

    _scrollView.delegate = self;
    _scrollView.contentMode = UIViewContentModeRedraw;
    
    _scrollView.levelName = _fileHandler->_filename.substr(_fileHandler->_filename.find("/")+1);
    [self.view addSubview:_scrollView];
    [_scrollView release];
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (!_scrollView.isPreViewed) {
        [_scrollView setNeedsDisplay];
    }
    
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
    _boundButtons.clear();
    _gates.clear();
    _fileHandler->_items.sort(Itemlesser);
    
    for (Item& item : _fileHandler->_items) {
        ItemView* itemView = [[ItemView alloc] init:item];
        [itemView itemAddGestureRecognizerWithTarget:self];
        
        //加入编辑器数组
        itemViews.push_back(itemView);
        ids.push_back(itemView.tag);
        [_scrollView insertSubview:itemView atIndex:itemViews.size()];
        [itemView release];
    }
    _fileHandler->_items.clear();
    
    //建立绑定关系
    for (ItemView* gearbutton : _boundButtons) {
        for (ItemView* gate : _gates) {
            if(((Features_GearButton*)gearbutton->features)->bindID == gate.tag)
            {
                gate.bindButton = gearbutton;
            }
        }
    }
    
}

-(void)createPolygonsFromFileHandler
{
    __Dictionary* polygonsDict = _fileHandler->_polygonsDict;
    DictElement* singlePolygon;
    int tag;
    Vec2 position;
    bool isConvex;
    __Array* vertexes;
    
    CCDICT_FOREACH(polygonsDict, singlePolygon){
        const char* mainKey = singlePolygon->getStrKey();
        char buf[10];
        strlcpy(buf, mainKey+strlen("polygon"), sizeof(buf));
        tag = atoi(buf);
        
        __Dictionary* propertyDict = (__Dictionary*)singlePolygon->getObject();
        position = PointFromString(static_cast<__String*>(propertyDict->objectForKey("position"))->getCString());
        isConvex = (static_cast<__Bool*>(propertyDict->objectForKey("isConvex")))->getValue();
        vertexes = static_cast<__Array*>(propertyDict->objectForKey("vertexes"));
        
        [_scrollView createNewPolygonOfTag:tag+1000 AtCCPercentPosition:position WithCCLocalVertxex:vertexes OfType:isConvex];
    }
    
    polygonsDict->removeAllObjects();
}

-(void)createPathPointsFromAllItems
{
    for(ItemView* itemview : itemViews){
        if (!itemview->_animationInfos.empty()) {
            [itemview createAllPathPoints];
            itemview->_animationInfos.clear();
        }
    }
}

-(void)handlePan:(UIPanGestureRecognizer*)recognizer
{
    CGPoint translation = [recognizer translationInView:self.view];
    if (!_scrollView.isScrollEnabled) {
        //移动道具
        for(std::set<ItemView*>::iterator it = _toDealWith.begin();it != _toDealWith.end();it++){
            ItemView* itemview = *it;
            itemview.center = CGPointMake(itemview.center.x + translation.x, itemview.center.y + translation.y);
            [itemview translateAllPathPoints:translation];
        }
        //移动多边形和路径点
        for (PolygonView* polygon in _scrollView.toDealWithPointView) {
            if ([polygon.pointType isEqualToString:@"pointview"]) {
                PolygonView* centerview = (PolygonView*)polygon.superview;
                if(centerview.isSelected) continue;
            }else if ([polygon.pointType isEqualToString:@"pathpoint"]){
                ItemView* parent = polygon->_pathParent;
                if ([parent getHeightLightState]) continue;
            }
            polygon.center = CGPointMake(polygon.center.x + translation.x, polygon.center.y + translation.y);
        }
    }else{
        CGPoint newOffset = CGPointMake(_scrollView.contentOffset.x,_scrollView.contentOffset.y - translation.y);
        newOffset.y = MIN(MAX(0, newOffset.y), _scrollView.contentSize.height-height);
        _scrollView.contentOffset = newOffset;
    }
    
    [recognizer setTranslation:CGPointZero inView:self.view];
    [_scrollView setNeedsDisplay];
    
}

-(void) handlePinch:(UIPinchGestureRecognizer*) recognizer
{
    recognizer.view.transform = CGAffineTransformScale(recognizer.view.transform, recognizer.scale, recognizer.scale);
    recognizer.scale = 1;
}

-(void) handleRotate:(UIRotationGestureRecognizer*) recognizer
{
    //    if (recognizer.view != self.view) {
    //        recognizer.view.transform = CGAffineTransformRotate(recognizer.view.transform, recognizer.rotation);
    //    }else{
    //        for (ItemView* itemView : _toDealWith) {
    //            itemView.transform = CGAffineTransformRotate(itemView.transform, recognizer.rotation);
    //        }
    //    }
    for (ItemView* itemView : _toDealWith) {
        itemView.transform = CGAffineTransformRotate(itemView.transform, recognizer.rotation);
    }
    recognizer.rotation = 0;

}

-(void) handleTap:(UITapGestureRecognizer*) recognizer
{
    if(recognizer.state == UIGestureRecognizerStateEnded)
    {
        self.isItemViewClicked = YES;
        if (![self myViewSingleTap:recognizer]) {
            ItemView* itemview = static_cast<ItemView*>(recognizer.view);
            BOOL isBoderCreated = [itemview isBorderCreated];
            if (![recognizer.view getHeightLightState]&&!isBoderCreated) {
                
                [itemview addBorder];
                [itemview setHeightLightState:YES];
                [itemview heightLightPathPoints];
                _toDealWith.insert(itemview);
            }else if(![recognizer.view getHeightLightState]&&isBoderCreated){
                
                [itemview showBorder];
                [itemview setHeightLightState:YES];
                [itemview heightLightPathPoints];
                _toDealWith.insert(itemview);
            }else{
                
                [itemview hideBorder];
                [itemview setHeightLightState:NO];
                [itemview cancelHeightLightPathPoints];
                _toDealWith.erase(find(_toDealWith.begin(),_toDealWith.end(),itemview));
            }
            //
            [self refreshSelectedItemInfo];
            
            //
            if (_toDealWith.size()>0||_scrollView.toDealWithPointView.count>0) {
                [_scrollView setScrollEnabled:NO];
            }else{
                [_scrollView setScrollEnabled:YES];
            }
        }
        
        isItemViewClicked = NO;
    }
    
}

-(void) handleLongPress:(UILongPressGestureRecognizer*) recognizer
{
    if(recognizer.state == UIGestureRecognizerStateBegan){
        [_scrollView popupFeaturesPickerPopoverWith:recognizer.view At:[recognizer locationInView:_scrollView]];
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
    
    _zoomInButton = [[UIButton alloc] init];
    _zoomInButton.center = CGPointMake(12.5/16.0*width, 0.95*height);
    _zoomInButton.bounds = CGRectMake(0, 0, 64*contentscale, 64*contentscale);
    [_zoomInButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_ZOOMIN_NORMAL] forState:UIControlStateNormal];
    [_zoomInButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_ZOOMIN_SELECTED] forState:UIControlStateSelected];
    [_zoomInButton addTarget:self action:@selector(zoomInOut:) forControlEvents:UIControlEventTouchUpInside];
    needHidenButtons.push_back(_zoomInButton);
    [self.view addSubview:_zoomInButton];
    [_zoomInButton release];
    needHidenButtons.push_back(_zoomInButton);
    
    _zoomOutButton = [[UIButton alloc] init];
    _zoomOutButton.center = CGPointMake(14/16.0*width, 0.95*height);
    _zoomOutButton.bounds = CGRectMake(0, 0, 64*contentscale, 64*contentscale);
    [_zoomOutButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_ZOOMOUT_NORMAL] forState:UIControlStateNormal];
    [_zoomOutButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_ZOOMOUT_SELECTED] forState:UIControlStateSelected];
    [_zoomOutButton addTarget:self action:@selector(zoomInOut:) forControlEvents:UIControlEventTouchUpInside];
    needHidenButtons.push_back(_zoomOutButton);
    [self.view addSubview:_zoomOutButton];
    [_zoomOutButton release];
    needHidenButtons.push_back(_zoomOutButton);
    
    
    _hideButton = [[UIButton alloc] init];
    _hideButton.center = CGPointMake(15/16.0*width, 0.95*height);
    _hideButton.bounds = CGRectMake(0, 0, 64*contentscale, 64*contentscale);
    [_hideButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_HIDE_NORMAL] forState:UIControlStateNormal];
    [_hideButton setImage:[UIImage imageNamed:@IMAGE_EDITOR_HIDE_SELECTED] forState:UIControlStateSelected];
    [_hideButton addTarget:self action:@selector(hide:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_hideButton];
    [_hideButton release];
    
    isButtonHiden = NO;
}

-(void)refreshSelectedItemInfo
{
    if (_toDealWith.size()!=1&&_selectedItemLabel) {
        if (!_selectedItemLabel.isHidden) {
            [_selectedItemLabel setHidden:YES];
        }
    }else if(_toDealWith.size()==1){
        ItemView* item = *_toDealWith.begin();
        NSString* labelContent = [NSString stringWithFormat:@"type:%s, id:%d,localZorder:%d,Animated:%s,GroupCount:%d",[item convertTypeToString].c_str(),item.tag,[item getSubviewIndex],item->isAnimated? "On" : "Off",item.animationGroupCount];
        if(!_selectedItemLabel){
            _selectedItemLabel = [[[UILabel alloc] initWithFrame:CGRectMake(0.1*width, 0, 0.9*width, 0.1*height)] autorelease];
            _selectedItemLabel.text = labelContent;
            _selectedItemLabel.font = [UIFont fontWithName:@"MarkerFelt-Thin" size:1.5*kDefaultFontSize];
            _selectedItemLabel.textAlignment = NSTextAlignmentLeft;
            _selectedItemLabel.textColor = [UIColor whiteColor];
            _selectedItemLabel.lineBreakMode = NSLineBreakByTruncatingHead ;
            
            [self.view addSubview:_selectedItemLabel];
        }else{
            _selectedItemLabel.text = labelContent;
            if (_selectedItemLabel.isHidden) {
                [_selectedItemLabel setHidden:NO];
            }
        }
    }
    //点刷新

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
    _fileHandler->_polygonsDict->removeAllObjects();
    
    for(ItemView* itemView: itemViews)
    {
        [self saveItemInformationInMemory:itemView];
    }
    _fileHandler->_items.sort(Itemlesser);
    
    int result = [_scrollView savePolygonsInfoInMemory];
    if (result!=0) {
        NSString* mess;
        switch (result) {
            case -1:
                mess = [NSString stringWithFormat:@"顶点数超过最大顶点数:%d!",b2_maxPolygonVertices];
                break;
            case 1:
                mess = @"线有交叉,请检查!";
                break;
            default:
                mess = @"未知错误!";
                break;
        }
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"保存失败!" message:mess delegate:nil cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
        [alertView show];
        [alertView release];
    }else{
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
}

-(void)reset:(id)sender
{
    //道具和路径点清空
    for(ItemView* itemview : itemViews)
    {
        for(PolygonView* pathPoint : itemview->_pathPoints){
            [pathPoint removeFromSuperview];
        }
        [itemview removeFromSuperview];
    }
    itemViews.clear();
    ids.clear();
    _toDealWith.clear();
    
    //多边形清空
    for(PolygonView* centerview in _scrollView.centerViews){
        [centerview removeFromSuperview];
    }
    [_scrollView.centerViews removeAllObjects];
    _scrollView->polygontags.clear();
    [_scrollView.toDealWithPointView removeAllObjects];
    //显示标签清空
    if (_selectedItemLabel) {
        [_selectedItemLabel removeFromSuperview];
        _selectedItemLabel = nil;
    }
    //在让文件管理器清空
    _fileHandler->reload();
    //最后重新展示出来
    [self createItemFromFileHandler];
    [self createPolygonsFromFileHandler];
    [self createPathPointsFromAllItems];
    //刷新前可能已经选中道具
    if (!_scrollView.isScrollEnabled) {
        [_scrollView setScrollEnabled:YES];
    }
    [_scrollView setNeedsDisplay];
}

-(void)copyItem:(id)sender
{
    //先拷贝多边形和点
    NSMutableArray* newPolygonViews = [NSMutableArray array];
    for(PolygonView* centerview in _scrollView.toDealWithPointView){
        if ([centerview.pointType isEqualToString:@"centerview"]) {
            int tag = [_scrollView firstUnusedTag];
            Vec2 percentPosition = Vec2(0.1+centerview.center.x/_scrollView.contentSize.width,PAGE_COUNTS-(0.08+centerview.center.y/(_scrollView.contentSize.height/PAGE_COUNTS)));
            __Array* vertexes = __Array::createWithCapacity(centerview.subviews.count);
            for(PolygonView* pointview in centerview.subviews){
                CGPoint CClocalpoint = CGPointMake(pointview.center.x/_scrollView.currentZoomFactor,-pointview.center.y/_scrollView.currentZoomFactor);
                vertexes->addObject(__String::createWithFormat("{%f,%f}",CClocalpoint.x,CClocalpoint.y));
            }
            BOOL isConvex = centerview.isConvex;
            
            PolygonView* newPolygon = [_scrollView createNewPolygonOfTag:tag AtCCPercentPosition:percentPosition WithCCLocalVertxex:vertexes OfType:isConvex];
            
            centerview.isSelected = NO;
            centerview.backgroundColor = centerview.defaultColor;
            
            newPolygon.isSelected = YES;
            newPolygon.backgroundColor = [UIColor greenColor];
            [newPolygonViews addObject:newPolygon];
        }else if([centerview.pointType isEqualToString:@"pointview"]){
            centerview.isSelected = NO;
            centerview.backgroundColor = centerview.defaultColor;
        }else if([centerview.pointType isEqualToString:@"pathpoint"]){
            if (![centerview->_pathParent getHeightLightState]) {
                PolygonView* newPathPoint = [_scrollView copyPathPoint:centerview];
                newPathPoint.isSelected = YES;
                newPathPoint.backgroundColor = newPathPoint.selectedColor;
                [newPolygonViews addObject:newPathPoint];
            }
            centerview.isSelected = NO;
            centerview.backgroundColor = centerview.defaultColor;
        }
    }
    [_scrollView.toDealWithPointView removeAllObjects];
    [_scrollView.toDealWithPointView addObjectsFromArray:newPolygonViews];
    
    //再拷贝道具及其路径点
    std::set<ItemView*> needToAdd;
    for(std::set<ItemView*>::iterator it = _toDealWith.begin();it!=_toDealWith.end();it++)
    {
        ItemView* itemView = *it;
        
        Item_Type type = itemView->itemtype;
        int id = [self firstUnusedId];
        float x = (itemView.center.x + 0.4*itemView.bounds.size.width)/_scrollView.contentSize.width;
        float y = PAGE_COUNTS - (itemView.center.y + 0.4*itemView.bounds.size.height)/(_scrollView.currentZoomFactor*height);
        float angle = [itemView getRotateAngle];
        float scale = kDefaultScale;
        int localZorder = itemViews.size();
        bool isAnimated = itemView->isAnimated;
        float triggerTime = itemView->triggerTime;
        std::map<std::string,bool> animationControlInstructions(itemView->_animationControlInstructions);
        std::vector<std::vector<AnimationInfo>> animationInfos;
        std::vector<AnimationInfo> eachAnimationGroupInfos;
        //empty animation group alse need to copy
        for(int i = 0;i<itemView.animationGroupCount;i++){
            for (PolygonView* pathpoint : itemView->_pathPoints) {
                //只有第一组需要刷新位置
                if (i == 0) {
                    Vec2 newPosition = Vec2((pathpoint.center.x-itemView.center.x)/_scrollView.currentZoomFactor,(itemView.center.y-pathpoint.center.y)/_scrollView.currentZoomFactor);
                    pathpoint->_eachGroupCorrespondInfos.at(i).position = newPosition;
                }
                eachAnimationGroupInfos.push_back(pathpoint->_eachGroupCorrespondInfos.at(i));
            }
            animationInfos.push_back(eachAnimationGroupInfos);
            eachAnimationGroupInfos.clear();
        }
       
        void* features = itemView->features;
        if(type == Gear_Button){
            Features_GearButton feat;
            feat.sinkSpeed = ((Features_GearButton*)itemView->features)->sinkSpeed;
            features = &feat;
        }
        
        Item item(type,id,x,y,angle,scale,localZorder,isAnimated,triggerTime,animationControlInstructions,animationInfos,features);
        
        ItemView* newItem = [[ItemView alloc] init:item];
        [newItem createAllPathPoints];
        newItem->_animationInfos.clear();
        //copy
//        newItem.transform = itemView.transform;
        //
        [newItem addBorder];
        [newItem heightLightPathPoints];
        [newItem setHeightLightState:YES];
        needToAdd.insert(newItem);
        
        [itemView hideBorder];
        [itemView cancelHeightLightPathPoints];
        [itemView setHeightLightState:NO];
        //
        [newItem itemAddGestureRecognizerWithTarget:self];
        
        [_scrollView insertSubview:newItem atIndex:localZorder];
        [newItem release];
        //添加到数组
        itemViews.push_back(newItem);
        ids.push_back(newItem.tag);
    }
    _toDealWith.clear();
    _toDealWith.insert(needToAdd.begin(),needToAdd.end());
    
    //
    [self refreshSelectedItemInfo];
}

-(void)deleteItem:(id)sender
{
    //先删多边形和路径点
    for(PolygonView* polygonview in _scrollView.toDealWithPointView){
        if([polygonview.pointType isEqualToString:@"pointview"]&&[polygonview.superview.subviews count]>3)
            [polygonview removeFromSuperview];
    }
    
    for(PolygonView* polygonview in _scrollView.toDealWithPointView){
        if ([polygonview.pointType isEqualToString:@"centerview"])
        {
            [_scrollView.centerViews removeObject:polygonview];
            _scrollView->polygontags.remove(polygonview.tag);
        }else if([polygonview.pointType isEqualToString:@"pathpoint"]){
            std::vector<PolygonView*>::iterator it = polygonview->_pathParent->_pathPoints.erase(find(polygonview->_pathParent->_pathPoints.begin(), polygonview->_pathParent->_pathPoints.end(),polygonview));
            for(;it!=polygonview->_pathParent->_pathPoints.end();it++){
                PolygonView* pathpoint = *it;
                pathpoint.tag -= 1;
                pathpoint.pathNum.text = [NSString stringWithFormat:@"%d" ,pathpoint.tag + 1];
            }
        }
        
        [polygonview removeFromSuperview];
    }
    [_scrollView.toDealWithPointView removeAllObjects];
    
    //再删道具
    for(std::set<ItemView*>::iterator it = _toDealWith.begin();it!=_toDealWith.end();it++)
    {
        ItemView* selectedItemview = *it;
        //从编辑器中删
        itemViews.erase(find(itemViews.begin(),itemViews.end(),selectedItemview));
        ids.remove(selectedItemview.tag);
        //将附加的路径点删除
        for (PolygonView* pathpoint : selectedItemview->_pathPoints) {
            [pathpoint removeFromSuperview];
        }
        //取消按钮与门的绑定关系
        if (selectedItemview->itemtype == Gear_Button) {
            Features_GearButton* feat = (Features_GearButton*)selectedItemview->features;
            if (feat->bindID != kDefaultGearButtonBindID) {
                for(ItemView* gate : _gates){
                    if (gate.tag == feat->bindID) {
                        gate.bindButton = nil;
                        break;
                    }
                }
            }
        }
        if (selectedItemview->itemtype == Gear_Gate) {
            if (selectedItemview.bindButton) {
                ((Features_GearButton*)selectedItemview.bindButton->features)->bindID = kDefaultGearButtonBindID;
            }
            _gates.erase(find(_gates.begin(),_gates.end(),selectedItemview));
        }
        
        //从视图中移除
        [selectedItemview removeFromSuperview];
    }
    _toDealWith.clear();
    
    //
    [self refreshSelectedItemInfo];
    
    if(!_scrollView.isScrollEnabled) [_scrollView setScrollEnabled:YES];
    [_scrollView setNeedsDisplay];
    
}

-(void)play:(id)sender
{
    _fileHandler->_items.clear();
    _fileHandler->_polygonsDict->removeAllObjects();
    
    for(ItemView* itemView : itemViews)
    {
        [self saveItemInformationInMemory:itemView];
    }
   // _fileHandler->_items.sort(OrderByHeight);
    
    int result = [_scrollView savePolygonsInfoInMemory];
    if (result!=0) {
        NSString* mess;
        switch (result) {
            case -1:
                mess = [NSString stringWithFormat:@"顶点数超过最大顶点数:%d!",b2_maxPolygonVertices];
                break;
            case 1:
                mess = @"线有交叉,请检查!";
                break;
            default:
                mess = @"未知错误!";
                break;
        }
        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"预览失败!" message:mess delegate:nil cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
        [alertView show];
        [alertView release];
    }else{
        _fileHandler->scrollViewZoomFactor = _scrollView.currentZoomFactor;
        _fileHandler->scrollViewContentOffSet = _scrollView.contentOffset.y;
        _gameManager->_levelEditor->playLevel();
    }
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
        //先把自己的清空
        for(ItemView* itemview : itemViews)
        {
            [itemview removeFromSuperview];
        }
        itemViews.clear();
        ids.clear();
        _toDealWith.clear();
        
        //
        for(PolygonView* centerview in _scrollView.centerViews){
            [centerview removeFromSuperview];
        }
        [_scrollView.centerViews removeAllObjects];
        _scrollView->polygontags.clear();
        [_scrollView.toDealWithPointView removeAllObjects];
        
        //在让文件管理器清空
        _fileHandler->reload();
        //最后重新展示出来
        [self createItemFromFileHandler];
        [self createPolygonsFromFileHandler];
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
    if (type == Polygon_) {
        [_scrollView createNewPolygonAtCenter];
        return [popupViewController backEditor:nil];
    }
    
    int id = [self firstUnusedId];
    float x = 0.5 ;
    float y = PAGE_COUNTS - (_scrollView.contentOffset.y + 0.5*height)/(_scrollView.currentZoomFactor*height);
    float angle = kDefaultAngle;
    float scale = kDefaultScale;
    int localZorder = itemViews.size();
    bool isAnimated = kDefaultAnimatedOnState;
    float triggerTime = kDefaultTriggerTime;
    std::map<std::string,bool> animationControlInstructions;
    std::vector<std::vector<AnimationInfo>> animationInfos;
    void* features = nullptr;
    
    Item item(type,id,x,y,angle,scale,localZorder,isAnimated,triggerTime,animationControlInstructions,animationInfos,features);
    
    ItemView* newItem = [[ItemView alloc] init:item];
    [newItem itemAddGestureRecognizerWithTarget:self];
    
    //添加到数组
    itemViews.push_back(newItem);
    ids.push_back(newItem.tag);
    
    [_scrollView insertSubview:newItem atIndex:localZorder];
    [newItem release];
    
    //返回
    [popupViewController backEditor:newItem];
}

-(void)saveItemInformationInMemory:(ItemView*)itemView
{
    Item_Type type = itemView->itemtype;
    int id = static_cast<int>(itemView.tag);
    float x = itemView.center.x/_scrollView.contentSize.width;
    float y = PAGE_COUNTS - itemView.center.y/(_scrollView.contentSize.height/PAGE_COUNTS);
    float a = itemView.transform.a;
    float b = itemView.transform.b;
    float c = itemView.transform.c;
    float d = itemView.transform.d;
    
    float angle = kDefaultAngle;
    float scale = kDefaultScale;
    int localZorder = [itemView getSubviewIndex];
    bool isAnimated = itemView->isAnimated;
    float triggerTime = itemView->triggerTime;
    std::map<std::string,bool> animationControlInstructions(itemView->_animationControlInstructions);
    std::vector<std::vector<AnimationInfo>> animationInfos;
    std::vector<AnimationInfo> eachAnimationGroupInfos;
    //empty animation group not nessary to save
    if (itemView->_pathPoints.size()!=0) {
        for(int i = 0;i<itemView.animationGroupCount;i++){
            for (PolygonView* pathpoint : itemView->_pathPoints) {
                //refresh inner vector information
                if (i == 0) {
                    Vec2 newPosition = Vec2((pathpoint.center.x-itemView.center.x)/_scrollView.currentZoomFactor,(itemView.center.y-pathpoint.center.y)/_scrollView.currentZoomFactor);
                    pathpoint->_eachGroupCorrespondInfos.at(i).position = newPosition;
                }
                
                eachAnimationGroupInfos.push_back(pathpoint->_eachGroupCorrespondInfos.at(i));
            }
            animationInfos.push_back(eachAnimationGroupInfos);
            eachAnimationGroupInfos.clear();
        }
    }else{
        animationControlInstructions.clear();
    }

    void* features = itemView->features;
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
    //已经取消缩放
    scale = kDefaultScale;
    
    switch (type) {
        case Cicada:
        {
            if(!features){
                Features_Cicada feat;
                features = &feat;
            }
        }
            break;
        case Dragon_Anti:
        {
            if(!features){
                Features_Dragon feat;
                features = &feat;
            }
        }
            break;
        case Dragon_Clockwise:
        {
            if(!features){
                Features_Dragon feat;
                features = &feat;
            }
        }
            break;
        case DoubDragon_Anti:
        {
            if (!features) {
                Features_DoubleDragon feat;
                features = &feat;
            }
        }
            break;
        case DoubDragon_Clockwise:
        {
            if (!features) {
                Features_DoubleDragon feat;
                features = &feat;
            }
        }
            break;
        case Serpent_:
        {
            if (!features) {
                Features_Serpent feat;
                features = &feat;
            }
        }
            break;
        case Gear_Button:
        {
            if (!features) {
                Features_GearButton feat;
                features = &feat;
            }
        }
            break;
        case Gear_Gate:
        {
            if (!features) {
                Features_GearGate feat;
                features = &feat;
            }
        }
            break;
        default:
            break;
    }
    
    Item item(type,id,x,y,angle,scale,localZorder,isAnimated,triggerTime,animationControlInstructions,animationInfos,features);
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
    UITapGestureRecognizer* tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(myViewSingleTap:)];
    tapRecognizer.numberOfTapsRequired = 1;
    [self.view addGestureRecognizer:tapRecognizer];
    [tapRecognizer release];
    
    UIPanGestureRecognizer* panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(myViewPaning:)];
    [self.view addGestureRecognizer:panRecognizer];
    [panRecognizer release];
    
    UIRotationGestureRecognizer* rotateRecognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(handleRotate:)];
    [self.view addGestureRecognizer:rotateRecognizer];
    [rotateRecognizer release];
    
//    UIPinchGestureRecognizer* pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(myScrollViewPreView:)];
//    [self.view addGestureRecognizer:pinchRecognizer];
//    [pinchRecognizer release];
    
    rotateRecognizer.delegate = self;
//    pinchRecognizer.delegate = self;
    
    [tapRecognizer requireGestureRecognizerToFail:panRecognizer];
    
}

-(PolygonView*)clickedPointView:(CGPoint)position
{
    NSEnumerator* reveCenterEnum = [_scrollView.centerViews reverseObjectEnumerator];
    NSEnumerator* revePointEnum;
    PolygonView* centerview = nil;
    PolygonView* pointview = nil;
    
    while(centerview = [reveCenterEnum nextObject]){
        CGPoint localposition = [centerview convertPoint:position fromView:self.view];
        revePointEnum = [centerview.subviews reverseObjectEnumerator];
        while (pointview = [revePointEnum nextObject]) {
            if (CGRectContainsPoint(pointview.frame, localposition)) {
                return pointview;
            }
        }
    }
    
    return nil;
}

-(BOOL)myViewSingleTap:(UITapGestureRecognizer*)recognizer
{
    CGPoint position = [recognizer locationInView:self.view];
    PolygonView* selectedpointview = nil;
    if ((selectedpointview = [self clickedPointView:position])) {
        selectedpointview.isSelected = !selectedpointview.isSelected;
        if (selectedpointview.isSelected) {
            selectedpointview.backgroundColor = selectedpointview.selectedColor;
            [_scrollView.toDealWithPointView addObject:selectedpointview];
        }else{
            selectedpointview.backgroundColor = selectedpointview.defaultColor;
            [_scrollView.toDealWithPointView removeObject:selectedpointview];
        }
        
        if(_toDealWith.size()>0||_scrollView.toDealWithPointView.count>0)
            [_scrollView setScrollEnabled:NO];
        else
            [_scrollView setScrollEnabled:YES];
        return YES;
    }
    
    if (!isItemViewClicked) {
        [self cancelHeightLight];
    }
    return NO;
}

-(void)cancelHeightLight
{
    std::set<ItemView*>::iterator it;
    for(it = _toDealWith.begin();it!=_toDealWith.end();it++)
    {
        ItemView* itemview = *it;
        if ([itemview getHeightLightState]) {
            [itemview hideBorder];
            [itemview cancelHeightLightPathPoints];
            [itemview setHeightLightState:NO];
        }
    }
    _toDealWith.clear();
    
    for(PolygonView* polygonview in _scrollView.toDealWithPointView){
        polygonview.isSelected = NO;
        polygonview.backgroundColor = polygonview.defaultColor;
    }
    [_scrollView.toDealWithPointView removeAllObjects];
    
    //
    [self refreshSelectedItemInfo];
    
    //
    if (!_scrollView.isScrollEnabled) {
        [_scrollView setScrollEnabled:YES];
    }
    
}

-(void)myViewPaning:(UIPanGestureRecognizer*)recognizer
{
    if (!_scrollView.isScrollEnabled) {
        CGPoint translation = [recognizer translationInView:self.view];
        
        //移动道具
        for(std::set<ItemView*>::iterator it = _toDealWith.begin();it != _toDealWith.end();it++){
            ItemView* itemview = *it;
            itemview.center = CGPointMake(itemview.center.x + translation.x, itemview.center.y + translation.y);
            [itemview translateAllPathPoints:translation];
        }
        //移动多边形和路径点
        for (PolygonView* polygon in _scrollView.toDealWithPointView) {
            if ([polygon.pointType isEqualToString:@"pointview"]) {
                PolygonView* centerview = (PolygonView*)polygon.superview;
                if(centerview.isSelected) continue;
            }else if([polygon.pointType isEqualToString:@"pathpoint"]){
                ItemView* parent = polygon->_pathParent;
                if ([parent getHeightLightState]) continue;
            }
            polygon.center = CGPointMake(polygon.center.x + translation.x, polygon.center.y + translation.y);
        }
        
        [recognizer setTranslation:CGPointZero inView:self.view];
        [_scrollView setNeedsDisplay];
        
    }
    
}

-(void)zoomInOut:(id)sender
{
    
    if (!_scrollView.isPreViewed) {
        _scrollView.isPreViewed = YES;
    }
    
    float variation = 0.0;
    if (sender == _zoomInButton) {
        variation = kDefaultZoomStep;
    }else{
        variation = -kDefaultZoomStep;
    }
    
    float newScale = 0.0;
    if (_scrollView.isPreViewed) {
        
        newScale = _scrollView.currentZoomFactor + variation;
        if (newScale<1.0&&newScale>1/PAGE_COUNTS) {
            //重排道具及其路径点
            for(ItemView* view : itemViews)
            {
                view.center = CGPointMake(view.center.x/_scrollView.contentSize.width*newScale*width, view.center.y/_scrollView.contentSize.height*newScale*PAGE_COUNTS*height);
                view.bounds = CGRectMake(0, 0, newScale*view.image.size.width, newScale*view.image.size.height);
                [view refreshBorder];
                
                for(PolygonView* pathpoint : view->_pathPoints){
                    pathpoint.center = CGPointMake(newScale*pathpoint.center.x/_scrollView.contentSize.width*width, newScale*pathpoint.center.y/_scrollView.contentSize.height*PAGE_COUNTS*height);
                    pathpoint.bounds = CGRectMake(0, 0, 2*newScale*kDefaultPathPointRadius, 2*newScale*kDefaultPathPointRadius);
                    //
                    pathpoint.pathNum.frame = CGRectMake(0, 0, 2*newScale*kDefaultPathPointRadius, 2*newScale*kDefaultPathPointRadius);
                    pathpoint.pathNum.font = [UIFont systemFontOfSize:newScale*kDefaultFontSize];
                }
            }
            //重排多边形及其顶点
            for (PolygonView* centerview in _scrollView.centerViews) {
                centerview.center = CGPointMake(newScale*centerview.center.x/_scrollView.contentSize.width*width, newScale*centerview.center.y/_scrollView.contentSize.height*PAGE_COUNTS*height);
                centerview.bounds = CGRectMake(-newScale*kDefaultPolygonPointRadius,-newScale*kDefaultPolygonPointRadius, 2*newScale*kDefaultPolygonPointRadius,2*newScale*kDefaultPolygonPointRadius);
                
                for (PolygonView* pointview in centerview.subviews) {
                    pointview.center = CGPointMake(newScale*pointview.center.x/_scrollView.contentSize.width*width, newScale*pointview.center.y/_scrollView.contentSize.height*PAGE_COUNTS*height);
                    pointview.bounds = CGRectMake(0, 0, 2*newScale*kDefaultPolygonPointRadius, 2*newScale*kDefaultPolygonPointRadius);
                }
            }
            
            float oldContentOffset = _scrollView.contentOffset.y;
            float ration = -1;
            float newContentOffset = 0;
            if (_scrollView.contentSize.height - oldContentOffset - height != 0 ) {
                ration = oldContentOffset/(_scrollView.contentSize.height - oldContentOffset - height);
            }
            
            _scrollView.currentZoomFactor = newScale;
            _scrollView.contentSize = CGSizeMake(newScale*width, newScale*PAGE_COUNTS*height);
            _scrollView.bounds = CGRectMake(0, 0, newScale*width, height);
            
            if (ration != -1) {
                newContentOffset = ration/(ration+1)*(_scrollView.contentSize.height - height);
            }else{
                newContentOffset = _scrollView.contentSize.height - height;
            }
            _scrollView.contentOffset = CGPointMake(_scrollView.contentOffset.x, newContentOffset);
            
        }else if(newScale<=1/PAGE_COUNTS){
            
            for (ItemView* view : itemViews) {
                view.center = CGPointMake(1/PAGE_COUNTS*view.center.x/_scrollView.contentSize.width*width, view.center.y/_scrollView.contentSize.height*height);
                view.bounds = CGRectMake(0, 0, 1/PAGE_COUNTS*view.image.size.width, 1/PAGE_COUNTS*view.image.size.height);
                [view refreshBorder];
                
                for (PolygonView* pathpoint : view->_pathPoints) {
                    pathpoint.center = CGPointMake(1/PAGE_COUNTS*pathpoint.center.x/_scrollView.contentSize.width*width, pathpoint.center.y/_scrollView.contentSize.height*height);
                    pathpoint.bounds = CGRectMake(0, 0, 1/PAGE_COUNTS*2*kDefaultPathPointRadius, 1/PAGE_COUNTS*2*kDefaultPathPointRadius);
                    //
                    pathpoint.pathNum.frame = CGRectMake(0,0 , 1/PAGE_COUNTS*2*kDefaultPathPointRadius, 1/PAGE_COUNTS*2*kDefaultPathPointRadius);
                    pathpoint.pathNum.font = [UIFont systemFontOfSize:1/PAGE_COUNTS*kDefaultFontSize];
                }
            }
            
            for (PolygonView* centerview in _scrollView.centerViews) {
                centerview.center = CGPointMake(1/PAGE_COUNTS*centerview.center.x/_scrollView.contentSize.width*width, centerview.center.y/_scrollView.contentSize.height*height);
                centerview.bounds = CGRectMake(-1/PAGE_COUNTS*kDefaultPolygonPointRadius, -1/PAGE_COUNTS*kDefaultPolygonPointRadius, 1/PAGE_COUNTS*2*kDefaultPolygonPointRadius, 1/PAGE_COUNTS*2*kDefaultPolygonPointRadius);
                
                for (PolygonView* pointview in centerview.subviews) {
                    pointview.center = CGPointMake(1/PAGE_COUNTS*pointview.center.x/_scrollView.contentSize.width*width, pointview.center.y/_scrollView.contentSize.height*height);
                    pointview.bounds = CGRectMake(0, 0, 1/PAGE_COUNTS*2*kDefaultPolygonPointRadius, 1/PAGE_COUNTS*2*kDefaultPolygonPointRadius);
                }
            }
            
            _scrollView.currentZoomFactor = 1/PAGE_COUNTS;
            _scrollView.contentSize = CGSizeMake(1/PAGE_COUNTS*width, height);
            _scrollView.bounds = CGRectMake(0, 0, 1/PAGE_COUNTS*width, height);
            
        }else if(newScale>=1.0){
            
            for (ItemView* view : itemViews) {
                view.center = CGPointMake(view.center.x/_scrollView.contentSize.width*width, view.center.y/_scrollView.contentSize.height*PAGE_COUNTS*height);
                view.bounds = CGRectMake(0, 0, view.image.size.width, view.image.size.height);
                [view refreshBorder];
                
                for (PolygonView* pathpoint : view->_pathPoints) {
                    pathpoint.center = CGPointMake(pathpoint.center.x/_scrollView.contentSize.width*width, pathpoint.center.y/_scrollView.contentSize.height*PAGE_COUNTS*height);
                    pathpoint.bounds = CGRectMake(0, 0, 2*kDefaultPathPointRadius, 2*kDefaultPathPointRadius);
                    //
                    pathpoint.pathNum.frame = CGRectMake(0, 0, 2*kDefaultPathPointRadius, 2*kDefaultPathPointRadius);
                    pathpoint.pathNum.font = [UIFont systemFontOfSize:kDefaultFontSize];
                }
            }
            
            for (PolygonView* centerview in _scrollView.centerViews) {
                centerview.center = CGPointMake(centerview.center.x/_scrollView.contentSize.width*width, centerview.center.y/_scrollView.contentSize.height*PAGE_COUNTS*height);
                centerview.bounds = CGRectMake(-kDefaultPolygonPointRadius, -kDefaultPolygonPointRadius, 2*kDefaultPolygonPointRadius, 2*kDefaultPolygonPointRadius);
                
                for (PolygonView* pointview in centerview.subviews) {
                    pointview.center = CGPointMake(pointview.center.x/_scrollView.contentSize.width*width, pointview.center.y/_scrollView.contentSize.height*PAGE_COUNTS*height);
                    pointview.bounds = CGRectMake(0, 0, 2*kDefaultPolygonPointRadius, 2*kDefaultPolygonPointRadius);
                }
            }
            
            float oldContentOffset = _scrollView.contentOffset.y;
            float ration = -1;
            float newContentOffset = 0;
            if (_scrollView.contentSize.height - oldContentOffset - height != 0 ) {
                ration = oldContentOffset/(_scrollView.contentSize.height - oldContentOffset - height);
            }
            
            _scrollView.currentZoomFactor = 1.0;
            _scrollView.contentSize = CGSizeMake(width, PAGE_COUNTS*height);
            _scrollView.bounds = CGRectMake(0, 0, width, height);
            
            if (ration != -1) {
                newContentOffset = ration/(ration+1)*(_scrollView.contentSize.height - height);
            }else{
                newContentOffset = _scrollView.contentSize.height - height;
            }
            _scrollView.contentOffset = CGPointMake(_scrollView.contentOffset.x, newContentOffset);
            
        }
    }
    

    if (_scrollView.currentZoomFactor == 1.0) {
        if (_scrollView.isPreViewed) {
            _scrollView.isPreViewed = NO;
        }
    }

}

-(void)hideControlPanelAndOperation
{
    for (UIButton* button : needHidenButtons) {
        if (button != _zoomInButton && button != _zoomOutButton) {
            [button setHidden:YES];
        }
    }
    [_hideButton setHidden:YES];
    [solidLayer setHidden:YES];
    [dashLayer setHidden:YES];
    
    for(ItemView* itemview : itemViews){
        [itemview setUserInteractionEnabled:NO];
        for (PolygonView* pathpoint : itemview->_pathPoints) {
            [pathpoint setUserInteractionEnabled:NO];
        }
    }
    for (PolygonView* centerview in _scrollView.centerViews) {
        [centerview setUserInteractionEnabled:NO];
    }
    
}

-(void)showControlPanelAndOperation
{
    for (UIButton* button : needHidenButtons) {
        [button setHidden:NO];
    }
    [_hideButton setHidden:NO];
    [solidLayer setHidden:NO];
    [dashLayer setHidden:NO];
    
    for (ItemView* itemview : itemViews) {
        [itemview setUserInteractionEnabled:YES];
        for (PolygonView* pathpoint : itemview->_pathPoints) {
            [pathpoint setUserInteractionEnabled:YES];
        }
    }
    for (PolygonView* centerview in _scrollView.centerViews) {
        [centerview setUserInteractionEnabled:YES];
    }
    
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



//
//  FeaturesPickerController.m
//  Grow
//
//  Created by 林纲 王 on 14-10-19.
//
//

#import "FeaturesPickerController.h"
#import "UIViewExtension.h"
#import "PolygonView.h"
#include "GameManager.h"
#include "LevelEditor.h"

@interface FeaturesPickerController ()

@end

@implementation FeaturesPickerController

@synthesize _pressedView;
@synthesize viewType;
@synthesize _options;
@synthesize _titleArray;
@synthesize _dataArray;
@synthesize _delegate;

- (id)initWithStyle:(UITableViewStyle)style andUIView:(UIView*)uiview
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
        self._pressedView = uiview;
        if ([uiview isKindOfClass:[ItemView class]]) {
            self.viewType = ITEM;
        }else if ([uiview isKindOfClass:[PolygonView class]]){
            PolygonView* view = (PolygonView*)uiview;
            if ([view.itemtype isEqualToString:@"Trigger"]) {
                self.viewType = TRIGGER;
            }else if ([view.pointType isEqualToString:@"pathpoint"]){
                self.viewType = PATHPOINT;
                
            }
        }
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    self.clearsSelectionOnViewWillAppear = NO;
    self._options = [NSMutableArray array];
    self._titleArray = [NSMutableArray array];
    self._dataArray = [NSMutableArray array];
    self.preferredContentSize = CGSizeMake(200, 200);
    switch (self.viewType) {
        case ITEM:
            [self._options addObject:@"upper"];
            [self._options addObject:@"lowest"];
            [self._options addObject:@"property"];
            [self._options addObject:@"addPathPoint"];
            break;
        case TRIGGER:
        {
            self.tableView.allowsMultipleSelection = YES;
            [_titleArray addObjectsFromArray:@[@"蝉",@"芽菜",@"岩石"]];
            
            NSMutableArray* allCicada = [NSMutableArray array];
            NSMutableArray* allSprouts = [NSMutableArray array];
            NSMutableArray* allRock = [NSMutableArray array];
            GameManager* gameManager = GameManager::getInstance();
            for (ItemView* itemview : gameManager->_levelEditor->_myViewController->itemViews) {
                switch (itemview->itemtype) {
                    case Cicada:
                        [allCicada addObject:itemview];
                        break;
                    case Sprouts_Dextro:
                        [allSprouts addObject:itemview];
                        break;
                    case Sprouts_Levo:
                        [allSprouts addObject:itemview];
                        break;
                    case Sprouts_Slope:
                        [allSprouts addObject:itemview];
                        break;
                        
                    default:
                        break;
                }
                if (itemview->itemtype>=Rock_Circle && itemview->itemtype<=Rock_Trape) {
                    [allRock addObject:itemview];
                }
                
            }
            [allCicada sortUsingSelector:@selector(compareWithID:)];
            [allSprouts sortUsingSelector:@selector(compareWithID:)];
            [allRock sortUsingSelector:@selector(compareWithID:)];
            
            [_dataArray addObject:allCicada];
            [_dataArray addObject:allSprouts];
            [_dataArray addObject:allRock];
            [self.tableView reloadData];
            
            bindIDs = ((PolygonView*)self._pressedView)._bindIDs;
            for (int i = 0; i<_dataArray.count; i++) {
                int count = ((NSArray*)_dataArray[i]).count;
                for (int j = 0; j<count; j++) {
                    ItemView* itemview = ((NSArray*)_dataArray[i])[j];
                    if (itemview->bindedTriggerID==self._pressedView.tag && [bindIDs containsObject:[NSNumber numberWithInt:itemview.tag]]) {
                        NSIndexPath* indexPath = [NSIndexPath indexPathForRow:j inSection:i];
                        [self.tableView selectRowAtIndexPath:indexPath animated:NO scrollPosition:UITableViewScrollPositionNone];
                    }
                }
            }
        }
            break;
        case PATHPOINT:
        {
            int count = ((PolygonView*)self._pressedView)->_eachGroupCorrespondInfos.size();
            for(int i = 0;i<count;i++){
                [self._options addObject:[NSString stringWithFormat:@"第%d组",i]];
            }
        }
            break;
        default:
            break;
    }
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    switch (self.viewType) {
        case TRIGGER:
            return self._titleArray.count;
            break;
        default:
            return 1;
            break;
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    switch (self.viewType) {
        case TRIGGER:
            return [_dataArray[section] count];
            break;
            
        default:
            return [self._options count];
            break;
    }
    
}

-(NSString*)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    switch (self.viewType) {
        case TRIGGER:
            return self._titleArray[section];
            break;
        default:
            return nil;
            break;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString* itemTableIdentifier = @"itemTableIdentifier";
    static NSString* triggerTableIdentifier = @"triggerTableIdentifier";
    static NSString* pathPointTableIdentifier = @"pathPointTableIdentifier";
    UITableViewCell* cell;
    
    switch (self.viewType) {
        case ITEM:
        {
            cell = [tableView dequeueReusableCellWithIdentifier:itemTableIdentifier];
            if (!cell) {
                cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:itemTableIdentifier] autorelease];
                switch (indexPath.row) {
                    case 0:
                    {
                        UIImageView* upIcon = [[[UIImageView alloc] initWithImage:[UIImage imageNamed:@"ToTop.png"]] autorelease];
                        upIcon.bounds = CGRectMake(0, 0, 30, 30);
                        cell.accessoryView = upIcon;
                    }
                        break;
                    case 1:
                    {
                        UIImageView* downIcon = [[[UIImageView alloc] initWithImage:[UIImage imageNamed:@"ToBottom.png"]] autorelease];
                        downIcon.bounds = CGRectMake(0, 0, 30, 30);
                        cell.accessoryView = downIcon;
                    }
                        break;
                    case 2:
                        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                        break;
                    case 3:
                        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                        break;
                        
                    default:
                        break;
                }
            }
            
            cell.textLabel.backgroundColor = [UIColor clearColor];
            cell.textLabel.text = [self._options objectAtIndex:indexPath.row];
            
        }
            break;
            
        case TRIGGER:
        {
            cell = [tableView dequeueReusableCellWithIdentifier:triggerTableIdentifier];
            if (!cell) {
                cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:triggerTableIdentifier] autorelease];
            }
            
            ItemView* itemview = ((NSArray*)_dataArray[indexPath.section])[indexPath.row];
            cell.textLabel.text = [NSString stringWithFormat:@"%d",itemview.tag];
            cell.textLabel.textColor = itemview->bindedTriggerID==kDefaultBindedTriggerID? [UIColor greenColor] : [UIColor redColor];
            NSArray* indexPaths = [self.tableView indexPathsForSelectedRows];
            cell.accessoryType = [indexPaths containsObject:indexPath]? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
        }
            break;
            
        case PATHPOINT:
        {
            cell = [tableView dequeueReusableCellWithIdentifier:pathPointTableIdentifier];
            if (!cell) {
                cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:pathPointTableIdentifier] autorelease];
                cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
            }
            cell.textLabel.text = [self._options objectAtIndex:indexPath.row];
        }
            break;
            
        default:
            break;
    }
    
    cell.showsReorderControl = YES;
    cell.selectionStyle = UITableViewCellSelectionStyleBlue;

    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (self.viewType) {
        case TRIGGER:
        {
            UITableViewCell* cell = [tableView cellForRowAtIndexPath:indexPath];
            cell.accessoryType = UITableViewCellAccessoryCheckmark;
            cell.textLabel.textColor = [UIColor redColor];
            
            ItemView* itemview = ((NSArray*)_dataArray[indexPath.section])[indexPath.row];
            NSNumber* number = [NSNumber numberWithInt:itemview.tag];
            if (![bindIDs containsObject:number]) {
                [bindIDs addObject:number];
            }
            
            GameManager* gameManager = GameManager::getInstance();
            if (itemview->bindedTriggerID != self._pressedView.tag) {
                //
                if (itemview->bindedTriggerID != kDefaultBindedTriggerID) {
                    for (PolygonView* polygonview in gameManager->_levelEditor->_myViewController->_scrollView.centerViews) {
                        if ([polygonview.itemtype isEqualToString:@"Trigger"] && polygonview.tag==itemview->bindedTriggerID) {
                            [polygonview._bindIDs removeObject:[NSNumber numberWithInt:itemview.tag]];
                            break;
                        }
                    }
                }
                //
                itemview->bindedTriggerID = self._pressedView.tag;
            }
            
        }
            break;
            
        default:
            if (self._delegate) {
                NSString* option = [self._options objectAtIndex:indexPath.row];
                [self._delegate optionSelected:option];
            }
            break;
    }
    
}

-(void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (self.viewType) {
        case TRIGGER:
        {
            UITableViewCell* cell = [tableView cellForRowAtIndexPath:indexPath];
            cell.accessoryType = UITableViewCellAccessoryNone;
            cell.textLabel.textColor = [UIColor greenColor];
            
            int Id = atoi([cell.textLabel.text UTF8String]);
            [bindIDs removeObject:[NSNumber numberWithInt:Id]];
            ItemView* itemview = ((NSArray*)_dataArray[indexPath.section])[indexPath.row];
            itemview->bindedTriggerID = kDefaultBindedTriggerID;
        }
            break;
            
        default:
            break;
    }
    
}

-(void)viewDidDisappear:(BOOL)animated
{
    self._pressedView = nil;
    self._options = nil;
    self._titleArray = nil;
    self._dataArray = nil;
    self._delegate = nil;
    [super viewDidDisappear:animated];
}

-(void)dealloc
{
    self._pressedView = nil;
    self._options = nil;
    self._titleArray = nil;
    self._dataArray = nil;
    self._delegate = nil;
    [super dealloc];
}

@end

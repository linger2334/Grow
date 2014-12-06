//
//  FeaturesPickerController.m
//  Grow
//
//  Created by 林纲 王 on 14-10-19.
//
//

#import "FeaturesPickerController.h"

@interface FeaturesPickerController ()

@end

@implementation FeaturesPickerController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    self.clearsSelectionOnViewWillAppear = NO;
    self.contentSizeForViewInPopover = CGSizeMake(100, 40);
    self._options = [NSMutableArray array];
    [self._options addObject:@"upper"];
    [self._options addObject:@"lowest"];
    [self._options addObject:@"property"];
    [self._options addObject:@"addPathPoint"];
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
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return [self._options count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString* simpleTableIdentifier = @"simpleTableIdentifier";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:simpleTableIdentifier];
    if (!cell) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:simpleTableIdentifier] autorelease];
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        cell.showsReorderControl = YES;
        cell.selectionStyle = UITableViewCellSelectionStyleBlue;
    }
    cell.textLabel.backgroundColor = [UIColor clearColor];
    cell.textLabel.text = [self._options objectAtIndex:indexPath.row];

    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (self._delegate) {
        NSString* option = [self._options objectAtIndex:indexPath.row];
        [self._delegate optionSelected:option];
    }
}

-(void)dealloc
{
    self._options = nil;
    self._delegate = nil;
    [super dealloc];
}

@end

//
//  PopupViewController.m
//  Grow
//
//  Created by wlg on 14-9-14.
//
//

#import "PopupViewController.h"
#import "Macro.h"
#import "GameManager.h"

@implementation PopupViewController

@synthesize scrollview;

-(void)viewDidLoad
{
    [super viewDidLoad];
    self.scrollview.contentSize = CGSizeMake(768, 2048);
    self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
    self.backButton.frame = CGRectMake(280, 900, 212, 110);
    [self.view addSubview:self.backButton];
    
    std::string imageName;
    int levelId = *(GameManager::getInstance()->_fileHandler->_filename.c_str() + strlen("levels/level ")) - ('0' - 0);
    std::string suffix = StringUtils::format("%d",levelId);
    std::set<UIView*> _toDelete;
    for(UIButton* button in self.scrollview.subviews){
        if (button.tag<=Rock_Trape && button.tag>=Rock_Circle) {
            switch (button.tag) {
                case Rock_Circle:
                    imageName = IMAGE_ROCK_CIRCLE;
                    break;
                case Rock_Cover:
                    imageName = IMAGE_ROCK_COVER;
                    break;
                case Rock_Crinkle:
                    imageName = IMAGE_ROCK_CRINKLE;
                    break;
                case Rock_Cross:
                    imageName = IMAGE_ROCK_CROSS;
                    break;
                case Rock_Dagger:
                    imageName = IMAGE_ROCK_DAGGER;
                    break;
                case Rock_Ellipse:
                    imageName = IMAGE_ROCK_ELLIPSE;
                    break;
                case Rock_Mount:
                    imageName = IMAGE_ROCK_MOUNT;
                    break;
                case Rock_MountInv:
                    imageName = IMAGE_ROCK_MOUNTINV;
                    break;
                case Rock_Ordinary:
                    imageName = IMAGE_ROCK_ORDINARY;
                    break;
                case Rock_Ovoid:
                    imageName = IMAGE_ROCK_OVOID;
                    break;
                case Rock_Pebble:
                    imageName = IMAGE_ROCK_PEBBLE;
                    break;
                case Rock_Pillar:
                    imageName = IMAGE_ROCK_PILLAR;
                    break;
                case Rock_Pocket:
                    imageName = IMAGE_ROCK_POCKET;
                    break;
                case Rock_Rect:
                    imageName = IMAGE_ROCK_RECT;
                    break;
                case Rock_Trape:
                    imageName = IMAGE_ROCK_TRAPE;
                    break;
                default:
                    break;
            }
            imageName += suffix;
            NSString* fullPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:imageName.c_str()] ofType:@".png"];
            if ([[NSFileManager defaultManager] fileExistsAtPath:fullPath]) {
                imageName += ".png";
                [button setImage:[UIImage imageNamed:[NSString stringWithUTF8String:imageName.c_str()]] forState:UIControlStateNormal];
            }else{
                _toDelete.insert(button);
            }
            
        }
    }
    
    for(UIView* view : _toDelete){
        [view removeFromSuperview];
    }
    
}


-(IBAction)backEditor:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)dealloc
{
    self.backButton = nil;
    [super dealloc];
}

@end

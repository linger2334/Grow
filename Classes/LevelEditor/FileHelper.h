//
//  FileHelper.h
//  GrowUp
//
//  Created by wlg on 8/21/14.
//
//

#import <Foundation/Foundation.h>

@interface FileHelper : NSObject

+(NSString*) fullFilenameInDocumentsDirectory:(NSString*)filename;
+(BOOL)fileExistsInDocumentsDirectory:(NSString*)filename;
+(NSString*) dataFilePathForFileWithName:(NSString*)filename withExtension:(NSString*)extension forSave:(BOOL)forSave;
+(void)createFolder:(NSString*)foldername;

@end

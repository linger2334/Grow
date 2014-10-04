//
//  FileHelper.m
//  GrowUp
//
//  Created by wlg on 8/21/14.
//
//

#import "FileHelper.h"

@implementation FileHelper

+(NSString*)fullFilenameInDocumentsDirectory:(NSString *)filename{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectoryPath = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectoryPath stringByAppendingPathComponent:filename];
    return filePath;
}

+(BOOL)fileExistsInDocumentsDirectory:(NSString *)filename{
    NSString* filePath = [FileHelper fullFilenameInDocumentsDirectory:filename];
    return [[NSFileManager defaultManager] fileExistsAtPath:filePath];
}

+(NSString *)dataFilePathForFileWithName:(NSString *)filename withExtension:(NSString *)extension forSave:(BOOL)forSave{
    NSString *filenameWithExt = [filename stringByAppendingString:extension];
    if (forSave||
        [FileHelper fileExistsInDocumentsDirectory:filenameWithExt]) {
        return [FileHelper fullFilenameInDocumentsDirectory:filenameWithExt];
    }
    else
    {
        return [[NSBundle mainBundle] pathForResource:filename  ofType:extension];
    }
}

+(void)createFolder:(NSString *)foldername{
    NSString *dataPath = [FileHelper fullFilenameInDocumentsDirectory:foldername];
    if (![[NSFileManager defaultManager] fileExistsAtPath:dataPath]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:dataPath withIntermediateDirectories:NO attributes:nil error:nil];
    }
}

@end

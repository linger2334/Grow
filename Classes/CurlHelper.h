//
//#ifndef __Grow_Goods_test__CurlHelper__
//#define __Grow_Goods_test__CurlHelper__
//#include <stdlib.h>
//#include <stdio.h>
//#include "curl.h"
//#include <sys/stat.h>
///* parse headers for Content-Length */
//size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//    int r;
//    long len = 0;
//    /* _snscanf() is Win32 specific */
//    //r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
//    r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
//    if (r) /* Microsoft: we don't read the specs */
//        *((long *) stream) = len;
//    return size * nmemb;
//}
///* discard downloaded data */
//size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//    return size * nmemb;
//}
////write data to upload
//size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//    return fwrite(ptr, size, nmemb, (FILE*)stream);
//}
///* read data to upload */
//size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//    FILE *f = (FILE*)stream;
//    size_t n;
//    if (ferror(f))
//        return CURL_READFUNC_ABORT;
//    n = fread(ptr, size, nmemb, f) * size;
//    return n;
//}
////int upload(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)
////{
////    FILE *f;
////    long uploaded_len = 0;
////    CURLcode r = CURLE_GOT_NOTHING;
////    int c;
////    f = fopen(localpath, "rb");
////    if (f == NULL) {
////        perror(NULL);
////        return 0;
////    }
////    curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
////    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
////    curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "spider:spider");
////    if (timeout)
////        curl_easy_setopt(curlhandle, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
////    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
////    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &uploaded_len);
////    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, discardfunc);
////    curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
////    curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
////    curl_easy_setopt(curlhandle, CURLOPT_FTPPORT, "-"); /* disable passive mode */
////    curl_easy_setopt(curlhandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
////    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
////    for (c = 0; (r != CURLE_OK) && (c < tries); c++) {
////        /* are we resuming? */
////        if (c) { /* yes */
////            /* determine the length of the file already written */
////            /*
////             * With NOBODY and NOHEADER, libcurl will issue a SIZE
////             * command, but the only way to retrieve the result is
////             * to parse the returned Content-Length header. Thus,
////             * getcontentlengthfunc(). We need discardfunc() above
////             * because HEADER will dump the headers to stdout
////             * without it.
////             */
////            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
////            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 1L);
////            r = curl_easy_perform(curlhandle);
////            if (r != CURLE_OK)
////                continue;
////            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 0L);
////            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 0L);
////            fseek(f, uploaded_len, SEEK_SET);
////            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 1L);
////        }
////        else { /* no */
////            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 0L);
////        }
////        r = curl_easy_perform(curlhandle);
////    }
////    fclose(f);
////    if (r == CURLE_OK)
////        return 1;
////    else {
////        fprintf(stderr, "%s\n", curl_easy_strerror(r));
////        return 0;
////    }
////}
//// 下载
//int download(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)
//{
//    FILE *f;
//    curl_off_t local_file_len = -1 ;
//    long filesize =0 ;
//    CURLcode r = CURLE_GOT_NOTHING;
//    struct stat file_info;
//    int use_resume = 0;
//    //获取本地文件大小信息
//    if(stat(localpath, &file_info) == 0)
//    {
//        local_file_len = file_info.st_size;
//        use_resume = 1;
//    }
//    //追加方式打开文件，实现断点续传
//    f = fopen(localpath, "ab+");
//    if (f == NULL) {
//        perror(NULL);
//        return 0;
//    }
//    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
//    curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "spider:spider");
//    //连接超时设置
//    curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);
//    //设置头处理函数
//    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
//    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
//    // 设置断点续传
//    curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
//    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, writefunc);
//    curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
//    curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
//    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
//    r = curl_easy_perform(curlhandle);
//    fclose(f);
//    if (r == CURLE_OK)
//        return 1;
//    else {
//        fprintf(stderr, "%s\n", curl_easy_strerror(r));
//        return 0;
//    }
//}
//
//#endif /* defined(__Grow_Goods_test__CurlHelper__) */

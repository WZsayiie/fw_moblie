//Don't change this file manually, it was generated by automation tool.

#import "CQFoundation.h"
#import "CQBridge.h"

CQ_C_LINK void CQ_Logger_info(CQValue message, CQValue file, CQValue line) {
    NSString *aa = CQGetNSString(message);
    NSString *bb = CQGetNSString(file);
    int32_t cc = CQGetInt32(line);
    [CQLogger info:aa file:bb line:cc];
}

CQ_C_LINK void CQ_Logger_error(CQValue message, CQValue file, CQValue line) {
    NSString *aa = CQGetNSString(message);
    NSString *bb = CQGetNSString(file);
    int32_t cc = CQGetInt32(line);
    [CQLogger error:aa file:bb line:cc];
}
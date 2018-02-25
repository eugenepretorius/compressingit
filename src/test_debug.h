//
//  test_debug.h
//  compress_console
//
//  Created by Eugene Pretorius on 2018/02/25.
//  Copyright © 2018 Eugene Pretorius. All rights reserved.
//

/*****************************************************
 Code to use for debugging. The basic philosophy is that
 debug code can be easily excluded at any point without
 having to go and remove every line. The MACROS defined
 below is basically changed to expand to empty macros.
 Original code from: http://c.learncodethehardway.org/book/ex20.html
 
 To print out debug information
 define DEBUG before the debug.h include or in makefile,
 e.g. -DDEBUG
 ******************************************************/

#ifndef test_debug_h
#define test_debug_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define debug(M, ...) printf(M, ##__VA_ARGS__)
#define log_err(M, ...) printf("[ERROR] (%s:%d) " M "\n", __FILE__, __LINE__,  ##__VA_ARGS__)
#define log_warn(M, ...) printf("[WARNING] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_info(M, ...) printf("[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
    
#else
// empty macros
#define debug(M, ...)
#define log_err(M, ...)
#define log_warn(M, ...)
#define log_info(M, ...)

#endif  //DEBUG
    
#ifdef __cplusplus
}
#endif
        
#endif /* test_debug_h */

//
//  MyOpenGLView.h
//  Neural Network Training
//
//  Created by Day Day Up on 2/5/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <GLUT/GLUT.h>

#define MAX_ITER    2000

@interface MyOpenGLView : NSOpenGLView {
    bool *precision;
    bool readyToDraw;
    int state;
    int elementCount;
    int n_iter,currentIter;
    float jmem[MAX_ITER+1],vmem[MAX_ITER+1],pmem[MAX_ITER+1];
}

- (void)initValues:(int)count;
- (void)setReady:(bool)value;
- (void)setPrecision:(bool*)numbers;
- (void)setState:(int)value;
- (void)setIter:(int)value;
- (void)setCurrentIter:(int)value;
- (void)setJValues:(float*)values;
- (void)setVValues:(float*)values;
- (void)setPValues:(float*)values;


@end

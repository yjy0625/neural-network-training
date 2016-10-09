//
//  MyOpenGLView.m
//  Neural Network Training
//
//  Created by Day Day Up on 2/5/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#import "MyOpenGLView.h"

@implementation MyOpenGLView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    glEnable(GL_ALPHA);
    glEnable(GL_ALPHA_TEST);
    
    if(readyToDraw){
        if(state==0){
            glClearColor(0,0,0,0);
            glClear (GL_COLOR_BUFFER_BIT);
            glPushMatrix ();
            glBegin (GL_LINES);
            
            for(int i=1;i<=currentIter-1;i++){
                glColor4f (1,1,0,1);
                glVertex2f ((float)(i-1)/(n_iter-1)*2-1, jmem[i]/jmem[1]*2-1);
                glVertex2f ((float)(i)/(n_iter-1)*2-1, jmem[i+1]/jmem[1]*2-1);
            }
            
            glEnd();
            
            glPopMatrix ();
            glFlush();
        }
        else if(state==1){
            glClearColor (0,0,0,1);
            glClear (GL_COLOR_BUFFER_BIT);
            glPushMatrix ();
            glBegin (GL_LINES);
            
            for(int i=1;i<=currentIter-1;i++){
                glColor4f (1,0,0,1);
                glVertex2f ((float)(i-1)/(n_iter-1)*2-1, vmem[i]*2-1);
                glVertex2f ((float)(i)/(n_iter-1)*2-1, vmem[i+1]*2-1);
                
                glColor4f (0,0,1,1);
                glVertex2f ((float)(i-1)/(n_iter-1)*2-1, pmem[i]*2-1);
                glVertex2f ((float)(i)/(n_iter-1)*2-1, pmem[i+1]*2-1);
            }
            
            glEnd();
            
            glPopMatrix ();
            glFlush();
        }
        else if(state==2){
            glClearColor (0,0,0,1);
            glClear (GL_COLOR_BUFFER_BIT);
            glPushMatrix ();
            
            int length = sqrt(elementCount);
            if(length*length!=elementCount)length++;
            
            for(int i=1;i<=length*length;i++){
                glBegin (GL_POLYGON);
                float x=(2.0*(i%length)-1.0-length)/length,y=-(2.0*(i/length+1)-1.0-length)/length;
                if(i%length==0){x+=2;y+=2.0/length;}
                if(i<=elementCount){
                    glColor3f (precision[i],precision[i],precision[i]);
                    //glColor3f ((float)(i%length)/length,(float)(i%length)/length,(float)(i/length)/length);
                }
                else {
                    glColor3f (0.5,0.5,0.5);
                }
                glVertex2f (x-0.9/length,y-0.9/length);
                glVertex2f (x-0.9/length,y+0.9/length);
                glVertex2f (x+0.9/length,y+0.9/length);
                glVertex2f (x+0.9/length,y-0.9/length);
                glEnd();
            }
            glPopMatrix ();
            glFlush();
        }
    }
}

- (void)initValues:(int)count{
    elementCount = count;
    readyToDraw=false;
    for(int i=1;i<=MAX_ITER+1;i++){
        jmem[i]=0;
        vmem[i]=0;
        pmem[i]=0;
    }
    state = 0;
    n_iter = 0;
    currentIter = 0;
}

- (void)setReady:(bool)value {
    readyToDraw = value;
}

- (void)setPrecision:(bool *)numbers {
    precision = numbers;
}

- (void)setJValues:(float *)values {
    for(int i=1;i<=MAX_ITER;i++){
        jmem[i]=values[i];
    }
}

- (void)setVValues:(float *)values {
    for(int i=1;i<=MAX_ITER;i++){
        vmem[i]=values[i];
    }
}


- (void)setPValues:(float *)values {
    for(int i=1;i<=MAX_ITER;i++){
        pmem[i]=values[i];
    }
}


- (void)setState:(int)value {
    state = value;
}

- (void)setIter:(int)value {
    n_iter = value;
}

- (void)setCurrentIter:(int)value {
    currentIter = value;
}

@end

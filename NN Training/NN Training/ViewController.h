//
//  ViewController.h
//  Neural Network Training
//
//  Created by Day Day Up on 2/5/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MyOpenGLView.h"
#import "NNTraining.h"

@interface ViewController : NSViewController <NSTextFieldDelegate, NSTableViewDataSource> {
    int nx,ny,nt;
    bool isTraining;
    float jmem[MAX_ITER+1],vmem[MAX_ITER+1],pmem[MAX_ITER+1];
    int currentIter,totalIter;
    bool *precision;
    bool itemsShouldBeSelectable;
    NNTraining newNN;
}

@property (weak) IBOutlet MyOpenGLView *theView;
@property (weak) IBOutlet NSSegmentedControl *segmentControl;

@property (weak) IBOutlet NSTextField *layerField;
@property (weak) IBOutlet NSTextField *node1Field;
@property (weak) IBOutlet NSTextField *node2Field;
@property (weak) IBOutlet NSTextField *node3Field;
@property (weak) IBOutlet NSTextField *node4Field;
@property (weak) IBOutlet NSTextField *node5Field;
@property (weak) IBOutlet NSTextField *trainField;
@property (weak) IBOutlet NSTextField *testField;
@property (weak) IBOutlet NSComboBox *xBox;
@property (weak) IBOutlet NSTextField *xDirField;
@property (weak) IBOutlet NSTextField *xPrefixField;
@property (weak) IBOutlet NSComboBox *yBox;
@property (weak) IBOutlet NSTextField *yDirField;
@property (weak) IBOutlet NSTextField *welcomeField;
@property (weak) IBOutlet NSTableView *tableView;
@property (weak) IBOutlet NSScrollView *scrollView;

- (IBAction)layerSet:(NSTextField *)sender;

@property (weak) IBOutlet NSTextField *aField;
@property (weak) IBOutlet NSTextField *lambdaField;
@property (weak) IBOutlet NSTextField *iterField;

@property (weak) IBOutlet NSButton *browseXBtn;
@property (weak) IBOutlet NSButton *browseYBtn;
@property (weak) IBOutlet NSButton *updateRateBtn;
@property (weak) IBOutlet NSButton *updateLambdaBtn;
@property (weak) IBOutlet NSButton *updateIterBtn;

@property (weak) IBOutlet NSButton *startBtn;
@property (weak) IBOutlet NSButton *stopBtn;

- (void)setSelectability;
- (void)setNodeCountAvalability;
- (void)drawRectInView;
- (IBAction)startTraining:(NSButton *)sender;
- (IBAction)stopTraining:(NSButton *)sender;
- (void)performTraining;
- (IBAction)setViewState:(NSSegmentedControl *)sender;
- (IBAction)browseFile:(NSButton *)sender;
- (IBAction)updateParameter:(NSButton *)sender;

@end


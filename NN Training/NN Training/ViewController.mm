//
//  ViewController.mm
//  Neural Network Training
//
//  Created by Day Day Up on 2/5/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#import "ViewController.h"
#include <fstream>

using namespace std;

#define X_FROM_FILE 0
#define X_FROM_IMGS 1
#define Y_FROM_BOOL 0
#define Y_FROM_INTS 1

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [_theView setReady:false];
    [self stopBtn].enabled=false;
    [self updateRateBtn].enabled=false;
    [self updateLambdaBtn].enabled=false;
    [self updateIterBtn].enabled=false;
    // Do any additional setup after loading the view.
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)setSelectability {
    bool boolValue = itemsShouldBeSelectable;
    [self layerField].enabled   = boolValue;
    [self node1Field].enabled   = boolValue;
    [self node2Field].enabled   = boolValue;
    [self node3Field].enabled   = boolValue;
    [self node4Field].enabled   = boolValue;
    [self node5Field].enabled   = boolValue;
    [self trainField].enabled   = boolValue;
    [self testField].enabled    = boolValue;
    [self xBox].enabled         = boolValue;
    [self browseXBtn].enabled   = boolValue;
    [self xPrefixField].enabled = boolValue;
    [self yBox].enabled         = boolValue;
    [self browseYBtn].enabled   = boolValue;
    
    [self setNodeCountAvalability];
    
    [self stopBtn].enabled=         !boolValue;
    [self startBtn].enabled=         boolValue;
    [self updateRateBtn].enabled=   !boolValue;
    [self updateLambdaBtn].enabled= !boolValue;
    [self updateIterBtn].enabled=   !boolValue;
}

- (IBAction)startTraining:(NSButton *)sender {
    _theView.hidden = false;
    [self welcomeField].hidden = true;
    //[self clearContent];
    [NSThread detachNewThreadSelector:@selector(performTraining)
                             toTarget:self
                           withObject:nil];
}

- (void)performTraining{
    itemsShouldBeSelectable = false;
    [self performSelectorOnMainThread:@selector(setSelectability) withObject:nil waitUntilDone:true];
    isTraining = true;
    [_theView initValues:[[self testField] intValue]];
    [_theView setReady:false];
    [_theView setIter:[[self iterField] intValue]];
    [_theView setState:(int)[[self segmentControl] selectedSegment]];
    
    int unitCounts[5];
    unitCounts[0]=[[self node1Field] intValue];
    unitCounts[1]=[[self node2Field] intValue];
    unitCounts[2]=[[self node3Field] intValue];
    unitCounts[3]=[[self node4Field] intValue];
    unitCounts[4]=[[self node5Field] intValue];
    
    newNN.setLayerNumber([[self layerField] intValue]);
    newNN.setUnitsNumber(unitCounts);
    newNN.setTrainNumber([[self trainField] intValue]);
    newNN.setTestsNumber([[self testField] intValue]);
    newNN.setLearningRate([[self aField] doubleValue]);
    newNN.setLambda([[self lambdaField] doubleValue]);
    newNN.presetTheta();
    
    nx = unitCounts[0];
    ny = unitCounts[[[self layerField] intValue]-1];
    nt = [[self trainField] intValue]+[[self testField] intValue];
    
    if([[self xBox] indexOfSelectedItem]==X_FROM_FILE){
        char* temp = (char*)[[[self xDirField] stringValue] UTF8String];
        ifstream finX(temp);
        double *X = new double[nx*nt+1];
        for(int trainIndex=1;trainIndex<=nt;trainIndex++){
            for(int i=1;i<=nx;i++){
                finX>>X[(trainIndex-1)*nx+i];
            }
        }
        newNN.inputX(X);
    }
    else {
        char dir[500];
        strcpy(dir,(char*)[[[self xDirField] stringValue] UTF8String]);
        strcat(dir, "/");
        strcat(dir, (char*)[[[self xPrefixField] stringValue] UTF8String]);
        newNN.inputXFromImg(dir,[[self trainField] intValue]+[[self testField] intValue],40,40);
    }
    
    if([[self yBox] indexOfSelectedItem]==Y_FROM_BOOL){
        ifstream finY((char*)[[[self yDirField] stringValue] UTF8String]);
        double *Y = new double[ny*nt+1];
        for(int trainIndex=1;trainIndex<=nt;trainIndex++){
            for(int i=1;i<=ny;i++){
                finY>>Y[(trainIndex-1)*ny+i];
            }
        }
        newNN.inputY(Y);
    }
    else {
        ifstream finY((char*)[[[self yDirField] stringValue] UTF8String]);
        double *Y = new double[ny*nt+1];
        for(int trainIndex=1;trainIndex<=nt;trainIndex++){
            int temp;
            finY>>temp;
            for(int i=1;i<=ny;i++){
                Y[(trainIndex-1)*ny+i]=(temp==i);
            }
        }
        newNN.inputY(Y);
    }
    
    for(int i=1;i<=MAX_ITER;i++){
        jmem[i]=0;
        vmem[i]=0;
        pmem[i]=0;
    }
    
    totalIter = (int)[[self iterField] integerValue];
    
    for(int iter=1;;iter++){
        if(!(iter<=totalIter))break;
        if(isTraining){
            printf("[%3d] N1 - ",iter);
            jmem[iter]=newNN.performIteration();
            printf("J=%.4f   ",jmem[iter]);
            vmem[iter]=newNN.outputTrainSetPrecision();
            printf("V=%.3f   ",vmem[iter]);
            pmem[iter]=newNN.outputTestsSetPrecision();
            printf("T=%.3f \n",pmem[iter]);
            precision = newNN.outputPrecision();
            [_theView setJValues:jmem];
            [_theView setVValues:vmem];
            [_theView setPValues:pmem];
            [_theView setPrecision:precision];
            [_theView setReady:true];
            [_theView setCurrentIter:iter];
            currentIter = iter;
            [self performSelectorOnMainThread:@selector(drawRectInView) withObject:nil waitUntilDone:false];
        }
        else{
            [_theView setIter:currentIter];
            [self performSelectorOnMainThread:@selector(drawRectInView) withObject:nil waitUntilDone:false];
            break;
        }
    }
    itemsShouldBeSelectable = true;
    [self performSelectorOnMainThread:@selector(setSelectability) withObject:nil waitUntilDone:true];
}

- (void)drawRectInView{
    bool scroll = (NSMaxY(self.tableView.visibleRect) == NSMaxY(self.tableView.bounds));
    [_tableView reloadData];
    NSPoint destinationPoint = NSMakePoint(0.0, [[self.scrollView documentView] bounds].size.height);
    if(scroll)[[self.scrollView documentView] scrollPoint:destinationPoint];
    [_theView drawRect:{0,0,1,1}];
}

- (IBAction)stopTraining:(NSButton *)sender {
    isTraining = false;
    itemsShouldBeSelectable = true;
    [self setSelectability];
}

- (IBAction)setViewState:(NSSegmentedControl *)sender{
    [_theView setState:(int)[[self segmentControl] selectedSegment]];
    NSRect rect={0,0,400,400};
    [_theView drawRect:rect];
}

- (IBAction)browseFile:(NSButton *)sender {
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setAllowsMultipleSelection:false];
    NSArray *fileTypesArray = [NSArray arrayWithObjects:@"txt", nil];
    
    if([sender.identifier isEqual:@"browseX"] && [[self xBox] indexOfSelectedItem]==X_FROM_IMGS){
        [openDlg setCanChooseFiles:false];
        [openDlg setCanChooseDirectories:true];
    }
    else {
        [openDlg setCanChooseFiles:true];
        [openDlg setAllowedFileTypes:fileTypesArray];
        [openDlg setCanChooseDirectories:false];
    }
    
    if ( [openDlg runModal] == NSOKButton ) {
        NSArray *files = [openDlg URLs];    // Gets list of all files selected
        if([sender.identifier isEqual:@"browseX"])[self.xDirField setStringValue:[[files objectAtIndex:0] path]];
        if([sender.identifier isEqual:@"browseY"])[self.yDirField setStringValue:[[files objectAtIndex:0] path]];
    }
}

- (IBAction)updateParameter:(NSButton *)sender {
    if([sender.identifier isEqualToString:@"rate"])newNN.setLearningRate([[self aField] doubleValue]);
    else if([sender.identifier isEqualToString:@"lambda"])newNN.setLambda([[self lambdaField] doubleValue]);
    else if([sender.identifier isEqualToString:@"iter"]){
        [_theView setIter:[[self iterField]intValue]];
        totalIter=[[self iterField] intValue];
    }
}

- (IBAction)layerSet:(NSTextField *)sender {
    [self setNodeCountAvalability];
}

- (void)setNodeCountAvalability {
    int tmp = [self.layerField intValue];
    [self node5Field].enabled = (tmp>=5);
    [self node4Field].enabled = (tmp>=4);
    [self node3Field].enabled = (tmp>=3);
    [self node2Field].enabled = (tmp>=2);
    [self node1Field].enabled = (tmp>=1);
}

#pragma mark - Table View Data Source

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    return currentIter+1;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(nullable NSTableColumn *)tableColumn row:(NSInteger)row {
    if ([tableColumn.identifier  isEqualToString: @"#"]){
        if (row==0) return [NSString stringWithFormat:@"Latest"];
        return [NSString stringWithFormat:@"%ld",(long)row];
    }
    else if ([tableColumn.identifier isEqualToString: @"jValue"]) {
        if (row==0) return [NSString stringWithFormat:@"%.4f",jmem[currentIter]];
        return [NSString stringWithFormat:@"%.4f",jmem[row]];
    }
    else if ([tableColumn.identifier isEqualToString: @"vValue"]) {
        if (row==0) return [NSString stringWithFormat:@"%2.1f%%",vmem[currentIter]*100];
        return [NSString stringWithFormat:@"%2.1f%%",(vmem[row]*100)];
    }
    else if ([tableColumn.identifier isEqualToString: @"pValue"]) {
        if (row==0) return [NSString stringWithFormat:@"%2.1f%%",pmem[currentIter]*100];
        return [NSString stringWithFormat:@"%2.1f%%",pmem[row]*100];
    }
    else return nil;
}
@end

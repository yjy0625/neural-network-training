//
//  welcomeViewController.h
//  Neural Network Training
//
//  Created by Day Day Up on 2/6/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface welcomeViewController : NSViewController

@property (weak) IBOutlet NSButton *closeBtn;

- (IBAction)exit:(NSButton *)sender;

- (IBAction)segueToTrain:(NSButton *)sender;

@end

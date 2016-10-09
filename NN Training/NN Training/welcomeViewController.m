//
//  welcomeViewController.m
//  Neural Network Training
//
//  Created by Day Day Up on 2/6/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#import "welcomeViewController.h"

@implementation welcomeViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (IBAction)exit:(NSButton *)sender {
    [self.view.window close];
}

- (IBAction)segueToTrain:(NSButton *)sender {
    [self performSegueWithIdentifier:@"welcomeToTraining" sender:nil];
    [self.view.window close];
}

@end

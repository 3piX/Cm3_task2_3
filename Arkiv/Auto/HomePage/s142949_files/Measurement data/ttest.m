close all
clear all
clc

%%
mod = csvread('left_mod.dat');
or = csvread('left.dat');

[h, p, ci ,stats] = ttest2(or,mod)

%%
mod = csvread('right_mod.dat');
or = csvread('right.dat');

[h, p, ci ,stats] = ttest2(or,mod)
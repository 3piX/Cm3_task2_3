clear all
close all
clc

lm1 = csvread('straight_mod.dat');
lm2 = csvread('straight.dat');

plot(lm1,'LineStyle','--','Color','k')
hold on;
plot(lm2,'LineStyle','--','Color','r');
ylim([5 25])

legend('Before optimisation','After optimisation')

title('Time needed to travel 3.66m')
xlabel('Sample no [#]')
ylabel('Time [s]')

%% Straight side run with original code
clc
lm = csvread('straight.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 15.5:0.1:19;
y = pdf(pd,x);


figure(1);
subplot(2,1,1)

hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
title('Straight with original code')

xlabel('Time [s]')
ylabel('Occurences [#]')

line([pd.mu pd.mu],[0 4],'LineStyle','--','Color','k');
%xlim([8.5 15])
hold off

%% straight side run with modified code
clc
lm = csvread('straight_mod.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 15.5:0.1:19;
y = pdf(pd,x);


figure(1);
subplot(2,1,2)

hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
title('Straight with modified code')

xlabel('Time [s]')
ylabel('Occurences [#]')

%xlim([8.5 15])
hold off


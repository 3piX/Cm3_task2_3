clear all
close all
clc


%% Left side run with modified code
clc
lm = csvread('left_mod.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 23:0.8:57;
y = pdf(pd,x);


figure(1);
subplot(2,2,3)

hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
title('Left side with modified code')

xlabel('Time [s]')
ylabel('Occurences [#]')

line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
%xlim([8.5 15])
hold off

%% Left side run with original code
clc
lm = csvread('left.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 23:0.8:57;
y = pdf(pd,x);

subplot(2,2,1)
hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
title('Left side with original code')

xlabel('Time [s]')
ylabel('Occurences [#]')

%xlim([8.5 15])
hold off

%% Right side run with modified code
clc
lm = csvread('right_mod.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 23:0.8:57;
y = pdf(pd,x);

subplot(2,2,4)
hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
title('Right side with modified code')

xlabel('Time [s]')
ylabel('Occurences [#]')

%xlim([8.5 15])
hold off
%% Right side run with original code
clc
lm = csvread('right.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 23:0.8:57;
y = pdf(pd,x);

subplot(2,2,2)
hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
title('Right side with original code')

xlabel('Time [s]')
ylabel('Occurences [#]')

%xlim([8.5 15])
hold off

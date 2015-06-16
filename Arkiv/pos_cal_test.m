
t = 1:1:3000;


%% leg1
phase = 0;
amplitude = 1;
offset = 0;

p = 1;

pos1 = offset + amplitude*sin((2*pi*t)/(1000*p)+phase);


%% leg2
phase = pi;
amplitude = 1;
offset = 0;
p = 1;

pos2 = offset + amplitude*sin((2*pi*t)/(1000*p)+phase);

figure(1)
plot(t,pos1)
hold on
plot(t,pos2,'r')

hold off
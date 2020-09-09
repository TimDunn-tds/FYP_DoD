clc; clear; close all;

d = 0.01;
maxV = 4;


% Step up
figure(4);
stepUp = [zeros(1,2/d), 4*ones(1,5/d), zeros(1,1/d)];
plot(stepUp);

% Step down
figure(5);
stepDown = [zeros(1,2/d), -4*ones(1,5/d), zeros(1,1/d)];
plot(stepDown);

% Sin wave
figure(6);
sinWave = maxV*sin(2*pi*0.5*linspace(0,10,10/d));
plot(sinWave)


% Sin wave
figure(3);
sinWave_005 = maxV*sin(2*pi*0.05*linspace(0,20,20/d));
plot(sinWave_005)


% Sin wave
figure(7);
sinWave_1 = maxV*sin(2*pi*1*linspace(0,10,10/d));
plot(sinWave_1)

% close all;
%% Save files
save('stepDown_ctrl.mat','stepDown');
save('stepUp_ctrl.mat','stepUp');
save('sinWave_ctrl.mat','sinWave');
save('sinWave_005_ctrl.mat','sinWave_005');

save('sinWave_1_ctrl.mat','sinWave_1');


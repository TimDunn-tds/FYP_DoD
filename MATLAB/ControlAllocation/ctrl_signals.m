clc; clear; close all;

d = 0.01;
maxT = 0.5;


% Step up
figure(4);
stepUp = [zeros(1,2/d), 0.3*ones(1,5/d), zeros(1,1/d)];
plot(stepUp);

% Step down
figure(5);
stepDown = [zeros(1,2/d), -0.3*ones(1,5/d), zeros(1,1/d)];
plot(stepDown);

% Sin wave
figure(6);
sinWave = maxT*sin(2*pi*0.5*linspace(0,10,10/d));
plot(sinWave)


% Sin wave
figure(3);
maxT = 0.3;
sinWave_005 = maxT*sin(2*pi*0.05*linspace(0,20,20/d));
plot(sinWave_005)


% Sin wave
figure(7);
sinWave_1 = maxT*sin(2*pi*1*linspace(0,10,10/d));
plot(sinWave_1)

% close all;
%% Save files
save('stepDown_ctrl.mat','stepDown');
save('stepUp_ctrl.mat','stepUp');
save('sinWave_ctrl.mat','sinWave');
save('sinWave_005_ctrl.mat','sinWave_005');

save('sinWave_1_ctrl.mat','sinWave_1');


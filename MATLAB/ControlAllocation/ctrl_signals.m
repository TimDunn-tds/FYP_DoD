clc; clear; close all;

d = 0.01;
maxT = 0.3;


% Step up
figure(4);
stepUp = [zeros(1,2/d), 0.3*ones(1,5/d), zeros(1,1/d)];
plot(stepUp);

% Step down
figure(5);
stepDown = [zeros(1,2/d), -0.3*ones(1,5/d), zeros(1,1/d)];
plot(stepDown);

figure(6);
sinWave = maxT*sin(2*pi*0.1*linspace(0,10,10/d));
plot(sinWave)



% close all;
%% Save files
save('stepDown_ctrl.mat','stepDown');
save('stepUp_ctrl.mat','stepUp');
save('sinWave_ctrl.mat','sinWave');



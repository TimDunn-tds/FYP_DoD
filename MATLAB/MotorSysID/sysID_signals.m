clc; clear; close all;

d = 0.01;
maxV = 4;

% Ramp up then down
figure(1);
rampUp = [0*ones(1,1/d),linspace(0,maxV,10/d),maxV*ones(1,1/d),linspace(maxV,0,10/d),0*ones(1,1/d)];
plot(rampUp);

% Ramp down then up
figure(2);
rampDown = [0*ones(1,1/d),linspace(0,-maxV,10/d),-maxV*ones(1,1/d),linspace(-maxV,0,10/d),0*ones(1,1/d)];
plot(rampDown);

% Test for deadzone
figure(3);
deadUp = [];
deadDown = [];
deadV = 4;
for k = 0:0.5:deadV
    deadUp = [deadUp, k*ones(1,0.5/d)];
    deadDown = [deadDown, (deadV-k)*ones(1,0.5/d)];
end
deadZone = [deadUp,deadDown,-deadUp,-deadDown];
plot(deadZone);

% Step up
figure(4);
stepUp = [zeros(1,2/d), 3*ones(1,5/d), zeros(1,1/d)];
plot(stepUp);

% Step down
figure(5);
stepDown = [zeros(1,2/d), -3*ones(1,5/d), zeros(1,1/d)];
plot(stepDown);

% Sin
figure(6);
V = 4; % Volts
freq = 0.5; % Hz

% sinWave = [zeros(1,0.5/d), V*sin(2*pi*freq*linspace(0,10,10/d))];
sinWave = [V*sin(2*pi*freq*linspace(0,10,10/d))];

plot(sinWave);


%% Save files
save('deadZone.mat','deadZone');
save('rampDown.mat','rampDown');
save('rampUp.mat','rampUp');
save('stepDown.mat','stepDown');
save('stepUp.mat','stepUp');
save('sinWave.mat','sinWave');



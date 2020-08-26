clc; clear; close all;

d = 0.01;
maxV = 5;

% Ramp up then down
figure(1);
rampUp = [0*ones(1,1/d),linspace(0,maxV,20/d),maxV*ones(1,1/d),linspace(maxV,0,20/d),0*ones(1,1/d)];
plot(rampUp);

% Ramp down then up
figure(2);
rampDown = [0*ones(1,1/d),linspace(0,-maxV,20/d),-maxV*ones(1,1/d),linspace(-maxV,0,20/d),0*ones(1,1/d)];
plot(rampDown);

% Test for deadzone
figure(3);
deadUp = [];
deadDown = [];
deadV = 4;
for k = 0:0.25:deadV
    deadUp = [deadUp, k*ones(1,1/d)];
    deadDown = [deadDown, (deadV-k)*ones(1,1/d)];
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

% Sin
figure(7);
V = 4; % Volts
freq = 1; % Hz

% sinWave = [zeros(1,0.5/d), V*sin(2*pi*freq*linspace(0,10,10/d))];
sinWave1hz = [V*sin(2*pi*freq*linspace(0,10,10/d))];

plot(sinWave1hz);

% Sin
figure(9);
V = 5; % Volts
freq = 4; % Hz

% sinWave = [zeros(1,0.5/d), V*sin(2*pi*freq*linspace(0,10,10/d))];
sinWave4hz = [V*sin(2*pi*freq*linspace(0,10,10/d))];

plot(sinWave4hz);


% Constant
figure(8);
V = 4;
constant4V = V*ones(1000,1);
plot(constant4V);

figure(10);
V = 5; % Volts
freq = 3; % Hz

% sinWave = [zeros(1,0.5/d), V*sin(2*pi*freq*linspace(0,10,10/d))];
sinWave3hz = [V*sin(2*pi*freq*linspace(0,10,10/d))];

plot(sinWave3hz);


% Sin Load
figure(11);
V = 4; % Volts
freq = 0.5; % Hz

% sinWave = [zeros(1,0.5/d), V*sin(2*pi*freq*linspace(0,10,10/d))];
sinWave_load_05hz = [V*sin(2*pi*freq*linspace(0,30,30/d))];

plot(sinWave_load_05hz);

% Chirp
figure(12);
f0 = 0;
f1 = 5;
t = 0:0.01:10;
chirp_sig = zeros(length(t)*2,1);
chirp_sig(1:1001) = 6.*chirp(t,f0,10,f1,'linear');
chirp_sig(1002:end) = 6.*chirp(t,f1,10,f0,'linear');

plot(chirp_sig);


% Positive sin wave
figure(13);
V = 3; % Volts
freq = 1; % Hz

sinWave1hzPos = 3 + [V*cos(pi + 2*pi*freq*linspace(0,10,10/d))];

plot(sinWave1hzPos);

% Negative sin wave
figure(14);
V = -3; % Volts
freq = 1; % Hz

sinWave1hzNeg = -3 - [V*cos(2*pi*freq*linspace(0,10,10/d))];

plot(sinWave1hzNeg);


% Constant
figure(15);
V = 6;
constant6V = V*ones(1000,1);
plot(constant6V);


close all;
%% Save files
save('deadZone.mat','deadZone');
save('rampDown.mat','rampDown');
save('rampUp.mat','rampUp');
save('stepDown.mat','stepDown');
save('stepUp.mat','stepUp');
save('sinWave.mat','sinWave');
save('sinWave1hz.mat','sinWave1hz');
save('constant4V.mat','constant4V');
save('sinWave4hz.mat','sinWave4hz');
save('sinWave3hz.mat','sinWave3hz');
save('chirp_sig.mat','chirp_sig');
save('sinWave_load_05hz.mat','sinWave_load_05hz');
save('sinWave1hzPos.mat','sinWave1hzPos');
save('sinWave1hzNeg.mat','sinWave1hzNeg');
save('constant6V.mat','constant6V');



% Clean up
clc; clear; close all;

% Pick sysID input
% type = "sinWave";
% type = "deadZone";
% type = "rampDown";
type = "rampUp";
% type = "stepDown";
% type = "stepUp";

filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);

figure(1)
subplot(2,1,1)
plot(time,V);

subplot(2,1,2)
plot(time,vel);







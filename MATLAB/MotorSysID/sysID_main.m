% Clean up
clc; clear; close all;

% Pick sysID input
% type = "sinWave";
type = "deadZone";
% type = "rampDown";
% type = "rampUp";
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

% data_linear = data(700:1700,:);
% plot(data_linear(:,1),data_linear(:,4))



% 
% saveFileName = sprintf("%s_linear_cut.mat", type);
% save(saveFileName, 'data_linear');





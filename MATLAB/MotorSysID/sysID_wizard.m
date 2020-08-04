% Import data
type = "sinWave";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time_sw    = data(:,1);
enc_sw     = data(:,2);
pos_sw     = data(:,3);
vel_sw     = data(:,4);
V_sw       = data(:,5);
current_sw = data(:,6);
%%
type = "deadZone";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time_dz    = data(:,1);
enc_dz     = data(:,2);
pos_dz     = data(:,3);
vel_dz     = data(:,4);
V_dz       = data(:,5);
current_dz = data(:,6);
%%
type = "rampDown";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time_rd    = data(:,1);
enc_rd     = data(:,2);
pos_rd     = data(:,3);
vel_rd     = data(:,4);
V_rd       = data(:,5);
current_rd = data(:,6);
%%
type = "rampUp";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time_ru    = data(:,1);
enc_ru     = data(:,2);
pos_ru     = data(:,3);
vel_ru     = data(:,4);
V_ru       = data(:,5);
current_ru = data(:,6);
%%
type = "stepDown";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time_sd    = data(:,1);
enc_sd     = data(:,2);
pos_sd     = data(:,3);
vel_sd     = data(:,4);
V_sd       = data(:,5);
current_sd = data(:,6);

%%
type = "stepUp";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time_su    = data(:,1);
enc_su     = data(:,2);
pos_su     = data(:,3);
vel_su     = data(:,4);
V_su       = data(:,5);
current_su = data(:,6);









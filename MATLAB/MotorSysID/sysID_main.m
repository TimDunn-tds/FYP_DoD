% Clean up
% clc; clear;

% Pick sysID input
% type = "chirp_sig";
% type = "sinWave_load_05hz";
type = "sinWave";
% type = "sinWave1hz";
% type = "sinWave2hz";
% type = "sinWave3hz";
% type = "sinWave4hz";
% type = "deadZone";
% type = "rampDown";
% type = "rampUpDown";
% type = "rampUp";
% type = "stepDown";
% type = "stepUp";
% type = "constant4V";
% type = "sinWave1hzPos";
% type = "sinWave1hzNeg";
% type = "constant6V";
% type = "oneV_torque";

filename = sprintf("%s_data.mat",type);
data = importdata(filename);
% data = data(1:250,:);
time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);
% 
k = find(V<0);
current(k) = -1.*current(k);

% data_linear = data(700:1700,:);
% plot(data_linear(:,1),data_linear(:,4))
% figure(1);
% plot(time,V)
% 
% figure(2);
% plot(time,vel);

figure(10); clf; hold on;
subplot(2,1,1);
% subplot(2,1,1);
% subplot(2,1,2);
% yyaxis right
% yyaxis left
plot(time,V,'DisplayName','Voltage [V]','linewidth',2); hold on;
plot(time,vel,'DisplayName','Velocity [rad/s]','linewidth',2);
grid on;
legend('location','best');
% ylim([0,1.4]);
subplot(2,1,2);
% yyaxis right
plot(time,current,'DisplayName','Current [A]','linewidth',1); hold on;
grid on;
legend('location','best');
% 
% saveFileName = sprintf("%s_linear_cut.mat", type);
% save(saveFileName, 'data_linear');

% figure(11); clf; hold on;
% plot(time,rad2deg(pos))





%% Clear parameters
clc; clear all;

%% Parameters
mo = 1.5;                   % kg
mh = 1.0;                   % kg

ro = 0.1;                   % m
rh = 0.2;                   % m

g = 9.8;                    % m/s^2

theta0 = 0*(pi/180);        % rad
phi0 = 2*(pi/180);          % rad

%% Build Matrices

MM = [
    rh^2*(mo + mh), -mo*rh*(ro + rh);
    -mo*rh*(ro + rh), 2*mo*(rh + ro)^2];




%% Run simulation
sim("DoD_simulink_model");


figure; hold on;
plot(tout,dtheta.signals.values.*180/pi);
plot(tout,dphi.signals.values.*180/pi);

% figure; hold on;
% plot(tout,tau.signals.values);
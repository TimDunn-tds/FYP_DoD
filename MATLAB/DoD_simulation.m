%% Clear parameters
clc; clear all;

T = 0.01;

%% Parameters
mo = 1.5;                   % kg
mh = 1.0;                   % kg

ro = 0.1;                   % m
rh = 0.2;                   % m

g = 9.82;                   % m/s^2

theta0 = 0*(pi/180);        % rad
phi0 = 2*(pi/180);          % rad

%% Build Matrices

% MM = [
%     rh^2*(mo + mh), -mo*rh*(ro + rh);
%     -mo*rh*(ro + rh), 2*mo*(rh + ro)^2];


%% Motor parameters
tau_max = 100;              % Nm
tau_min = -100;             % Nm


%% Linearised Plant Model (linearised about upright balancing position)
% State variables:
% x(1) = hand angular velocity [rad/s]
% x(2) = object angular velocity about hand CoM [rad/s]
% x(3) = hand angle [rad]
% x(4) = object CoM angle relative to hand CoM [rad]

MM = [
    (mo + mh)*rh^2, -mo*rh*(ro + rh);
    -mo*rh*(ro + rh), 2*mo*(rh + ro)^2];

KK = [0, 0; 0, -mo*g*(rh + ro)];

DD = zeros(2);

e = [1;0];

% Create matrices 
A = [MM\-DD, MM\-KK; eye(2,2), zeros(2,2)];

B = [MM\e; zeros(2,1)];

Cr = [1, 0, 0, 0]; % Regulate hand velocity
Dr = 0; % No feedthrough?

Cm = [0, 0, 1, 1]; % Measure hand and object positions
Dm = 0;


% May need to reduce the amount of states as the position is potentiall
% unbounded. At this stage it is left in to allow position control of the
% hand

[Acd, Bcd] = c2d(A,B,T);


%% Create MPC Controller

% System dimensions
n_in = size(Bcd,2);     % No. actuators
n_out = size(Cr,1);     % No. outputs to regulate 
n_states = size(Acd,1); % No. states


N_MPC = 20; % Horizon

Ru = 1;         % Penalty on input moves (i.e. ||u[k]-u[k-1]||)
Qy = 1;      	% Penalty on output error




% %% Run simulation
% sim("DoD_simulink_model");
% 
% 
% figure; hold on;
% plot(tout,dtheta.signals.values.*180/pi);
% plot(tout,dphi.signals.values.*180/pi);
% 
% % figure; hold on;
% % plot(tout,tau.signals.values);
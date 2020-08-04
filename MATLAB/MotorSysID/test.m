clc; clear;

% Pick sysID input
type = "sinWave";
% type = "deadZone";
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
voltage = data(:,5);
current = data(:,6);

% Number of timesteps to test
Nt = 100;
vel = vel(1:10:Nt);

% Define parameters
N = 98.78;
Ra  = 2.14; % Ohm
La  = 0.1; % Henry
Jh  = 3.908e-3 + 1.103e-6 + 2.38e-6; % kg*m^2
% Jh = 0.000121 * N^2;
% J   = Jh/(N^2); % kg*m^2 % Not needed. Done inside motor model
B   = 1e-3; % N*m*sec
kv  = 0.04;
kt  = 0.0566;
alpha1 = 0.1;
alpha2 = 0.1;
alpha3 = 0.1;

% Pack params
params(1) = Ra;
params(2) = La;
params(3) = kv;
params(4) = kt;
params(5) = B;
params(6) = Jh;
params(7) = N;

params(8)   = alpha1;
params(9)   = alpha2;
params(10)  = alpha3;


% Solve the ODE
% x = [w,I]
dt = 0.1;
tspan   = time(1):dt:time(Nt);
z0      = [0;0];

[x1,x2] = ode45(@(t,x) dcMotorModel(t,x,time,voltage,params),tspan,z0);



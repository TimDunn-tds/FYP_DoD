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

[x1,x2] = ode45(@(t,x) dcMotorModel(t,x,time,voltage,params),tspan,z0)
%%
% Fmincon all the things
A = [];
b = [];
Aeq = [];
beq = [];
lb = [0, 0, 0, 0, 0, 0, N, 0, 0, 0];
ub = [100, 10, 10, 10, inf, inf, N, inf, inf, inf];
nonlcon = [];
options = optimoptions('fmincon','Display','Iter');

x0 = params;
fun =@(x) minimizeMe(time, voltage, x, tspan, z0, vel);

[sol,fval] = fmincon(fun, x0, A, b, Aeq, beq, lb, ub, nonlcon, options);



%% Plot things
[t,z] = ode45(@(t,x) dcMotorModel(t,x,time,voltage,sol), tspan, z0);

%{
% figure(1); clf;
% subplot(2,1,1)
% plot(time,voltage); hold on
% title('Time vs Voltage')
% ylabel('Armature voltage [V]');
% xlabel('Time [s]');
% 
% subplot(2,1,2)
% plot(t,z(:,1),'DisplayName','Simulated');
% hold on;
% plot(t,vel,'DisplayName','Measured');
% title('Time vs Velocity')
% ylabel('Velocity [rad/s]');
% legend('location','best');
% xlabel('Time [s]');
%}

figure(1); clf;
plot(time,voltage,'DisplayName','Voltage'); hold on
title('Time vs Voltage and Time vs Velocity')
ylabel('Armature voltage [V]');
xlabel('Time [s]');

yyaxis right
plot(t,z(:,1),'DisplayName','Simulated');
plot(t,vel,'r-','DisplayName','Measured');
ylabel('Velocity [rad/s]');
legend('location','best');



%% Functions go here
function cost = minimizeMe(time, voltage, params, tspan, z0, vel)

    [~,z] = ode45(@(t,x) dcMotorModel(t,x,time,voltage,params), tspan, z0);

%     err = vel-z(:,1);
%     cost = sum(err.^2);
    cost = (vel-z(:,1)).'*(vel-z(:,1));
    
%     V_y = @(y) 0.5*(y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1)) + 0.5*(y(:,2) - y_true(:,2)).'*(y(:,2) - y_true(:,2));

end


function dzdt = dcMotorModel(t,z,Vat,Va,params)
    % Unpack params
    Ra  = params(1);
    La  = params(2);
    kv  = params(3);
    kt  = params(4);
    B   = params(5);
    Jh  = params(6);
    N   = params(7);
    
    alpha1 = params(8);
    alpha2 = params(9);
    alpha3 = params(10);

    J = Jh/(N^2);

    % Unpack states
    omega_a = z(1);
    Ia      = z(2);

    % Get the correct voltage value
    Va = interp1(Vat,Va,t);

    % Calculate some things
    tauM    = kt*Ia;
    Vb      = kv*omega_a;
    tauF    = alpha1*sign(omega_a) ...
                + alpha2*exp(-alpha3*abs(omega_a))*sign(omega_a);

    % Solve ODE
    % omega_a, Ia
    dzdt(1) = (tauM - B*omega_a - tauF)/J;
    dzdt(2) = (Va - Ra*Ia - Vb)/La;
%     if Va<1.1
%         dzdt(1) = 0;
%     end
    dzdt = dzdt';
end

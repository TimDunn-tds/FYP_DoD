% Clean up
clc;
clear;

%% Get data
% Pick sysID input
% type = "sinWave";
type = "sinWave1hz";
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
V       = data(:,5);
current = data(:,6);


% Simulation time
dt = 0.01;
t_sim = time;
y_true = [V, vel, current];
x0 = 0;
% accel = zeros(length(t_sim),1);
% accel(2:end) = (vel(2:end) - vel(1:end-1))./dt;
input = [V, vel, current];

%% Run system identification
% R1, R2, R3, R4, R5, La, Kt, B, Kw
% param_vec = [0.7788; 1.0077e-5; 0.3383; 1.0; 1.0; 1.6746e-5; 1.0; 1.0; 6.457e-4];
param_vec = [1; 1; 1; 1; 1; 0.1; 1; 1; 1];

% Compute cost
V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));

V_theta = @(theta) V_y(runSim(theta, t_sim, x0, input));


%% Run optimisation with fmincon
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
% ub = [inf inf inf inf inf 1 inf inf inf];
options = optimoptions('fmincon','Display','iter');
param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], [], [], options);
final_cost = V_theta(param_opt)

%% Plot results 
y_cts = runSim(param_opt, t_sim, x0, input);
figure(1); clf;
ax1 = subplot(3,1,1);
plot(t_sim,y_true(:,1),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,1),'DisplayName','Simulated');
xlabel('Time [s]');
ylabel('Voltage [V]');
grid on;
legend('location','best');

ax2 = subplot(3,1,2);
plot(t_sim,y_true(:,2),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,2),'DisplayName','Simulated');
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
grid on;
legend('location','best');

ax3 = subplot(3,1,3);
iplot = y_cts(:,3);
iplot(iplot<0) = iplot(iplot<0) * -1;
plot(t_sim,y_true(:,3),'DisplayName','Measured Current'); hold on;
plot(t_sim,iplot,'DisplayName','Simulated Current');
ylabel('Current [A]');
xlabel('Time [s]');
grid on;
title("Lol current sense doesnt even work when it's braking");
legend;

linkaxes([ax1,ax2,ax3],'x');

%% Additional Functions
function y = runSim(param_vec, t_sim, x0, input)
    % Unpack params
    % R1, R2, R3, R4, R5, La, Kt, B, Kw, 
    R1      = param_vec(1);
    R2      = param_vec(2);
    R3      = param_vec(3);
    R4      = param_vec(4);
    R5      = param_vec(5);
    La      = param_vec(6);
    Kt      = param_vec(7);
    B       = param_vec(8);
    Kw      = param_vec(9);

    % Fixed parameters
    N = 98.78;
    Jh = 0.0039;
    J = Jh/(N^2); % Inertia at the motor (low torque/High speed) side of gearbox
    
    % Unpack input
    w_m_in = N.*input(:,2);
    Ia = input(:,1);
    
    % CCR's
%     Ra      =@(Ia) R1;
    Ra      =@(Ia) R1 + R2.*exp(-R3.*abs(Ia));
%     Ra      =@(Ia) R1 + R2.*exp(-R3.*abs(Ia)) + R4.*exp(-R5.*abs(Ia).^2);
    eb      =@(w_m) Kw*w_m;

    % SSR's
    dIa =@(w_m,Ia,Va) (Va - Ra(Ia)*Ia - eb(w_m))/La;
    Va =@(t) 4*sin(2*pi*0.5*t);
    w_m =@(t) interp1(t_sim,w_m_in,t);
    
    dx_wrap = @(t,x) dIa(w_m(t), x(1), Va(t));
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);


    V_sim =@(w_m,Ia) (Ra(Ia).*Ia + Kw.*w_m);

    
    y = [V_sim(w_m_in,res.x), w_m_in./N, res.x];
    
end





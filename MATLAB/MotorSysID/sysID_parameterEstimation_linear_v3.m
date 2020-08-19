% Clean up
clc;
clear;

%% Get data
% Pick sysID input
type = "chirp_sig";
% type = "sinWave_load_05hz";
% type = "sinWave";
% type = "sinWave1hz";
% type = "sinWave2hz";
% type = "sinWave3hz";
% type = "sinWave4hz";
% type = "deadZone";
% type = "rampDown";
% type = "rampUp";
% type = "stepDown";
% type = "stepUp";
% type = "constant4V";
% type = "sinWave1hzPos";
% type = "sinWave1hzNeg";
% type = "constant6V";

filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);
% Correct current
k       = find(V<0);
current(k) = current(k)*-1;

% Simulation time
dt = 0.01;
t_sim = time;
y_true = [V, vel, current];
x0 = [0];
input = [V, vel, current];

%% Run system identification
% R1, Kw1, La, Kt, B,
param_vec = [1; 0.01; 0.00011; 0.001];

% Compute cost
V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));

V_theta = @(theta) V_y(runSim(theta, t_sim, x0, input));

%% Run optimisation with fmincon
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
% ub = [inf inf inf inf inf 1 inf inf inf];
options = optimoptions('fmincon','Display','iter', 'StepTolerance', 1e-12);
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
title('Voltage');
legend('location','best');

ax2 = subplot(3,1,2);
plot(t_sim,y_true(:,2),'DisplayName','Measured'); hold on;
% plot(t_sim,y_cts(:,2),'DisplayName','Simulated');
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
grid on;
legend('location','best');

ax3 = subplot(3,1,3);
% iplot = y_cts(:,3);
% iplot(iplot<0) = iplot(iplot<0) * -1;
plot(t_sim,y_true(:,3),'DisplayName','Measured Current'); hold on;
% plot(t_sim,iplot,'DisplayName','Simulated Current');
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
    Kw      = param_vec(2);
%     K1      = param_vec(3);
%     K2      = param_vec(4);
    

    % Fixed parameters
    N = 98.78;
    Jh = 0.0039;
    J = Jh/(N^2); % Inertia at the motor (low torque/High speed) side of gearbox
    
    % Unpack input
%     V = input(:,1);
    w_in = N*input(:,2); % Correct for gearbox
    Ia_in = input(:,3);
%     Va  =@(t) interp1(t_sim,V,t);
%     w_m =@(t) interp1(t_sim,w_in,t);
%     Ia =@(t) interp1(t_sim,Ia_in,t);
    
    % CCR's
    Ra      =@(Ia) R1;
    eb      =@(w_m) Kw.*w_m;
    
    % Define output
    V_sim   =@(w_m,Ia) Ra(Ia).*Ia + eb(w_m);
%     Ia_sim  =@(Va, w_m) (Va - Kw.*w_m)/Ra(Ia)
%     output  =@(x) [V_sim(w_m,x(1)); x(1)];
    
    y = V_sim(w_in,Ia_in);
%     V_sim =@(w_m, Ia, t_sim) Ra(Ia(t_sim)).*Ia(t_sim) + Kw.*w_m(t_sim);
    
end





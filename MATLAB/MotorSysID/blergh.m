% Clean up
clc;
clear;

%% Get data
% Pick sysID input
% type = "sinWave_load_05hz";
type = "sinWave";
% type = "sinWave1hz";
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
x0 = [0, 0];
input = [V];

%% Run system identification
% R1, R2, R3, R4, R5, La, Kt, B, Kw
param_vec = [1; 1; 1; 1; 1; 0.1; 1; 1; 1; 0.1; 1];
% param_vec = [    0.0004    0.1979    0.0640    0.0035    0.0035    0.0003    0.0800    0.0341    0.0104];

% Compute cost
V_y = @(y) (y(:,1) - y_true(:,2)).'*(y(:,1) - y_true(:,2));
% V_y = @(y) 0.5.*(y(:,1) - y_true(:,2)).'*(y(:,1) - y_true(:,2))...
%         + 0.5.*(y(:,2) - y_true(:,3)).'*(y(:,2) - y_true(:,3));
    
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
plot(t_sim,y_cts(:,3),'DisplayName','Simulated');
xlabel('Time [s]');
ylabel('Voltage [V]');
grid on;
legend('location','best');

ax2 = subplot(3,1,2);
plot(t_sim,y_true(:,2),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,1),'DisplayName','Simulated');
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
grid on;
legend('location','best');

ax3 = subplot(3,1,3);
iplot = y_cts(:,2);
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
    a1      = param_vec(10);
    b1      = param_vec(11);

    % Fixed parameters
    N = 98.78;
    Jh = 0.0039;
    J = Jh/(N^2); % Inertia at the motor (low torque/High speed) side of gearbox
    
    % Input
    V = input(:,1);
    Va =@(t) interp1(t_sim,V,t);

    % CCR's
%     Ra      =@(Ia) R1;
    Ra      =@(Ia)  R1 + R2*exp(-R3*abs(Ia));
    eb      =@(w_m) Kw*w_m;
    tauM    =@(Ia)  Kt*Ia;
    tauF    =@(w_m) (b1*w_m)/(sqrt(a1 + w_m^2));


        %SSR's
    dw_m =@(w_m,Ia,Va) (tauM(Ia) - B*w_m - tauF(w_m))/J;
%     dw_m    =@(w_m,Ia,Va) (tauM(Ia))/J;
    dIa     =@(w_m,Ia,Va) (Va - Ra(Ia)*Ia - eb(w_m))/La;
    
    % ODE Function
    dx =@(w_m,Ia,Va) [dw_m(w_m,Ia,Va); dIa(w_m,Ia,Va)];
    
    % Create wrapper function
    dx_wrap = @(t,x) dx(x(1),x(2),Va(t));
    
    % Run simulation
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
    
    V_linear =@(w_m,Ia) Ra(Ia)*Ia + Kw*w_m;
    % Define output
    output  =@(x) [x(1)./N; x(2); V_linear(x(1),x(2))];
    
    
    % Generate output
    y = zeros(length(res.t),3);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end
    
end





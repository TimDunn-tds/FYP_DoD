% Clean up
clc;
clear;

%% Get data
% Pick sysID input
type = "sinWave_ctrl";
% type = 'sinWave_005_ctrl';
% type = 'stepUp_ctrl';

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
% Define intial conditions
w_a0 = 0;
Ia0 = 0;
Va0 = 0;
x0 = [w_a0];

% Simulation time
dt = 0.01;
t_sim = time;
y_true = [V, vel, current];
% Get the signal desired
filename = sprintf("%s.mat",type);
input = importdata(filename);
input(2,:) = current;

%% Run system identification
% Define initial parameter guess
% B, J
param_vec = [0.1; 0.0001];

% Compute cost
% V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));
V_y = @(y) (y(:,1) - y_true(:,2)).'*(y(:,1) - y_true(:,2));

V_theta = @(theta) V_y(runSim(theta,x0,t_sim,input));


%% Pattern search
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
lb = [1e-8, 1e-8];
options = optimoptions('patternsearch','Display','iter','StepTolerance',1e-10, 'MaxIterations', 2000);
param_opt = patternsearch(V_theta, param_vec, A, b, [], [], lb, [], [], options);
final_cost = V_theta(param_opt)


%% Plot results 
y_cts = runSim(param_opt,x0,t_sim,input);
figure(1); clf;
ax1 = subplot(3,1,1);
plot(t_sim,y_true(:,1),'DisplayName','Voltage', 'LineWidth', 2); hold on;
ylabel('Voltage [V]');
yyaxis right
ylabel('Torque [Nm]');
plot(time, input(1,:), 'DisplayName', 'Torque', 'LineWidth', 2); 
% plot(t_sim,y_cts(:,1),'DisplayName','Simulated');
xlabel('Time [s]');
grid on;
legend('location','best');

ax2 = subplot(3,1,2);
plot(t_sim,y_true(:,2),'DisplayName','Measured', 'LineWidth', 2); hold on;
plot(t_sim,y_cts(:,1),'DisplayName','Simulated', 'LineWidth', 2);
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
grid on;
legend('location','best');

ax3 = subplot(3,1,3);
plot(t_sim,y_true(:,3),'DisplayName','Measured Current', 'LineWidth', 2); hold on;
% plot(t_sim,y_cts(:,2),'DisplayName','Simulated Current');
ylabel('Current [A]');
xlabel('Time [s]');
grid on;
title("Lol current sense doesnt even work when it's braking");
legend;

linkaxes([ax1,ax2,ax3],'x');

%% Prompt for save of file
answer = questdlg("Save parameters?","Save prompt","Yes","No","No");
if answer == "Yes"
    saveFileName = sprintf("%s_params.mat", type);
    save(saveFileName, 'param_opt');
end

%% Additional functions
function y = runSim(param_vec, x0, t_sim, input)

    % Unpack params
    B = param_vec(1);
    J = param_vec(2);
    
    % Fixed parameters
    Ra = 2.5019;
    Kw = 0.0089;
    La = 0.0046;
    N = 98.78;
    
    % Unpack input
    Tm_in = input(1,:);
    Ia_in = input(2,:);
    
    % Interpolation functions
    Tm =@(t) interp1(t_sim,Tm_in,t);
%     Ia =@(t) interp1(t_sim,Ia_in,t);
    
    % CCRs
%     tauM =@(Ia) 0.06761.*Ia.^4 + 0.2721.*Ia.^3 - 0.02975.*Ia.^2 + 1.417.*Ia;

    
    % SSRs
%     dw_m =@(w_m,Ia,Va) (tauM(Ia) - B*w_m - tauF(w_m))/J; % w_m (not speed of hand)
    dw_m =@(w_m,Tm) (Tm - B*w_m)/J; 
    
    % Create ode function
    dx =@(w_m,Tm) [dw_m(w_m,Tm)];
    
    % Create wrapper function
    dx_wrap = @(t,x) dx(x(1), Tm(t));

    % Run simulation
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
    
    % Define output w_m
    output =@(x) [x(1)];
    % Generate output
    y = zeros(length(res.t),1);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end
    
end

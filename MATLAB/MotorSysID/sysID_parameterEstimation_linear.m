% Clean up
clc;
clear;

%% Get data
% Pick sysID input
type = "sinWave";
% type = "deadZone";
% type = "rampDown";
% type = "rampUp";
% type = "stepDown";
% type = "stepUp";

filename = sprintf("%s_data.mat",type);

% filename = sprintf("%s_linear_cut.mat",type);
data = importdata(filename);

time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);

% Shift so start is 0,0
% V = V - mean(V(1:10));
% vel = vel - mean(vel(1:10));

% Define intial conditions
% q = omega;
% p = Ia
q0 = vel(1);
p0 = 0;
x0 = [q0; p0; V(1)];

% Simulation time
dt = 0.01;
% t_sim = 0:dt:time(500);
% y_true = vel(1:1/dt:500);
t_sim = time;
y_true = vel;

%% Run system identification
% Define initial parameter guess
% Ra, La, kt, B, kv
param_vec = [5; 1e-8; 0; 0; 0];
y_initial = runSim(param_vec,x0,t_sim,V);

% Compute cost
V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));
V_theta = @(theta) V_y(runSim(theta,x0,t_sim,V));

%% Run optimisation with fmincon
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
% ub = [10, inf, inf, inf, 1e-6, inf, 1e-5, 1e-5]';
options = optimoptions('fmincon','Display','iter',...
    'StepTolerance',1e-10);
param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], [], [], options);
final_cost = V_theta(param_opt)

%% Run optimisation with patternsearch
% A = -eye(length(param_vec));
% b = zeros(length(param_vec),1);
% lb = [0; 1e-4; 0; 0; 0; 0; 0; 0];
% ub = [inf, 1e2, inf, inf, inf, inf, inf, inf]';
% options = optimoptions('patternsearch','Display','iter','StepTolerance',1e-9,'UseCompletePoll',false);
% param_opt = patternsearch(V_theta, param_vec, A, b, [], [], lb, [], [], options);
% final_cost = V_theta(param_opt)

%% Run optimisation with ga
% A = -eye(length(param_vec));
% b = zeros(length(param_vec),1);
% ub = [10, 1e5, 100, 10, 1e-8, 100, 1e-8, 1e-8]';
% options = optimoptions('ga','Display','iter');
% param_opt = ga(V_theta, 8, param_vec, A, b, [], [], [], ub, [], options);
% final_cost = V_theta(param_opt)


%% Plot results 
y_cts = runSim(param_opt,x0,t_sim,V);
figure(1); clf;
% subplot(2,1,1)
% plot(time,V);
% xlabel('Time [s]');
% ylabel('Armature Voltage [V]');
% subplot(2,1,2);
plot(t_sim,y_true,'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,1),'DisplayName','Simulated');
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
legend;


%% Prompt for save of file
% answer = questdlg("Save parameters?","Save prompt","Yes","No","No");
% if answer == "Yes"
%     saveFileName = sprintf("%s_params.mat", type);
%     save(saveFileName, 'param_opt');
% end


%% Additional functions
function y = runSim(param_vec, x0, t_sim, V)
% q = omega;
% p = Ia
    
% Unpack params
    Ra      = param_vec(1);
    La      = param_vec(2);
    kt      = param_vec(3);
    B       = param_vec(4);
    kv      = param_vec(5);

    N = 98.78;
    Jh = 0.0039;
    J = Jh/N^2;
    
    % CCRs
    tauM    =@(p) kt*p;
    Vb      =@(q) kv*q;

    
    % SSRs
    dq =@(q,p)      (tauM(p) - B*q)/J;
    dp =@(q,p,Va)   (Va - Ra*p - Vb(q))/La;
    dx =@(q,p,Va)   [dq(q,p); dp(q,p,Va); Va];
    
    % Define output
    output = @(x) [x(1);x(2);x(3)];

    % Inputs
    Va = @(t) interp1(t_sim,V,t);
    
    % Create wrapper function
    dx_wrap = @(t,x) dx(x(1),x(2),Va(t));

    % Run simulation
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
%     [res.t,res.x] = ode45(dx_wrap, t_sim, x0);

    % Generate output
    y = zeros(length(res.t),3);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end

end













clear
clc

%% Generate data
% Define initial conditions
q0 = 0;
p0 = 0;
x0 = [q0; p0];

% true system parameters
params.c = 2;
params.b = 1.8;
params.m = 2.9;
% generate parameter vector
param_vec_true = [params.c; params.b; params.m];

% Define simulation time
t_sim = [0:0.1:10];
y_true = runSim(param_vec_true,x0,t_sim) + 0.05*randn(length(t_sim),1);

% Plot output
plot(t_sim,y_true,'+')
legend('q','dq')

%% Run system identification
% Define initial parameter guess
param_vec = [1; 1; 1];

% Compute cost
V_y = @(y) 0.5*(y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));
V_theta = @(theta) V_y(runSim(theta,x0,t_sim));

% run optimisation
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
options = optimset('Display','iter');
param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], [], [], options);
final_cost = V_theta(param_opt)

%% Plot results 
% Run simulation with parameters
y_cts = runSim(param_opt,x0,t_sim);
figure(1)
plot(t_sim,y_cts,t_sim,y_true,'+')
figure(2)
plot(t_sim,y_cts-y_true,'+')

%% Additional functions
function y = runSim(param_vec,x0,t_sim)
    % unpack parameters
    params.c = param_vec(1);
    params.b = param_vec(2);
    params.m = param_vec(3);
    
    % CCRs
    Fs = @(q) q/params.c;
    Ff = @(q,p) params.b*p/params.m;

    % SSRs
    dq = @(q,p) p/params.m;
    dp = @(q,p,Fi) - Fs(q) - Ff(q,p) + Fi;
    dx = @(q,p,Fi) [dq(q,p); dp(q,p,Fi)];

    % Define output
    output = @(x) [x(1)];

    % Inputs
    Force = @(t) sin(t);
    
    % Create wrapper function
    dx_wrap = @(t,x) dx(x(1),x(2),Force(t));

    % Run simulation
    [res.t,res.x] = ode45(dx_wrap, t_sim, x0, odeset('RelTol',1e-6));
    
    % generate output
    y = zeros(length(res.t),1);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end
end
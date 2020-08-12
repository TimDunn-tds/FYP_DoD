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
data = importdata(filename);

time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);

% Define intial conditions
% q = omega;
% p = Ia
q0 = vel(1);
p0 = 0;
% x0 = [q0; p0; V(1)];
x0 = [q0; p0; V(1); 0];

% Simulation time
dt = 0.01;
% t_sim = 0:dt:time(500);
% y_true = vel(1:1/dt:500);
t_sim = time;
% y_true = vel;
y_true = [vel,pos];

%% Run system identification
% Define initial parameter guess
% Ra, La, kt, B, alpha1, kv, alpha2, alpha3
% param_vec = [0.2337; 8.0593e-4; 0.1874; 0.1215; 0; 0.3964; 0; 0]; %rampUp
param_vec = [2.14; 0.8177; 0.3522; 0.0576; 4e-9; 0.1348; 1e-10; 0.8750];
% 4.3421 0.0032 0.3522 0.0576 0.1308
% param_vec = [0; 1e0; 1e-10; 1e-10; 1e-10; 1e-10; 1e-10; 1e-10];
y_initial = runSim(param_vec,x0,t_sim,V);

% Compute cost
% V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));
V_y = @(y) (y(:,4) - y_true(:,2)).'*(y(:,4) - y_true(:,2));

% V_y = @(y) ((max(y)-y(:,1)) - (max(y_true) - y_true(:,1))).'*((max(y)-y(:,1)) - (max(y_true) - y_true(:,1)));

V_theta = @(theta) V_y(runSim(theta,x0,t_sim,V));

%% Run optimisation with fmincon
% A = -eye(length(param_vec));
% b = zeros(length(param_vec),1);
% ub = [10, inf, inf, inf, 1e-6, inf, 1e-5, 1e-5]';
% options = optimoptions('fmincon','Display','iter',...
%     'StepTolerance',1e-10);
% param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], ub, [], options);
% final_cost = V_theta(param_opt)

%% Run optimisation with patternsearch
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
lb = [0; 1e-4; 0; 0; 0; 0; 0; 0];
ub = [inf, 1e0, inf, inf, inf, inf, inf, inf]';
options = optimoptions('patternsearch','Display','iter','StepTolerance',1e-10);
param_opt = patternsearch(V_theta, param_vec, A, b, [], [], lb, ub, [], options);
final_cost = V_theta(param_opt)


%% Plot results 
y_cts = runSim(param_opt,x0,t_sim,V);
figure(1); clf;
subplot(2,1,1)
plot(t_sim,y_true(:,1),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,1),'DisplayName','Simulated');
xlabel('Time [s]');
ylabel('Velocity [rad/s]');
subplot(2,1,2);
plot(t_sim,y_true(:,2),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,4),'DisplayName','Simulated');
ylabel('Position [rad]');
xlabel('Time [s]');
grid on;
legend;


%% Prompt for save of file
answer = questdlg("Save parameters?","Save prompt","Yes","No","No");
if answer == "Yes"
    saveFileName = sprintf("%s_params.mat", type);
    save(saveFileName, 'param_opt');
end


%% Additional functions
function y = runSim(param_vec, x0, t_sim, V)
% q = omega;
% p = Ia
    
% Unpack params
%{
    Ra  = param_vec(1);
    La  = param_vec(2);
    kv  = param_vec(3);
    kt  = param_vec(4);
    B   = param_vec(5);
    Jh  = param_vec(6);
    N   = param_vec(7);
    
    alpha1 = param_vec(8);
%     alpha2 = param_vec(9);
%     alpha3 = param_vec(10);    
%}
    Ra      = param_vec(1);
    La      = param_vec(2);
    kt      = param_vec(3);
    B       = param_vec(4);
    alpha1  = param_vec(5);
    kv      = param_vec(6);
    alpha2  = param_vec(7);
    alpha3  = param_vec(8);

    N = 98.78;
    Jh = 0.0039;
    J = Jh/N^2;
    
    % CCRs
    tauM    =@(p) kt*p;
    Vb      =@(q) kv*q;
    % alpha1 = magnitude of deadzone (more +ve = bigger deadzone)
    % alpha2 = magnitude of deadzone (more +ve = bigger deadzone)
    % alpha3 = settling time after overshoot ( more +ve = faster settle)
%     tauF    =@(q) (alpha1 + alpha2*exp(-alpha3*abs(q)))*sign(q);
    tauF    =@(q) alpha1*sign(q);
    
    % SSRs
%     dq =@(q,p,Va) ((tauM(p) - B*abs(q))/J)*double(~(abs(q)<2e-1 && abs(Va)<2.2));
%     dq =@(q,p) (tauM(p) - B*q)/J;
    dq =@(q,p,Va) N*((tauM(p) - B*q - tauF(q))/J); % w_motor is N*w_output 
    dp =@(q,p,Va) (Va - Ra*p - Vb(q))/La;
    theta =@(q,p,Va) q;



    % omega_a, Ia, Va, theta_a
    dx =@(q,p,Va) [dq(q,p,Va); dp(q,p,Va); Va; theta(q,p,Va)];
    
    % Define output
    output = @(x) [x(1); x(2); x(3); x(4)];

    % Inputs
    Va = @(t) interp1(t_sim,V,t);

    % Create wrapper function
    dx_wrap = @(t,x) dx(x(1),x(2),Va(t));

    % Run simulation
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
    
    % Generate output
    y = zeros(length(res.t),4);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end

end













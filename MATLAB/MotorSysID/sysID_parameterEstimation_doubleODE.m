% Clean up
clc;
clear;

%% Get data
% Pick sysID input
% type = "chirp_sig";
% type = "sinWave_load_05hz";
% type = "sinWave";
% type = "sinWave1hz";
% type = "sinWave2hz";
% type = "sinWave3hz";
% type = "sinWave4hz";
% type = "deadZone";
% type = "rampDown";
% type = "rampUp";
type = "rampUpDown";
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

k = find(V<0);
current(k) = -1.*current(k);

% Define intial conditions
w_a0 = 0;
Ia0 = 0;
Va0 = 0;
x0 = [Ia0; w_a0];

% Simulation time
dt = 0.01;
t_sim = time();
input = [V, vel, current];


%% Run system identification
% Define initial parameter guess
%             Kt,  B,   a1,   b1,    a2,    w_c
% param_vec = [0.1; 0.01; 0.01; 0.01; 0.001; 0.001];

Jh = 0.0039;
% 2.2256 0.0047 6.3369e-4 1.9722e3 464.6250 4.5630 3.6914 1.0414 0.2510
% 1e-3 991 0.5 212.1250
% param_vec = [2; 0.1; 0.00001; 1000; 1000; 10000; 1.5; 1; 0.1; 0.0001; 1000; 1; 10000];
param_vec = [4.2005e3; 0.0872; 6.0509e-4; 1.0011e3; 1.0023e3; 7.9317e4; 3.6622; 1.8451; 0.3583; 2.8311e-4; 2.2251e5; 1.5441e4; 3.2077e4];

% param_vec = [2; 0.1; 1e-3; 3e4; 2e5; 2e4; 1.5; 1; 0.1; 1e-3; 2e4; 1e3; 1e4];
% param_vec = [2; 0.1; 1e-4; 3e3; 1e3; 2.1e5; 1.5; 1; 0.1; 1e-3; 2e4; 3.5e4; 2e4];


% %%%%%%%%%%%%%%%
% 4.2005e3  0.0872  6.0509e-4   1.0011e3    1.0023e3    7.9317e4    3.6622
% 1.8451    0.3583  2.8311e-4   2.2251e5    1.5441e4    3.2077e4 
% %%%%%%%%%%%%%%

% La = 0.0135 or 0.021615
% Ra = 2.5625;
% Kw = 0.0086;

% Compute cost
y_true = [V, current, vel];

% V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));
% V_y = @(y) (y(:,2) - y_true(:,2)).'*(y(:,2) - y_true(:,2));
% V_y = @(y) (y(:,3) - y_true(:,3)).'*(y(:,3) - y_true(:,3));
V_y =@(y) 0.3*((y(:,3) - y_true(:,3)).'*(y(:,3) - y_true(:,3)))...
            + 0.7*((y(:,2) - y_true(:,2)).'*(y(:,2) - y_true(:,2)));

V_theta = @(theta) V_y(runSim(theta,x0,t_sim,input));

%% Run optimisation with fmincon
% A = -eye(length(param_vec));
% b = zeros(length(param_vec),1);
% % lb = [0; 0; 1e-9];
% options = optimoptions('fmincon','Display','iter','StepTolerance',1e-10);
% param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], [], [], options);
% final_cost = V_theta(param_opt)

%% Run optimisation with patternsearch
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
lb = 1e-8.*ones(length(param_vec),1);
% ub = 2.1e5.*ones(length(param_vec),1);
options = optimoptions('patternsearch','Display','iter','StepTolerance',1e-10, 'MaxFunctionEvaluations', 5000);
param_opt = patternsearch(V_theta, param_vec, A, b, [], [], lb, [], [], options);
final_cost = V_theta(param_opt)

%% Plot results 
y_cts = runSim(param_opt,x0,t_sim,input);
% y_cts = runSim(param_vec,x0,t_sim,input);

figure(1); clf;
ax1 = subplot(3,1,1);
plot(t_sim,y_true(:,1),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,1),'DisplayName','Simulated');
xlabel('Time [s]');
ylabel('Voltage [V]');
grid on;
legend('location','best');

ax2 = subplot(3,1,2);
plot(t_sim,y_true(:,3),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,3),'DisplayName','Simulated');
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
grid on;
legend('location','best');

ax3 = subplot(3,1,3);
plot(t_sim,y_true(:,2),'DisplayName','Measured Current'); hold on;
plot(t_sim,y_cts(:,2),'DisplayName','Simulated Current');
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
    % Kt, B, a1, a2, a3, VaC
    a1      = param_vec(1);
    a2      = param_vec(2);
    a3      = param_vec(3);
    B1      = param_vec(4);
    B2      = param_vec(5);
    B3      = param_vec(6);
    Jh      = param_vec(7);
    a4      = param_vec(8);
    a5      = param_vec(9);
    a6      = param_vec(10);
    B4      = param_vec(11);
    B5      = param_vec(12);
    B6      = param_vec(13);
    
    % Fixed parameters
    Ra = 2.5625;
    Kw = 0.0086;
%     La = 0.0135;
    La = 0.0233;
    N  = 98.78;
    J  = Jh/(N^2); % Inertia at the motor (low torque/High speed) side of gearbox

    % Unpack input
    V = input(:,1);
    % Interpolation function
    Va  =@(t) interp1(t_sim,V,t);
    
    % Useful functions
    heavi_p =@(x) 1./(1 + exp(-2.*50.*x));
    heavi_m =@(x) 1./(1 + exp(-2.*50.*-x));
    
    % CCRs
    eb      =@(w_m) Kw.*w_m;
    tauF    =@(w_m) (a1.*(tanh(B1.*w_m) - tanh(B2.*w_m)) + a2.*tanh(B3.*w_m) + a3.*w_m).*heavi_p(w_m)...
                    + (a4.*(tanh(B4.*w_m) - tanh(B5.*w_m)) + a5.*tanh(B6.*w_m) + a6.*w_m).*heavi_m(w_m);
%      tauF =@(w_m) (a1.*(tanh(B1.*w_m) - tanh(B2.*w_m)) + a2.*tanh(B3.*w_m)).*heavi_p(w_m)...
%                 + (a4.*(tanh(B4.*w_m) - tanh(B5.*w_m)) + a5.*tanh(B6.*w_m)).*heavi_m(w_m);       
            
            
%     tauF =@(w_m) (a1.*(tanh(B1.*w_m) - tanh(B2.*w_m)) + a2.*tanh(B3.*w_m) + a3.*w_m);

%     tauM=@(Ia) (0.06085.*Ia.^2 + 0.6843.*Ia).*heavi_m(Ia) + (0.5702.*exp(0.3512.*Ia) - 0.5703.*exp(-1.82.*Ia)).*heavi_p(Ia);
    tauM =@(Ia) 0.06761.*Ia.^4 + 0.2721.*Ia.^3 - 0.02975.*Ia.^2 + 1.417.*Ia;


    % SSRs
%     dw_m    =@(w_m,Ia,Va) (motorTorque(Ia) - tauF(w_m))./J; 
    dw_m    =@(w_m,Ia,Va) (tauM(Ia) - tauF(w_m))./J; 
    dIa     =@(w_m,Ia,Va) (Va - Ra*Ia - eb(w_m))/La;
    
    % Create ode function
    dx =@(w_m,Ia,Va) [dIa(w_m,Ia,Va); dw_m(w_m,Ia,Va)];
    
    % Create wrapper function
    dx_wrap = @(t,x) dx(x(2),x(1),Va(t));

    % Run simulation
%     options = odeset("OutputFcn",@odeplot);
%     [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
    [res.t,res.x] = ode23s(dx_wrap, t_sim, x0);

    % Calculate dIa
    w_sim = res.x(:,2);
    Ia_sim = res.x(:,1);
    try
        dIa_eq = (Va(t_sim) - Ra.*Ia_sim - eb(w_sim))./La;
    catch
       pause; 
    end
    % Simulated Voltage function
%     V_hat =@(Ia,w_m) Ra.*Ia + Kw.*w_m + La.*dIa_eq;
    V_hat =@(Ia,w_m) Ra.*Ia + Kw.*w_m;

    % Define output
    % V_hat, Ia, w_m
    res.x(:,1) = V_hat(Ia_sim,w_sim);
    res.x(:,2) = Ia_sim;
    res.x(:,3) = w_sim;
    output =@(x) [x(1); x(2); x(3)./N];
    % Generate output
    y = zeros(length(res.t),3);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end
    
end









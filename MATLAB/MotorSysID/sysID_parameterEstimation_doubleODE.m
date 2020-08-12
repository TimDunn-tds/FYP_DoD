% Clean up
clc;
clear;

%% Get data
% Pick sysID input
% type = "chirp_sig";
type = "sinWave";
% type = "sinWave1hz";
% type = "sinWave3hz";
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
% Correct current
k       = find(V<0);
current(k) = current(k)*-1;
% Define intial conditions
w_a0 = 0;
Ia0 = 0;
Va0 = 0;
x0 = [Ia0; w_a0];

% Simulation time
dt = 0.01;
t_sim = time;
y_true = [V, current, vel];
input = [V, vel, current];

%% Run system identification
% Define initial parameter guess
% Kt, B, 
param_vec = [1; 0.1; 0.1; 1];

% La = 0.0135 or 0.021615
% Ra = 2.5625;
% Kw = 0.0086;

% Compute cost
% V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));
% V_y = @(y) (y(:,2) - y_true(:,2)).'*(y(:,2) - y_true(:,2));
V_y = @(y) (y(:,3) - y_true(:,3)).'*(y(:,3) - y_true(:,3));
% V_y =@(y) 0.5*((y(:,2) - y_true(:,3)).'*(y(:,2) - y_true(:,3)))...
%             + 0.5*((y(:,3) - y_true(:,2)).'*(y(:,3) - y_true(:,2)));

V_theta = @(theta) V_y(runSim(theta,x0,t_sim,input));

%% Run optimisation with fmincon
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
% lb = [0; 0; 1e-9];
options = optimoptions('fmincon','Display','iter','StepTolerance',1e-10);
param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], [], [], options);
final_cost = V_theta(param_opt)

%% Plot results 
y_cts = runSim(param_opt,x0,t_sim,input);
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
% answer = questdlg("Save parameters?","Save prompt","Yes","No","No");
% if answer == "Yes"
%     saveFileName = sprintf("%s_params.mat", type);
%     save(saveFileName, 'param_opt');
% end

%% Additional functions
function y = runSim(param_vec, x0, t_sim, input)

    % Unpack params
    % Kt, B
    Kt      = param_vec(1);
    B       = param_vec(2);
    a1      = param_vec(3);
    b1      = param_vec(4);
        
    % Fixed parameters
    Ra = 2.5625;
    Kw = 0.0086;
    La = 0.0135;
    N = 98.78;
    Jh = 0.0039;
    J = Jh/(N^2); % Inertia at the motor (low torque/High speed) side of gearbox
    
    % Unpack input
    V = input(:,1);
    % Interpolation function
    Va  =@(t) interp1(t_sim,V,t);
    
    % CCRs
    tauM    =@(Ia) Kt.*Ia;
    tauF    =@(w_m) (b1*w_m)/(sqrt(a1 + w_m^2));
    eb      =@(w_m) Kw.*w_m;
    
    % SSRs
    dw_m =@(w_m,Ia,Va) (tauM(Ia) - B*w_m - tauF(w_m))/J; % w_m (not speed of hand)
%     dw_m =@(w_m,Ia,Va) (tauM(Ia) - B*w_m)/J; 
    dIa     =@(w_m,Ia,Va) (Va - Ra*Ia - eb(w_m))/La;
    
    % Create ode function
    dx =@(w_m,Ia,Va) [dIa(w_m,Ia,Va); dw_m(w_m,Ia,Va)];
    
    % Create wrapper function
    dx_wrap = @(t,x) dx(x(2),x(1),Va(t));

    % Run simulation
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
    
    % Calculate dIa
    w_sim = res.x(:,2);
    Ia_sim = res.x(:,1);
    dIa_eq = (Va(t_sim) - Ra.*Ia_sim - eb(w_sim))./La;
    
    % Simulated Voltage function
    V_hat =@(Ia,w_m) Ra.*Ia + Kw.*w_m + La.*dIa_eq;
    
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









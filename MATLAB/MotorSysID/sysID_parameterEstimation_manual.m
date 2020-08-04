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

x0 = [q0; p0];

% Simulation time
Nt = 300;
% dt = time(2) - time(1);
dt = 0.01;
t_sim = 0:dt:time(end);
% y_true = vel(1:1/dt:end);
y_true = vel;


%% Run system identification
% Define initial parameter guess
% Ra, La, kt, B, alpha1, kv, alpha2, alpha 3
% param_vec = [7.4099e-06; 1.0861; 0.7985; 0.1];
param_vec = [0.123; 0.5; 200; 27.09; 1e-6; 0.8; 0.6e-6; 2e-7; 2e-8; 2e-8; 2e-8];
% param_vec = [7.4099e-06; 1.0861; 0.7985; 0.1; 0];
y_initial = runSim(param_vec,x0,t_sim,V);

% Ra, La, kv, kt, B, Jh, N, alpha1, alpha2, alpha3
% param_vec = [2.14; 0.1; 0.01; 0.01; 0.001; 0.0039; 98.78; 0.1; 0.1; 0.1];

% Compute cost
% V_y = @(y) (y(:,1) - y_true(1:1/dt:Nt,1)).'*(y(:,1) - y_true(1:1/dt:Nt,1));
V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));

V_theta = @(theta) V_y(runSim(theta,x0,t_sim,V));



% Plot results 
y_cts = runSim(param_vec,x0,t_sim,V);
figure(1); clf;
% subplot(2,1,1)
% plot(time,V);
% xlabel('Time [s]');
% ylabel('Armature Voltage [V]');
% subplot(2,1,2);
plot(t_sim,y_true,'DisplayName','Measured'); hold on;
plot(t_sim,y_cts,'DisplayName','Simulated');
ylabel('Velocity [rad/s]');
xlabel('Time [s]');

final_cost = V_theta(param_vec)

% %% Prompt for save of file
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
    alpha4  = param_vec(9);
    alpha5  = param_vec(10);
    alpha6  = param_vec(11);

    N = 98.78;
    Jh = 0.0039;
    J = Jh/N^2;
    
    % CCRs
    tauM    =@(p) kt*p;
    Vb      =@(q) kv*q;
    tauF    =@(q) (alpha1 + alpha2*exp(-alpha3*abs(q)))*sign(q)...
                    + (alpha4 + alpha5*exp(-alpha6*abs(q)))*sign(q);
%     tauF    =@(q) alpha1*sign(q);
    
    % SSRs
%     dq =@(q,p) (tauM(p) - B*q)/J;
    dq =@(q,p) (tauM(p) - B*q - tauF(q))/J;
    dp =@(q,p,Va) (Va - Ra*p - Vb(q));
    
    dx =@(q,p,Va) [dq(q,p); dp(q,p,Va)];
    
    % Define output
    output = @(x) [x(1)];

    % Inputs
    Va = @(t) interp1(t_sim,V,t);
%     Va =@(t) 4*sin(2*pi*0.5*t);

    % Create wrapper function
    dx_wrap = @(t,x) dx(x(1),x(2),Va(t));

    % Run simulation
%     [res.t,res.x] = ode45(dx_wrap, t_sim, x0);
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);

    % Generate output
    y = zeros(length(res.t),1);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end

end









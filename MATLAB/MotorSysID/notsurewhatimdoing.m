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

% Define intial conditions
% q = omega;
% p = Ia
q0 = 0;
p0 = 0;
x0 = [q0];

% Simulation time
dt = 0.01;
t_sim = time;
y_true = [V, vel, current];

%% Run system identification
% Define initial parameter guess
% R1, R2, R3, R4, R5, La, Kt, B, Kw, 
param_vec = [1; 1; 1; 1; 1; 1; 1; 1; 1];

% Compute cost
V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));

V_theta = @(theta) V_y(runSim(theta,x0,t_sim));

%% Run optimisation with fmincon
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
options = optimoptions('fmincon','Display','iter');
param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], [], [], options);
final_cost = V_theta(param_opt)


%% Plot results 
y_cts = runSim(param_opt,x0,t_sim);
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
plot(t_sim,y_true(:,3),'DisplayName','Measured Current'); hold on;
plot(t_sim,y_cts(:,3),'DisplayName','Simulated Current');
ylabel('Current [A]');
xlabel('Time [s]');
accel = zeros(length(t_sim),1);
accel(2:end) = (y_cts(2:end,3) - y_cts(1:end-1,3))./dt;
yyaxis right
ylabel('Gradient of \omega [rad/s^2]');
plot(t_sim,accel,'DisplayName','d/dt(\omega)');
grid on;
legend;

linkaxes([ax1,ax2,ax3],'x');


%% Prompt for save of file
answer = questdlg("Save parameters?","Save prompt","Yes","No","No");
if answer == "Yes"
    saveFileName = sprintf("%s_params.mat", type);
    save(saveFileName, 'param_opt');
end


%% Additional functions
function y = runSim(param_vec, x0, t_sim)
    % q = w_m;
    % p = Ia

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

    N = 98.78;
    Jh = 0.0039;
    J = Jh/N^2; % Inertia at the motor low torque/High speed side of gearbox
    
    % CCRs
    tauM    =@(p) Kt*p;
    Ra      =@(Ia) R1 + R2*exp(-R3*abs(Ia)) + R4*exp(-R5*abs(Ia)^2);
    w       =@(Va,Ia) (Va*La - Ra(Ia)*Ia)/Kw;
    eb      =@(q) Kw*q;

    % SSRs
    dp =@(p,Va) (Va - Ra(p)*p - Kw*w(Va,p))/La;

    % w_a, Ia
    dx =@(p,Va) [dp(p,Va)];

    % Define output
%     V_sim =@(q,p) Ra(p)*p + Kw*q;
    V_sim =@(p,Va) (Ra(p)*p + Kw*w(Va,p))/La;
    output = @(x) [V_sim(x(1),x(2)); x(1); x(2)]; 
    % Velocity on high torque/low speed side of gearbox = w_a/N = x(1)/N

    % Input
%     Va =@(t) 4*sin(2*pi*0.5*t);
    Va =@(t) 4*sin(2*pi*1*t);

    % Create wrapper function
    dx_wrap = @(t,x) dx(x(1),Va(t));

    % Run simulation
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
    
    % Generate output
    y = zeros(length(res.t),3);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end

end













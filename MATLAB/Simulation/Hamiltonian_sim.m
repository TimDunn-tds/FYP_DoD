%% Clear parameters
clear; 

T = 0.12;
tsim = 10;

%% Parameters
% mo = 0.1;                   % kg
% mh = 0.3;                   % kg

ro = 0.06;                  % m
% ro = 0.15;
rh = 0.12;                   % m

thick = 0.01;               % m

p = 1200;                   % kg/m3

mo = pi*(ro^2)*thick*p;
mh = pi*(rh^2)*thick*p;

Jo = 0.5*mo*(ro^2);
% Jh = 0.5*mh*(rh^2);
%    Jdisk      + Jcoupling + Jshaft
% Jh = 3.908e-3   + 1.103e-6  + 2.38e-6;

Jh = 0.0225;

g = 9.82;                   % m/s^2

theta0 = 0*(pi/180);        % rad
phi0 = 1*(pi/180);          % rad

% SysID parameters
Ra  = 2.5625;
Kw  = 0.0086;
N   = 98.78;

%% Friction Parameters
a1 = 6.5035;
a2 = 6.3853e-4;
a3 = 0.0011;
B1 = 3.0021e3;
B2 = 986.7121;
B3 = 210000;
a4 = 0.0250;
a5 = 0.1042;
a6 = 0.0012;
B4 = 19998;
B5 = 35000;
B6 = 19998;

%% Motor parameters
alpha_max = 20/T;              % Nm
alpha_min = -20/T;             % Nm
p1 = 0.7427;
p2 = -0.2106;
p3 = 1.121;

%% Controller Model (linearised about upright balancing position)
% State variables:
% x(1) = hand angular velocity [rad/s]      dtheta
% x(2) = object angular velocity [rad/s]    dphi
% x(3) = object angle [rad/s]               phi

bot = mo*((rh+ro)^2) + Jo*((rh^2)/(ro^2));

A = [0, 0, 0;
     0, 0, (mo*g*(rh+ro))/bot;
     0, 1, 0];
 
B = [1; (Jo*(rh^2/ro^2))/bot; 0];


% Cr = [1, 0, 0, 0]; % Regulate hand velocity
Cr = [1, 0, 0]; % Regulate hand velocity

Dr = 0; % No feedthrough
% 
% Cm = [0, 0, 1, 0;
%       0, 0, 0, 1;
%       1, 0, 0, 0]; % Measure hand and object positions


% Reduced-state continuous-time model for control design
idx = [1 2 3];                % Only include these states
Ac  = A(idx,idx);
Bc  = B(idx,:);
Cc  = Cr(:,idx);
Dc  = Dr;
sysc = ss(Ac,Bc,Cc,Dc,T);

p1 = -4 + 0.5i;
p2 = -4 - 0.5i;
p3 = -1.9;

k = place(Ac,Bc,[p1 p2 p3]);
pole(ss(Ac,Bc,Cc,Dc,T));
   
% [Acd, Bcd] = c2d(Ac,Bc,T);

%% Design LQG
Qc = [
    1, 0, 0;
    0, 1, 0;
    0, 0, 1];           % penalise state
% Qc = eye(4);
Rc = 1e-0 ;                 % penalise actuator 

Kc = zeros(1,length(idx));
Kc(:,idx)   = lqrd(Ac, Bc, Qc, Rc, T);	% Feedback gain matrix

% Design feedforward gains (see eqn 8.73 from Franklin et. al. (1998))
[Acd, Bcd]  = c2d(Ac, Bc, T);
NN          = [Acd - eye(length(idx)), Bcd; Cc, Dc]\[zeros(length(idx),1);1];
Nx          = zeros(length(Cr),1);
Nx(idx,:)   = NN(1:length(idx));
Nu          = NN(end);

%% Design Observer
% % initial uncertainty and intial state
mup_init = [0; 0; theta0; phi0];
% % Pp_init = diag([1, 1, 1, 1]);
% Pp_init = 1e1*eye(4);
% % mup_init = [theta0; phi0];
Pp_init = diag([1e0, 1e0, 1e0, 1e0]);
% 
% % State variables:
% % x(1) = hand angular velocity [rad/s]                    dtheta
% % x(2) = object angular velocity about hand CoM [rad/s]   dphi
% % x(4) = object CoM angle relative to hand CoM [rad]      phi
% 
% MM = [
%     Jh + Jo*(rh^2/ro^2), -Jo*(rh^2/ro^2);
%     -Jo*(rh^2/ro^2), Jo*(rh^2/ro^2) + mo*((rh + ro)^2)];
% 
% KK = [0, 0; 0, -mo*g*(rh + ro)];
% 
% DD = [0.1895, 0; 0, 0];
% 
% e = [1;0];


% Create matrices 
% Ao = [MM\-DD, MM\-KK; eye(2,2), zeros(2,2)];
% 
% Bo = [MM\e; zeros(2,1)];

% Cm = [0, 0, 1, 0;
%       0, 0, 0, 1;
%       1, 0, 0, 0]; % Measure hand and object positions
%   
% Dm = [0;0;0];

% x(1) = hand angular velocity [rad/s]      dtheta
% x(2) = object angular velocity [rad/s]    dphi
% x(3) = hand angle [rad]                 theta
% x(4) = object angle [rad]                   phi

Ao = [0, 0, 0, 0;
     0, 0, 0, (mo*g*(rh+ro))/bot;
     1, 0, 0, 0;
     0, 1, 0, 0];
 
Bo = [1; (Jo*((rh^2/ro^2)))/bot; 0; 0];

Cm = [1, 0, 0, 0;
      0, 0, 1, 0;
      0, 0, 0, 1];
  
Dm = [0; 0; 0];

[Aod, Bod] = c2d(Ao, Bo, T);
% 
% Qo = diag([1e-5, 1e8, 1e-5, 1e8]);
% Qo = eye(4);
% Qo = diag([1e1, 1e1, 1e0, 1e-3]);
Qo = diag([1e-10, 1e-10, 1e-10, 1e-10]);

% Ro = 2.5e-07*eye(size(Cm,1));
% Ro = diag([1e-3, 1e-3, 1e-2]);
Ro = diag([1e3, 1e3, 1e3]);

% Ro = eye(3);

% % Ro = 1e0*eye(3);
% Lo = dlqr(Aod.',Cm.',Qo,Ro).';
sys = ss(Aod, [Bod eye(length(Bod))], Cm, [Dm zeros(size(Dm,1),length(Bod))], -1);
[~,Lo] = kalman(sys, Qo, Ro);
% % [~,Lo] = kalmd(ss(Aod, [Bod eye(length(B))], Cm, [Dm zeros(size(Dm,1),length(B))]), Qo, Ro, T)
% %    2.1695e-07


%% Old MPC
%% Create MPC Controller

% System dimensions
n_in = size(Bcd,2);     % No. actuators
n_out = size(Cc,1);     % No. outputs to regulate 
n_states = size(Acd,1); % No. states


N_MPC = 10; % Horizon

% Quadratic cost weighting
Ru = 1;         % Penalty on input moves (i.e. ||u[k]-u[k-1]||)
Qy = 1;      	% Penalty on output error


% Consider the augmented system
%
% [ x[k+1] ] = [ A B ]*[ x[k]   ] + [ B ]*(u[k] - u[k-1])
% [ u[k]   ]   [ 0 I ] [ u[k-1] ]   [ I ]
% \________/   \_____/ \________/   \___/ \_____________/
%   z[k+1]   =   Az   *  z[k]     +  Bz  * du[k]
%
% y[k] = [ C D ]*[ x[k]   ] + D*(u[k] - u[k-1])
%                [ u[k-1] ]
%        \_____/ \________/   | \_____________/
%          Cz   *  z[k]     + Dz * du[k]

Az = [Acd, Bcd; zeros(n_in, n_states), eye(n_in)];
Bz = [Bcd; eye(n_in)];
Cz = [Cc, Dc];
Dz = Dc;

n_z = n_states + n_in;     % Number of augmented states

AA = zeros((N_MPC+1)*n_z,n_z);
for i = 1:N_MPC+1
    AA(1+(i-1)*n_z:i*n_z,:) = Az^(i-1);
end
AB = zeros((N_MPC+1)*n_z,n_in);
for i = 2:N_MPC+1
    AB(1+(i-1)*n_z:i*n_z,:) = Az^(i-2)*Bz;
end
BB = zeros((N_MPC+1)*n_z,N_MPC*n_in);
for i = 1:N_MPC
    BB(1+(i-1)*n_z:end,1+(i-1)*n_in:i*n_in) = AB(1:(N_MPC-i+2)*n_z,:);
end
WW = kron(eye(N_MPC)-diag(ones(1,N_MPC-1),-1),eye(n_in));
EE = zeros(N_MPC*n_in,n_states+n_in);
EE(1:n_in,1+n_states:end) = -eye(n_in);

Qz = Cz.'*Qy*Cz;
Sz = Cz.'*Qy*Dz;
Rz = Dz.'*Qy*Dz + Ru;
[~,Qf] = dlqr(Az,Bz,Qz,Rz,Sz);
% [~,Qf] = lqry(ss(Az,Bz,Cz,Dz,T),Qy,Ru);

QQ = blkdiag(kron(eye(N_MPC),Qz),Qf);
SS = kron(eye(N_MPC+1,N_MPC),Sz);
RR = kron(eye(N_MPC),Ru);

% Compute feedforward gains (we assume n_in == n_out)
NN = [Acd - eye(n_states), Bcd; Cc, Dc]\[zeros(n_states,n_out);eye(n_out)];

%-----------------------------------------------
% Generate the Hessian H and gradient coefficient
%-----------------------------------------------
H = WW.'*( BB.'*QQ*BB + BB.'*SS + SS.'*BB + RR )*WW;
Gamma = WW.'*[(BB.'*QQ + SS.')*(AA + BB*EE) + (BB.'*SS + RR)*EE, -(BB.'*QQ + SS.')*kron(ones(N_MPC+1,1),NN)];
% Note: The gradient is g = Gamma*[x1; u0; ref], but we do this
%       multiplication inside the controller

% The mpcqpsolver function used in Simulink requires the first input to be 
% the inverse of the lower-triangular Cholesky decomposition of the 
% Hessian matrix H.
L_H = chol(H,'lower');
Linv = L_H\eye(size(H,1));

opt = mpcqpsolverOptions;
opt.FeasibilityTol = 1e-6; %Default is 1e-6
opt.IntegrityChecks = true; %False is recommended for code generation only

%-----------------------------------------------
% Create plant constraints
%-----------------------------------------------

% Inequality constraints: Aineq*U <= bineq

% Aineq = zeros(1,N_MPC*n_in);    % TODO
% bineq = zeros(1,1);             % TODO
Aineq = [eye(N_MPC);-eye(N_MPC)];
fracU_min = zeros(N_MPC,1)+alpha_min;
fracU_max = zeros(N_MPC,1)+alpha_max;
bineq = [fracU_max; -fracU_min];




%% Run simulation
sim('Hamiltonian_model');


%% Plot
figure(1);  clf;

ax1 = subplot(2,2,1);
plot(tout,phi.signals.values.*180/pi,'DisplayName','phi','LineWidth',2); hold on;
plot(tout,mup.signals.values(:,4).*180/pi,'DisplayName','kf phi','LineWidth',2);
% plot(tout,mup_L.signals.values(:,4).*180/pi,'DisplayName','L phi','LineWidth',2);
% plot(tout, theta.signals.values.*180/pi);
legend;
grid on;
ylabel('Angle [\circ]');
xlabel('Time [s]');
title('Object Angle');

ax2 = subplot(2,2,2);
plot(tout, dphi.signals.values.*180/pi,'DisplayName','dphi','LineWidth',2); hold on;
plot(tout, mup.signals.values(:,2).*180/pi,'DisplayName','kf dphi','LineWidth',2);
% plot(tout, mup_L.signals.values(:,2).*180/pi,'DisplayName','L dphi','LineWidth',2);
% yyaxis right;
% plot(tout,phi.signals.values.*180/pi);
legend;
grid on;
ylabel('Angular Velocity [\circ/s]');
xlabel('Time [s]');
title('Object Angular Velocity');

ax3 = subplot(2,2,3); hold on;
plot(tout,dtheta_dem.signals.values,'DisplayName','dtheta','LineWidth',2);
plot(tout,mup.signals.values(:,1),'DisplayName','kf dtheta','LineWidth',2);
% plot(tout,mup_L.signals.values(:,1),'DisplayName','L dtheta','LineWidth',2);
plot(tout,ref.signals.values,'r','DisplayName','ref','LineWidth',2);
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
legend('location','best')
grid on;
title('Hand Angular Velocity');

ax4 = subplot(2,2,4); hold on;
% plot(dtheta_dem.time,dtheta_dem.signals.values,'LineWidth',2,'DisplayName','Demanded Velocity');
plot(tout,theta.signals.values,'DisplayName','theta','LineWidth',2);
plot(tout,mup.signals.values(:,3),'DisplayName','kf theta','LineWidth',2);
% plot(tout,mup_L.signals.values(:,3),'DisplayName','L theta','LineWidth',2);
% ylabel('Torque [Nm]');
% yyaxis right
% plot(tout, Va.signals.values,'LineWidth',2,'DisplayName','Demanded Voltage')
% ylabel('Voltage [V]')
legend('location','best');
grid on;
% title('Demanded Torque');
title('Hand angle');
xlabel('Time [s]');

linkaxes([ax1 ax2 ax3 ax4],'x');


% figure(2); clf; hold on; grid on;
% plot(tout,Va.signals.values,'DisplayName','Demanded Armature Voltage','LineWidth',2);
% plot(tout,
% ylabel('Voltage [V]')
% xlabel('Time [s]')
% title('Demanded voltage');
   

% figure(3); clf; hold on;
% alph = (rh/ro).*(phi.signals.values - theta.signals.values);
% plot(theta.time, abs(alph), 'DisplayName', 'alpha');
% plot(theta.time, abs(theta.signals.values), 'DisplayName', 'theta');
% legend;

% 
% figure(3); clf; hold on;
% plot(tout, squeeze(Pp.signals.values(1,1,:)), 'DisplayName','dtheta');
% plot(tout, squeeze(Pp.signals.values(2,2,:)), 'DisplayName','dphi');
% plot(tout, squeeze(Pp.signals.values(3,3,:)), 'DisplayName','theta');
% plot(tout, squeeze(Pp.signals.values(4,4,:)), 'DisplayName','phi');
% legend;


%% Visulisation?

F = runVis(rh, ro, phi, theta, dphi, dtheta_dem, tout);

%% Save video
% writerObj = VideoWriter('DoD_vid.avi');
% writerObj.FrameRate = 100;
% 
% open(writerObj);
% 
% for i=1:length(F)
%     frame = F(i);
%     writeVideo(writerObj, frame);
% end
% close(writerObj);
% 






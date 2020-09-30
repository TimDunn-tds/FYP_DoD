%% Clean up
clc; clear all

tsim    = 50;
loopRate = 20; % Hz
T = 1/loopRate; % seconds

%% Add simulation folder to path
addpath('..\Simulation');

%% Get parameters
plantParams     = getPlantParams();
plantModel      = getPlantModel(T);
obs             = getObserver(T);
KF              = getKalmanFilter(T);

%% Load data
load('data_PE3.mat');

%% Run optimisation problem
% Stack Qo and Ro diagonals into vector
param_vec = [1e1, 1e2, 1e0, 1e-4, 1e-2, 1e-3, 1e-1, 1e2, 1e2, 1e1, 1e1];

func =@(x) costFunction(x, data, plantModel, tsim, KF, obs);
lb = 1e-10.*ones(length(param_vec),1);
ub = 1e10.*ones(size(param_vec));
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
options = optimoptions('patternsearch','Display','iter','StepTolerance',1e-10,...
    'MeshTolerance', 1e-10, 'UseCompletePoll',true,'UseCompleteSearch',true);

param_opt = patternsearch(func, param_vec, A, b, [], [], lb, ub, [], options).';


%% Run global search
% s1points = 400;
% % gs = GlobalSearch('NumStageOnePoints', s1points, 'NumTrialPoints', 10*s1points);
% gs = GlobalSearch;
% 
% % Stack Qo and Ro diagonals into vector
% param_vec = [1e8, 1e8, 1e8, 1e8, 1e-3, 1e-6, 1e-8];
% 
% func =@(x) costFunction(x, data, plantModel, tsim, KF, obs);
% lb = 1e-12.*ones(length(param_vec),1);
% A = -eye(length(param_vec));
% b = zeros(length(param_vec),1);
% options = optimoptions('fmincon','Display','iter','StepTolerance', 1e-12);
% problem = createOptimProblem('fmincon','x0', param_vec, 'objective', func, 'lb', lb, 'options', options);
% param_opt = run(gs,problem)';
% final_cost = func(param_opt)


%% Plot solution
% param_vec = [1e10, 6e1, 4e4, 1e-2, 2e-4, 1e10, 7e0, 1e2, 1e-4, 1e-4, 1e10];
% param_vec = 1e-10.*ones(size(param_vec));

% plotter(param_vec, data, plantModel, tsim, KF, obs);
plotter(param_opt, data, plantModel, tsim, KF, obs);



%% Additional Functions
function cost = costFunction(param_vec, data, plantModel, tsim, KF, obs)
    T = KF.T;
%     loopRate = 1/T;

 % Unpack variables and pack observer
    Qo = diag(param_vec(1:4));
    Ro = diag(param_vec(5:7));
   
    obs.Qo = Qo;
    obs.Ro = Ro;
    sys = ss(obs.Aod, [obs.Bod eye(length(obs.Bod))], obs.Cm, [obs.Dm zeros(size(obs.Dm,1),length(obs.Bod))], -1);
    [~,obs.Lo] = kalman(sys, obs.Qo, obs.Ro);

    KF.Qo = Qo;
    KF.Ro = Ro;
    
    % Unpack data
    theta_m = data(:,2)';
    phi_m = data(:,6)';
    dtheta_m = data(:,3)';
    wHat = data(:,7);
    dphi_m = data(:,8)';
    alphaHat = data(:,9)';
    mupO = nan(size(KF.Aod,1),tsim + 1);
    mupO(:,1)    = KF.mup_init; 
    
    mupK = nan(size(KF.Aod,1),tsim + 1);
    mupK(:,1)    = KF.mup_init; 
    Pp          = nan([size(KF.Aod), tsim + 1]);
    Pp(:,:,1)   = diag(param_vec(8:11));
    
    % Run loop 
    for k = 2:tsim
        y = [dtheta_m(k); theta_m(k); phi_m(k)];
%         u = wHat(k);
        u = alphaHat(k);
       
        mupO = runObserver(mupO, u, y, plantModel, obs, k);
        [mupK(:,k),Pp(:,:,k)] = runKF(mupK(:,k-1), u, y, Pp(:,:,k-1), KF, k);
    end

%     Comput cost
%     cost = 1*norm(mupO(1,1:end-1) - dtheta_m)...
%            + 1*norm(mupO(3,1:end-1) - theta_m)...
%            + 1*norm(mupO(4,1:end-1) - phi_m)...
%            + 1*norm(mupO(2,1:end-1) - dphi_m);
       
   cost =    1*norm(mupK(1,1:end-1) - dtheta_m)...
           + 1*norm(mupK(3,1:end-1) - theta_m)...
           + 1*norm(mupK(4,1:end-1) - phi_m)...
           + 1*norm(mupK(2,1:end-1) - dphi_m);

end



function plotter(param_vec, data, plantModel, tsim, KF, obs)
    T = KF.T;
    loopRate = 1/T;

 % Unpack variables and pack observer
    Qo = diag(param_vec(1:4));
    Ro = diag(param_vec(5:7));

    obs.Qo = Qo;
    obs.Ro = Ro;
    sys = ss(obs.Aod, [obs.Bod eye(length(obs.Bod))], obs.Cm, [obs.Dm zeros(size(obs.Dm,1),length(obs.Bod))], -1);
    [~,obs.Lo] = kalman(sys, obs.Qo, obs.Ro);

    KF.Qo = Qo;
    KF.Ro = Ro;
    
    % Unpack data
    theta_m     = data(:,2)';
    phi_m       = data(:,6)';
    dtheta_m    = data(:,3)';
    wHat        = data(:,7);
    dphi_m      = data(:,8)';
    alphaHat    = data(:,9)';
    mupO        = nan(size(KF.Aod,1),tsim + 1);
    mupO(:,1)   = KF.mup_init; 
    
    mupK        = nan(size(KF.Aod,1),tsim + 1);
    mupK(:,1)   = KF.mup_init; 
    Pp          = nan([size(KF.Aod), tsim + 1]);
    Pp(:,:,1)   = diag(param_vec(8:11));
    % Run loop 
    for k = 2:tsim
        y = [dtheta_m(k); theta_m(k); phi_m(k)];
%         u = wHat(k);
        u = alphaHat(k);
       
        mupO = runObserver(mupO, u, y, plantModel, obs, k);
        [mupK(:,k),Pp(:,:,k)] = runKF(mupK(:,k-1), u, y, Pp(:,:,k-1), KF, k);
    end
    

    % Comput cost
%     cost = 0.3*norm(mup(1,1:end-1)/max(mup(1,1:end-1)) - dtheta_m/max(mup(1,1:end-1)))...
%            + 0.3*norm(mup(3,1:end-1)/max(mup(3,1:end-1)) - theta_m/max(mup(3,1:end-1)))...
%            + 0.3*norm(mup(4,1:end-1)/max(mup(4,1:end-1)) - phi_m/max(mup(3,1:end-1)));
%        

    mup = mupK;
%     mup = mupO;

    figure(2); clf; grid on; hold on;
    tplot = 0:T:(tsim-1)/loopRate;
    ax1 = subplot(2,2,1);
%     phi_m(1:end-1) = phi_m(2:end);
    p1 = plot(tplot,mup(4,1:end-1).*180/pi,'DisplayName','Estimated','LineWidth',2); hold on;
    p2 = plot(tplot,phi_m.*180/pi,'DisplayName','Measured','LineWidth',2); hold on;
    p3 = plot(tplot,8.2*ones(size(tplot)),'r','LineWidth',1.5, 'DisplayName', 'Bumper');
    plot(tplot,-9.5*ones(size(tplot)),'r','LineWidth',1.5);
    title('Measured vs Estimated Object Angle');
    xlabel('Time [s]');
    legend([p1, p2, p3], 'location','best');

    % legend;
    grid on;
    ylabel('Angle [\circ]');
    xlabel('Time [s]');
    title('Object Angle');


    ax2 = subplot(2,2,2);
    plot(tplot,mup(3,1:end-1).*180/pi,'DisplayName','Estimated','LineWidth',2); hold on;
    plot(tplot,theta_m.*180/pi,'DisplayName','Measured','LineWidth',2); hold on;
    legend('location','best');
    grid on;
    ylabel('Hand [\circ]');
    xlabel('Time [s]');
    title('Hand Angle');


    ax3 = subplot(2,2,3); hold on;
    plot(tplot,mup(1,1:end-1),'DisplayName','Estimated','LineWidth',2);
    plot(tplot,dtheta_m,'DisplayName','Measured','LineWidth',2); hold on;
    ylabel('Velocity [rad/s]');
    xlabel('Time [s]');
%     yyaxis right
%     plot(tplot, mup(2,2:end), 'g', 'DisplayName', 'dPhi', 'LineWidth', 2); hold on;
%     ylabel('Velocity [rad/s]');
    legend('location','SouthWest');
    grid on;
    title('Hand Angular Velocity');
    set(gca, 'Ycolor', 'k');


    ax4 = subplot(2,2,4); hold on; grid on;
    plot(tplot,mup(2,1:end-1),'DisplayName','Estimated','LineWidth',2);
    plot(tplot,dphi_m,'DisplayName','Measured','LineWidth',2);
    ylabel('Dphi [rad/s]')
    xlabel('Time [s]');
    legend('location','best');
    linkaxes([ax1 ax2 ax3 ax4],'x');
    set(gca, 'Ycolor', 'k');
    
       
       
end
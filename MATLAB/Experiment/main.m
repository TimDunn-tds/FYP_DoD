%% Clean up
clc; clear;

tsim    = 100;

%% Load parameters
plantParams     = getPlantParams();
frictParams     = getFrictionParams();
motorParams     = getMotorParams();
cameraParams    = getCameraParams();


%% Load Plant Model, LQG and Kalman Filter
plantModel  = getPlantModel();
LQG         = getLQG();
KF          = getKalmanFilter();

% Initialise Kalman Filter
x0          = getInitialPosition();
mup         = nan(size(plantModel.A,1),tsim + 1);
mup(:,1)    = KF.mup_init; 
Pp          = nan([size(plantModel.A), tsim + 1]);
Pp(:,:,1)   = KF.Pp_init;
% muf         = zeros(size(plantModel.A,1), tsim);
% Pf          = zeros(size(Pp));


%% Make serial connection
stm32 = openSerial();


%% Connect to webcam
vid = webcam(1);
vid.Resolution = '640x480';
preview(vid);
pause(3);


%% Control Loop
% Make space
data    = zeros(tsim,6);
theta   = zeros(tsim,1);
phi     = zeros(tsim,1);
dtheta  = zeros(tsim,1);
tauHat  = zeros(tsim,1);
Vd      = zeros(tsim,1);

frames  = uint8(zeros(480,640,3,tsim));
tic
% Set reference
ref = 0;

% Start measurement logging
fprintf(stm32, 'log dc %f\n', tsim);
% Get first row and discard (title row)
rxStr = fgets(stm32);

% Start control loop
for k = 1:tsim
    % Get measurements from STM32
    data(k,:)   = getMeasurements(stm32);
    theta_m     = data(k,3);
    dtheta_m    = data(k,4);

    % Get measurements from CV
    [phi_m, frames(:,:,:,k)] = getCV(vid, cameraParams, phi(k));

    % Run kalman filter
    y = [theta_m; phi_m; dtheta_m];
    u = tauHat(k);
    [mup(:,k+1),Pp(:,:,k+1)] = runKF(mup(:,k), u, y, Pp(:,:,k), KF, plantModel);

%     theta(k)    = mup(3,k);
%     phi(k)      = mup(4,k);
%     dtheta(k)   = mup(1,k);

    theta(k)    = theta_m;
    phi(k)      = phi_m;
    dtheta(k)   = dtheta_m;


    % Run controller
%     tauHat(k+1) = runController(mup(:,k+1), ref, LQG);
    in = [dtheta_m; mup(2,k+1); theta_m; phi_m];
    tauHat(k+1) = runController(in, ref, LQG);

    % Control allocation
    Vd(k+1) = controlAllocation(tauHat(k+1), dtheta(k), frictParams, motorParams);

    % Command voltage 
    fprintf(stm32, 'motor %f supOut\n',Vd(k+1));
%     pause(0.05);
end
elapsed_time = toc;   

%% Close serial connection
fprintf(stm32, 'motor %f supOut\n',0);

fclose(stm32); delete(stm32); clear stm32;   % Close serial connection and clean up
fprintf('Serial connection terminated\n');


%% Plotting
figure(2); clf; grid on; hold on;
tplot = 1:1:tsim;
ax1 = subplot(2,2,1);
plot(tplot,phi.*180/pi,'DisplayName','phi','LineWidth',2); hold on;
% legend;
grid on;
ylabel('Angle [\circ]');
xlabel('Time [s]');
title('Object Angle');

ax2 = subplot(2,2,2);
plot(tplot,theta.*180/pi,'DisplayName','theta','LineWidth',2); hold on;
% legend;
grid on;
ylabel('Hand [\circ]');
xlabel('Time [s]');
title('Hand Angle');

ax3 = subplot(2,2,3); hold on;
plot(tplot,dtheta,'DisplayName','dtheta','LineWidth',2);
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
% legend('location','best')
grid on;
title('Hand Angular Velocity');

ax4 = subplot(2,2,4); hold on;
plot(tplot,Vd(1:tsim),'LineWidth',2,'DisplayName','Demanded Voltage');
ylabel('Voltage [V]');
% legend('location','best');
grid on;
title('Demanded Voltage');
xlabel('Time [s]');

linkaxes([ax1 ax2 ax3 ax4],'x');


%% Save video
writerObj = VideoWriter('lastExperiment.mp4');
writerObj.FrameRate = tsim/elapsed_time;
open(writerObj);

for i=1:tsim
    frame = frames(:,:,:,i);
    writeVideo(writerObj, frame);
end
close(writerObj);

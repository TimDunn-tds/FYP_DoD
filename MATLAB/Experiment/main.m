%% Clean up
clc; 

tsim    = 100;
loopRate = 20; % Hz
T = 1/loopRate; % seconds

%% Load parameters
plantParams     = getPlantParams();
frictParams     = getFrictionParams();
motorParams     = getMotorParams();
cameraParams    = getCameraParams();


%% Load Plant Model, LQG and Kalman Filter
plantModel  = getPlantModel(T);
LQG         = getLQG(T);
KF          = getKalmanFilter(T);

% Initialise Kalman Filter
x0          = getInitialPosition();
mup         = nan(size(plantModel.A,1),tsim + 1);
mup(:,1)    = KF.mup_init; 
Pp          = nan([size(plantModel.A), tsim + 1]);
Pp(:,:,1)   = KF.Pp_init;
% muf         = zeros(size(plantModel.A,1), tsim);
% Pf          = zeros(size(Pp));

% Get observer gain Lo
obs = getObserver(T);


%% Make serial connection
% flush(serialport("COM3",921600));
stm32 = openSerial();


%% Connect to webcam
vid = webcam(1);
vid.Resolution = '640x480';
preview(vid);
pause(5);


%% Control Loop
% Make space
data    = zeros(tsim,5);
theta   = zeros(tsim,1);
phi     = zeros(tsim,1);
dtheta  = zeros(tsim,1);
dphi    = zeros(tsim,1);
tauHat  = zeros(tsim,1); 
Vd      = zeros(tsim,1);
Va      = zeros(tsim,1);
Ia      = zeros(tsim,1);
phi_m   = zeros(tsim,1);
theta_m = zeros(tsim,1);
dtheta_m = zeros(tsim,1);
frames  = uint8(zeros(480,640,3,tsim));

% Set reference
ref = 0;

% Start measurement logging
fprintf(stm32, 'log reset 1\n');

% Start control loop
r = robotics.Rate(loopRate);
fprintf(stm32, 'ctrl start\n');
rxStr = fgets(stm32);

% if ~strcmp(fgets(stm32), 'controller started')
%     fprintf("Issue starting controller.\n");
%     pause
% end

for k = 1:tsim
    % Get measurements from STM32
    try
        data(k,:)   = getMeasurements(stm32);
        theta_m(k)  = data(k,2);
        dtheta_m(k) = data(k,3);
        Va(k)       = data(k,4);
        Ia(k)       = data(k,4);
    catch
        data(k,:)   = data(k-1,:);
        theta_m(k)  = data(k,2);
        dtheta_m(k) = data(k,3);
        Va(k)       = data(k,4);
        Ia(k)       = data(k,4);
        fprintf("Measurement timed out %f\n", k);
    end
    % Get measurements from CV
    [phi_m(k), frames(:,:,:,k)] = getCV(vid, cameraParams, phi(k));
    % Run kalman filter
    y = [theta_m(k); phi_m(k); dtheta_m(k)];
%     u = (tauHat(k)*plantParams.Jh)/plantParams.Jm;
    u = tauHat(k);

    [mup(:,k+1),Pp(:,:,k+1)] = runKF(mup(:,k), u, y, Pp(:,:,k), KF, plantModel);
%     mup(:,k+1) = runObserver(mup(:,k), u, y, plantModel, obs);
    
    % Store estimated states
    theta(k)    = mup(3,k+1);
    phi(k)      = mup(4,k+1);
    dtheta(k)   = mup(1,k+1);
    dphi(k)     = mup(2,k+1);

    % Run controller
    tauHat(k+1) = runController(mup(:,k+1), ref, LQG);

    % Command torque
%     fprintf(stm32, 'ctrl set %f\n', tauHat(k+1));
    
    % Control allocation
%     Vd(k+1) = controlAllocation(tauHat(k+1), dtheta(k), frictParams, motorParams);

    % Command voltage 
%     fprintf(stm32, 'motor %f supOut\n',Vd(k+1));

    % Stop if it get stupid
    if abs(phi(k))>pi/6
        fprintf(stm32, 'motor 0\n');
        fprintf(stm32, 'ctrl stop\n');
        break
    end
%     fprintf('dtheta: %2.2f, dphi: %2.2f, theta: %2.2f, phi: %2.2f\n', mup(1,k), mup(2,k), mup(3,k), mup(4,k));
    waitfor(r);
end

%% Close serial connection
fprintf(stm32, 'motor 0\n');
fprintf(stm32, 'ctrl stop\n');
pause(2);
fclose(stm32); delete(stm32); clear stm32;   % Close serial connection and clean up
fprintf('Serial connection terminated\n');

%% Close video
delete(vid); clear vid;   % Close video connection
fprintf('Video connection terminated\n');


%% Plotting
figure(2); clf; grid on; hold on;
tplot = 0:1:tsim-1;
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
plot(tplot,Va,'LineWidth',2,'DisplayName','Va');
ylabel('Voltage [V]');
plot(tplot,tauHat(1:length(tplot)),'LineWidth',2,'DisplayName','tauHat');
% legend('location','best');
grid on;
% yyaxis right
% plot(tplot,phi.*180/pi,'DisplayName','phi','LineWidth',2); hold on;
title('Demanded Voltage');
xlabel('Time [s]');
legend('location','best');
linkaxes([ax1 ax2 ax3 ax4],'x');


%% Save video
writerObj = VideoWriter('lastExperiment.mp4');
writerObj.FrameRate = loopRate;
open(writerObj);

for i=1:tsim
    frame = frames(:,:,:,i);
    writeVideo(writerObj, frame);
end
close(writerObj);

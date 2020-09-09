%% Clean up
clc; 

tsim    = 75;
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
if (exist('stm32','var'))
    closeSerial(stm32);
    delete(stm32); clear stm32
end
stm32 = openSerial();


%% Connect to webcam
if (exist('vid','var'))
    closeVideo(vid);
    clear vid;
end
vid = videoinput('winvideo',1, 'RGB24_320x240');
triggerconfig(vid, 'manual');
start(vid);
pause(3);


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
frames  = uint8(zeros(240,320,3,tsim));
% frames  = (zeros(240,320,3,tsim));

% Set reference
ref = 0;

% Start measurement logging
fprintf(stm32, 'log reset 1\n');

% Start control loop
r = robotics.Rate(loopRate);
fprintf(stm32, 'ctrl start\n');
rxStr = fgets(stm32);

for k = 2:tsim
%     tic
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
    [phi_m(k), frames(:,:,:,k)] = getCV(vid, cameraParams, mup(4,k));
    % Run kalman filter
    y = [theta_m(k); phi_m(k); dtheta_m(k)];
    u = tauHat(k);

%     [mup(:,k+1),Pp(:,:,k+1)] = runKF(mup(:,k), u, y, Pp(:,:,k), KF, plantModel);
    [mup,Pp] = runKF(mup, u, y, Pp, KF, plantModel, k);

%     mup = runObserver(mup, u, y, plantModel, obs, k);
    
    % Store estimated states
%     dtheta(k-1)   = mup(1,k);
%     dphi(k-1)     = mup(2,k);
%     theta(k-1)    = mup(3,k);
%     phi(k-1)      = mup(4,k);
    
    % Run controller
    dphi(k) = (phi_m(k) - phi_m(k-1))/T;
    in = [dtheta_m(k); dphi(k); theta_m(k); phi_m(k)]; 
%     tauHat(k+1) = runController(mup(:,k+1), ref, LQG);
    tauHat(k+1) = runController(in, ref, LQG);
%     tauHat(k+1) = runController(mup(:,k), ref, LQG);
    text = ['Angle: ', num2str(phi_m(k)*180/pi, 3), ', tau: ', num2str(tauHat(k+1),3)];
    frames(:,:,:,k) = insertText(frames(:,:,:,k), [1,120], text);

    % Command torque
    waitfor(r);
    fprintf(stm32, 'ctrl set %f\n', tauHat(k+1));
    
    % Control allocation
%     Vd(k+1) = controlAllocation(tauHat(k+1), dtheta(k), frictParams, motorParams);

    % Command voltage 
%     fprintf(stm32, 'motor %f supOut\n',Vd(k+1));

    % Stop if it get stupid
    if abs(phi(k))>pi/4
        fprintf(stm32, 'motor 0\n');
        fprintf(stm32, 'ctrl set 0\n');
        fprintf(stm32, 'ctrl stop\n');
        break
    end
    
end


%% Close serial connection
closeSerial(stm32);
delete(stm32); clear stm32


%% Close video
closeVideo(vid);
clear vid;

%% Plotting
figure(2); clf; grid on; hold on;
tplot = 0:T:(tsim-1)/loopRate;
ax1 = subplot(2,2,1);
phi_m(1:end-1) = phi_m(2:end);
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
plot(tplot,mup(1,1:end-1).*180/pi,'DisplayName','Estimated','LineWidth',2);
plot(tplot,dtheta_m.*180/pi,'DisplayName','Measured','LineWidth',2); hold on;
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
legend('location','best');
grid on;
title('Hand Angular Velocity');

ax4 = subplot(2,2,4); hold on;
% plot(tplot,Va,'LineWidth',2,'DisplayName','Va');
% ylabel('Voltage [V]');
plot(tplot,tauHat(1:length(tplot)),'LineWidth',2,'DisplayName','tauHat');
% legend('location','best');
grid on;
ylim([-2, 2])
yyaxis right
ylim([-10,10])
p2 = plot(tplot,phi_m.*180/pi,'DisplayName','Phi','LineWidth',2); hold on;
% title('Demanded Voltage');
xlabel('Time [s]');
legend('location','best');
linkaxes([ax1 ax2 ax3 ax4],'x');


% figure(3); clf; hold on; grid on;
% plot(tplot,squeeze(Pp(1,1,1:end-1)), 'DisplayName','dtheta', 'linewidth',2);
% plot(tplot,squeeze(Pp(2,2,1:end-1)), 'DisplayName','dphi', 'linewidth',2);
% plot(tplot,squeeze(Pp(3,3,1:end-1)), 'DisplayName','theta', 'linewidth',2);
% plot(tplot,squeeze(Pp(4,4,1:end-1)), 'DisplayName','phi', 'linewidth',2);
% legend('location','best');
% % 



%% Save video
writerObj = VideoWriter('lastExperiment.mp4');
writerObj.FrameRate = loopRate;
open(writerObj);

for i=1:tsim
    frame = frames(:,:,:,i);
    writeVideo(writerObj, frame);
end
close(writerObj);


%% Additional Functions
    function closeSerial(stm32)
        fprintf(stm32, 'motor 0\n');
        fprintf(stm32, 'ctrl set 0\n');
        fprintf(stm32, 'ctrl stop\n');
        pause(2);
        fclose(stm32);   % Close serial connection and clean up
        fprintf('Serial connection terminated\n');
    end
    
    
    function closeVideo(vid)
        delete(vid);   % Close video connection
        fprintf('Video connection terminated\n');
    end
    


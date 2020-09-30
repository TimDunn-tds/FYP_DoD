%% Clean up
clc; 

tsim    = 20;
loopRate = 1/0.15; % Hz
% loopRate = 25; % Hz

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
MPC         = getMPC(LQG,T);

% Initialise Kalman Filter
x0          = getInitialPosition();
mup         = nan(size(KF.Aod,1),tsim + 1);
mup(:,1)    = KF.mup_init; 
Pp          = nan([size(KF.Aod), tsim + 1]);
Pp(:,:,1)   = KF.Pp_init;

muL         = nan(size(KF.Aod,1),tsim + 1);
muL(:,2)    = KF.mup_init; 
% muf         = zeros(size(plantModel.A,1), tsim);
% Pf          = zeros(size(Pp));

% Get observer gain Lo
obs = getObserver(T);


%% Make serial connection
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

if (exist('mypi','var'))
    clear mypi;
end

% vid = videoinput('winvideo',1, 'RGB24_320x240');
% triggerconfig(vid, 'manual');
% vid.FramesPerTrigger = 1;
% vid.TriggerRepeat = inf;
% start(vid);
% cam = imaqhwinfo; % Get Camera information
% cameraName = char(cam.InstalledAdaptors(end));
% cameraInfo = imaqhwinfo(cameraName);
% cameraId = 1;
% cameraFormat ='RGB24_320x240';

% vid = imaq.VideoDevice(cameraName, cameraId, cameraFormat, ... % Input Video from current adapter
%                 'ReturnedColorSpace', 'RGB');
% vid = ipcam('http://rbpi:8080/?action=stream');


mypi = raspi('rbpi.local','pi','password');

% ROI = [235 0 488-235 0]./640;
vid = cameraboard(mypi,...
    'Quality', 10,...
    'Rotation', 90,...
    'FrameRate',ceil(loopRate),...
    'AWBMode', 'fluorescent',...
    'ExposureMode','sports');

pause(3);
          
% Get it going.
snapshot(vid);
% No, REALLY get it going.
snapshot(vid);
snapshot(vid);
snapshot(vid);
snapshot(vid);
snapshot(vid);

% No, instantiate the blob analysis.
getCV(vid, cameraParams, zeros(4,2), zeros(2,2), 2, T);

fprintf('Connected to video\n');


%% Control Loop
% Make space
% data    = zeros(tsim,5);
data    = zeros(tsim,2);
theta   = zeros(tsim,1);
phi     = zeros(tsim,1);
dtheta  = zeros(tsim,1);
dphi    = zeros(tsim,1);
wHat    = zeros(tsim,1); 
Vd      = zeros(tsim,1);
Va      = zeros(tsim,1);
Ia      = zeros(tsim,1);
phi_m   = zeros(tsim,1);
theta_m = zeros(tsim,1);
dtheta_m = zeros(tsim,1);
alphaHat = zeros(tsim,1);
alphaMPC = zeros(tsim,1);
% frames  = uint8(zeros(240,320,3,tsim));
% frames  = single(zeros(240,320,3,tsim));
frames  = uint8(zeros(480,640,3,tsim));

tSerial = zeros(tsim,1);
tCV     = zeros(tsim,1);
tTotal  = zeros(tsim,1);

% Set reference
ref = 0;

% Start measurement logging
fprintf(stm32, 'log reset 1\n');

% Start control loop
r = robotics.Rate(loopRate);
fprintf(stm32, 'ctrl start\n');
rxStr = fgets(stm32);
fprintf(stm32, 'light 0 0 0\n');
tStart = tic;

for k = 2:tsim
    tic
    % Poll for stm32 measurements
    fprintf(stm32, 'log ctrl 1\n');

    % Get measurements from CV
    [phi_m(k), frames(:,:,:,k)] = getCV(vid, cameraParams, mup, phi_m, k, T);
    tCV(k-1) = toc;
    
    % Retrieve stm32 measurements
    try
        data(k,:)   = getMeasurements(stm32);
        theta_m(k)  = data(k,1);
        dtheta_m(k) = data(k,2);
    catch
        data(k,:)   = data(k-1,:);
        theta_m(k)  = data(k,1);
        dtheta_m(k) = data(k,2);
        fprintf("Measurement timed out %f\n", k);
    end
    
    tSerial(k-1) = toc - tCV(k-1);

    
    % Run kalman filter
    y = [dtheta_m(k); theta_m(k); phi_m(k)];
%     u = alphaHat(k);
    u = alphaMPC(k);

%     [mup,Pp] = runKF(mup, u, y, Pp, KF, k);
    [mup(:,k),Pp(:,:,k)] = runKF(mup(:,k-1), u, y, Pp(:,:,k-1), KF, k);
%     muL = runObserver(muL, u, y, plantModel, obs, k);
    

    % Run controller (integrate output and apply)
    alphaHat(k+1)   = runController(mup(KF.idx,k), ref, LQG);
    alphaMPC(k+1)   = runMPC(ref,mup(KF.idx,k),alphaMPC(k),MPC);

    alphaHat(k+1)   = min(alphaHat(k+1),4/T)*double(alphaHat(k+1)>0) + max(alphaHat(k+1),-4/T)*double(alphaHat(k+1)<0);
    
%     wHat(k+1)       = wHat(k) + 1*T*alphaHat(k+1);
    wHat(k+1)       = wHat(k) + 1*T*alphaMPC(k+1);

    % Limit input 
    wHat(k+1) = min(wHat(k+1),4)*double(wHat(k+1)>0) + max(wHat(k+1),-4)*double(wHat(k+1)<0);
    
%     tauHat(k+1) = runController(in, ref, LQG);
%     tauHat(k+1) = runController(mup(:,k), ref, LQG);
    text = ['Angle: ', num2str(phi_m(k)*180/pi, 3), ', wHat: ', num2str(wHat(k+1), 3)];
    frames(:,:,:,k) = insertText(frames(:,:,:,k), [1,240], text);
%     text = ['Time: ', num2str((k-1)*T, 2)];


    % Command velocity
    if toc(tStart)>1
        fprintf(stm32, 'light 0 100 100\n');
        text = 'Light on';
        frames(:,:,:,k) = insertText(frames(:,:,:,k), [1,286], text);
    end
    text = ['Time: ', num2str(toc(tStart))];
    frames(:,:,:,k) = insertText(frames(:,:,:,k), [1,263], text);
    fprintf(stm32, 'ctrl set %f\n', wHat(k+1));
    waitfor(r);
    tTotal(k-1) = toc;
%     dt = toc;
end


%% Close serial connection
fprintf(stm32, 'light 0 0 0\n');
closeSerial(stm32);
delete(stm32); clear stm32


%% Close video
closeVideo(vid);
clear vid;

%% Plotting
set(0,'DefaultLineLineWidth',2)

figure(2); clf; grid on; hold on;
tplot = 0:T:(tsim-1)/loopRate;
ax1 = subplot(2,2,1);
dphi_m = [0; phi_m(2:end) - phi_m(1:end-1)./T];
% phi_m(1:end-1) = phi_m(2:end);
p1 = plot(tplot,mup(4,2:end).*180/pi,'DisplayName','Estimated','LineWidth',2); hold on;
p2 = plot(tplot(1:end-1),phi_m(2:end).*180/pi,'DisplayName','Measured','LineWidth',2); hold on;
p3 = plot(tplot,8.2*ones(size(tplot)),'r','LineWidth',1.5, 'DisplayName', 'Bumper');
plot(tplot,-9.5*ones(size(tplot)),'r','LineWidth',1.5);
title('Measured vs Estimated Object Angle');
ylabel('Angle [\circ]');
legend([p1, p2, p3], 'location','best');
set(gca, 'Ycolor', 'k');
% legend;
grid on;
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
plot(tplot,wHat(1:end-1),'DisplayName','Demanded','LineWidth',2)
ylabel('Velocity [rad/s]');
xlabel('Time [s]');

legend('location','best');
grid on;
title('Hand Angular Velocity');
set(gca, 'Ycolor', 'k');


ax4 = subplot(2,2,4); hold on; grid on;
plot(tplot,mup(2,1:end-1),'DisplayName','Estimated','LineWidth',2);
plot(tplot,dphi_m,'DisplayName','Measured','LineWidth',2);
ylabel('Dphi [rad/s]')
xlabel('Time [s]');
legend('location','best');
title('Object Angular Velocity');
linkaxes([ax1 ax2 ax3 ax4],'x');
set(gca, 'Ycolor', 'k');

% 
% figure(3); clf; hold on; grid on;
% plot(tplot,squeeze(Pp(1,1,1:end-1)), 'DisplayName','dtheta', 'linewidth',2);
% % plot(tplot,squeeze(Pp(2,2,1:end-1)), 'DisplayName','dphi', 'linewidth',2);
% plot(tplot,squeeze(Pp(3,3,1:end-1)), 'DisplayName','theta', 'linewidth',2);
% plot(tplot,squeeze(Pp(4,4,1:end-1)), 'DisplayName','phi', 'linewidth',2);
% legend('location','best');
% 
% figure(4); clf; hold on; grid on;
% p1 = plot(tplot,mup(4,2:end).*180/pi,'DisplayName','phi','LineWidth',2); hold on;
% ylabel('Angle [rad]');
% yyaxis right
% ylabel('Angular velocity [rad/s]');
% plot(tplot, dtheta_m, 'DisplayName', 'dtheta', 'linewidth' ,2);
% legend()

% 
figure(5); clf; hold on; grid on;
plot(tSerial, 'DisplayName', 'Serial timing');
plot(tCV, 'DisplayName', 'CV timing');
plot(tTotal, 'DisplayName', 'Loop timing');
ylabel('Time [s]');
xlabel('Timestep');
legend();

%% Save video
writerObj = VideoWriter('lastExperiment.mp4');
% writerObj = VideoWriter('cameraDelay_rbpi.mp4');
writerObj.FrameRate = loopRate;
open(writerObj);

for i=1:tsim
    frame = frames(:,:,:,i);
    writeVideo(writerObj, frame);
end
close(writerObj);


%% Save data
% dphi_m = [0; phi_m(2:end) - phi_m(1:end-1)/T];
% data(:,6) = phi_m;
% data(:,7) = wHat(2:end);
% data(:,8) = dphi_m;
% data(:,9) = alphaHat(2:end);
% save('data_PE3','data');


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
%         delete(vid);   % Close video connection
        fprintf('Video connection terminated\n');
    end
    
    
    
    % Get measurements from STM32
%     try
%         data(k,:)   = getMeasurements(stm32);
%         theta_m(k)  = data(k,2);
%         dtheta_m(k) = data(k,3);
%         Va(k)       = data(k,4);
%         Ia(k)       = data(k,4);
%     catch
%         data(k,:)   = data(k-1,:);
%         theta_m(k)  = data(k,2);
%         dtheta_m(k) = data(k,3);
%         Va(k)       = data(k,4);
%         Ia(k)       = data(k,4);
%         fprintf("Measurement timed out %f\n", k);
%     end

    


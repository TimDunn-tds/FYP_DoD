% Clea up
clc; clear;

TIMEOUT = 0.5; % Time to wait for data before aborting


% Pick sysID input
type = "sinWave_1_ctrl";
% type = "sinWave_ctrl";
% type = 'sinWave_005_ctrl';
% type = "stepUp_ctrl";
% type = "stepDown_ctrl";


% Get the signal desired
filename = sprintf("%s.mat",type);
input = importdata(filename);

% Get number of samples
N = length(input);
data = nan(N,6);

d = 0.1;
try
    % create serial object
    stm32 = serial('COM3', ...
        'BaudRate', 921600, ...
        'Timeout', TIMEOUT, ... 
        'Terminator', 'LF');  
    fopen(stm32); % open serial connection
    
    % Tell user its starting
    disp("Connection made, starting data logging");
    
    % Start controller
    fprintf(stm32, 'ctrl start\n');
    rxStr = fgets(stm32);
    
    % Start logging process
    fprintf(stm32, 'log dc %f\n',N);
    % Get first row and ignore it (title row)
    rxStr = fgets(stm32);
    
    for k = 1:N
        fprintf(stm32, 'ctrl set %f\n',input(k));
        fprintf('ctrl set %f\n', input(k));
        rxStr = fgets(stm32);
        data(k,:) = sscanf(rxStr,'%f,%f,%f,%f,%f,%f\n',[1,6]);
    end
    
%{    
%     % Start logging for N samples
%     N = 10;
%     data = zeros(N,6);
%     fprintf(stm32,'log dc %i\n',N);
%     % Get first row and ignore it (title row)
%     rxStr = fgets(stm32);
%     fprintf(rxStr);
%     
%     % Set motor voltage
%     fprintf(stm32,'motor 3 supOut\n');
%     
%     for k = 1:N
%         % Get data
%         rxStr = fgets(stm32);
%         fprintf(rxStr);
%         % Store data
%         data(k,:) = sscanf(rxStr,'%f,%f,%f,%f,%f,%f\n',[1,6]);
% %         pause(0.01);
%     end
%}
    % Turn motor off
    fprintf(stm32, 'ctrl set 0\n');
    fprintf(stm32, 'ctrl stop\n');
    fprintf(stm32,'motor 0 supOut\n');
    
    disp("Finished data logging");
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
        
catch
    disp('An error occured.');
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
end


%% Plot
time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);

k = find(V<0);
current(k) = -1.*current(k);

figure(10); clf; hold on;
subplot(3,1,1);
plot(time,input,'DisplayName','Demanded Velocity','linewidth',2); hold on;
plot(time,vel,'DisplayName','Velocity [rad/s]','linewidth',2); hold on;
grid on;
legend('location','best');
subplot(3,1,2);
plot(time,V,'DisplayName','Voltage [V]','linewidth',2); hold on;
grid on;
legend('location','best');
subplot(3,1,3);
plot(time,pos,'DisplayName','position','linewidth',2); hold on;
grid on;
legend('location', 'best');


%% Prompt for save of file
answer = questdlg("Save file?","Save prompt","Yes","No","No");
if answer == "Yes"
    saveFileName = sprintf("%s_data.mat", type);
    save(saveFileName, 'data');
end

    
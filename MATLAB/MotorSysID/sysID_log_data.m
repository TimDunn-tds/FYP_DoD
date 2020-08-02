% Clea up
clc; clear; close all

TIMEOUT = 0.5; % Time to wait for data before aborting


% Pick sysID input
% type = "sinWave";
% type = "deadZone";
% type = "rampDown";
type = "rampUp";
% type = "stepDown";
% type = "stepUp";

% Get the signal desired
filename = sprintf("%s.mat",type);
input = importdata(filename);

% Get number of samples
N = length(input);
data = zeros(N,6);

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
    
    % Start logging process
    fprintf(stm32, 'log dc %f\n',N);
    % Get first row and ignore it (title row)
    rxStr = fgets(stm32);
    
    for k = 1:N
        fprintf(stm32, 'motor %f supOut\n',input(k));
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
    fprintf(stm32,'motor 0 supOut\n');
    
    disp("Finished data logging");
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
        
catch
    disp('An error occured.');
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
end


% Prompt for save of file
answer = questdlg("Save file?","Save prompt","Yes","No","No");
if answer == "Yes"
    saveFileName = sprintf("%s_data.mat", type);
    save(saveFileName, 'data');
end




    
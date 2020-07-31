% Clea up
clc; clear;

TIMEOUT = 0.5; % Time to wait for data before aborting

try
    % create serial object
    stm32 = serial('COM3', ...
        'BaudRate', 921600, ...
        'Timeout', TIMEOUT, ... 
        'Terminator', 'LF');  
    fopen(stm32); % open serial connection
    
    % Start logging for N samples
    N = 10;
    data = zeros(N,5);
    fprintf(stm32,'log dc %i\n',N);
    % Get first row and ignore it (title row)
    rxStr = fgets(stm32);
    fprintf(rxStr);
    
    % Set motor voltage
    fprintf(stm32,'motor 3 supOut\n');
    
    for k = 1:N
        % Get data
        rxStr = fgets(stm32);
        fprintf(rxStr);
        % Store data
        data(k,:) = sscanf(rxStr,'%f,%f,%f,%f,%f\n',[1,5]);
%         pause(0.01);
    end
    
    % Turn motor off
    fprintf(stm32,'motor 0 supOut\n');
    
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
        
catch
    disp('failed');
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
end


    
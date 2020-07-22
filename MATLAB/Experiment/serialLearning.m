TIMEOUT = 0.5; % Time to wait for data before aborting

try
    % create serial object
    stm32 = serial('COM3', ...
        'BaudRate', 921600, ...
        'Timeout', TIMEOUT, ... 
        'Terminator', 'LF');
    
%     set(stm,'Timeout',TIMEOUT); % Adjust timeout
    
    fopen(stm32); % open serial connection
    
    for k = 1:10
        fprintf(stm32,'motor %f\n', k);
        rxStr = fgets(stm32);
        data = sscanf(rxStr,'%f\n');
        disp(data);
    end
    
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
        
catch
    disp('failed');
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
end


    
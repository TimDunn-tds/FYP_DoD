function stm32 = openSerial()
TIMEOUT = 0.5; % Time to wait for data before aborting

try
    % create serial object
    stm32 = serial('COM3', ...
        'BaudRate', 1843200,...
        'Timeout', TIMEOUT, ... 
        'Terminator', 'LF',...
        'DataBits', 8);  
    fopen(stm32); % open serial connection
    
    % Tell user its starting
    fprintf("Connection to STM32 successful.\n");
    
    % Turn motor off.
    fprintf(stm32, 'motor %f supOut\n',0);
    pause(2);
catch
    fprintf('An error occured while attempting to connect.\n');
    fclose(stm32); delete(stm32); clear stm32   % Close serial connection and clean up
    stm32 = [];
end


function data = getMeasurements(stm32)
% fprintf(stm32, 'log ctrl 1\n');
% rxStr = fgets(stm32);
% data = fscanf(stm32,'%f,%f,%f,%f,%f\n',[1,5]);
% tic
data = fscanf(stm32,'%f,%f\n',[1,2]);
% toc
% data = sscanf(rxStr,'%f,%f,%f,%f,%f\n',[1,5]);





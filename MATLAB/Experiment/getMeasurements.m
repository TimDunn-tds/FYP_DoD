function data = getMeasurements(stm32)
fprintf(stm32, 'log ctrl 1\n');
rxStr = fgets(stm32);
data = sscanf(rxStr,'%f,%f,%f,%f,%f\n',[1,5]);





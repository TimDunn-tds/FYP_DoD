function data = getMeasurements(stm32)

rxStr = fgets(stm32);
data = sscanf(rxStr,'%f,%f,%f,%f,%f,%f\n',[1,6]);





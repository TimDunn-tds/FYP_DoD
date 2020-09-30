% Get port names
[name1, name2, name3, name4, name5, name6] = getPortNames; 

% Set baudrate
baudrate = 921600;

% Open port and save pointer
portPointer = openPort(name1, baudrate);

% Set string to send
string = 'log data 1\n';
% Must convert to uint8????
dataOut = uint8(string);

% Write string
writePort(portPointer, dataOut);

%Try and read 50 bytes
numBytesToRead = 31;
tic;
[dataIn , leftOverBytesInBuffer] = readPort(portPointer, numBytesToRead);
toc;

% Close port
closePort(portPointer);

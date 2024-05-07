
s = serialport("COM6",9600);        %Initialize serial port variable
data = read(s,100,"uint32");          %read 64 bytes of data from port
plot(data);                         %plot data
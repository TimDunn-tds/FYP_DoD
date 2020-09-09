function writeArrayDef(x, file)

if nargin < 2
    file = 1;
end

if isa(file, 'numeric') && file < 3
    fid = file;
    fileOpened = false;
elseif isa(file, 'char')
    fid = fopen(file,'w');
    fileOpened = true;
else
    error('Expecting second argument to be a fileID or a filename')
end

x = x(:);
len = length(x);
for j = 1:len
    fprintf(fid,'%#gf',x(j));
    if j ~= len
        fprintf(fid,', ');
    end
end
fprintf(fid,'\n');

if fileOpened
    fclose(fid);
end
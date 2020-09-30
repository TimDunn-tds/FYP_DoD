% Pi testing
mypi = raspi('rbpi.local','pi','password')

vid = cameraboard(mypi,...
    'Quality', 5,...
    'Rotation', 90,...
    'FrameRate',60)
pause(3);
imshow(snapshot(vid))


%%
clear vid
clear mypi

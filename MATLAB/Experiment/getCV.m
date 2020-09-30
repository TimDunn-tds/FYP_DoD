function [phi, frame] = getCV(vid, cameraParams, mup, phi_m, k, T)
%% Unpack params
% R = cameraParams.RotationMatrices(:,:,end);
% T = cameraParams.TranslationVectors(end,:);

%% Get image
% Capture image from video stream
% imOrig = getsnapshot(vid);
% trigger(vid)
% imOrig = getdata(vid);
imOrig = snapshot(vid);
% imOrig = step(vid);
% Undistort the image

im = undistortImage(imOrig, cameraParams, 'OutputView', 'same');

%% Segment the circles
% magnification = 100;

% Convert to HSV colour space
imHSV = rgb2hsv(im);
% imshow(imHSV, 'InitialMagnification', magnification);

% Get the saturation channel.
sat = imHSV(:, :, 2);

% Attempt levels of thresholding
% t = multithresh(sat,2);
t = graythresh(sat);

imCircles = (sat > t(1));

%% Detect circles
% Find connected components.
persistent blobAnalysis
if isempty(blobAnalysis)  
    blobAnalysis = vision.BlobAnalysis('AreaOutputPort', true,...
    'CentroidOutputPort', true,...
    'BoundingBoxOutputPort', true,...
    'MaximumBlobArea', 6000,...
    'MinimumBlobArea', 2500,...
    'ExcludeBorderBlobs', true,...
    'MaximumCount', 3);
end

[areas, centroids, boxes] = blobAnalysis(imCircles);

% Sort connected components in descending order by area
[~, idx] = sort(areas, 'Descend');

if size(boxes,1) >= 2    
    % Get the two largest components.
    boxes = double(boxes(idx(1:2), :));
    centroids = double(centroids(idx(1:2),:));
    circles = [centroids, [mean(boxes(1,3:4))/2; mean(boxes(2,3:4))/2]];

    % Reduce the size of the image for display.
%     scale = magnification / 100;
%     imDetectedCircles = imresize(im, scale);

    % Insert labels for the circles.
%     im = insertObjectAnnotation(im, 'circle', circles(1,:), 'hand');
%     im = insertObjectAnnotation(im, 'circle', circles(2,:), 'object');
%     im = insertObjectAnnotation(im, 'circle', circles, [1 2]);
%     im = insertObjectAnnotation(im, 'circle', circles(2,:), 'object');
    im = insertMarker(im, centroids);


    %% Measure the circles and the angle
    % Calculate world points from image points
%     centWP = pointsToWorld(cameraParams, R, T, centroids);

    % Calculate the angle
    phi_mark = atan2(centroids(1,2) - centroids(2,2), centroids(2,1) - centroids(1,1)) + pi/2;

    phi = -1*asin(0.13*sin(pi-phi_mark)/0.18);
    phi = min(phi, 8*pi/180)*double(phi>0)...
        + max(phi, -9.5*pi/180)*double(phi<0);
%     text = ['Angle: ', num2str(phi*180/pi)];
%     imDetectedCircles = insertText(imDetectedCircles, [1,120], text);
    
    frame = im;
%     frame = imHSV;

%     imshow(frame, 'InitialMagnification', magnification);

else
    phi = min(phi_m(k-1) + T*mup(2,k), 8*pi/180)*double(phi_m(k)>0)...
        + max(phi_m(k-1) + T*mup(2,k), -8*pi/180)*double(phi_m(k)<0);
%     scale = magnification / 100;
%     frame = imresize(im, scale);
    frame = im;

end

    
